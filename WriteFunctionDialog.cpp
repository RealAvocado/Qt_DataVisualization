#include "WriteFunctionDialog.h"
#include "ui_WriteFunctionDialog.h"

WriteFunctionDialog::WriteFunctionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::WriteFunctionDialog)
{
    ui->setupUi(this);
}

WriteFunctionDialog::~WriteFunctionDialog()
{
    delete ui;
    delete function_list;
    delete selected_func_str;
    delete exceptionDialog;
    delete is_dialog_accepted;
}

void WriteFunctionDialog::setFunctionList(QStringList function_list)
{
    *(this->function_list)=function_list;
    this->selected_func_str->clear();
}

void WriteFunctionDialog::clearFunctionList()
{
    this->function_list->clear();
    this->selected_func_str->clear();
}

void WriteFunctionDialog::clearLineEdit()
{
    ui->lineEdit->clear();
}

void WriteFunctionDialog::setUserHint(QStringList function_list, int selected_dataset_num)
{
    ui->lineEdit->setVisible(true);
    QString num = QString::number(selected_dataset_num);
    QString hint_message = "Available functions for " + num + " datasets you selected are:\r\n";
    int func_sequence = 1;
    foreach (QString func_str, function_list) {
        hint_message.append(QString::number(func_sequence)).append(": ").append(func_str).append(", ");
        func_sequence++;
    }
    hint_message.append("\r\nPlease enter your choice below.");
    ui->label->setText(hint_message);
}

QString WriteFunctionDialog::getSelectedFunction()
{
    return *(this->selected_func_str);
}

void WriteFunctionDialog::remindUserToSelectDatasets()
{
    ui->label->setText("Please select more than one dataset first.");
    ui->lineEdit->setVisible(false);
}

void WriteFunctionDialog::closeEvent(QCloseEvent *e)
{
    *(this->is_dialog_accepted)=false;
}

bool WriteFunctionDialog::getIsDialogAccepted()
{
    return *(this->is_dialog_accepted);
}

void WriteFunctionDialog::on_buttonBox_accepted()
{
    if(!this->function_list->isEmpty()){ //if it now provides functions to users
        QString user_written_func=ui->lineEdit->text();
        if(!this->function_list->contains(user_written_func)&&!user_written_func.isEmpty()){
            this->selected_func_str->clear();
            this->exceptionDialog->setDialogMessage("Please write a correct available function!");
            this->exceptionDialog->exec();
        }else{
            *(this->selected_func_str)=user_written_func;
        }
    }
    *(this->is_dialog_accepted)=true;
}


void WriteFunctionDialog::on_buttonBox_rejected()
{
    *(this->is_dialog_accepted)=false;
}

