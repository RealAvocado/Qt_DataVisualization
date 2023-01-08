#include "ExceptionDialog.h"
#include "ui_ExceptionDialog.h"

ExceptionDialog::ExceptionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExceptionDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/error.png"));
    this->setSizeGripEnabled(true);
}

ExceptionDialog::~ExceptionDialog()
{
    delete ui;
}

void ExceptionDialog::setDialogMessage(QString content)
{
    ui->textBrowser->setText(content);
}
