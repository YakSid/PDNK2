#ifndef CSTAGE_H
#define CSTAGE_H

#include <QtGlobal>

/** @class class CStage
 * Класс этапа квеста.
 *
 * Содержит в себе информацию описания этапа и варианты действий.
 * Хранит id исходов или ссылки на них и на родительский исход.
 */

class CStage
{
public:
    CStage(qint32 id);
    ~CStage();
    qint32 getId() const { return m_id; }
    void setParentId(qint32 parentId) { m_parentId = parentId; }
    qint32 getParentId() const { return m_parentId; }

private:
    qint32 m_id;
    qint32 m_parentId;
};

#endif // CSTAGE_H
