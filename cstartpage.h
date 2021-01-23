#ifndef CSTARTPAGE_H
#define CSTARTPAGE_H

#include <QDialog>
#include <QFileDialog>

/** @class class CStartPage
 * Класс стартового окна.
 *
 * Организует будущую работу.
 * Осуществляет авторизацию и настройку базы данных.
 */

namespace Ui {
class CStartPage;
}

class CStartPage : public QDialog
{
    Q_OBJECT

public:
    explicit CStartPage(QWidget *parent = nullptr);
    ~CStartPage();
    void init();

    qint8 closeMode { 0 };
    QString jFilename { "" };

public slots:
    void databaseConnected(bool connected);

private slots:
    void on_pb_create_new_order_clicked();

    void on_pb_edit_order_clicked();

private:
    Ui::CStartPage *ui;

    // WARNING: Указывать здесь автора перед сборкой
    QString m_writer { "DEV" };
};

#endif // CSTARTPAGE_H
