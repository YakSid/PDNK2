#include "cstatisticspage.h"
#include "ui_cstatisticspage.h"

CStatisticsPage::CStatisticsPage(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CStatisticsPage)
{
    ui->setupUi(this);
}

CStatisticsPage::~CStatisticsPage()
{
    delete ui;
}
