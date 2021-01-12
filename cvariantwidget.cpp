#include "cvariantwidget.h"
#include "ui_cvariantwidget.h"
#include "cconstants.h"
#include <QMouseEvent>

CVariantWidget::CVariantWidget(QWidget *parent) : QWidget(parent), ui(new Ui::CVariantWidget)
{
    ui->setupUi(this);
    auto font = this->font();
    font.setPointSize(12);
    font.setBold(false);
    this->setFont(font);
    ui->ch_needResource->setMinimumHeight(35);
    ui->cb_resources->addItems(RESOURCE_TYPES);
    ui->cb_resources->setVisible(false);
    ui->sp_resourceCount->setVisible(false);
    connect(ui->pb_toStage, &QPushButton::clicked, this, &CVariantWidget::slotToOutcomeClicked);
}

CVariantWidget::~CVariantWidget()
{
    delete ui;
}

void CVariantWidget::setOutcomeId(qint32 outcomeId)
{
    m_outcomeId = outcomeId;
}

QString CVariantWidget::getText()
{
    return ui->te_text->toPlainText();
}

qint32 CVariantWidget::getOutcomeId()
{
    return m_outcomeId;
}

qint32 CVariantWidget::getResource()
{
    qint32 result = -1;
    if (ui->ch_needResource->isChecked()) {
        result = ui->cb_resources->currentIndex();
    }
    return result;
}

qint32 CVariantWidget::getResourceCount()
{
    qint32 result = -1;
    if (ui->ch_needResource->isChecked()) {
        result = ui->sp_resourceCount->value();
    }
    return result;
}

void CVariantWidget::updateData(QString text, qint32 outcomeId, qint32 resource, qint32 resourceCount)
{
    ui->te_text->setText(text);
    m_outcomeId = outcomeId;
    if (resource != -1) {
        on_ch_needResource_stateChanged(1);
        ui->ch_needResource->setChecked(true);
        ui->cb_resources->setCurrentIndex(resource);
        ui->sp_resourceCount->setValue(resourceCount);
    } else {
        on_ch_needResource_stateChanged(0);
    }
}

void CVariantWidget::slotToOutcomeClicked()
{
    if (m_outcomeId == -1) {
        emit s_createOutcomeClicked();
    } else {
        emit s_toOutcomeClicked(m_outcomeId);
    }
}

void CVariantWidget::mousePressEvent(QMouseEvent *event)
{
    if (m_outcomeId != -1)
        emit s_markLineToOutcome(m_outcomeId);
    QWidget::mousePressEvent(event);
}

void CVariantWidget::on_ch_needResource_stateChanged(int arg1)
{
    ui->cb_resources->setVisible(arg1);
    ui->sp_resourceCount->setVisible(arg1);
}
