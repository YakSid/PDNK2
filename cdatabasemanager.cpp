#include "cdatabasemanager.h"

#include <QDebug>

CDatabaseManager::CDatabaseManager(QObject *parent) : QObject(parent)
{
    //
}

CDatabaseManager::~CDatabaseManager()
{
    //
}

void CDatabaseManager::connectDatabase(QString pathDatabase)
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(pathDatabase);
    bool dbConnected = db.open();
    if (!dbConnected) {
        qDebug() << db.lastError().text();
    }
    emit s_databaseСonnected(dbConnected);
}
