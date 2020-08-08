#ifndef CCONSTANTS_H
#define CCONSTANTS_H

#include <QStringList>

const QStringList TRAITS = { "Заражённый",
                             "Раненый",
                             "С отрицательным психологическим состоянием (-2)",
                             "С отрицательным психологическим состоянием (-1)",
                             "С любым отрицательным психологическим состоянием",
                             "С любым положительным психологическим состоянием",
                             "С положительным психологическим состоянием (+1)",
                             "С положительным психологическим состоянием (+2)",
                             "Осведомлённый на 100%",
                             "Не осведомлённый на 100%" };

const QStringList RESOURCE_TYPES = { "Деньги", "Оружие", "Серебро", "Опиум" };

const QStringList REWARD_TYPES = {
    "Ресурс",          "Опыт",   "Опыт каждому",          "Изменение псих. состояния",    "Ранение",
    "Тяжёлое ранение", "Смерть", "Продвижение по сюжету", "Изменение благополучия клетки"
};

const QStringList TIME_PERIODS = {
    "Пн Рассвет", "Пн День",    "Пн Сумерки", "Пн Ночь",    "Вт Рассвет", "Вт День",    "Вт Сумерки",
    "Вт Ночь",    "Ср Рассвет", "Ср День",    "Ср Сумерки", "Ср Ночь",    "Чт Рассвет", "Чт День",
    "Чт Сумерки", "Чт Ночь",    "Пт Рассвет", "Пт День",    "Пт Сумерки", "Пт Ночь",    "Сб Рассвет",
    "Сб День",    "Сб Сумерки", "Сб Ночь",    "Вс Рассвет", "Вс День",    "Вс Сумерки", "Вс Ночь",
};

const QStringList CHARACTERS_MALE = { "Адам", "Блейк",  "Валентайн", "Генри",  "Джон",
                                      "Итан", "Коннор", "Льюис",     "Мэттью", "Николас" };
const QStringList CHARACTERS_FEMALE = { "Абигейл", "Барбара", "Виктория", "Грейс", "Джейн",
                                        "Илейн",   "Кейт",    "Лара",     "Мэри",  "Нора" };

const QStringList PSY_CHANGES = { "Поднять на 1 единицу", "Поднять на 2 единицы",  "Поднять на 3 единицы",
                                  "Нормализовать в 0",    "Опустить на 1 единицу", "Опустить на 2 единицы",
                                  "Опустить на 3 единицы" };

#endif // CCONSTANTS_H
