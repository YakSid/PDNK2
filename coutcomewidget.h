#ifndef COUTCOMEWIDGET_H
#define COUTCOMEWIDGET_H

/** @class class COutcomeWidget
 * (На самом деле по логике это класс проверки, а не исхода)
 * Класс виджета исхода для главного окна.
 *
 * Хранит id этапов на них и на родительский этап.
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
    enum class EOutcomeButton { trait, success, positive, barely, failStageOrAuto };

public:
    explicit COutcomeWidget(QWidget *parent = nullptr);
    COutcomeWidget(qint32 id);
    ~COutcomeWidget();
    void setStageIdForNewButton(qint32 stageId, EOutcomeButton btn);
    qint32 getType();
    qint32 getTrait();
    QList<qint32> getSpinValues();
    QMap<EOutcomeButton, qint32> getStagesId();
    void updateData(qint32 type, qint32 trait, QList<qint32> spinValues, QMap<EOutcomeButton, qint32> stagesId);

public slots:
    void slotToStageClicked();

signals:
    //! btn - кнопка, которой назначается этап
    void s_createStageClicked(EOutcomeButton btn);
    void s_toStageClicked(qint32 id);
    //! Нужно окрасить линию к стейджам
    void s_markLineToStages(QList<qint32> stages);

protected:
    virtual void mousePressEvent(QMouseEvent *event);

private slots:
    void on_cb_type_currentIndexChanged(int index);
    void on_ch_positive_stateChanged(int arg1);
    void on_ch_barely_stateChanged(int arg1);
    void on_ch_success_toggled(bool checked);

private:
    Ui::COutcomeWidget *ui;
    //! Номер кнопки и ссылка на неё
    QMap<EOutcomeButton, QPushButton *> m_buttons;
    //! Номер кнопки и айди к которому ведёт кнопка
    QMap<EOutcomeButton, qint32> m_stages;
};

#endif // COUTCOMEWIDGET_H
