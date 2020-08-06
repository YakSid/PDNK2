#include "crewardwidget.h"
#include "ui_crewardwidget.h"

#include "cconstants.h"

CRewardWidget::CRewardWidget(QWidget *parent) : QWidget(parent), ui(new Ui::CRewardWidget)
{
    ui->setupUi(this);
    ui->cb_rewardType->addItems(REWARD_TYPES);
}

CRewardWidget::~CRewardWidget()
{
    delete ui;
}
