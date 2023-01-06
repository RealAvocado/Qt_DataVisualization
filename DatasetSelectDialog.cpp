#include "DatasetSelectDialog.h"
#include "ui_DatasetSelectDialog.h"

DatasetSelectDialog::DatasetSelectDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DatasetSelectDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/plot.png"));
    this->setSizeGripEnabled(true);
    ui->listWidget->setSelectionMode(QAbstractItemView::MultiSelection);
}

DatasetSelectDialog::~DatasetSelectDialog()
{
    delete ui;
    delete selected_datasets_list;
    delete is_dialog_accepted;
}

void DatasetSelectDialog::setFilesInList(QStringList file_name_list)
{
    ui->listWidget->clear();
    ui->listWidget->addItems(file_name_list);
}

void DatasetSelectDialog::clearFilesInList()
{
    ui->listWidget->clear();
}

void DatasetSelectDialog::clearSelection()
{
    ui->listWidget->clearSelection();
}

QList<QListWidgetItem*>* DatasetSelectDialog::getSelected_datasets_list()
{
    return this->selected_datasets_list;
}

bool DatasetSelectDialog::getIsDialogAccepted()
{
    return *(this->is_dialog_accepted);
}

void DatasetSelectDialog::on_buttonBox_accepted()
{
    *(this->is_dialog_accepted)=true;
    *(this->selected_datasets_list)=ui->listWidget->selectedItems();
    ui->listWidget->clearSelection();
}


void DatasetSelectDialog::on_buttonBox_rejected()
{   
    *(this->is_dialog_accepted)=false;
    ui->listWidget->clearSelection();
}

void DatasetSelectDialog::closeEvent(QCloseEvent *e)
{
    ui->listWidget->clearSelection();
    *(this->is_dialog_accepted)=false;
}
