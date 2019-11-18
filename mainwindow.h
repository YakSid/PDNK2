#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "cdatabasemanager.h"
#include "cstartpage.h"

/// Класс основного рабочего окна.

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

private:
    Ui::MainWindow *ui;
    CDatabaseManager *m_databaseManager;

    void init();
};

#endif // MAINWINDOW_H
