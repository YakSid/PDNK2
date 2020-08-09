#include "corder.h"

COrder::COrder()
{
    //Заполнить стартовые параметры
}

COrder::~COrder() {}

void COrder::saveOrder(QString path) {}

qint32 COrder::addOutcome()
{
    auto outcome = new COutcome(_makeMinId(eOutcome));
    m_outcomes.insert(outcome->getId(), outcome);
    return outcome->getId();
}

qint32 COrder::addStage()
{
    auto stage = new CStage(_makeMinId(eStage));
    m_stages.insert(stage->getId(), stage);
    return stage->getId();
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
