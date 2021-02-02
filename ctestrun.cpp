#include "ctestrun.h"
#include "ui_ctestrun.h"
#include "QLabel"
#include "QPushButton"
#include <QDebug>

CTestRun::CTestRun(const COrder *order, QWidget *parent) : QDialog(parent), ui(new Ui::CTestRun)
{
    m_orderPtr = order;
    ui->setupUi(this);
}

CTestRun::~CTestRun()
{
    delete ui;
}

void CTestRun::startFromBegining()
{
    //
}

void CTestRun::startFromStage(qint32 stageId)
{
    _showStage(stageId);
}

void CTestRun::_showStage(qint32 id)
{
    for (int i = 0; i < ui->lw_variants->count(); i++) {
        auto wgt = ui->lw_variants->itemWidget(ui->lw_variants->item(i));
        delete wgt;
    }
    ui->lw_variants->clear();

    auto info = m_orderPtr->getConstStageInfo(id);
    ui->text->setPlainText(info.text);

    auto rewards = *m_orderPtr->getConstStageRewards(id);
    //

    auto variants = *m_orderPtr->getConstStageVariants(id);
    for (auto variant : variants) {
        auto wgt = new QWidget;
        auto font = wgt->font();
        // font.setPointSize(12);
        // font.setBold(false);
        // wgt->setFont(font);
        auto layout = new QHBoxLayout;
        /*auto lbl = new QLabel;
        lbl->setObjectName("label");
        lbl->setText(variant->text);
        layout->addWidget(lbl);*/
        auto pb = new QPushButton;
        pb->setObjectName("pushButton");
        pb->setMinimumHeight(40);
        pb->setText(variant->text);
        pb->setFont(font);

        layout->addWidget(pb);

        layout->setAlignment(Qt::AlignLeft);
        wgt->setLayout(layout);
        layout->setContentsMargins(15, 6, 6, 6);
        layout->setSpacing(2);
        wgt->setMaximumHeight(55);

        auto item = new QListWidgetItem(ui->lw_variants);
        item->setSizeHint(wgt->sizeHint());
        ui->lw_variants->setItemWidget(item, wgt);
    }
}
