#include "cstartpage.h"
#include "ui_cstartpage.h"

CStartPage::CStartPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CStartPage)
{
    ui->setupUi(this);
}

CStartPage::~CStartPage()
{
    delete ui;
}
