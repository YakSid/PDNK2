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
    QList<bool> hexagonType = { false, false, false, false, false, false, false, false, false, false, false, false };
    QList<bool> hexagonWelfare = { false, false, false, false, false };
    QList<qint32> staff = { 0, 0, 0, 0 };
    QList<qint32> resources = { 0, 0, 0, 0 };
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
    SMainSettings loadFromJSON(QString filename);

    qint32 addOutcome(qint32 parentId);
    void updateOutcome(qint32 id, const QList<SCheck *> &checks);
    void deleteOutcome(qint32 id, qint32 deletableId = -1, bool afterCopying = false);
    const QList<SCheck *> *getOutcomeChecks(qint32 outcomeId);
    qint32 addStage(qint32 parentId);
    void updateStage(qint32 id, const QList<SVariant *> &variants, qint32 time, QString text,
                     const QList<SReward *> rewards);
    void deleteStage(qint32 id, qint32 deletableId = -1, bool afterCopying = false);
    const SStageInfo getStageInfo(qint32 id);
    const QList<SReward *> *getStageRewards(qint32 id);
    void setStageFinal(qint32 id, bool final);
    //! Узнать является ли стейдж финальным
    bool isStageFinal(qint32 stageId);
    const QList<SVariant *> *getStageVariants(qint32 stageId);
    //! Узнать id основного родителя по id и типу сына
    qint32 getMainParentId(qint32 id, ENodeType type);
    //! Добавить доп.родителя ноду
    void addAdditionalParentToNode(qint32 id, ENodeType type, qint32 additionalParentId);
    QList<qint32> getAdditionalParentsId(qint32 id, ENodeType type);
    QList<qint32> getChildrenId(qint32 id, ENodeType type);
    //! Получить подпись хэдэра для этого нода из названия кнопки родителя
    QString getHeaderString(qint32 id, ENodeType type);

private:
    //! Пройтись по всем нодам указанного типа и вернуть минимальный незанятый id
    qint32 _makeMinId(ENodeType type);

private:
    QMap<qint32, COutcome *> m_outcomes;
    QMap<qint32, CStage *> m_stages;
};

#endif // CORDER_H
