#include "cdatabasemanager.h"

#include <QDebug>

CDatabaseManager::CDatabaseManager(QObject *parent) : QObject(parent)
{
    init();
}

CDatabaseManager::~CDatabaseManager()
{
    db.close();
}

void CDatabaseManager::connectDatabase()
{
    // TODO: переделать под постгрес
    db = QSqlDatabase::addDatabase("QPSQL");
    db.setDatabaseName("PDNK2DB");
    db.setHostName("host");
    db.setUserName("admin");
    db.setPassword("admin");
    db.setPort(5432);
    bool dbConnected = db.open();
    if (!dbConnected) {
        qDebug() << db.lastError().text();
    }
    emit s_databaseConnected(dbConnected);
}

void CDatabaseManager::init()
{
    connectDatabase();
}
