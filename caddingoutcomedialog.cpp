#include "caddingoutcomedialog.h"
#include "ui_caddingoutcomedialog.h"

CAddingOutcomeDialog::CAddingOutcomeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CAddingOutcomeDialog)
{
    ui->setupUi(this);
}

CAddingOutcomeDialog::~CAddingOutcomeDialog()
{
    delete ui;
}
