#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>
#include <coutcomewidget.h>

// TODO: реализовать механику всех элементов в mainwindow
// TODO: продумать ui этапов
// TODO: продумать механику этапов
// TODO: продумать ui исходов
// TODO: продумать механику исходов
// TODO: назвать все ui элементы в этапах и исходах
// TODO: реализовать механику этапов и исходов

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
    // TODO: предусмотреть добавление результатов
    _createOutcome();
    QString outcomesCountText = "У этого варианта сейчас %1 исходов";
    outcomesCountText.arg(QString::number(ui->lw_outcomes->count())); // TODO: подставить значение
    ui->lb_outcomesCount->setText(outcomesCountText);
}

void MainWindow::on_pb_deleteOutcome_clicked()
{
    _deleteOutcome(ui->lw_outcomes->currentItem());
    QString outcomesCountText = "У этого варианта сейчас %1 исходов";
    outcomesCountText.arg(QString::number(ui->lw_outcomes->count())); // TODO: подставить значение
    ui->lb_outcomesCount->setText(outcomesCountText);
}

void MainWindow::on_lw_outcomes_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    if (!current) {
        ui->pb_deleteOutcome->setEnabled(false);
    } else {
        ui->pb_deleteOutcome->setEnabled(true);
    }
}
