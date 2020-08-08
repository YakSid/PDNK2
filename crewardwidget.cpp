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
        // TODO: позже добавить предупреждение, что нельзя указать "Изменилось до отрицательного числа"
        break;
    }
}
