#ifndef DATASETSELECTDIALOG_H
#define DATASETSELECTDIALOG_H

#include <QDialog>
#include <QListWidgetItem>

namespace Ui {
class DatasetSelectDialog;
}
class DatasetSelectDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DatasetSelectDialog(QWidget *parent = nullptr);
    ~DatasetSelectDialog();

    void setFilesInList(QStringList file_name_list); //display loaded files
    void clearFilesInList();
    void clearSelection();
    QList<QListWidgetItem*>* getSelected_datasets_list(); //get selected items by users
    bool getIsDialogAccepted(); //get the last status of the dialog, is accepted or rejected
    void closeEvent(QCloseEvent *e);

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::DatasetSelectDialog *ui;
    QList<QListWidgetItem*> *selected_datasets_list = new QList<QListWidgetItem*>;
    bool *is_dialog_accepted = new bool;
};

#endif // DATASETSELECTDIALOG_H
