#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QCheckBox>
#include <QSpinBox>
#include "cdatabasemanager.h"
#include "cmapmanager.h"
#include "cstartpage.h"
#include "corder.h"

/** @class class MainWindow
 * Класс основного рабочего окна.
 */

struct SCurrentNode {
    qint32 id;
    ENodeType type;

    void update(qint32 newId, ENodeType newType)
    {
        id = newId;
        type = newType;
    }
    bool isOutcome()
    {
        if (type == eOutcome)
            return true;
        else {
            return false;
        }
    }
};

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
    void slotCreateStageClicked(COutcomeWidget::EOutcomeButton btn);
    //! Нажата кнопка "К исходам..." на виджете варианта
    void slotToOutcomeClicked(qint32 id);
    void slotCreateOutcomeClicked();

private slots:
    void on_action_save_triggered();
    void on_action_saveAndExit_triggered();
    void updateWindow();
    void on_cb_type_currentIndexChanged(int index);
    void on_spb_first_rank_valueChanged(int arg1);
    void on_spb_second_rank_valueChanged(int arg1);
    void on_spb_third_rank_valueChanged(int arg1);
    void on_spb_common_valueChanged(int arg1);
    void on_grp_req_resources_toggled(bool arg1);
    void on_cb_department_currentIndexChanged(int index);
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
    void _prepareAllFromLoadedOrder();
    //! Подготовить виджет исхода и заполнить данными из COrder
    void _prepareOutcomeUi(qint32 id);
    //! Подготовить виджет этапа и заполнить данными из COrder
    void _prepareStageUi(qint32 id);

    void _changeGrpNumberStaffTitle();
    qint32 _createOutcome();
    qint32 _createStage();
    // TODO: СЕЙЧАС автоматическое добавление проверки?, копирование нода, удаление, сделать подсветку линий или детей
    // выбранного.
    void _saveCurrentOutcome();
    void _saveCurrentStage();
    //! Сохранить данные текущего ауткома, перейти к стейджу и подготовить его ui
    void _saveOutcomeLoadStage(qint32 stageId);
    //! Сохранить данные текущего стейджа, перейти к ауткому и подготовить его ui
    void _saveStageLoadOutcome(qint32 outcomeId);
    void _setStageUiFinal(bool st);

private:
    Ui::MainWindow *ui;
    CDatabaseManager *m_databaseManager;
    CMapManager *m_mapManager;
    //! Текущий приказ
    COrder *m_order;
    //! Текущий нод
    SCurrentNode m_currentNode;
    // Группы чекбоксов
    QList<QCheckBox *> m_hexCheckBoxes;
    QList<QCheckBox *> m_welfareCheckBoxes;
    QList<QSpinBox *> m_staffSpinBoxes;
    QList<QSpinBox *> m_resSpinBoxes;
};

#endif // MAINWINDOW_H
