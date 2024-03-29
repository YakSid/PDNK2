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
    explicit CRewardWidget(qint32 staffCount, QWidget *parent = nullptr);
    ~CRewardWidget();

    qint32 getType();
    qint32 getObject();
    qint32 getCount();
    qint32 getPsyState();
    QString getNote();
    void updateData(qint32 type, qint32 object, qint32 count, qint32 psyState, QString note);

private slots:
    void on_cb_rewardType_currentIndexChanged(int index);

    void on_cb_object_currentIndexChanged(const QString &arg1);

private:
    Ui::CRewardWidget *ui;
    qint32 m_staffCount;
};

#endif // CREWARDWIDGET_H
