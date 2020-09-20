#ifndef CSTAGE_H
#define CSTAGE_H

#include <QtGlobal>
#include "cvariantwidget.h"

/** @class class CStage
 * Класс этапа квеста.
 *
 * Содержит в себе информацию описания этапа и варианты действий.
 * Хранит id исходов или ссылки на них и на родительский исход.
 */

//! Визуальное отображение варианта и его данные
struct SVariant {
    QString text;
    qint32 outcomeId { -1 };
    qint32 resource { -1 };
    qint32 resourceCount { -1 };
};
//! Награда за достижение этапа
struct SReward {
    qint32 type { -1 };
    qint32 object { -1 };
    qint32 count { -1 };
    qint32 psyState { -1 };
};
//! Информация этапа
struct SStageInfo {
    qint32 time { -1 };
    QString text;
    bool isFinal { false };
};

class CStage
{
public:
    CStage(qint32 id) { m_id = id; }
    ~CStage();
    qint32 getId() const { return m_id; }
    void setParentId(qint32 parentId) { m_parentId = parentId; }
    qint32 getParentId() const { return m_parentId; }
    const QList<SVariant *> *getVariants() { return &m_variants; }
    void setVariants(const QList<SVariant *> &variants) { m_variants = variants; }
    void updateInfo(const QList<SVariant *> &variants, qint32 time, QString text, const QList<SReward *> rewards);
    void setTime(qint32 time) { m_time = time; }
    qint32 getTime() const { return m_time; }
    void setText(QString text) { m_text = text; }
    QString getText() const { return m_text; }
    const SStageInfo getStageInfo();
    const QList<SReward *> *getRewards() { return &m_rewards; }
    void setRewards(const QList<SReward *> &rewards) { m_rewards = rewards; }
    void setFinal(bool final) { m_final = final; }
    bool isFinal() const { return m_final; }

private:
    qint32 m_id;
    qint32 m_parentId;
    //Данные этапа
    qint32 m_time;
    QString m_text;
    bool m_final { false };
    //! Список всех наград в этом этапе
    QList<SReward *> m_rewards;
    //! Список всех вариантов в этом этапе
    QList<SVariant *> m_variants;
};

#endif // CSTAGE_H
