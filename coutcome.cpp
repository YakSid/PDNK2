#include "coutcome.h"

COutcome::~COutcome()
{
    qDeleteAll(m_checks);
    m_checks.clear();
}
