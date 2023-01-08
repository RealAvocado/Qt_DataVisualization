#ifndef WRITEFUNCTIONDIALOG_H
#define WRITEFUNCTIONDIALOG_H

#include <QDialog>
#include <QMessageBox>

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
    void reset();
    void setUserHint(QStringList function_list, int selected_dataset_num);
    QString getSelectedFunction();
    void closeEvent(QCloseEvent *e);
    bool getIsDialogAccepted();

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();

private:
    Ui::WriteFunctionDialog *ui;
    QStringList* function_list = new QStringList;
    QString* selected_func_str = new QString;
    bool *is_dialog_accepted = new bool;
};

#endif // WRITEFUNCTIONDIALOG_H
