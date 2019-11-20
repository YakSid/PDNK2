#ifndef CSTARTPAGE_H
#define CSTARTPAGE_H

#include <QDialog>

/// Класс стартового окна. Организует будущую работу, осуществляет авторизацию и настройку базы данных.

namespace Ui {
class CStartPage;
}

class CStartPage : public QDialog
{
    Q_OBJECT

public:
    explicit CStartPage(QWidget *parent = nullptr);
    ~CStartPage();

    qint8 closeMode { 0 };

public slots:
    void databaseConnected(bool connected);

signals:
    void s_connectDatabase();

private slots:
    void on_pb_connectDatabase_clicked();

    void on_pb_create_new_order_clicked();

    void on_pb_edit_order_clicked();

private:
    Ui::CStartPage *ui;
};

#endif // CSTARTPAGE_H
