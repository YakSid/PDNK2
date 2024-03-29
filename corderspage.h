#ifndef CORDERSPAGE_H
#define CORDERSPAGE_H

#include <QDialog>

/** @class class COrdersPage
 * Класс окна выбора приказов.
 *
 * Показывает таблицу всех созданных и доступных приказов для редактирования или экспортирования.
 * Отображает краткую информацию о приказах.
 */

namespace Ui {
class COrdersPage;
}

class COrdersPage : public QDialog
{
    Q_OBJECT

public:
    explicit COrdersPage(QWidget *parent = nullptr);
    ~COrdersPage();

private:
    Ui::COrdersPage *ui;
};

#endif // CORDERSPAGE_H
