#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include <QCheckBox>
#include <QSpinBox>
#include <QTimer>
#include "cdatabasemanager.h"
#include "cmapmanager.h"
#include "cstartpage.h"
#include "corder.h"

/** @class class MainWindow
 * Класс основного рабочего окна.
 */

const QString PROGRAM_NAME = "ПДНК";

struct SNode {
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
    ENodeType anotherType()
    {
        if (type == eOutcome)
            return eStage;
        else {
            return eOutcome;
        }
    }
};

// TODO: релиз энд: tooltip id заменить на какой-нибудь текст?
// TODO: релиз энд: сделать предупреждение, чтобы с циклами не перебарщивали и после уменьшения кол-ва сотрудников во
// время создания, нужно проверить не остались ли пустые выборы в наградах. Предупреждение "есть ли провал у каждой из
// первых трех". В будущем добавить проверку после компиляции перед прогоном с выводом всех возможных предупреждений.
// Предупреждение: если изменили в ауткоме проверку, то нужно создать то же количество кнопок
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
    //! Проверка можно ли копировать нод
    void slotPrepareNodesCopy(qint32 copiedId, ENodeType copiedType, qint32 selectedId, ENodeType selectedType);
    //! На карте нод кликнут дважды
    void slotNodeDoubleClicked();
    // Нажат вариант или проверка, нужно окрасить линию
    void slotMarkLineToOutcome(qint32 destinationOutcomeId);
    void slotMarkLinesToStages(QList<qint32> destinationStages);

protected:
    void closeEvent(QCloseEvent *event);

private slots:
    void on_action_save_triggered();
    void on_action_saveAndExit_triggered();
    void on_action_runTest_triggered();
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
    //! Переход к табу этапов
    void on_pb_createQuest_clicked();
    void on_pb_toParentStage_clicked();
    void on_pb_toParentOutcome_clicked();
    void on_pb_addReward_clicked();
    void on_pb_deleteReward_clicked();
    void on_lw_rewards_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_pb_cancelCopy_clicked();
    void _hideWarning();
    void on_tabWidget_currentChanged(int index);

private:
    void _initOrder(bool newOrder);
    void _prepareView();
    //! Заполнить основные настройки загруженными данными
    void _prepareMainSettings(const SMainSettings &sett);
    //! Подготовить карту приказа после загрузки готового
    void _prepareMapAfterOrderLoad();
    //! Подготовить виджет исхода и заполнить данными из COrder
    void _prepareOutcomeUi(qint32 id);
    //! Подготовить виджет этапа и заполнить данными из COrder
    void _prepareStageUi(qint32 id);

    //Вывод сообщений на экран
    void _showMessage(QString text, QString title = PROGRAM_NAME);
    bool _showQuestion(QString text, QString textYes = "Да", QString textNo = "Нет", QString title = PROGRAM_NAME);

    void _changeGrpNumberStaffTitle();
    qint32 _createOutcome();
    qint32 _createStage();
    void _saveCurrentOutcome();
    void _saveCurrentStage();
    //! Сохранить данные текущего ауткома, перейти к стейджу и подготовить его ui
    void _saveOutcomeLoadStage(qint32 stageId);
    //! Сохранить данные текущего стейджа, перейти к ауткому и подготовить его ui
    void _saveStageLoadOutcome(qint32 outcomeId);
    // Изменение ui
    void _setStageUiFinal(bool st);
    void _setRewardsVisible(bool st);
    //!Добавить условие для сотрудников (Если -1, то добавить новый)
    void _addTerm(qint32 count = -1, qint32 req = -1);
    //! Рекурсивная функция для заполнения карты из загруженного приказа
    void _addLoadedNodeInMap(SNode node);
    //! Удалить из памяти объекты визуального представления ui(варианты, проверки, награды)
    void _deleteAllObjectsFromLw(QListWidget *lw);

private:
    Ui::MainWindow *ui;
    CDatabaseManager *m_databaseManager;
    CMapManager *m_mapManager;
    //! Текущий приказ
    COrder *m_order { nullptr };
    //! Текущий нод
    SNode m_currentNode;
    // Группы чекбоксов
    QList<QCheckBox *> m_hexCheckBoxes;
    QList<QCheckBox *> m_welfareCheckBoxes;
    QList<QSpinBox *> m_staffSpinBoxes;
    QList<QSpinBox *> m_resSpinBoxes;

    bool m_haveUnsavedChanges { false };
    //! Список нодов, которые после загрузки уже нарисованы на карте
    QList<SNode *> m_paintedNodes; //Нужно, чтобы не нарисовать нод дважды, если у него два родителя (после копирования)
    QTimer *m_warningTimer;

    //! Количество сотрудников в приказе
    qint32 m_staffCount { 0 };
    //! Создан ли уже квест
    bool m_questCreated { false };
};

#endif // MAINWINDOW_H
