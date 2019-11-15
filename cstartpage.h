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

private:
    Ui::CStartPage *ui;
};

#endif // CSTARTPAGE_H
