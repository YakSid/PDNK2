#ifndef CORDER_H
#define CORDER_H

/** @class class COrder
 * Класс приказа/квеста.
 *
 * Содержит основные настройки и этапы с исходами. В него записывается вся информация.
 * Потом этот класс записывыается в базу данных или файл.
 * На его основе строится графическая карта.
 * Хранит массив элементов CStage и массив элементов COutcome.
 */

#include "coutcome.h"
#include "cstage.h"
#include <QMap>

class COrder
{
public:
    enum EType { eInner, ePatrol };

    EType type;

public:
    COrder();
    ~COrder();

private:
    QMap<qint32, COutcome *> m_outcomes;
    QMap<qint32, CStage *> m_stages;
    /// Основные параметры приказа
};

#endif // CORDER_H
