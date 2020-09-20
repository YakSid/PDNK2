#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <QFile>
#include "crewardwidget.h"
#include "cconstants.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    auto m_startPage = new CStartPage();
    m_databaseManager = new CDatabaseManager();
    connect(m_startPage, &CStartPage::s_connectDatabase, m_databaseManager, &CDatabaseManager::connectDatabase);
    connect(m_databaseManager, &CDatabaseManager::s_databaseConnected, m_startPage, &CStartPage::databaseConnected);
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
        // TODO: потом-потом, отобразить список сохранённых приказов с разными метками в corderspage
        ui->setupUi(this);
        // TODO: СЕЙЧАС не забыть добавить логику из on_pb_createQuest_clicked, а то ордер не инициализирован
        _prepareView();
        _prepareMainSettings(m_order->loadFromJSON(m_startPage->jFilename));
        break;
    }
}

MainWindow::~MainWindow()
{
    delete m_order;
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
        ui->lb_briefReminder->setText("Начальный этап");
    } else {
        auto wgt = qobject_cast<CVariantWidget *>(sender());
        wgt->setOutcomeId(id);
        _saveStageLoadOutcome(id);
    }
}

void MainWindow::on_action_save_triggered()
{
    //Сохранение текущего нода
    if (m_currentNode.isOutcome()) {
        _saveCurrentOutcome();
    } else {
        _saveCurrentStage();
    }
    // Сохранить всё в базу
    // TODO: указать имя сохраняемого приказа
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
    }
}

void MainWindow::on_action_saveAndExit_triggered()
{
    on_action_save_triggered();
    // TODO: чуть позже: и выйти
}

void MainWindow::updateWindow()
{
    //
    // TODO: позже: показывать условия приказа соответствующие типу вн или птр
}

void MainWindow::_prepareView()
{
    // TODO: чуть позже привести к начальному значению поля после сохранения или выбора другого приказа
    ui->cb_type->setCurrentIndex(0);
    ui->swgt_order_type->setCurrentWidget(ui->wgt_inner_order);
    ui->cb_time->addItems(TIME_PERIODS);
    ui->grp_stageReward->setVisible(false);
    ui->tabWidget->setCurrentIndex(0);
    ui->tabWidget->tabBar()->setTabEnabled(1, false);
    m_hexCheckBoxes.append({ ui->hex_poor_resident, ui->hex_rich_resident, ui->hex_works, ui->hex_commercial,
                             ui->hex_science, ui->hex_farmer, ui->hex_wild_land, ui->hex_police, ui->hex_church,
                             ui->hex_medicine, ui->hex_aristocracy, ui->hex_culture });
    m_welfareCheckBoxes.append({ ui->hex_welfare_prosper, ui->hex_welfare_normal, ui->hex_welfare_cluttered,
                                 ui->hex_welfare_devastation, ui->hex_welfare_abandoned });
    m_staffSpinBoxes.append({ ui->spb_common, ui->spb_third_rank, ui->spb_second_rank, ui->spb_first_rank });
    m_resSpinBoxes.append({ ui->spb_money, ui->spb_first_res, ui->spb_second_res, ui->spb_third_res });
}

void MainWindow::_prepareMainSettings(const SMainSettings &sett)
{
    // TODO: СЕЙЧАС заполнить настройки из полученных данных
    // TODO: СЕЙЧАС здесь или где-то заполнить mapManager этапами и ауткомами ?ui подготовить для первого?
}

void MainWindow::_prepareOutcomeUi(qint32 id)
{
    m_currentNode.update(id, eOutcome);
    ui->stackedWidget->setCurrentIndex(1);
    ui->gb_stagesOutcomes->setTitle("Исходы выбора");
    if (id == 0)
        ui->pb_toParentStage->setVisible(false);
    else {
        ui->pb_toParentStage->setVisible(true);
    }
    ui->lb_briefReminder->setText("краткое описание");
    //Подготовка проверок
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
    ui->lb_briefReminder->setText("краткое описание"); // TODO: чуть позже заполнить хэдэр из текста родителя
    //Подготовка времени и текста
    auto stageInfo = m_order->getStageInfo(id);
    ui->cb_time->setCurrentIndex(stageInfo.time);
    ui->te_stageText->setText(stageInfo.text);
    _setStageUiFinal(stageInfo.isFinal);
    //Подготовка наград
    ui->lw_rewards->clear();
    auto rewards = m_order->getStageRewards(id);
    if (rewards != nullptr) {
        for (auto reward : *rewards) {
            on_pb_addReward_clicked();
            auto wgt = qobject_cast<CRewardWidget *>(
                    ui->lw_rewards->itemWidget(ui->lw_rewards->item(ui->lw_rewards->count() - 1)));
            wgt->updateData(reward->type, reward->object, reward->count, reward->psyState);
        }
    }
    _setRewardsVisible(!rewards->isEmpty());
    //Подготовка вариантов
    ui->lw_variants->clear();
    auto variantsList = m_order->getStageVariants(id);
    if (variantsList != nullptr) {
        for (auto variant : *variantsList) {
            on_pb_addVariant_clicked();
            auto wgt = qobject_cast<CVariantWidget *>(
                    ui->lw_variants->itemWidget(ui->lw_variants->item(ui->lw_variants->count() - 1)));
            wgt->updateData(variant->text, variant->outcomeId, variant->resource, variant->resourceCount);
        }
    }
}

void MainWindow::_changeGrpNumberStaffTitle()
{
    qint32 numberStaff = ui->spb_first_rank->value() + ui->spb_second_rank->value() + ui->spb_third_rank->value()
            + ui->spb_common->value();
    ui->grp_number_staff->setTitle("Необходимо сотрудников: " + QString::number(numberStaff));
}

qint32 MainWindow::_createOutcome()
{
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
}

void MainWindow::_saveCurrentStage()
{
    QList<SReward *> rewards;
    for (int i = 0; i < ui->lw_rewards->count(); i++) {
        auto wgt = qobject_cast<CRewardWidget *>(ui->lw_rewards->itemWidget(ui->lw_rewards->item(i)));
        auto reward = new SReward;
        reward->type = wgt->getType();
        reward->object = wgt->getObject();
        reward->count = wgt->getCount();
        reward->psyState = wgt->getPsyState();
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
        // TODO: сделать в map_manager нод визуально финальным и свойство ему дать?
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
}

void MainWindow::_setRewardsVisible(bool st)
{
    if (st) {
        ui->grp_stageReward->setVisible(true);
        ui->pb_showRewardGroup->setText("Убрать результат(награду)");
    } else {
        ui->grp_stageReward->setVisible(false);
        ui->pb_showRewardGroup->setText("Добавить результат(награду)");
    }
}

void MainWindow::on_cb_type_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
        // Если выбран внутренний приказ (0)
        ui->swgt_order_type->setCurrentWidget(ui->wgt_inner_order);
        break;
    case 1:
        // Если выбран внешний приказ (патруль) (1)
        ui->swgt_order_type->setCurrentWidget(ui->wgt_patrol_order);
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

void MainWindow::on_cb_department_currentIndexChanged(int index)
{
    // TODO: позже: вписать значения и энумы сделать
}

// TODO: позже сделать алгоритм автоматического появления проверки "неудача", если есть одна из трёх первых и этой нет
void MainWindow::on_pb_addCheck_clicked()
{
    //Визуальная часть ui
    auto wgt = new COutcomeWidget();
    connect(wgt, &COutcomeWidget::s_createStageClicked, this, &MainWindow::slotCreateStageClicked);
    connect(wgt, &COutcomeWidget::s_toStageClicked, this, &MainWindow::slotToStageClicked);
    auto item = new QListWidgetItem(ui->lw_outcomes);
    auto wgtSize = wgt->sizeHint();
    wgtSize.setHeight(121);
    item->setSizeHint(wgtSize);
    ui->lw_outcomes->setItemWidget(item, wgt);
}

void MainWindow::on_pb_deleteCheck_clicked()
{
    //Невизуальная часть
    // TODO: чуть позже запустить удаление всех следующих?

    //Визуальная часть ui
    ui->lw_outcomes->takeItem(ui->lw_outcomes->currentRow());

    //Визуальная часть map
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
    auto item = new QListWidgetItem(ui->lw_variants);
    item->setSizeHint(wgt->sizeHint());
    ui->lw_variants->setItemWidget(item, wgt);

    if (!ui->pb_deleteVariant->isEnabled()) {
        ui->pb_deleteVariant->setEnabled(true);
    }
}

void MainWindow::on_pb_deleteVariant_clicked()
{
    if (!ui->lw_variants->currentItem())
        return;

    auto wgt = ui->lw_variants->itemWidget(ui->lw_variants->currentItem());
    ui->lw_variants->takeItem(ui->lw_variants->row(ui->lw_variants->currentItem()));
    delete wgt;

    if (!ui->lw_variants->count()) {
        ui->pb_deleteVariant->setEnabled(false);
    }
}

void MainWindow::on_pb_setFinal_clicked()
{
    if (ui->pb_setFinal->text() == "Сделать финальным") {
        _setStageUiFinal(true);
    } else {
        _setStageUiFinal(false);
    }
}

void MainWindow::on_pb_showRewardGroup_clicked()
{
    if (ui->grp_stageReward->isVisible()) {
        _setRewardsVisible(false);
    } else {
        _setRewardsVisible(true);
    }
}

void MainWindow::on_pb_createQuest_clicked()
{
    if (ui->tabWidget->tabBar()->isTabEnabled(1)) {
        //Создание квеста уже было произведено, нужно просто перейти на первый этап
        m_mapManager->setSelected(0, eOutcome);
    } else {
        //Нажимается впервые, нужно создать квест
        ui->tabWidget->tabBar()->setTabEnabled(1, true);
        m_order = new COrder();
        // TODO: чуть позже записать основные параметры квеста в сордер
        m_mapManager = new CMapManager();
        connect(m_mapManager, &CMapManager::s_newNodeSelected, this, &MainWindow::slotNewNodeSelected);
        ui->gb_map->layout()->addWidget(m_mapManager);
        m_currentNode.update(-1, eStage);
        slotCreateOutcomeClicked();
    }
    ui->tabWidget->setCurrentIndex(1);
}

void MainWindow::on_pb_toParentStage_clicked()
{
    auto parentId = m_order->getParentId(m_currentNode.id, m_currentNode.type);
    _saveOutcomeLoadStage(parentId);
}

void MainWindow::on_pb_toParentOutcome_clicked()
{
    auto parentId = m_order->getParentId(m_currentNode.id, m_currentNode.type);
    _saveStageLoadOutcome(parentId);
}

void MainWindow::on_pb_addReward_clicked()
{
    auto wgt = new CRewardWidget;
    auto item = new QListWidgetItem(ui->lw_rewards);
    item->setSizeHint(wgt->sizeHint());
    ui->lw_rewards->setItemWidget(item, wgt);
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
