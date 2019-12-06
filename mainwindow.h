#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cdatabasemanager.h"
#include "cstartpage.h"

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

private:
    Ui::MainWindow *ui;
    CDatabaseManager *m_databaseManager;

    void _prepareView();

    void _changeGrpNumberStaffTitle();
};

#endif // MAINWINDOW_H
