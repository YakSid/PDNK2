#include "corderspage.h"
#include "ui_corderspage.h"

COrdersPage::COrdersPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::COrdersPage)
{
    ui->setupUi(this);
}

COrdersPage::~COrdersPage()
{
    delete ui;
}
