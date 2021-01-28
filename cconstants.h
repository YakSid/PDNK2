#ifndef CCONSTANTS_H
#define CCONSTANTS_H

#include <QStringList>

//-------------------------------------------------------------------------//
// ПРИ ИЗМЕНЕНИИ КАКОЙ-ЛИБО КОНСТАНТЫ НУЖНО ПРОВЕРИТЬ ВСЕ ЕЁ УПОТРЕБЛЕНИЯ //
//-----------------------------------------------------------------------//

enum ENodeType { eOutcome, eStage };

enum EDepartments { ePolice, eChurch, eMedicine, eAristocracy, eCulture };

const QStringList TRAITS = { "Заражённый",
                             "Раненый",
                             "Местный",
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
    "Ресурс",          "Опыт",   "Опыт каждому",          "Изменение псих. состояния",     "Ранение",
    "Тяжёлое ранение", "Смерть", "Продвижение по сюжету", "Изменение благополучия клетки", "Запись",
    "Запись каждому"
};

const QStringList CHECK_TYPES = { "Физическая проверка", "Умственная проверка",
                                  "Командная проверка",  "Проверка на наличие особенности",
                                  "Проверки не нужны",   "Провал" };

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

// MAIN SETTINGS

const QStringList THREAT_LEVELS = { "[1] Подготовка департаментов", "[2] Паника от неизвестности",
                                    "[3] Открытое противостояние" };
const QStringList AWARNESS_INDEXES = { "[1] Департамент в неведении", "[2] Некоторые люди подозревают что-то",
                                       "[3] Почти все уверены", "[4] Департамент осведомлён" };
const QStringList VAMPIRES_MENTIONED = { "Точно да", "Неоднозначно", "Точно нет" };
const QStringList INNER_ORDER_TYPES = { "[1] Поднять П.О.", "[2] Получить очки знаний", "[3] Увеличить ресурсы",
                                        "[4] Прокачать сотрудников" };

// ТЕКСТОВЫЕ ОПИСАНИЯ

const QStringList DEPARTMENTS_PATROL_DESC = {
    "Расследует преступление в гексе, где произошло преступление",
    "Успокаивает население в гексе и прилегающих (поднимает благополучие)",
    "Проводит исследование в гексе и соседних/других (поднимает благополучие/улучшает иное)",
    "Прикрепляется к гексу (мб депа) и проводит сделку принося деньги и/или ресурсы",
    "Поднимает П.О. в выбранном депе или получает прибыль за освещение событий на гексах"
};

#endif // CCONSTANTS_H
