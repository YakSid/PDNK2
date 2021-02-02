#ifndef CTESTRUN_H
#define CTESTRUN_H

#include <QDialog>
#include "corder.h"

/** @class class CStatisticsPage
 * Класс тестового прогона.
 *
 * Позволяет прогнать приказ от лица пользователя, класс отображает в гуи варианты и в зависимости от выборов
 * перемещается по этапам. В управлении разработчика можно выбирать системные состяния приказа, типа какие сотрудники до
 * этого были выбраны и проходить этапы независимо от проверок.
 */

namespace Ui {
class CTestRun;
}

class CTestRun : public QDialog
{
    Q_OBJECT

public:
    explicit CTestRun(const COrder *order, QWidget *parent = nullptr);
    ~CTestRun();

    void startFromBegining();
    void startFromStage(qint32 stageId);

private:
    void _showStage(qint32 id);

private:
    Ui::CTestRun *ui;
    const COrder *m_orderPtr;
};

#endif // CTESTRUN_H
