#include "coutcomewidget.h"
#include "ui_coutcomewidget.h"

COutcomeWidget::COutcomeWidget(QWidget *parent) : QWidget(parent), ui(new Ui::COutcomeWidget)
{
    ui->setupUi(this);
    ui->cb_trait->setVisible(false);
    ui->pb_toTrait->setVisible(false);
    ui->pb_tofailStageOrAuto->setVisible(false);
    ui->ch_success->setChecked(false);
    ui->ch_positive->setChecked(false);
    ui->ch_barely->setChecked(false);
}

COutcomeWidget::~COutcomeWidget()
{
    delete ui;
}

void COutcomeWidget::on_cb_type_currentIndexChanged(int index)
{
    if (index == 3) {
        ui->cb_trait->setVisible(true);
        ui->pb_toTrait->setVisible(true);
    } else {
        ui->cb_trait->setVisible(false);
        ui->pb_toTrait->setVisible(false);
    }
    if (index == 4 || index == 5) {
        ui->pb_tofailStageOrAuto->setVisible(true);
    } else {
        ui->pb_tofailStageOrAuto->setVisible(false);
    }
    if (index < 3) {
        ui->ch_success->setVisible(true);
        ui->sp_successLow->setVisible(true);
        ui->lb_andMore->setVisible(true);
        ui->pb_toSuccess->setVisible(true);
        ui->ch_positive->setVisible(true);
        ui->sp_positiveLow->setVisible(true);
        ui->sp_positiveHigh->setVisible(true);
        ui->pb_toPositive->setVisible(true);
        ui->ch_barely->setVisible(true);
        ui->sp_barelyLow->setVisible(true);
        ui->sp_barelyHigh->setVisible(true);
        ui->pb_toBarely->setVisible(true);
    } else {
        ui->ch_success->setVisible(false);
        ui->sp_successLow->setVisible(false);
        ui->lb_andMore->setVisible(false);
        ui->pb_toSuccess->setVisible(false);
        ui->ch_positive->setVisible(false);
        ui->sp_positiveLow->setVisible(false);
        ui->sp_positiveHigh->setVisible(false);
        ui->pb_toPositive->setVisible(false);
        ui->ch_barely->setVisible(false);
        ui->sp_barelyLow->setVisible(false);
        ui->sp_barelyHigh->setVisible(false);
        ui->pb_toBarely->setVisible(false);
    }
}

void COutcomeWidget::on_ch_success_toggled(bool checked)
{
    ui->sp_successLow->setEnabled(checked);
    ui->lb_andMore->setEnabled(checked);
    ui->pb_toSuccess->setEnabled(checked);
}

void COutcomeWidget::on_ch_positive_toggled(bool checked)
{
    ui->sp_positiveLow->setEnabled(checked);
    ui->sp_positiveHigh->setEnabled(checked);
    ui->pb_toPositive->setEnabled(checked);
}

void COutcomeWidget::on_ch_barely_toggled(bool checked)
{
    ui->sp_barelyLow->setEnabled(checked);
    ui->sp_barelyHigh->setEnabled(checked);
    ui->pb_toBarely->setEnabled(checked);
}
