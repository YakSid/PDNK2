#include "cstartpage.h"
#include "ui_cstartpage.h"

CStartPage::CStartPage(QWidget *parent) : QDialog(parent), ui(new Ui::CStartPage)
{
    ui->setupUi(this);
    ui->lb_writer->setText(m_writer);
    // ui->pushButton_5->setVisible(false);

    //Пока БД не используется
    /*ui->pb_connectDatabase->setVisible(false);
    ui->pic_connection->setVisible(false);
    ui->lbl_connected->setVisible(false);*/
}

CStartPage::~CStartPage()
{
    delete ui;
}

void CStartPage::init()
{
    // emit s_connectDatabase();
}

void CStartPage::databaseConnected(bool connected)
{
    /*QPixmap pixConnection;
    if (connected) {
        ui->lbl_connected->setText("БД подключена успешно");
        pixConnection.load(":/pictures/pictures/success.png");
        ui->pb_connectDatabase->setEnabled(false);
    } else {
        ui->lbl_connected->setText("Подключение не удалось");
        pixConnection.load(":/pictures/pictures/error.png");
        ui->pb_connectDatabase->setEnabled(true);
    }
    ui->pic_connection->setPixmap(pixConnection);*/
}

void CStartPage::on_pb_create_new_order_clicked()
{
    closeMode = 1;
    CStartPage::close();
}

void CStartPage::on_pb_edit_order_clicked()
{
    jFilename = QFileDialog::getOpenFileName(this, "Выберите приказ", QString(), tr("JSON (*.json)"));
    if (!jFilename.isEmpty())
        closeMode = 2;
    CStartPage::close();
}
