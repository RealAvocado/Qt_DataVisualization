#include "ExceptionDialog.h"
#include "ui_ExceptionDialog.h"

ExceptionDialog::ExceptionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExceptionDialog)
{
    ui->setupUi(this);
}

void ExceptionDialog::setDialogMessage(QString content)
{
    ui->textBrowser->setText(content);
}

ExceptionDialog::~ExceptionDialog()
{
    delete ui;
}
