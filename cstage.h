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
    void setMainParentId(qint32 mainParentId) { m_mainParentId = mainParentId; }
    qint32 getMainParentId() const { return m_mainParentId; }
    void addAdditionalParent(qint32 additionaParentId) { m_additionalParentsId.append(additionaParentId); }
    QList<qint32> getAdditionalParentsId() const { return m_additionalParentsId; }
    void removeAdditionalParent(qint32 id) { m_additionalParentsId.removeOne(id); }
    //! NOTE: Осторожно, константность не работает, но я этим пользуюсь при копировании и удалении
    const QList<SVariant *> *getVariants() { return &m_variants; }
    void setVariants(const QList<SVariant *> &variants) { m_variants = variants; }
    void updateInfo(const QList<SVariant *> &variants, qint32 time, QString text, const QList<SReward *> rewards);
    void setTime(qint32 time) { m_time = time; }
    qint32 getTime() const { return m_time; }
    void setText(QString text) { m_text = text; }
    QString getText() const { return m_text; }
    //! Осторожно, константность не работает
    const SStageInfo getStageInfo();
    //! Осторожно, константность не работает
    const QList<SReward *> *getRewards() { return &m_rewards; }
    void setRewards(const QList<SReward *> &rewards) { m_rewards = rewards; }
    void setFinal(bool final) { m_final = final; }
    bool isFinal() const { return m_final; }
    //! Найти какой вариант указывает на этот нод и взять его текст
    QString getTextFromVariantOutcome(qint32 outcomeId);

private:
    qint32 m_id;
    qint32 m_mainParentId;
    QList<qint32> m_additionalParentsId;
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
