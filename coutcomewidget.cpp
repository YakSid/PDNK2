#include "coutcomewidget.h"
#include "ui_coutcomewidget.h"
#include <QDebug>
#include "cconstants.h"

COutcomeWidget::COutcomeWidget(QWidget *parent) : QWidget(parent), ui(new Ui::COutcomeWidget)
{
    ui->setupUi(this);
    ui->cb_trait->setVisible(false);
    ui->lb_trait->setVisible(false);
    ui->cb_trait->addItems(TRAITS);
    ui->pb_toTrait->setVisible(false);
    ui->pb_toFailStageOrAuto->setVisible(false);
    ui->ch_success->setChecked(false);
    ui->ch_positive->setChecked(false);
    ui->ch_barely->setChecked(false);
    ui->cb_type->addItems(CHECK_TYPES);
    m_buttons.insert(EOutcomeButton::trait, ui->pb_toTrait);
    m_buttons.insert(EOutcomeButton::success, ui->pb_toSuccess);
    m_buttons.insert(EOutcomeButton::positive, ui->pb_toPositive);
    m_buttons.insert(EOutcomeButton::barely, ui->pb_toBarely);
    m_buttons.insert(EOutcomeButton::failStageOrAuto, ui->pb_toFailStageOrAuto);
    for (auto btn : m_buttons) {
        connect(btn, &QPushButton::clicked, this, &COutcomeWidget::slotToStageClicked);
    }
}

COutcomeWidget::~COutcomeWidget()
{
    delete ui;
}
// TODO: позже предусмотреть удаление и изменение id этапов к кнопкам
void COutcomeWidget::setStageIdForNewButton(qint32 stageId, EOutcomeButton btn)
{
    auto it = m_stages.find(btn);
    if (it != m_stages.end()) {
        m_stages.remove(it.key());
    }
    m_stages.insert(btn, stageId);
}

qint32 COutcomeWidget::getType()
{
    return ui->cb_type->currentIndex();
}

qint32 COutcomeWidget::getTrait()
{
    if (ui->cb_trait->isVisible()) {
        return ui->cb_trait->currentIndex();
    } else {
        return -1;
    }
}

QList<qint32> COutcomeWidget::getSpinValues()
{
    QList<qint32> result;
    if (ui->ch_success->isVisible()) {
        if (ui->ch_success->isChecked()) {
            result.append({ ui->sp_successLow->value(), 100 });
        }
        if (ui->ch_positive->isChecked()) {
            result.append({ ui->sp_positiveLow->value(), ui->sp_positiveHigh->value() });
        }
        if (ui->ch_barely->isChecked()) {
            result.append({ ui->sp_barelyLow->value(), ui->sp_barelyHigh->value() });
        }
    }
    return result;
}

QMap<COutcomeWidget::EOutcomeButton, qint32> COutcomeWidget::getStagesId()
{
    return m_stages;
}

void COutcomeWidget::updateData(qint32 type, qint32 trait, QList<qint32> spinValues,
                                QMap<COutcomeWidget::EOutcomeButton, qint32> stagesId)
{
    ui->cb_type->setCurrentIndex(type);
    if (trait != -1) {
        ui->cb_trait->setCurrentIndex(trait);
    }
    switch (spinValues.count()) {
    case 6:
        ui->ch_barely->setChecked(true);
        ui->sp_barelyHigh->setValue(spinValues.last());
        spinValues.removeLast();
        ui->sp_barelyLow->setValue(spinValues.last());
        spinValues.removeLast();
        [[clang::fallthrough]];
    case 4:
        ui->ch_positive->setChecked(true);
        ui->sp_positiveHigh->setValue(spinValues.last());
        spinValues.removeLast();
        ui->sp_positiveLow->setValue(spinValues.last());
        spinValues.removeLast();
        [[clang::fallthrough]];
    case 2:
        spinValues.removeLast();
        ui->ch_success->setChecked(true);
        ui->sp_successLow->setValue(spinValues.last());
        spinValues.removeLast();
        break;
    case 0:
        break;
    default:
        return;
    }

    m_stages.clear();
    m_stages = stagesId;
}

void COutcomeWidget::slotToStageClicked()
{
    auto btnSender = qobject_cast<QPushButton *>(sender());
    EOutcomeButton clickedButton;
    for (auto it = m_buttons.begin(); it != m_buttons.end(); it++) {
        if (it.value() == btnSender) {
            clickedButton = it.key();
            break;
        }
    }

    auto it = m_stages.find(clickedButton);
    if (it == m_stages.end()) {
        emit s_createStageClicked(clickedButton);
    } else {
        emit s_toStageClicked(it.value());
    }
}

void COutcomeWidget::on_cb_type_currentIndexChanged(int index)
{
    if (index == 3) {
        ui->cb_trait->setVisible(true);
        ui->lb_trait->setVisible(true);
        ui->pb_toTrait->setVisible(true);
    } else {
        ui->cb_trait->setVisible(false);
        ui->lb_trait->setVisible(false);
        ui->pb_toTrait->setVisible(false);
    }
    if (index == 4 || index == 5) {
        ui->pb_toFailStageOrAuto->setVisible(true);
    } else {
        ui->pb_toFailStageOrAuto->setVisible(false);
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
