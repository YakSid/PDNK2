#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    auto m_startPage = new CStartPage();
    auto m_databaseManager = new CDatabaseManager();
    connect(m_startPage, &CStartPage::s_connectDatabase, m_databaseManager, &CDatabaseManager::connectDatabase);
    connect(m_databaseManager, &CDatabaseManager::s_databaseСonnected, m_startPage, &CStartPage::databaseConnected);
    // запустить стартПейдж
    ui->setupUi(this);
    init(); // Пока ничего не делает
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_action_save_triggered()
{
    // Сохранить всё в базу
}

void MainWindow::on_action_saveAndExit_triggered()
{
    on_action_save_triggered();
    // и выйти
}

void MainWindow::init()
{
    //
}
