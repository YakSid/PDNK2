#include "corder.h"

COrder::COrder()
{
    //Заполнить стартовые параметры
}

COrder::~COrder() {}

void COrder::saveOrder(QString path) {}

qint32 COrder::addOutcome(qint32 parentId)
{
    auto outcome = new COutcome(_makeMinId(eOutcome));
    outcome->setParentId(parentId);
    m_outcomes.insert(outcome->getId(), outcome);
    return outcome->getId();
}

void COrder::updateOutcome(qint32 id)
{
    // TODO: обновить уже при сохранении видимо?
}

qint32 COrder::addStage(qint32 parentId)
{
    auto stage = new CStage(_makeMinId(eStage));
    stage->setParentId(parentId);
    m_stages.insert(stage->getId(), stage);
    return stage->getId();
}

void COrder::updateStage(qint32 id)
{
    //
}

qint32 COrder::getParentId(qint32 id, ENodeType type)
{
    qint32 result;
    if (type == eOutcome) {
        auto it = m_outcomes.find(id);
        result = it.value()->getParentId();
    } else {
        auto it = m_stages.find(id);
        result = it.value()->getParentId();
    }
    return result;
}

qint32 COrder::_makeMinId(ENodeType type)
{
    qint32 minId = 0;
    bool exist;
    if (type == eOutcome) {
        for (int i = 1; true; i++) {
            exist = false;
            for (auto outcome : m_outcomes) {
                if (outcome->getId() == i)
                    exist = true;
            }
            if (!exist) {
                minId = i;
                break;
            }
        }
    } else {
        for (int i = 1; true; i++) {
            exist = false;
            for (auto stage : m_stages) {
                if (stage->getId() == i)
                    exist = true;
            }
            if (!exist) {
                minId = i;
                break;
            }
        }
    }
    return minId;
}
