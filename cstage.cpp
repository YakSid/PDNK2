#include "cstage.h"

CStage::~CStage()
{
    qDeleteAll(m_rewards);
    m_rewards.clear();
    qDeleteAll(m_variants);
    m_variants.clear();
}

void CStage::updateInfo(const QList<SVariant *> &variants, qint32 time, QString text, const QList<SReward *> rewards)
{
    m_variants = variants;
    m_time = time;
    m_text = text;
    m_rewards = rewards;
}

const SStageInfo CStage::getStageInfo()
{
    SStageInfo result;
    result.text = m_text;
    result.time = m_time;
    result.isFinal = m_final;
    return result;
}
