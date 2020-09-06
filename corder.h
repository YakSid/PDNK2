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

struct SReqToStaff {
    qint32 count;
    qint32 req;
};

struct SMainSettings {
    qint32 locationType;
    qint32 department;
    qint32 threatLevel;
    qint32 departmentPO;
    qint32 areVampires;
    qint32 innerOrderType;
    bool hexagonType[12] = { false, false, false, false, false, false, false, false, false, false, false, false };
    bool hexagonWelfare[5] = { false, false, false, false, false };
    qint32 staff[4] = { 0, 0, 0, 0 };
    qint32 resources[4] = { 0, 0, 0, 0 };
    QList<SReqToStaff> staffReq;
    QString text;
    QString needToDiscuss;
};

class COrder
{

public:
    COrder();
    ~COrder();
    void saveToJSON(QString filename, const SMainSettings &settings);
    void loadFromJSON(QString filename);

    qint32 addOutcome(qint32 parentId);
    void updateOutcome(qint32 id, const QList<SCheck *> &checks);
    const QList<SCheck *> *getOutcomeChecks(qint32 outcomeId);
    qint32 addStage(qint32 parentId);
    void updateStage(qint32 id, const QList<SVariant *> &variants);
    const QList<SVariant *> *getStageVariants(qint32 stageId);
    //! Узнать id родителя по id и типу сына
    qint32 getParentId(qint32 id, ENodeType type);

private:
    //! Пройтись по всем нодам указанного типа и вернуть минимальный незанятый id
    qint32 _makeMinId(ENodeType type);

private:
    QMap<qint32, COutcome *> m_outcomes;
    QMap<qint32, CStage *> m_stages;
};

#endif // CORDER_H
