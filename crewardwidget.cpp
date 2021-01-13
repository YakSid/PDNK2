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

qint32 CRewardWidget::getType()
{
    return ui->cb_rewardType->currentIndex();
}

qint32 CRewardWidget::getObject()
{
    return ui->cb_object->currentIndex();
}

qint32 CRewardWidget::getCount()
{
    return ui->sp_rewardCount->value();
}

qint32 CRewardWidget::getPsyState()
{
    return ui->cb_psyState->currentIndex();
}

QString CRewardWidget::getNote()
{
    if (ui->ln_note->isVisible())
        return ui->ln_note->text();
    else
        return "";
}

void CRewardWidget::updateData(qint32 type, qint32 object, qint32 count, qint32 psyState, QString note)
{
    if (type != -1)
        ui->cb_rewardType->setCurrentIndex(type);
    if (object != -1)
        ui->cb_object->setCurrentIndex(object);
    if (count != -1)
        ui->sp_rewardCount->setValue(count);
    if (psyState != -1)
        ui->cb_psyState->setCurrentIndex(psyState);
    if (!note.isEmpty())
        ui->ln_note->setText(note);
}

void CRewardWidget::on_cb_rewardType_currentIndexChanged(int index)
{
    ui->cb_object->clear();
    ui->cb_object->setVisible(true);
    ui->cb_psyState->setVisible(false);
    ui->ln_note->setVisible(false);
    ui->ln_note->clear();
    ui->sp_rewardCount->setVisible(true);
    ui->sp_rewardCount->setMinimum(1);
    ui->sp_rewardCount->setMaximum(99999);

    if (index != 0 && index != 2 && index != 7 && index != 8 && index != 10) {
        // TODO: релиз: уточнить сколько сотрудников сейчас выбрано
        ui->cb_object->addItems(CHARACTERS_MALE);
    } else if (index == 0) {
        ui->cb_object->addItems(RESOURCE_TYPES);
    } else if (index == 2 || index == 10) {
        ui->cb_object->setVisible(false);
    }

    switch (index) {
    case 0:
        //Ресурс
        break;
    case 1:
        //Опыт
        break;
    case 2:
        //Опыт каждому
        break;
    case 3:
        //Изменение психологического состояния
        ui->cb_psyState->addItems(PSY_CHANGES);
        ui->cb_psyState->setVisible(true);
        ui->sp_rewardCount->setVisible(false);
        break;
    case 4:
        //Ранение
        ui->sp_rewardCount->setVisible(false);
        break;
    case 5:
        //Тяжёлое ранение
        ui->sp_rewardCount->setVisible(false);
        break;
    case 6:
        //Смерть
        ui->sp_rewardCount->setVisible(false);
        break;
    case 7:
        //Продвижение по сюжету
        ui->cb_object->addItem("Основной сюжет");
        ui->sp_rewardCount->setVisible(false);
        break;
    case 8:
        //Изменение благополучия клетки
        ui->cb_object->addItems({ "Изменилось на", "Изменилось до" });
        ui->sp_rewardCount->setMinimum(-100);
        ui->sp_rewardCount->setMaximum(100);
        break;
    case 9:
        //Запись персонажу
        ui->sp_rewardCount->setVisible(false);
        ui->ln_note->setVisible(true);
        break;
    case 10:
        //Запись всем
        ui->sp_rewardCount->setVisible(false);
        ui->ln_note->setVisible(true);
        break;
    }
}

void CRewardWidget::on_cb_object_currentIndexChanged(const QString &arg1)
{
    if (arg1.right(2) == "до") {
        ui->sp_rewardCount->setMinimum(0);
    } else {
        ui->sp_rewardCount->setMinimum(-100);
    }
}
