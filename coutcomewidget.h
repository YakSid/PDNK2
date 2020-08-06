#ifndef COUTCOMEWIDGET_H
#define COUTCOMEWIDGET_H

/** @class class COutcomeWidget
 * Класс виджета исхода для главного окна.
 *
 * Содержит информацию о необходимых навыках и шансах успешного и не успешного прохождения родительского этапа.
 * Хранит id этапов или ссылки на них и на родительский этап.
 */

#include <QWidget>

namespace Ui {
class COutcomeWidget;
}

class COutcomeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit COutcomeWidget(QWidget *parent = nullptr);
    ~COutcomeWidget();

private slots:
    void on_cb_type_currentIndexChanged(int index);
    void on_ch_success_toggled(bool checked);
    void on_ch_positive_toggled(bool checked);
    void on_ch_barely_toggled(bool checked);

private:
    Ui::COutcomeWidget *ui;
};

#endif // COUTCOMEWIDGET_H
