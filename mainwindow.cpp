#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDebug>

// TODO: назвать все ui элементы
// TODO: Окна продумать: требуются ресурсы, требуются сотрудники

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    auto m_startPage = new CStartPage();
    m_databaseManager = new CDatabaseManager();
    connect(m_startPage, &CStartPage::s_connectDatabase, m_databaseManager, &CDatabaseManager::connectDatabase);
    connect(m_databaseManager, &CDatabaseManager::s_databaseConnected, m_startPage, &CStartPage::databaseConnected);
    m_startPage->init();
    m_startPage->setModal(true);
    m_startPage->exec();
    switch (m_startPage->closeMode) {
    case 0:
        exit(0);
    case 1:
        // Создать новый приказ
        // Подготовить главное меню
        init(); // Пока ничего не делает
        break;
    case 2:
        // Редактировать старый приказ
        // запустить COrdersPage
        break;
    }
    ui->setupUi(this);
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

void MainWindow::updateWindow()
{
    //
    // TODO: показывать условия приказа соответствующие типу вн или птр
}

void MainWindow::init()
{
    //
}
