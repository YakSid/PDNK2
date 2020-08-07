#ifndef CREWARDWIDGET_H
#define CREWARDWIDGET_H

/** @class class CRewardWidget
 * Класс виджета награды.
 *
 * Отображает, какой результат и/или награду получает игрок выбрав предыдущий этап и пройдя отбор исходов.
 * К примеру: игрок выбрал на каком-либо этапе вариант "Атаковать", в исходах физическая проверка не была пройдена -
 * в результате на этапе отобразится виджет награды(результата): "один человек ранен".
 */

#include <QWidget>

namespace Ui {
class CRewardWidget;
}

class CRewardWidget : public QWidget
{
    Q_OBJECT

public:
    explicit CRewardWidget(QWidget *parent = nullptr);
    ~CRewardWidget();

private slots:
    void on_cb_rewardType_currentIndexChanged(int index);

private:
    Ui::CRewardWidget *ui;
};

#endif // CREWARDWIDGET_H
