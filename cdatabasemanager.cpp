#include "cdatabasemanager.h"

#include <QDebug>
#include <QMessageBox>

CDatabaseManager::CDatabaseManager(QObject *parent) : QObject(parent)
{
    //
}

CDatabaseManager::~CDatabaseManager()
{
    db.close();
}

void CDatabaseManager::connectDatabase()
{
    // NOTE: отключил пока не пользуемся бд
    /*if (!QSqlDatabase::isDriverAvailable("QPSQL")) {
        QMessageBox msg;
        msg.setText("Драйвер PostgreSQL не установлен");
        msg.setWindowTitle("Ошибка");
        msg.exec();
    }

    db = QSqlDatabase::addDatabase("QPSQL");

    db.setDatabaseName("PDNK2DB");
    db.setUserName("postgres");
    db.setPassword("admin");
    db.setHostName("localhost");
    db.setPort(5432);
    bool dbConnected = db.open();
    if (!dbConnected) {
        QSqlError error = db.lastError();
        QString baseText = error.databaseText();
        QString driverText = error.driverText();
        QMessageBox msg;
        msg.setText("При подключении к базе данных произошла ошибка: " + baseText
                    + " Сообщение драйвера: " + driverText);
        msg.setWindowTitle("Ошибка");
        msg.exec();
    }

    emit s_databaseConnected(dbConnected);*/
}
