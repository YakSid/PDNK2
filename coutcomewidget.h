#ifndef COUTCOMEWIDGET_H
#define COUTCOMEWIDGET_H

/** @class class COutcomeWidget
 * (На самом деле по логике это класс проверки, а не исхода)
 * Класс виджета исхода для главного окна.
 *
 * Содержит информацию о необходимых навыках и шансах успешного и не успешного прохождения родительского этапа.
 * Хранит id этапов или ссылки на них и на родительский этап.
 */

#include <QWidget>
#include <QPushButton>
#include <QMap>

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
    void setStageIdForNewButton(qint32 id);
    qint32 getType();
    qint32 getTrait();
    QList<qint32> getSpinValues();
    QList<qint32> getStagesId();
    void updateData(qint32 type, qint32 trait, QList<qint32> spinValues, QList<qint32> stagesId);

public slots:
    void on_checkBoxToStageClicked();

signals:
    void s_createStageClicked();
    void s_toStageClicked(qint32 id);

private slots:
    void on_cb_type_currentIndexChanged(int index);
    void on_ch_success_toggled(bool checked);
    void on_ch_positive_toggled(bool checked);
    void on_ch_barely_toggled(bool checked);

private:
    Ui::COutcomeWidget *ui;
    //! Мап с указанием к какому этапу ведёт какая кнопка (-1 только у только что добавленных кнопок)
    QMap<qint32, QPushButton *> m_stageIdButton;
    //! Лист всех кнопок на виджете (все "к этапу...")
    QList<QPushButton *> m_buttons;
};

#endif // COUTCOMEWIDGET_H
