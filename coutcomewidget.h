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
    COutcomeWidget(qint32 id);
    ~COutcomeWidget();
    void setId(qint32 id) { m_id = id; }
    qint32 getId() const { return m_id; }

private slots:
    void on_cb_type_currentIndexChanged(int index);
    void on_ch_success_toggled(bool checked);
    void on_ch_positive_toggled(bool checked);
    void on_ch_barely_toggled(bool checked);

private:
    Ui::COutcomeWidget *ui;
    qint32 m_id;
};

#endif // COUTCOMEWIDGET_H
