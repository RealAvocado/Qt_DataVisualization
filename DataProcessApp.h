#ifndef DATAPROCESSAPP_H
#define DATAPROCESSAPP_H

#include <QFile>
#include <QFileDialog>
#include <QTextStream>
#include <QMainWindow>
#include <QMenu>
#include <QVector>
#include <QDebug>
#include <QListWidgetItem>
#include <QMap>
#include <QRegularExpression>
#include <QPen>
#include <qcustomplot.h>

#include <math.h>
#include "point.h"
#include "DatasetSelectDialog.h"
#include "ExceptionDialog.h"
#include "WriteFunctionDialog.h"

QT_BEGIN_NAMESPACE
namespace Ui { class DataProcessApp; }
QT_END_NAMESPACE

class DataProcessApp : public QMainWindow
{
    Q_OBJECT

public:
    DataProcessApp(QWidget *parent = nullptr);
    ~DataProcessApp();

    bool isFileFormatValid(QStringList &file_content_list);

    bool isEveryXValueUnique(QVector<Point> &point_vec);

    bool areDomainsIdentical(QVector<QVector<double> > &dataset_domains_vec);

    void setSelected_datasets_list(QList<QListWidgetItem*> *list_widget);

    void addGraphFromFunction(QVector<QVector<double> > &dataset_domains_vec, QVector<QVector<double> > &dataset_y_values_vec);

    void runFunctionDialog();

    void runColorDialog();

    void constructContextMenu();

private slots:
    // slots used to load and plot graphs

    void on_actionLoad_Datasets_triggered();

    void on_actionSelect_Datasets_to_Plot_triggered();

    void on_actionWrite_Function_for_Plots_triggered();

    // slots used to change colors

    void graphDoubleClicked(QCPAbstractPlottable*);

    void legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item);

    void plotContextMenuRequest(QPoint pos);

    void on_actionChange_Graph_Color_triggered();

    void zoomGraphByWheel();

    void dragScreenByPress();

private:
    Ui::DataProcessApp *ui;

    QStringList *file_name_list = new QStringList;
    QMap<QString, QVector<Point>> *fileName_dataset_map = new QMap<QString, QVector<Point>>;

    DatasetSelectDialog *selectDialog = new DatasetSelectDialog();
    QStringList *selected_datasets_list = new QStringList;
    QVector<QVector<double>>* dataset_domains_vec_attr = new QVector<QVector<double>>;
    QVector<QVector<double>>* dataset_y_values_vec_attr = new QVector<QVector<double>>;

    QStringList *invalid_file_name_list = new QStringList;
    ExceptionDialog *exceptionDialog = new ExceptionDialog();
    WriteFunctionDialog *functionDialog = new WriteFunctionDialog();

    QMenu *contextMenu=new QMenu();

};
#endif // DATAPROCESSAPP_H
