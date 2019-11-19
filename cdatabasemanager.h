#ifndef CDATABASEMANAGER_H
#define CDATABASEMANAGER_H

#include <QObject>
#include <QtSql>
#include <QSqlQuery>

/// Класс базы данных. Управляет подключением к базе данных, обновлением базы данных, импортом и экспортом внешних
/// файлов.
/// Управляет записью приказов в базу данных и получением приказов из базы данных.

class CDatabaseManager : public QObject
{
    Q_OBJECT
public:
    explicit CDatabaseManager(QObject *parent = nullptr);
    ~CDatabaseManager();

    QSqlDatabase db;

signals:
    void s_databaseConnected(bool connected);

public slots:
    void connectDatabase(QString pathDatabase);
};

#endif // CDATABASEMANAGER_H
