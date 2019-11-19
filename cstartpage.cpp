#include "cstartpage.h"
#include "ui_cstartpage.h"

CStartPage::CStartPage(QWidget *parent) : QDialog(parent), ui(new Ui::CStartPage)
{
    ui->setupUi(this);
}

CStartPage::~CStartPage()
{
    delete ui;
}

void CStartPage::databaseConnected(bool connected)
{
    QPixmap pixConnection; //("/pictures/success.png");
    if (connected) {
        ui->lbl_connected->setText("БД подключена успешно");
        pixConnection.load(":/pictures/pictures/success.png");
    } else {
        ui->lbl_connected->setText("Подключение не удалось");
        pixConnection.load(":/pictures/pictures/error.png");
    }
    ui->pic_connection->setPixmap(pixConnection);
}

void CStartPage::on_pb_connectDatabase_clicked()
{
    emit s_connectDatabase(ui->ln_dbPath->text());
}

void CStartPage::on_pb_create_new_order_clicked()
{
    closeMode = 1;
    CStartPage::close();
}

void CStartPage::on_pb_edit_order_clicked()
{
    closeMode = 2;
    CStartPage::close();
}
