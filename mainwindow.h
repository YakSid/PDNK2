#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "cdatabasemanager.h"
#include "cmapmanager.h"
#include "cstartpage.h"
#include "corder.h"

/** @class class MainWindow
 * Класс основного рабочего окна.
 */

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void needReqResToggled(bool checked);

public slots:
    //! На карте выбран другой нод
    void slotNewNodeSelected(qint32 id, ENodeType type);
    //! Нажата кнопка "К этапу..." на coutomewidget
    void slotToStageClicked(qint32 id);
    void slotCreateStageClicked();

private slots:
    void on_action_save_triggered();
    void on_action_saveAndExit_triggered();
    void updateWindow();
    void on_cmb_type_currentIndexChanged(int index);
    void on_spb_first_rank_valueChanged(int arg1);
    void on_spb_second_rank_valueChanged(int arg1);
    void on_spb_third_rank_valueChanged(int arg1);
    void on_spb_common_valueChanged(int arg1);
    void on_grp_req_resources_toggled(bool arg1);
    void on_cmb_department_currentIndexChanged(int index);
    void on_pb_addCheck_clicked();
    void on_pb_deleteCheck_clicked();
    void on_lw_outcomes_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_pb_addTerm_clicked();
    void on_pb_deleteTerm_clicked();
    void on_stackedWidget_currentChanged(int arg1);
    void on_pb_addVariant_clicked();
    void on_pb_deleteVariant_clicked();
    void on_pb_setFinal_clicked();
    void on_pb_showRewardGroup_clicked();
    void on_pb_createQuest_clicked();
    void on_pb_toParentStage_clicked();
    void on_pb_toParentOutcome_clicked();
    void on_pb_addReward_clicked();
    void on_pb_deleteReward_clicked();
    void on_lw_rewards_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);

private:
    void _prepareView();
    //! Подготовить виджет исхода для создания первого
    void _prepareFirstOutcomeUi();
    //! Подготовить виджет исхода для создания нового
    void _prepareOutcomeUi();
    //! Подготовить виджет этапа для создания нового
    void _prepateStageUi();
    //! Подготовить виджет исхода и заполнить данными из COrder
    void _prepareOutcomeUi(qint32 id);
    //! Подготовить виджет этапа и заполнить данными из COrder
    void _prepateStageUi(qint32 id);

    void _changeGrpNumberStaffTitle();
    qint32 _createOutcome();
    qint32 _createStage();
    // TODO: СЕЙЧАС для исходов привязать добавление к кнопкам "К исходу..." на варианте в стейдже. Потом сделать
    // сохранение данных в аутком и стейдж, алгоритм открытия чекбоксов на
    //      сауткомвиджете, наладить алгоритмы размещения/рисовки, рисовку линий, перемещение по кнопке "к родителю",
    //      удаление

private:
    Ui::MainWindow *ui;
    CDatabaseManager *m_databaseManager;
    CMapManager *m_mapManager;
    //! Текущий приказ
    COrder *m_order;
};

#endif // MAINWINDOW_H
