#include "coutcome.h"
#include "cconstants.h"

COutcome::~COutcome()
{
    qDeleteAll(m_checks);
    m_checks.clear();
}

QString COutcome::getTextFromCheckStage(qint32 stageId)
{
    QString result;
    for (auto check : m_checks) {
        //Если у проверки только одна кнопка
        for (auto it = check->stagesId.begin(); it != check->stagesId.end(); it++) {
            if (it.value() == stageId) {
                result = CHECK_TYPES.at(check->type);
                switch (it.key()) {
                case COutcomeWidget::EOutcomeButton::success:
                    result += " \"Успех\"";
                    break;
                case COutcomeWidget::EOutcomeButton::positive:
                    result += " \"Положительно\"";
                    break;
                case COutcomeWidget::EOutcomeButton::barely:
                    result += " \"Кое-как\"";
                    break;
                case COutcomeWidget::EOutcomeButton::trait:
                    result += " \"" + TRAITS.at(check->trait) + "\"";
                    break;
                case COutcomeWidget::EOutcomeButton::failStageOrAuto:
                    break;
                }
                break;
            }
        }
    }
    return result;
}
