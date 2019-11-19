#include "cdatabasemanager.h"

#include <QDebug>

CDatabaseManager::CDatabaseManager(QObject *parent) : QObject(parent)
{
    //
}

CDatabaseManager::~CDatabaseManager()
{
    db.close();
}

void CDatabaseManager::connectDatabase(QString pathDatabase)
{
    // TODO: сделать стартовую инициализацию
    if (db.isOpen())
        db.close();
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(pathDatabase);
    bool dbConnected = db.open();
    if (!dbConnected) {
        qDebug() << db.lastError().text();
    }
    emit s_databaseConnected(dbConnected);
}
