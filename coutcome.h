#ifndef COUTCOME_H
#define COUTCOME_H

#include <QtGlobal>
#include "coutcomewidget.h"

/** @class class COutcome
 * Класс исхода.
 *
 * Содержит информацию о необходимых навыках и шансах успешного и не успешного прохождения родительского этапа.
 * Хранит id этапов или ссылки на них и на родительский этап.
 */

//! Визуальное отображение проверки и все данные
struct SCheck {
    //Если значение -1, значит отсутствует
    qint32 type { -1 };
    qint32 trait { -1 };
    //! Значения спинбоксов, если есть
    QList<qint32> spinValues;
    //! К каким этапам ведёт кнопка. По порядку сверху вниз
    QMap<COutcomeWidget::EOutcomeButton, qint32> stagesId;
};

class COutcome
{
public:
    COutcome(qint32 id) { m_id = id; }
    ~COutcome();
    qint32 getId() const { return m_id; }
    void setParentId(qint32 parentId) { m_parentId = parentId; }
    qint32 getParentId() const { return m_parentId; }
    //! NOTE: Осторожно, константность не работает, но я этим пользуюсь при копировании
    const QList<SCheck *> *getChecks() { return &m_checks; }
    void update(const QList<SCheck *> &checks) { m_checks = checks; }

private:
    qint32 m_id;
    qint32 m_parentId;
    //! Список всех проверок в этом исходе
    QList<SCheck *> m_checks;
};

#endif // COUTCOME_H
