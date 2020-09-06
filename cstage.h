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

//! Визуальное отображение варианта и все данные
struct SVariant {
    QString text;
    qint32 outcomeId { -1 };
    qint32 resource { -1 };
    qint32 resourceCount { -1 };
};
// TODO: СЕЙЧАС забыл сохранять текст стейджа, возможно награду и статус финальный ли (ui элементы вне вариантов и
// проверок чекнуть)
class CStage
{
public:
    CStage(qint32 id) { m_id = id; }
    ~CStage();
    qint32 getId() const { return m_id; }
    void setParentId(qint32 parentId) { m_parentId = parentId; }
    qint32 getParentId() const { return m_parentId; }
    const QList<SVariant *> *getVariants() { return &m_variants; }
    void update(const QList<SVariant *> &variants) { m_variants = variants; }

private:
    qint32 m_id;
    qint32 m_parentId;
    //! Список всех вариантов в этом этапе
    QList<SVariant *> m_variants;
};

#endif // CSTAGE_H
