#ifndef COUTCOME_H
#define COUTCOME_H

#include <QtGlobal>

/** @class class COutcome
 * Класс исхода.
 *
 * Содержит информацию о необходимых навыках и шансах успешного и не успешного прохождения родительского этапа.
 * Хранит id этапов или ссылки на них и на родительский этап.
 */

class COutcome
{
public:
    COutcome(qint32 id);
    ~COutcome();
    qint32 getId() const { return m_id; }
    void setParentId(qint32 parentId) { m_parentId = parentId; }
    qint32 getParentId() const { return m_parentId; }

private:
    qint32 m_id;
    qint32 m_parentId;
};

#endif // COUTCOME_H
