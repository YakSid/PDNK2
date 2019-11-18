#include "cstartpage.h"
#include "ui_cstartpage.h"

CStartPage::CStartPage(QWidget *parent) : QWidget(parent), ui(new Ui::CStartPage)
{
    ui->setupUi(this);
}

CStartPage::~CStartPage()
{
    delete ui;
}

void CStartPage::databaseConnected(bool connected)
{
    if (connected) {
        ui->lbl_connected->setText("БД подключена успешно");
    } else {
        ui->lbl_connected->setText("Подключение не удалось");
    }
}

void CStartPage::on_pb_connectDatabase_clicked()
{
    emit s_connectDatabase(ui->ln_dbPath->text());
}
