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

    void addGraphFromFunction(QVector<QVector<double> > &dataset_key_domains_vec, QVector<QVector<double> > &dataset_value_domains_vec);

    void runFunctionDialog();

    void runColorDialog();

    void constructContextMenu();

    void resetFiles();

private slots:
    // slots used to load, plot and delete graphs and datasets

    void on_actionLoad_Datasets_triggered();

    void on_actionSelect_Datasets_to_Plot_triggered();

    void on_actionWrite_Function_for_Plots_triggered();

    void on_actionClear_Datasets_and_Graphs_triggered();

    void on_actionRemove_the_Function_Graph_triggered();

    void on_actionClean_the_Screen_triggered();

    // slots used to change colors

    void graphDoubleClicked(QCPAbstractPlottable*);

    void legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item);

    void plotContextMenuRequest(QPoint pos);

    void on_actionChange_Graph_Color_triggered();

    void zoomGraphByWheel();

    void dragScreenByPress();

private:
    Ui::DataProcessApp *ui;

    QStringList *file_name_list = new QStringList; //store the file name of loaded datasets
    QMap<QString, QVector<Point>> *fileName_dataset_map = new QMap<QString, QVector<Point>>; //link the file name to datas one to one

    DatasetSelectDialog *selectDialog = new DatasetSelectDialog(); //the dialog used to select loaded datasets
    QStringList *selected_datasets_list = new QStringList; //record datasets selected by the user
    QVector<QVector<double>>* selected_dataset_key_domains_vec = new QVector<QVector<double>>; //store the x domain value of selected datasets
    QVector<QVector<double>>* selected_dataset_value_domains_vec = new QVector<QVector<double>>; //store the y value of selected datasets

    QStringList *invalid_file_name_list = new QStringList; //record datasets that have invalid formats
    ExceptionDialog *exceptionDialog = new ExceptionDialog(); //the dialog used to show error messages when exception occurs
    WriteFunctionDialog *functionDialog = new WriteFunctionDialog(); //the dialog used to allow the user wo write functions

    QMenu *contextMenu=new QMenu(); //used to display context menu
    QMenu *secondary_contextMenu=new QMenu();

    QMenu *graph_selectedMenu=new QMenu(); //only display when graph(s) have been selected by clicking

};
#endif // DATAPROCESSAPP_H
