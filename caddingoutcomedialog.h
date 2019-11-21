#ifndef CADDINGOUTCOMEDIALOG_H
#define CADDINGOUTCOMEDIALOG_H

#include <QDialog>

/** @class class CAddingOutcomeDialog
 * Класс диалога добавления исхода.
 *
 * Предлагает варианты кнопок для исхода и тип исхода.
 * Вызывается модально из MainWindow, пользователь выбирает желаемые настройки, нажимает добавить и создаётся новый
 * COutcome с отображением в MainWindow, а также CNode в CMapManager.
 */

namespace Ui {
class CAddingOutcomeDialog;
}

class CAddingOutcomeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CAddingOutcomeDialog(QWidget *parent = nullptr);
    ~CAddingOutcomeDialog();

private:
    Ui::CAddingOutcomeDialog *ui;
};

#endif // CADDINGOUTCOMEDIALOG_H
