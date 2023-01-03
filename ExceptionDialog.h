#ifndef EXCEPTIONDIALOG_H
#define EXCEPTIONDIALOG_H

#include <QDialog>
#include <QPixmap>
#include <QPainter>
#include <QPaintEvent>

namespace Ui {
class ExceptionDialog;
}

class ExceptionDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExceptionDialog(QWidget *parent = nullptr);
    ~ExceptionDialog();

    void setDialogMessage(QString content);

private:
    Ui::ExceptionDialog *ui;
};

#endif // EXCEPTIONDIALOG_H
