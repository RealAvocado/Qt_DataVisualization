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

    bool isFileFormatValid(QStringList &file_content_list); //check the format of file content using regular expression

    bool isEveryKeyUnique(QVector<Point> &point_vec); //check is there are repeated x axis keys

    bool areDomainsIdentical(QVector<QVector<double> > &dataset_domains_vec); //check if datasets have the same x axis

    QString getTruncFileName(QString &file_name); //truncate the file name for legend

    void setSelected_datasets_list(QList<QListWidgetItem*> *list_widget); //receive users input of selected datasets

    void addGraphFromFunction(QVector<QVector<double> > &dataset_key_domains_vec, QVector<QVector<double> > &dataset_value_domains_vec); //plot new graph according to the function written by user

    void runFunctionDialog(); //run the dialog for user to write function

    void runColorDialog(); //run the dialog for user to pick colour

    void runMessageBox(QMessageBox::Icon icon, QString message); //run the dialog to show messages to users

    void constructContextMenu(); //context menu

    void resetFiles(); //clear all the datasets

private slots:
    // slots used to load, plot and delete graphs and datasets

    void on_actionLoad_Datasets_triggered(); //load files in QFileDialog

    void on_actionSelect_Datasets_to_Plot_triggered(); //let users select datasets to be plotted

    void on_actionWrite_Function_for_Plots_triggered(); //write function

    void on_actionDelete_All_Datasets_and_Graphs_triggered(); //clear all the datasets

    void on_actionRemove_the_Function_Graph_triggered(); //delete the function graph

    void on_actionClean_the_Screen_triggered(); //clean the screen but don't delete datasets

    void on_actionRestore_to_Original_View_triggered(); //restore to original view after zooming and dragging

    // slots used for other user interaction

    void graphDoubleClicked(QCPAbstractPlottable*); //execute the QColorDialog after graph is double clicked

    void legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item); //used to change legend label

    void plotContextMenuRequest(QPoint pos); //pop up the context menu when requested in customPlot

    void on_actionChange_Graph_Color_triggered(); //change colour

    void zoomGraphByWheel(); //zoom in and out

    void dragScreenByPress(); //drag graphs

private:
    Ui::DataProcessApp *ui;

    QStringList *file_name_list = new QStringList; //store the file name of loaded datasets
    QMap<QString, QVector<Point>> *fileName_dataset_map = new QMap<QString, QVector<Point>>; //link the file name to datas one to one

    DatasetSelectDialog *selectDialog = new DatasetSelectDialog(); //the dialog used to select loaded datasets
    QStringList *selected_datasets_list = new QStringList; //record datasets selected by the user
    QVector<QVector<double>>* selected_dataset_key_domains_vec = new QVector<QVector<double>>; //store the x domain value of selected datasets
    QVector<QVector<double>>* selected_dataset_value_domains_vec = new QVector<QVector<double>>; //store the y value of selected datasets

    QStringList *invalid_file_name_list = new QStringList; //record datasets that have invalid formats
    WriteFunctionDialog *functionDialog = new WriteFunctionDialog(); //the dialog used to allow the user wo write functions

    QMenu *contextMenu=new QMenu(); //used to display context menu
    QMenu *secondary_contextMenu=new QMenu(); //secondary menu

    QMenu *graph_selectedMenu=new QMenu(); //only display when graph(s) have been selected by clicking

};
#endif // DATAPROCESSAPP_H
