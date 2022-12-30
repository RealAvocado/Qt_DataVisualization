#ifndef WRITEFUNCTIONDIALOG_H
#define WRITEFUNCTIONDIALOG_H

#include <QDialog>
#include <ExceptionDialog.h>

namespace Ui {
class WriteFunctionDialog;
}

class WriteFunctionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WriteFunctionDialog(QWidget *parent = nullptr);
    ~WriteFunctionDialog();

    void setFunctionList(QStringList function_list);
    void clearFunctionList();
    void clearLineEdit();
    void setUserHint(QStringList function_list, int selected_dataset_num);
    QString getSelectedFunction();
    void remindUserToSelectDatasets();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::WriteFunctionDialog *ui;
    QStringList* function_list = new QStringList;
    QString* selected_func_str = new QString;
    ExceptionDialog *exceptionDialog = new ExceptionDialog();
};

#endif // WRITEFUNCTIONDIALOG_H
