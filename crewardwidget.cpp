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

void CRewardWidget::on_cb_rewardType_currentIndexChanged(int index)
{
    ui->cb_object->clear();
    ui->cb_object->setVisible(true);

    if (index != 0 && index != 2 && index != 7 && index != 8) {
        // TODO: сделать доп.окно для записей о нынешних сотрудниках, кто через что прошёл и кто есть
        //Может потом записывать в историю сотрудников в godot, через что они прошли? Да
        // TODO: уточнить сколько сотрудников сейчас выбрано
        ui->cb_object->addItems(CHARACTERS_MALE);
    } else if (index == 0) {
        ui->cb_object->addItems(RESOURCE_TYPES);
    } else if (index == 2) {
        ui->cb_object->setVisible(false);
    }
    // TODO: СЕЙЧАС продолжить с case 3

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
        break;
    case 4:
        //Ранение
        break;
    case 5:
        //Тяжёлое ранение
        break;
    case 6:
        //Смерть
        break;
    case 7:
        //Продвижение по сюжету
        break;
    case 8:
        //Изменение благополучия клетки
        break;
    }
}
