#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFile>
#include <QMessageBox>
#include <QtWidgets>
#include "crewardwidget.h"
#include "cconstants.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    auto m_startPage = new CStartPage();
    m_databaseManager = new CDatabaseManager();
    // connect(m_startPage, &CStartPage::s_connectDatabase, m_databaseManager, &CDatabaseManager::connectDatabase);
    // connect(m_databaseManager, &CDatabaseManager::s_databaseConnected, m_startPage, &CStartPage::databaseConnected);
    m_startPage->init();
    m_startPage->setModal(true);
    m_startPage->exec();
    switch (m_startPage->closeMode) {
    case 0:
        exit(0);
    case 1:
        // Создать новый приказ
        ui->setupUi(this);
        _prepareView();
        break;
    case 2:
        // Редактировать старый приказ
        // TODO: 3 mid: отобразить список сохранённых приказов с разными метками в corderspage
        ui->setupUi(this);
        _prepareView();
        _initOrder(false);
        _prepareMainSettings(m_order->loadFromJSON(m_startPage->jFilename));
        _prepareMapAfterOrderLoad();
        m_haveUnsavedChanges = false;
        break;
    }
}

MainWindow::~MainWindow()
{
    _deleteAllObjectsFromLw(ui->lw_outcomes);
    _deleteAllObjectsFromLw(ui->lw_variants);
    _deleteAllObjectsFromLw(ui->lw_rewards);
    _deleteAllObjectsFromLw(ui->lw_terms);

    if (m_order != nullptr)
        delete m_order;
    m_warningTimer->stop();
    delete m_warningTimer;
    delete ui;
}

void MainWindow::needReqResToggled(bool checked)
{
    if (auto ch = qobject_cast<QCheckBox *>(sender())) {
        if (auto cmb = ch->parent()->findChild<QComboBox *>()) {
            cmb->setVisible(checked);
        }
        if (auto spin = ch->parent()->findChild<QSpinBox *>()) {
            spin->setVisible(checked);
        }
    }
}

void MainWindow::slotNewNodeSelected(qint32 id, ENodeType type)
{
    //Сохраняем текущий нод
    if (m_currentNode.isOutcome()) {
        _saveCurrentOutcome();
    } else {
        _saveCurrentStage();
    }
    //Загружаем выбранный
    if (type == eOutcome) {
        _prepareOutcomeUi(id);
    } else {
        _prepareStageUi(id);
    }
}

void MainWindow::slotToStageClicked(qint32 id)
{
    _saveOutcomeLoadStage(id);
}

void MainWindow::slotCreateStageClicked(COutcomeWidget::EOutcomeButton btn)
{
    qint32 id = _createStage();
    auto wgt = qobject_cast<COutcomeWidget *>(sender());
    wgt->setStageIdForNewButton(id, btn);
    _saveOutcomeLoadStage(id);
}

void MainWindow::slotToOutcomeClicked(qint32 id)
{
    _saveStageLoadOutcome(id);
}

void MainWindow::slotCreateOutcomeClicked()
{
    qint32 id = _createOutcome();
    if (id == 0) {
        //Начальный исход
        m_mapManager->setSelected(0, eOutcome);
        _prepareOutcomeUi(0);
    } else {
        auto wgt = qobject_cast<CVariantWidget *>(sender());
        wgt->setOutcomeId(id);
        _saveStageLoadOutcome(id);
    }
}

void MainWindow::slotPrepareNodesCopy(qint32 copiedId, ENodeType copiedType, qint32 selectedId, ENodeType selectedType)
{
    bool answer = true;
    if (copiedType != selectedType) {
        ui->lb_warningsLog->setText("Выбран несоответствующий тип этапа\nкопирование отменено");
        answer = false;
    }
    if (copiedId == -1) {
        //Заскриптованная ситуация: пользователь сам отменил копирование
        ui->lb_warningsLog->setText("");
        answer = false;
    } else if (!m_order->getChildrenId(copiedId, copiedType).isEmpty()) {
        ui->lb_warningsLog->setText("У копируемого есть дети\nкопирование отменено");
        answer = false;
    }

    if (answer) {
        //Замена адреса нода копируемого на адрес выбранного в кнопке родителя копируемого
        auto copiedParentId = m_order->getMainParentId(copiedId, copiedType);
        auto copiedAdditionalParentsList = m_order->getAdditionalParentsId(copiedId, copiedType);
        if (copiedType == eOutcome) {
            //Удаление копируемого нода ауткома
            m_order->deleteOutcome(copiedId, -1, true);
            //родитель - стейдж
            auto variants = *m_order->getStageVariants(copiedParentId);
            for (auto variant : variants) {
                if (variant->outcomeId == copiedId) {
                    variant->outcomeId = selectedId;
                    break;
                }
            }
            //Запись родителя копируемого в доп.родители выбранному
            m_order->addAdditionalParentToNode(selectedId, selectedType, copiedParentId);
            //Повтор предыдущих двух операций для доп.родителей копируемого нода
            for (auto prntId : copiedAdditionalParentsList) {
                auto variants = *m_order->getStageVariants(prntId);
                for (auto variant : variants) {
                    if (variant->outcomeId == copiedId) {
                        variant->outcomeId = selectedId;
                        break;
                    }
                }
                m_order->addAdditionalParentToNode(selectedId, selectedType, prntId);
            }
        } else {
            //Удаление копируемого нода стейджа
            m_order->deleteStage(copiedId, -1, true);
            //родитель - аутком
            auto checks = *m_order->getOutcomeChecks(copiedParentId);
            for (auto check : checks) {
                for (auto it = check->stagesId.begin(); it != check->stagesId.end(); it++) {
                    if (it.value() == copiedId) {
                        it.value() = selectedId;
                    }
                }
            }
            //Запись родителя копируемого в доп.родители выбранному
            m_order->addAdditionalParentToNode(selectedId, selectedType, copiedParentId);
            //Повтор предыдущих двух операций для доп.родителей копируемого нода
            for (auto prntId : copiedAdditionalParentsList) {
                auto checks = *m_order->getOutcomeChecks(prntId);
                for (auto check : checks) {
                    for (auto it = check->stagesId.begin(); it != check->stagesId.end(); it++) {
                        if (it.value() == copiedId) {
                            it.value() = selectedId;
                        }
                    }
                }
                m_order->addAdditionalParentToNode(selectedId, selectedType, prntId);
            }
        }
        m_haveUnsavedChanges = true;
    }

    ui->lb_warningsLog->setVisible(!answer);
    ui->pb_cancelCopy->setVisible(false);
    m_mapManager->canCopy(answer);
    if (!answer) {
        m_warningTimer->start();
    }
}

void MainWindow::slotNodeDoubleClicked()
{
    ui->pb_cancelCopy->setVisible(true);
}

void MainWindow::slotMarkLineToOutcome(qint32 destinationOutcomeId)
{
    m_mapManager->markLineFromStageToOutcome(m_currentNode.id, destinationOutcomeId);
}

void MainWindow::slotMarkLinesToStages(QList<qint32> destinationStages)
{
    m_mapManager->markLineFromOutcomeToStages(m_currentNode.id, destinationStages);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    if (m_haveUnsavedChanges) {
        bool answer = _showQuestion("В проекте остались несохранённые изменения, хотите выйти без сохранения?");
        if (answer) {
            event->accept();
        } else {
            event->ignore();
        }
    }
}

void MainWindow::on_action_save_triggered()
{
    if (m_order == nullptr)
        return;
    //Сохранение текущего нода
    if (m_currentNode.isOutcome()) {
        _saveCurrentOutcome();
    } else {
        _saveCurrentStage();
    }
    // Сохранить всё в базу
    QString jName = QFileDialog::getSaveFileName(this, "Сохранить приказ", "", "*.json");
    if (!jName.isEmpty()) {
        SMainSettings settings;
        settings.locationType = ui->cb_type->currentIndex();
        settings.department = ui->cb_department->currentIndex();
        settings.threatLevel = ui->cb_threatLevel->currentIndex();
        settings.departmentPO = ui->cb_awarenessIndex->currentIndex();
        settings.areVampires = ui->cb_vampiresMentioned->currentIndex();
        settings.innerOrderType = ui->cb_innerType->currentIndex();
        for (int i = 0; i < m_hexCheckBoxes.count(); i++) {
            if (m_hexCheckBoxes.at(i)->isChecked()) {
                settings.hexagonType[i] = true;
            }
        }
        for (int i = 0; i < m_welfareCheckBoxes.count(); i++) {
            if (m_welfareCheckBoxes.at(i)->isChecked()) {
                settings.hexagonWelfare[i] = true;
            }
        }
        for (int i = 0; i < m_staffSpinBoxes.count(); i++) {
            settings.staff[i] = m_staffSpinBoxes.at(i)->value();
        }
        for (int i = 0; i < m_resSpinBoxes.count(); i++) {
            settings.resources[i] = m_resSpinBoxes.at(i)->value();
        }
        for (auto termsChild : ui->lw_terms->children()) {
            auto wgt = qobject_cast<QWidget *>(termsChild);
            SReqToStaff req;
            auto spin = wgt->findChild<QSpinBox *>("spin");
            if (spin != nullptr) {
                req.count = spin->value();
                auto cb = wgt->findChild<QComboBox *>("cb");
                req.req = cb->currentIndex();
                settings.staffReq.append(req);
            }
        }
        settings.text = ui->te_order_text->toPlainText();
        settings.needToDiscuss = ui->te_need_to_discuss->toPlainText();

        m_order->saveToJSON(jName, settings);
        m_haveUnsavedChanges = false;
    }
}

// TODO: 2 mid: отслеживать изменились ли mainSettings и сохранять их или предупреждать перед выходом
void MainWindow::on_action_saveAndExit_triggered()
{
    on_action_save_triggered();
    qApp->quit();
}

void MainWindow::on_action_runTest_triggered()
{
    // TODO: 2 mid: сделать механику тест прогона приказа
    // m_testRun->startFromBegining();
    // m_testRun->setModal(true);
    // m_testRun->exec();
}

void MainWindow::on_action_runTestFromCurrent_triggered()
{
    if (m_currentNode.isOutcome()) {
        _showMessage("Выберите этап, чтобы провести тестовый прогон с указанного этапа");
    } else {
        m_testRun->startFromStage(m_currentNode.id);
        m_testRun->setModal(true);
        m_testRun->exec();
    }
}

void MainWindow::_prepareView()
{
    // TODO: 2 mid: привести к начальному значению поля после сохранения или выбора другого приказа
    //Настройка полей главных настроек приказа
    ui->cb_threatLevel->addItems(THREAT_LEVELS);
    ui->cb_awarenessIndex->addItems(AWARNESS_INDEXES);
    ui->cb_vampiresMentioned->addItems(VAMPIRES_MENTIONED);
    ui->cb_vampiresMentioned->setCurrentIndex(1);
    ui->cb_innerType->addItems(INNER_ORDER_TYPES);
    //
    setWindowTitle(PROGRAM_NAME);
    ui->cb_type->setCurrentIndex(0);
    ui->lb_patrolDesc->setVisible(false);
    ui->lb_patrolDesc->setText(DEPARTMENTS_PATROL_DESC[0]);
    ui->swgt_order_type->setCurrentWidget(ui->wgt_inner_order);
    ui->cb_time->addItems(TIME_PERIODS);
    ui->grp_stageReward->setVisible(false);
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->tabBar()->setTabEnabled(1, false);
    m_hexCheckBoxes.clear();
    m_welfareCheckBoxes.clear();
    m_staffSpinBoxes.clear();
    m_resSpinBoxes.clear();
    m_hexCheckBoxes.append({ ui->hex_poor_resident, ui->hex_rich_resident, ui->hex_works, ui->hex_commercial,
                             ui->hex_science, ui->hex_farmer, ui->hex_wild_land, ui->hex_police, ui->hex_church,
                             ui->hex_medicine, ui->hex_aristocracy, ui->hex_culture });
    m_welfareCheckBoxes.append({ ui->hex_welfare_prosper, ui->hex_welfare_normal, ui->hex_welfare_cluttered,
                                 ui->hex_welfare_devastation, ui->hex_welfare_abandoned });
    m_staffSpinBoxes.append({ ui->spb_common, ui->spb_third_rank, ui->spb_second_rank, ui->spb_first_rank });
    m_resSpinBoxes.append({ ui->spb_money, ui->spb_first_res, ui->spb_second_res, ui->spb_third_res });
    ui->lb_warningsLog->setStyleSheet("color: rgb(255,165,00)");
    ui->lb_warningsLog->setVisible(false);
    ui->pb_cancelCopy->setVisible(false);
    m_warningTimer = new QTimer();
    m_warningTimer->setInterval(10000);
    m_warningTimer->setSingleShot(true);
    connect(m_warningTimer, &QTimer::timeout, this, &MainWindow::_hideWarning);
}

void MainWindow::_prepareMainSettings(const SMainSettings &sett)
{
    // Заполнение основных настроек
    ui->cb_type->setCurrentIndex(sett.locationType);
    ui->cb_department->setCurrentIndex(sett.department);
    ui->cb_threatLevel->setCurrentIndex(sett.threatLevel);
    ui->cb_awarenessIndex->setCurrentIndex(sett.departmentPO);
    ui->cb_vampiresMentioned->setCurrentIndex(sett.areVampires);
    ui->cb_innerType->setCurrentIndex(sett.innerOrderType);
    for (int i = 0; i < m_hexCheckBoxes.count() && i < sett.hexagonType.count(); i++) {
        m_hexCheckBoxes.at(i)->setChecked(sett.hexagonType.at(i));
    }
    for (int i = 0; i < m_welfareCheckBoxes.count() && i < sett.hexagonWelfare.count(); i++) {
        m_welfareCheckBoxes.at(i)->setChecked(sett.hexagonWelfare.at(i));
    }
    for (int i = 0; i < m_staffSpinBoxes.count() && i < sett.staff.count(); i++) {
        m_staffSpinBoxes.at(i)->setValue(sett.staff.at(i));
    }
    for (int i = 0; i < m_resSpinBoxes.count() && i < sett.resources.count(); i++) {
        m_resSpinBoxes.at(i)->setValue(sett.resources.at(i));
    }
    for (auto req : sett.staffReq) {
        _addTerm(req.count, req.req);
    }
    ui->te_order_text->setText(sett.text);
    ui->te_need_to_discuss->setText(sett.needToDiscuss);
}

void MainWindow::_prepareMapAfterOrderLoad()
{
    m_mapManager->addFirstNode();

    SNode currentNode;
    currentNode.update(0, eOutcome);
    _addLoadedNodeInMap(currentNode);

    m_mapManager->setSelected(currentNode.id, currentNode.type);
    _prepareOutcomeUi(currentNode.id);
}

void MainWindow::_prepareOutcomeUi(qint32 id)
{
    m_currentNode.update(id, eOutcome);
    ui->stackedWidget->setCurrentIndex(1);
    ui->gb_stagesOutcomes->setTitle("Исходы выбора");
    if (id == 0) {
        ui->pb_toParentStage->setVisible(false);
        ui->lb_briefReminderOutcome->setText("Проверка после назначения");
    } else {
        ui->pb_toParentStage->setVisible(true);
        ui->lb_briefReminderOutcome->setText(m_order->getHeaderString(id, eOutcome));
    }
    //Подготовка проверок
    _deleteAllObjectsFromLw(ui->lw_outcomes);
    ui->lw_outcomes->clear();
    auto checksList = m_order->getOutcomeChecks(id);
    if (checksList != nullptr) {
        for (auto check : *checksList) {
            on_pb_addCheck_clicked();
            auto wgt = qobject_cast<COutcomeWidget *>(
                    ui->lw_outcomes->itemWidget(ui->lw_outcomes->item(ui->lw_outcomes->count() - 1)));
            wgt->updateData(check->type, check->trait, check->spinValues, check->stagesId);
        }
    }
}
void MainWindow::_prepareStageUi(qint32 id)
{
    m_currentNode.update(id, eStage);
    ui->stackedWidget->setCurrentIndex(0);
    ui->gb_stagesOutcomes->setTitle("Этап с выбором");
    ui->pb_toParentOutcome->setVisible(true);
    ui->lb_briefReminderStage->setText(m_order->getHeaderString(id, eStage));
    //Подготовка времени и текста
    auto stageInfo = m_order->getStageInfo(id);
    ui->cb_time->setCurrentIndex(stageInfo.time);
    ui->te_stageText->setText(stageInfo.text);
    _setStageUiFinal(stageInfo.isFinal);
    //Подготовка наград
    _deleteAllObjectsFromLw(ui->lw_rewards);
    ui->lw_rewards->clear();
    auto rewards = m_order->getStageRewards(id);
    if (rewards != nullptr) {
        for (auto reward : *rewards) {
            on_pb_addReward_clicked();
            auto wgt = qobject_cast<CRewardWidget *>(
                    ui->lw_rewards->itemWidget(ui->lw_rewards->item(ui->lw_rewards->count() - 1)));
            wgt->updateData(reward->type, reward->object, reward->count, reward->psyState, reward->note);
        }
    }
    if (!stageInfo.isFinal)
        _setRewardsVisible(!rewards->isEmpty());
    //Подготовка вариантов
    _deleteAllObjectsFromLw(ui->lw_variants);
    ui->lw_variants->clear();
    auto variantsList = m_order->getStageVariants(id);
    if (variantsList != nullptr) {
        for (auto variant : *variantsList) {
            on_pb_addVariant_clicked();
            auto wgt = qobject_cast<CVariantWidget *>(
                    ui->lw_variants->itemWidget(ui->lw_variants->item(ui->lw_variants->count() - 1)));
            wgt->updateData(variant->text, variant->outcomeId, variant->resource, variant->resourceCount);
        }
        if (variantsList->isEmpty()) {
            ui->pb_deleteVariant->setEnabled(false);
        }
    }
}

void MainWindow::_showMessage(QString text, QString title)
{
    QMessageBox msg;
    msg.setText(text);
    msg.setWindowTitle(title);
    msg.exec();
}

bool MainWindow::_showQuestion(QString text, QString textYes, QString textNo, QString title)
{
    bool result = false;
    QMessageBox msgBox(QMessageBox::Question, title, text, QMessageBox::Yes | QMessageBox::No, this);
    msgBox.setButtonText(QMessageBox::Yes, textYes);
    msgBox.setButtonText(QMessageBox::No, textNo);
    qint32 resMsg = msgBox.exec();
    if (resMsg == QMessageBox::Yes)
        result = true;
    return result;
}

void MainWindow::_changeGrpNumberStaffTitle()
{
    qint32 previousStaffCount = m_staffCount;
    m_staffCount = ui->spb_first_rank->value() + ui->spb_second_rank->value() + ui->spb_third_rank->value()
            + ui->spb_common->value();
    ui->grp_number_staff->setTitle("Необходимо сотрудников: " + QString::number(m_staffCount));

    if (m_staffCount < 1) {
        _showMessage("Должен быть назначен хотя бы один сотрудник");
        ui->pb_createQuest->setEnabled(false);
        if (m_questCreated)
            ui->tabWidget->tabBar()->setTabEnabled(1, false);
        return;
    }

    if (m_staffCount > 10) {
        //Если увеличилось количество сотрудников
        if (m_staffCount > previousStaffCount)
            _showMessage("Количество персонажей больше 10, пожалуйста, уменьшите состав");
        ui->pb_createQuest->setEnabled(false);
        if (m_questCreated)
            ui->tabWidget->tabBar()->setTabEnabled(1, false);
    } else if (!ui->pb_createQuest->isEnabled()) {
        ui->pb_createQuest->setEnabled(true);
        if (m_questCreated)
            ui->tabWidget->tabBar()->setTabEnabled(1, true);
    }
}

qint32 MainWindow::_createOutcome()
{
    m_haveUnsavedChanges = true;
    //Невизуальная часть
    qint32 id = m_order->addOutcome(m_currentNode.id);

    //Визуальная часть map
    if (id == 0) {
        m_mapManager->addFirstNode();
    } else {
        m_mapManager->addNode(id, eOutcome);
    }

    return id;
}

qint32 MainWindow::_createStage()
{
    m_haveUnsavedChanges = true;
    //Невизуальная часть
    qint32 id = m_order->addStage(m_currentNode.id);

    //Визуальная часть map
    m_mapManager->addNode(id, eStage);

    return id;
}

void MainWindow::_saveCurrentOutcome()
{
    QList<SCheck *> checks;
    for (int i = 0; i < ui->lw_outcomes->count(); i++) {
        auto wgt = qobject_cast<COutcomeWidget *>(ui->lw_outcomes->itemWidget(ui->lw_outcomes->item(i)));
        auto check = new SCheck;
        check->type = wgt->getType();
        check->trait = wgt->getTrait();
        check->spinValues = wgt->getSpinValues();
        check->stagesId = wgt->getStagesId();
        checks.append(check);
    }
    m_order->updateOutcome(m_currentNode.id, checks);
    m_haveUnsavedChanges = true;
}

void MainWindow::_saveCurrentStage()
{
    // TODO: позже-позже проверить утечки памяти, не быстрее ли будет проверять были ли изменения?
    QList<SReward *> rewards;
    for (int i = 0; i < ui->lw_rewards->count(); i++) {
        auto wgt = qobject_cast<CRewardWidget *>(ui->lw_rewards->itemWidget(ui->lw_rewards->item(i)));
        auto reward = new SReward;
        reward->type = wgt->getType();
        reward->object = wgt->getObject();
        reward->count = wgt->getCount();
        reward->psyState = wgt->getPsyState();
        reward->note = wgt->getNote();
        rewards.append(reward);
    }

    QList<SVariant *> variants;
    for (int i = 0; i < ui->lw_variants->count(); i++) {
        auto wgt = qobject_cast<CVariantWidget *>(ui->lw_variants->itemWidget(ui->lw_variants->item(i)));
        auto variant = new SVariant;
        variant->text = wgt->getText();
        variant->outcomeId = wgt->getOutcomeId();
        variant->resource = wgt->getResource();
        variant->resourceCount = wgt->getResourceCount();
        variants.append(variant);
    }
    m_order->updateStage(m_currentNode.id, variants, ui->cb_time->currentIndex(), ui->te_stageText->toPlainText(),
                         rewards);
    m_haveUnsavedChanges = true;
}

void MainWindow::_saveOutcomeLoadStage(qint32 stageId)
{
    _saveCurrentOutcome();

    m_mapManager->setSelected(stageId, eStage);
    _prepareStageUi(stageId);
}

void MainWindow::_saveStageLoadOutcome(qint32 outcomeId)
{
    _saveCurrentStage();

    m_mapManager->setSelected(outcomeId, eOutcome);
    _prepareOutcomeUi(outcomeId);
}

void MainWindow::_setStageUiFinal(bool st)
{
    if (st) {
        ui->pb_setFinal->setText("Сделать обычным");
        ui->groupVariants->setTitle("");
        ui->lw_variants->setVisible(false);
        ui->pb_addVariant->setVisible(false);
        ui->pb_deleteVariant->setVisible(false);
        ui->groupStageDescription->setTitle("Описание результатов приказа");
        ui->grp_stageReward->setMaximumHeight(16777215);
        m_order->setStageFinal(m_currentNode.id, true);
        _setRewardsVisible(true);
    } else {
        ui->pb_setFinal->setText("Сделать финальным");
        ui->groupVariants->setTitle("Варианты действий");
        ui->lw_variants->setVisible(true);
        ui->pb_addVariant->setVisible(true);
        ui->pb_deleteVariant->setVisible(true);
        ui->groupStageDescription->setTitle("Описание этапа");
        ui->grp_stageReward->setMaximumHeight(135);
        m_order->setStageFinal(m_currentNode.id, false);
    }
    m_haveUnsavedChanges = true;
}

void MainWindow::_setRewardsVisible(bool st)
{
    if (st) {
        ui->grp_stageReward->setVisible(true);
        ui->pb_showRewardGroup->setText("Убрать результаты(награды)");
    } else {
        ui->grp_stageReward->setVisible(false);
        ui->pb_showRewardGroup->setText("Добавить результаты(награды)");
    }
}

void MainWindow::_addTerm(qint32 count, qint32 req)
{
    auto wgt = new QWidget;
    auto font = wgt->font();
    font.setPointSize(12);
    font.setBold(false);
    wgt->setFont(font);
    auto layout = new QHBoxLayout;
    auto spin = new QSpinBox;
    spin->setObjectName("spin");
    spin->setMinimum(1);
    spin->setMaximumWidth(45);
    layout->addWidget(spin);
    auto cb = new QComboBox;
    cb->setObjectName("cb");
    cb->addItems(TRAITS);
    layout->addWidget(cb);
    wgt->setLayout(layout);
    layout->setContentsMargins(15, 6, 6, 6);
    layout->setSpacing(2);
    wgt->setMaximumHeight(55);

    auto item = new QListWidgetItem(ui->lw_terms);
    item->setSizeHint(wgt->sizeHint());
    ui->lw_terms->setItemWidget(item, wgt);

    if (!ui->pb_deleteTerm->isEnabled()) {
        ui->pb_deleteTerm->setEnabled(true);
    }

    if (count != -1 && req != -1) {
        spin->setValue(count);
        cb->setCurrentIndex(req);
    }
}

void MainWindow::_addLoadedNodeInMap(SNode node)
{
    auto childrenId = m_order->getChildrenId(node.id, node.type);
    if (!childrenId.isEmpty()) {
        for (auto child : childrenId) {
            //Проверка не нарисован ли уже этот нод
            bool needToPaint = true;
            for (auto paintedNode : m_paintedNodes) {
                if (paintedNode->type == node.anotherType() && paintedNode->id == child) {
                    needToPaint = false;
                }
            }
            //
            if (needToPaint) {
                m_mapManager->setSelected(node.id, node.type);
                m_mapManager->addNode(child, node.anotherType());
                //Если этап и финальный то сделать мап нод финальным
                if (node.anotherType() == eStage) {
                    if (m_order->isStageFinal(child)) {
                        m_mapManager->setFinal(child, eStage);
                    }
                }
                //Добавление нода в список, чтобы не нарисовать его второй раз
                auto paintedNode = new SNode;
                paintedNode->update(child, node.anotherType());
                m_paintedNodes.append(paintedNode);

                SNode nextNode;
                nextNode.update(child, node.anotherType());
                _addLoadedNodeInMap(nextNode);
            } else {
                m_mapManager->addAdditionalParentToNode(child, node.anotherType(), node.id, node.type);
            }
        }
    }
}

void MainWindow::_deleteAllObjectsFromLw(QListWidget *lw)
{
    for (int i = 0; i < lw->count(); i++) {
        auto wgt = lw->itemWidget(lw->item(i));
        delete wgt;
    }
}

void MainWindow::on_cb_type_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        // Если выбран внутренний приказ (0)
        ui->swgt_order_type->setCurrentWidget(ui->wgt_inner_order);
        ui->lb_patrolDesc->setVisible(false);
        break;
    case 1:
        // Если выбран внешний приказ (патруль) (1)
        ui->swgt_order_type->setCurrentWidget(ui->wgt_patrol_order);
        ui->lb_patrolDesc->setVisible(true);
        break;
    }
}

void MainWindow::on_spb_first_rank_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    _changeGrpNumberStaffTitle();
}

void MainWindow::on_spb_second_rank_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    _changeGrpNumberStaffTitle();
}

void MainWindow::on_spb_third_rank_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    _changeGrpNumberStaffTitle();
}

void MainWindow::on_spb_common_valueChanged(int arg1)
{
    Q_UNUSED(arg1);
    _changeGrpNumberStaffTitle();
}

void MainWindow::on_grp_req_resources_toggled(bool arg1)
{
    if (!arg1) {
        ui->spb_money->setValue(0);
        ui->spb_first_res->setValue(0);
        ui->spb_second_res->setValue(0);
        ui->spb_third_res->setValue(0);
    }
}

// TODO: 2 min: вписать значения в константы и энумы сделать
void MainWindow::on_cb_department_currentIndexChanged(int index)
{
    if (index >= 0 && index <= DEPARTMENTS_PATROL_DESC.count())
        ui->lb_patrolDesc->setText(DEPARTMENTS_PATROL_DESC[index]);
}

void MainWindow::on_pb_addCheck_clicked()
{
    //Визуальная часть ui
    auto wgt = new COutcomeWidget();
    connect(wgt, &COutcomeWidget::s_createStageClicked, this, &MainWindow::slotCreateStageClicked);
    connect(wgt, &COutcomeWidget::s_toStageClicked, this, &MainWindow::slotToStageClicked);
    connect(wgt, &COutcomeWidget::s_markLineToStages, this, &MainWindow::slotMarkLinesToStages);
    auto item = new QListWidgetItem(ui->lw_outcomes);
    auto wgtSize = wgt->sizeHint();
    wgtSize.setHeight(121);
    item->setSizeHint(wgtSize);
    ui->lw_outcomes->setItemWidget(item, wgt);
    m_haveUnsavedChanges = true;
}

void MainWindow::on_pb_deleteCheck_clicked()
{
    if (!ui->lw_outcomes->currentItem()) {
        _showMessage("Проверка не выбрана");
        return;
    } else {
        bool answer = _showQuestion("Удалить выбранный набор проверок и всех их наследников?");
        if (!answer)
            return;
    }

    auto outcomeWgt = qobject_cast<COutcomeWidget *>(ui->lw_outcomes->itemWidget(ui->lw_outcomes->currentItem()));

    for (auto stageId : outcomeWgt->getStagesId()) {
        //Удалить данные из order
        m_order->deleteStage(stageId, m_currentNode.id);
        //Удалить из map
        m_mapManager->deleteNode(stageId, eStage, m_currentNode.id, eOutcome);
    }

    //Удалить виджет из текущего ui
    ui->lw_outcomes->takeItem(ui->lw_outcomes->currentRow());
    delete outcomeWgt;

    if (!ui->lw_outcomes->count()) {
        ui->pb_deleteCheck->setEnabled(false);
    }

    m_haveUnsavedChanges = true;
}

void MainWindow::on_lw_outcomes_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous);
    if (!current) {
        ui->pb_deleteCheck->setEnabled(false);
    } else {
        ui->pb_deleteCheck->setEnabled(true);
    }
}

void MainWindow::on_pb_addTerm_clicked()
{
    _addTerm();
}

void MainWindow::on_pb_deleteTerm_clicked()
{
    if (!ui->lw_terms->currentItem())
        return;

    auto wgt = ui->lw_terms->itemWidget(ui->lw_terms->currentItem());
    ui->lw_terms->takeItem(ui->lw_terms->row(ui->lw_terms->currentItem()));
    delete wgt;

    if (!ui->lw_terms->count()) {
        ui->pb_deleteTerm->setEnabled(false);
    }
}

void MainWindow::on_stackedWidget_currentChanged(int arg1)
{
    if (arg1 == 0) {
        //Переключаемся к табу этапа
        ui->gb_stagesOutcomes->setTitle("Этап с выбором");
    } else {
        //Переключаемся к табу исхода
        ui->gb_stagesOutcomes->setTitle("Исходы выбора");
    }
}

void MainWindow::on_pb_addVariant_clicked()
{
    //Визуальная часть ui
    auto wgt = new CVariantWidget();
    connect(wgt, &CVariantWidget::s_createOutcomeClicked, this, &MainWindow::slotCreateOutcomeClicked);
    connect(wgt, &CVariantWidget::s_toOutcomeClicked, this, &MainWindow::slotToOutcomeClicked);
    connect(wgt, &CVariantWidget::s_markLineToOutcome, this, &MainWindow::slotMarkLineToOutcome);
    auto item = new QListWidgetItem(ui->lw_variants);
    item->setSizeHint(wgt->sizeHint());
    ui->lw_variants->setItemWidget(item, wgt);

    if (!ui->pb_deleteVariant->isEnabled()) {
        ui->pb_deleteVariant->setEnabled(true);
    }

    m_haveUnsavedChanges = true;
}

void MainWindow::on_pb_deleteVariant_clicked()
{
    if (!ui->lw_variants->currentItem()) {
        _showMessage("Вариант не выбран");
        return;
    } else {
        bool answer = _showQuestion("Удалить выбранный вариант и всех его наследников?");
        if (!answer)
            return;
    }

    auto variantWgt = qobject_cast<CVariantWidget *>(ui->lw_variants->itemWidget(ui->lw_variants->currentItem()));

    if (variantWgt->getOutcomeId() != -1) {
        //Удалить данные из order
        m_order->deleteOutcome(variantWgt->getOutcomeId(), m_currentNode.id);
        //Удалить из map
        m_mapManager->deleteNode(variantWgt->getOutcomeId(), eOutcome, m_currentNode.id, eStage);
    }

    //Удалить виджет из текущего ui
    ui->lw_variants->takeItem(ui->lw_variants->row(ui->lw_variants->currentItem()));
    delete variantWgt;

    if (!ui->lw_variants->count()) {
        ui->pb_deleteVariant->setEnabled(false);
    }

    m_haveUnsavedChanges = true;
}

void MainWindow::on_pb_setFinal_clicked()
{
    if (ui->pb_setFinal->text() == "Сделать финальным") {
        _setStageUiFinal(true);
        m_mapManager->setFinal(m_currentNode.id, m_currentNode.type, true);
    } else {
        _setStageUiFinal(false);
        m_mapManager->setFinal(m_currentNode.id, m_currentNode.type, false);
    }
}

void MainWindow::on_pb_showRewardGroup_clicked()
{
    auto stageInfo = m_order->getStageInfo(m_currentNode.id);
    if (stageInfo.isFinal) {
        _showMessage("Результат финального звена должен существовать");
        return;
    }

    if (ui->grp_stageReward->isVisible()) {
        _setRewardsVisible(false);
    } else {
        _setRewardsVisible(true);
    }
}

void MainWindow::on_pb_createQuest_clicked()
{
    if (m_questCreated) {
        //Создание квеста уже было произведено, нужно просто перейти на первый этап
        m_mapManager->setSelected(0, eOutcome);
        _prepareOutcomeUi(0);
    } else {
        //Нажимается впервые, нужно создать квест
        _initOrder(true);
    }
    ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::on_pb_toParentStage_clicked()
{
    auto parentId = m_order->getMainParentId(m_currentNode.id, m_currentNode.type);
    _saveOutcomeLoadStage(parentId);
}

void MainWindow::on_pb_toParentOutcome_clicked()
{
    auto parentId = m_order->getMainParentId(m_currentNode.id, m_currentNode.type);
    _saveStageLoadOutcome(parentId);
}

void MainWindow::on_pb_addReward_clicked()
{
    auto wgt = new CRewardWidget(m_staffCount);
    auto item = new QListWidgetItem(ui->lw_rewards);
    item->setSizeHint(wgt->sizeHint());
    ui->lw_rewards->setItemWidget(item, wgt);

    m_haveUnsavedChanges = true;
}

void MainWindow::on_pb_deleteReward_clicked()
{
    if (!ui->lw_rewards->currentItem())
        return;

    auto wgt = ui->lw_rewards->itemWidget(ui->lw_rewards->currentItem());
    ui->lw_rewards->takeItem(ui->lw_rewards->row(ui->lw_rewards->currentItem()));
    delete wgt;

    if (!ui->lw_rewards->count()) {
        ui->pb_deleteReward->setEnabled(false);
    }

    m_haveUnsavedChanges = true;
}

void MainWindow::on_lw_rewards_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous);
    if (!current) {
        ui->pb_deleteReward->setEnabled(false);
    } else {
        ui->pb_deleteReward->setEnabled(true);
    }
}

void MainWindow::_initOrder(bool newOrder)
{
    ui->tabWidget->tabBar()->setTabEnabled(1, true);
    m_order = new COrder();
    m_testRun = new CTestRun(m_order, this);
    m_mapManager = new CMapManager();
    connect(m_mapManager, &CMapManager::s_newNodeSelected, this, &MainWindow::slotNewNodeSelected);
    connect(m_mapManager, &CMapManager::s_askToCopy, this, &MainWindow::slotPrepareNodesCopy);
    connect(m_mapManager, &CMapManager::s_nodeDoubleClicked, this, &MainWindow::slotNodeDoubleClicked);
    ui->gb_map->layout()->addWidget(m_mapManager);
    m_currentNode.update(-1, eStage);
    m_questCreated = true;
    if (newOrder)
        slotCreateOutcomeClicked();
}

void MainWindow::on_pb_cancelCopy_clicked()
{
    ui->lb_warningsLog->setVisible(false);
    ui->pb_cancelCopy->setVisible(false);
    m_mapManager->canCopy(false);
}

void MainWindow::_hideWarning()
{
    ui->lb_warningsLog->setVisible(false);
}

void MainWindow::on_tabWidget_currentChanged(int index)
{
    if (!m_questCreated)
        return;

    if (index == 0) {
        if (m_currentNode.isOutcome()) {
            _saveCurrentOutcome();
        } else {
            _saveCurrentStage();
        }
    } else {
        if (m_currentNode.isOutcome()) {
            _prepareOutcomeUi(m_currentNode.id);
        } else {
            _prepareStageUi(m_currentNode.id);
        }
    }
}
