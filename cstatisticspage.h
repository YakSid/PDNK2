#ifndef CSTATISTICSPAGE_H
#define CSTATISTICSPAGE_H

#include <QDialog>

/** @class class CStatisticsPage
 * Класс окна статистики.
 *
 * Отображает статистику по приказу.
 */

namespace Ui {
class CStatisticsPage;
}

class CStatisticsPage : public QDialog
{
    Q_OBJECT

public:
    explicit CStatisticsPage(QWidget *parent = nullptr);
    ~CStatisticsPage();

private:
    Ui::CStatisticsPage *ui;
};

#endif // CSTATISTICSPAGE_H
