#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include "coutcomewidget.h"
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
        // запустить COrdersPage
        ui->setupUi(this);
        break;
    }
}

MainWindow::~MainWindow()
{
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

void MainWindow::on_action_save_triggered()
{
    // Сохранить всё в базу
}

void MainWindow::on_action_saveAndExit_triggered()
{
    on_action_save_triggered();
    // и выйти
}

void MainWindow::updateWindow()
{
    //
    // TODO: показывать условия приказа соответствующие типу вн или птр
}

void MainWindow::_prepareView()
{
    ui->cmb_type->setCurrentIndex(0);
    ui->swgt_order_type->setCurrentWidget(ui->wgt_inner_order);
    ui->cb_time->addItems(TIME_PERIODS);
    ui->grp_stageReward->setVisible(false);
}

void MainWindow::_changeGrpNumberStaffTitle()
{
    qint32 numberStaff = ui->spb_first_rank->value() + ui->spb_second_rank->value() + ui->spb_third_rank->value()
            + ui->spb_common->value();
    ui->grp_number_staff->setTitle("Необходимо сотрудников: " + QString::number(numberStaff));
}

void MainWindow::_createOutcome()
{
    //Невизуальная часть

    //Визуальная часть
    auto wgt = new COutcomeWidget;
    auto item = new QListWidgetItem(ui->lw_outcomes);
    item->setSizeHint(wgt->sizeHint());
    ui->lw_outcomes->setItemWidget(item, wgt);
}

void MainWindow::_deleteOutcome(QListWidgetItem *item)
{
    //Невизуальная часть

    //Визуальная часть
    auto wgt = ui->lw_outcomes->itemWidget(item);
    ui->lw_outcomes->takeItem(ui->lw_outcomes->row(item));
    delete wgt;
}

void MainWindow::on_cmb_type_currentIndexChanged(int index)
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

void MainWindow::on_cmb_department_currentIndexChanged(int index)
{
    // TODO: вписать значения и энумы сделать
}

void MainWindow::on_pb_addOutcome_clicked()
{
    _createOutcome();
}

void MainWindow::on_pb_deleteOutcome_clicked()
{
    _deleteOutcome(ui->lw_outcomes->currentItem());
}

void MainWindow::on_lw_outcomes_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current) {
        ui->pb_deleteOutcome->setEnabled(false);
    } else {
        ui->pb_deleteOutcome->setEnabled(true);
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
    spin->setMinimum(1);
    spin->setMaximumWidth(45);
    layout->addWidget(spin);
    auto cmb = new QComboBox;
    cmb->addItems(TRAITS);
    layout->addWidget(cmb);
    wgt->setLayout(layout);
    layout->setMargin(6);
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
    auto wgt = new QWidget;
    auto font = wgt->font();
    font.setBold(false);
    wgt->setFont(font);

    auto layoutHigh = new QHBoxLayout;
    auto te = new QTextEdit;
    te->setMinimumHeight(55);
    te->setMaximumHeight(55);
    layoutHigh->addWidget(te);
    auto btn = new QPushButton("К исходам...");
    btn->setMinimumWidth(105);
    layoutHigh->addWidget(btn);
    layoutHigh->setSpacing(4);

    auto layoutBottom = new QHBoxLayout;
    auto ch = new QCheckBox("Требуются ресурсы");
    ch->setChecked(true);
    connect(ch, &QCheckBox::toggled, this, &MainWindow::needReqResToggled);
    layoutBottom->addWidget(ch);
    auto cmb = new QComboBox;
    cmb->addItems(REQ_RESOURCES);
    layoutBottom->addWidget(cmb);
    auto spin = new QSpinBox;
    spin->setMinimum(1);
    spin->setMaximum(99999);
    spin->setMinimumWidth(65);
    layoutBottom->addWidget(spin);
    layoutBottom->addStretch();
    layoutBottom->setSpacing(4);

    auto layoutGeneral = new QVBoxLayout;
    layoutGeneral->setContentsMargins(6, 6, 6, 6);
    layoutGeneral->setSpacing(4);
    layoutGeneral->addLayout(layoutHigh);
    layoutGeneral->addLayout(layoutBottom);
    wgt->setLayout(layoutGeneral);

    auto item = new QListWidgetItem(ui->lw_variants);
    item->setSizeHint(QSize(200, 100));
    ui->lw_variants->setItemWidget(item, wgt);

    ch->setMinimumHeight(31);
    ch->setChecked(false);

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
        //Сделать финальным
        ui->pb_setFinal->setText("Сделать обычным");
        ui->groupVariants->setTitle("Результаты приказа");
        ui->lw_variants->setVisible(false);
        ui->pb_addVariant->setVisible(false);
        ui->pb_deleteVariant->setVisible(false);
        ui->groupStageDescription->setTitle("Описание результатов приказа");
    } else {
        //Сделать обычным
        ui->pb_setFinal->setText("Сделать финальным");
        ui->groupVariants->setTitle("Варианты действий");
        ui->lw_variants->setVisible(true);
        ui->pb_addVariant->setVisible(true);
        ui->pb_deleteVariant->setVisible(true);
        ui->groupStageDescription->setTitle("Описание этапа");
    }
}
// TODO: СЕЙЧАС доделать окно результатов(наград)
void MainWindow::on_pb_showRewardGroup_clicked()
{
    if (ui->grp_stageReward->isVisible()) {
        //Скрыть
        ui->grp_stageReward->setVisible(false);
        ui->pb_showRewardGroup->setText("Добавить результат(награду)");
    } else {
        //Отобразить
        ui->grp_stageReward->setVisible(true);
        ui->pb_showRewardGroup->setText("Убрать результат(награду)");
    }
}
