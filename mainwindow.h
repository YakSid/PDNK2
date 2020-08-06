#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QListWidgetItem>
#include "cdatabasemanager.h"
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
    void on_pb_addOutcome_clicked();
    void on_pb_deleteOutcome_clicked();
    void on_lw_outcomes_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous);
    void on_pb_addTerm_clicked();
    void on_pb_deleteTerm_clicked();
    void on_stackedWidget_currentChanged(int arg1);
    void on_pb_addVariant_clicked();
    void on_pb_deleteVariant_clicked();
    void on_pb_setFinal_clicked();
    void on_pb_showRewardGroup_clicked();

private:
    void _prepareView();
    void _changeGrpNumberStaffTitle();
    void _createOutcome();
    void _deleteOutcome(QListWidgetItem *item);

private:
    Ui::MainWindow *ui;
    CDatabaseManager *m_databaseManager;
};

#endif // MAINWINDOW_H
