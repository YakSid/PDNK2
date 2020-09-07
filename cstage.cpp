#include "cstage.h"

void CStage::updateInfo(const QList<SVariant *> &variants, qint32 time, QString text, const QList<SReward *> rewards)
{
    m_variants = variants;
    m_time = time;
    m_text = text;
    m_rewards = rewards;
}
