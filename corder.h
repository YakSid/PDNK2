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
#include "cconstants.h"
#include <QMap>

class COrder
{

public:
    COrder();
    ~COrder();
    void saveOrder(QString path);

    qint32 addOutcome();
    void updateOutcome(qint32 id);
    qint32 addStage();
    void updateStage(qint32 id);
    //! Узнать id родителя по id и типу сына
    qint32 getParentId(qint32 id, ENodeType type);

private:
    //! Пройтись по всем нодам указанного типа и вернуть минимальный незанятый id
    qint32 _makeMinId(ENodeType type);

private:
    QMap<qint32, COutcome *> m_outcomes;
    QMap<qint32, CStage *> m_stages;
    // Основные параметры приказа
    enum EType { eInner, ePatrol };
    EType locationType;
};

#endif // CORDER_H
