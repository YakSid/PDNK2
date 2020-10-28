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

void CRewardWidget::updateData(qint32 type, qint32 object, qint32 count, qint32 psyState)
{
    if (type != -1)
        ui->cb_rewardType->setCurrentIndex(type);
    if (object != -1)
        ui->cb_object->setCurrentIndex(object);
    if (count != -1)
        ui->sp_rewardCount->setValue(count);
    if (psyState != -1)
        ui->cb_psyState->setCurrentIndex(psyState);
}

void CRewardWidget::on_cb_rewardType_currentIndexChanged(int index)
{
    ui->cb_object->clear();
    ui->cb_object->setVisible(true);
    ui->cb_psyState->setVisible(false);
    ui->sp_rewardCount->setVisible(true);
    ui->sp_rewardCount->setMinimum(1);
    ui->sp_rewardCount->setMaximum(99999);

    if (index != 0 && index != 2 && index != 7 && index != 8) {
        // TODO: позже сделать доп.окно для записей о нынешних сотрудниках, кто через что прошёл и кто есть
        //Может потом записывать в историю сотрудников в godot, через что они прошли? Да
        // TODO: позже уточнить сколько сотрудников сейчас выбрано
        ui->cb_object->addItems(CHARACTERS_MALE);
    } else if (index == 0) {
        ui->cb_object->addItems(RESOURCE_TYPES);
    } else if (index == 2) {
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
