#include "ExceptionDialog.h"
#include "ui_ExceptionDialog.h"

ExceptionDialog::ExceptionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExceptionDialog)
{
    ui->setupUi(this);
    this->setSizeGripEnabled(true);
    this->setWindowIcon(QIcon(":/error.png"));
    update();
}

ExceptionDialog::~ExceptionDialog()
{
    delete ui;
}

void ExceptionDialog::setDialogMessage(QString content)
{
    ui->textBrowser->setText(content);
}
