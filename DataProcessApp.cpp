#include "DataProcessApp.h"
#include "ui_DataProcessApp.h"

DataProcessApp::DataProcessApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DataProcessApp)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/DataVisualization.png"));
    setCentralWidget(ui->customPlot);
    constructContextMenu();

    ui->customPlot->legend->setVisible(true);
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
    ui->customPlot->setInteractions(QCP::iSelectPlottables | QCP::iMultiSelect | QCP::iRangeZoom | QCP::iRangeDrag | QCP::iSelectAxes);
    ui->customPlot->setContextMenuPolicy(Qt::CustomContextMenu);

    //connect slots that are used to change colors
    connect(ui->customPlot, SIGNAL(plottableDoubleClick(QCPAbstractPlottable*,int,QMouseEvent*)), this, SLOT(graphDoubleClicked(QCPAbstractPlottable*)));
    connect(ui->customPlot, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(plotContextMenuRequest(QPoint)));

    //connect slot that are used to change graph legend names
    connect(ui->customPlot, SIGNAL(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*,QMouseEvent*)), this, SLOT(legendDoubleClick(QCPLegend*,QCPAbstractLegendItem*)));

    //connect slots that are used to manipulate the screen zooming and draging
    connect(ui->customPlot, SIGNAL(mouseWheel(QWheelEvent*)), this, SLOT(zoomGraphByWheel()));
    connect(ui->customPlot, SIGNAL(mousePress(QMouseEvent*)), this, SLOT(dragScreenByPress()));

}

DataProcessApp::~DataProcessApp()
{
    delete ui;
    delete file_name_list;
    delete fileName_dataset_map;
    delete selectDialog;
    delete selected_datasets_list;
    delete selected_dataset_key_domains_vec;
    delete selected_dataset_value_domains_vec;
    delete invalid_file_name_list;
    delete exceptionDialog;
    delete functionDialog;
    delete contextMenu;
    delete secondary_contextMenu;
    delete graph_selectedMenu;
}

bool DataProcessApp::isFileFormatValid(QStringList &file_content_list)
{
    QRegularExpression re("^[+-]?[0-9]+(\.[0-9]+)?,[+-]?[0-9]+(\.[0-9]+)?$", QRegularExpression::UseUnicodePropertiesOption);
    QRegularExpressionMatch match;
    foreach (QString point_str, file_content_list) {
        match=re.match(point_str);
        if(!match.hasMatch()){
            return false;
        }
    }
    return true;
}

/*if regex is valid, then check if there are multiple y values mapped to a same x coordinate value, which is not allowed.
*/
bool DataProcessApp::isEveryXValueUnique(QVector<Point> &point_vec)
{
    Point current_point;
    Point previous_point;
    for (int i = 0; i < point_vec.size(); ++i) {
        current_point = point_vec.at(i);
        if(i>0){
            if(current_point.x==previous_point.x){
                return false;
            }
        }
        //traverse
        previous_point=current_point;
    }
    return true;
}


/* check whether the domains of datasets selected to draw a new function graph are identical
 * if not identical, the function written by the user can't be applied on these datasets
*/
bool DataProcessApp::areDomainsIdentical(QVector<QVector<double>> &dataset_domains_vec)
{
    QVector<double> current_domain_vec;
    QVector<double> previous_domain_vec;
    for (int i = 0; i < dataset_domains_vec.size(); ++i) {
        current_domain_vec=dataset_domains_vec.at(i);
        if(i>0){
            if(current_domain_vec.size()!=previous_domain_vec.size()){//compare size first
                return false;
            }
            if(current_domain_vec!=previous_domain_vec){//then compare content
                return false;
            }
        }
        previous_domain_vec=current_domain_vec;
    }
    return true;
}

void DataProcessApp::setSelected_datasets_list(QList<QListWidgetItem*> *list_widget)
{
    this->selected_datasets_list->clear();
    foreach (QListWidgetItem* list_widget_item, *list_widget) {
        this->selected_datasets_list->append(list_widget_item->text());
    }
}


/* add data to a new graph according to the function written by the user
 * ready for plot
*/
void DataProcessApp::addGraphFromFunction(QVector<QVector<double> > &dataset_key_domains_vec, QVector<QVector<double> > &dataset_value_domains_vec)
{
    runFunctionDialog();
    if(this->functionDialog->getIsDialogAccepted()&&!(this->functionDialog->getSelectedFunction().isEmpty())){
        //assign the data of the new graph according to the function written by the user
        QVector<double> new_y_value_vec;
        if(areDomainsIdentical(dataset_key_domains_vec)){
            if(this->functionDialog->getSelectedFunction()=="product"){
                for (int i = 0; i < dataset_value_domains_vec.at(0).size(); ++i) {
                    double new_y_value=1;
                    foreach (QVector<double> y_value_vec, dataset_value_domains_vec) {
                        new_y_value=new_y_value*y_value_vec.at(i);
                    }
                    new_y_value_vec.push_back(new_y_value);
                }
            }else if(this->functionDialog->getSelectedFunction()=="sum"){
                for (int i = 0; i < dataset_value_domains_vec.at(0).size(); ++i) {
                    double new_y_value=0;
                    foreach (QVector<double> y_value_vec, dataset_value_domains_vec) {
                        new_y_value=new_y_value+y_value_vec.at(i);
                    }
                    new_y_value_vec.push_back(new_y_value);
                }
            }else if(this->functionDialog->getSelectedFunction()=="square sum"){
                for (int i = 0; i < dataset_value_domains_vec.at(0).size(); ++i) {
                    double new_y_value=0;
                    foreach (QVector<double> y_value_vec, dataset_value_domains_vec) {
                        new_y_value=new_y_value+std::pow(y_value_vec.at(i),2);
                    }
                    new_y_value_vec.push_back(new_y_value);
                }
            }else if(this->functionDialog->getSelectedFunction()=="root sum"){
                for (int i = 0; i < dataset_value_domains_vec.at(0).size(); ++i) {
                    double new_y_value=0;
                    foreach (QVector<double> y_value_vec, dataset_value_domains_vec) {
                        new_y_value=new_y_value+std::sqrt(y_value_vec.at(i));
                    }
                    new_y_value_vec.push_back(new_y_value);
                }
            }

            if(ui->customPlot->graphCount()>this->selected_datasets_list->size()){
                ui->customPlot->removeGraph(this->selected_datasets_list->size()); //clear the graph drawn by the previous written function
            }

            ui->customPlot->addGraph();
            ui->customPlot->graph(this->selected_datasets_list->size())->setName(this->functionDialog->getSelectedFunction());
            ui->customPlot->graph(this->selected_datasets_list->size())->setData(dataset_key_domains_vec.at(0),new_y_value_vec);
            ui->customPlot->graph(this->selected_datasets_list->size())->setPen(QPen(Qt::red));
            ui->customPlot->rescaleAxes();
            ui->customPlot->replot();
        }else{
            this->exceptionDialog->setDialogMessage("Function failed! Please ensure the domain of datasets are identical.");
            this->exceptionDialog->exec();
        }
    }
}

void DataProcessApp::runFunctionDialog()
{
    QStringList available_func_for_user;
    if(this->selected_datasets_list->size()>=2){
        available_func_for_user.append("product");
        available_func_for_user.append("sum");
        available_func_for_user.append("square sum");
        available_func_for_user.append("root sum");
        this->functionDialog->setFunctionList(available_func_for_user);
        this->functionDialog->setUserHint(available_func_for_user, this->selected_datasets_list->size());
        this->functionDialog->reset();
        this->functionDialog->exec();
    } else{
        QMessageBox msgBox;
        msgBox.setText("Please plot more than one dataset.");
        msgBox.exec();
        this->functionDialog->reset();
    }
}

void DataProcessApp::runColorDialog()
{
    if(ui->customPlot->selectedGraphs().isEmpty()){
        QMessageBox msgBox;
        msgBox.setText("No graphs have been selected yet. Please select graphs first.");
        msgBox.exec();
    } else{
        QColor color = QColorDialog::getColor();
        if(color.isValid()){
            foreach (QCPGraph* graph, ui->customPlot->selectedGraphs()) {
                graph->setPen(QPen(color));
            }
        }
        ui->customPlot->replot();
    }
}

void DataProcessApp::constructContextMenu()
{
    this->contextMenu->addAction(ui->actionLoad_Datasets);
    ui->actionLoad_Datasets->setIcon(QIcon(":/file.png"));
    this->contextMenu->addSeparator();

    this->contextMenu->addAction(ui->actionSelect_Datasets_to_Plot);
    ui->actionSelect_Datasets_to_Plot->setIcon(QIcon(":/plot.png"));
    this->contextMenu->addSeparator();

    this->contextMenu->addAction(ui->actionWrite_Function_for_Plots);
    ui->actionWrite_Function_for_Plots->setIcon(QIcon(":/function.png"));

    this->secondary_contextMenu->setTitle("Clean");

    this->secondary_contextMenu->addAction(ui->actionDelete_All_Datasets_and_Graphs);
    ui->actionDelete_All_Datasets_and_Graphs->setIcon(QIcon(":/delete_file.png"));
    this->secondary_contextMenu->addSeparator();

    this->secondary_contextMenu->addAction(ui->actionRemove_the_Function_Graph);
    ui->actionRemove_the_Function_Graph->setIcon(QIcon(":/remove.png"));
    this->secondary_contextMenu->addSeparator();

    this->secondary_contextMenu->addAction(ui->actionClean_the_Screen);
    ui->actionClean_the_Screen->setIcon(QIcon(":/eraser.png"));

    this->contextMenu->addMenu(this->secondary_contextMenu);

    this->graph_selectedMenu->addAction(ui->actionChange_Graph_Color);
    ui->actionChange_Graph_Color->setIcon(QIcon(":/painter.png"));
}

void DataProcessApp::resetFiles()
{
    ui->customPlot->clearGraphs();
    ui->customPlot->replot();
    this->file_name_list->clear();
    this->fileName_dataset_map->clear();
    this->selectDialog->clearFilesInList();
    this->selected_datasets_list->clear();
    this->selected_dataset_key_domains_vec->clear();
    this->selected_dataset_value_domains_vec->clear();
    this->invalid_file_name_list->clear();
}


/*=====================================================SLOTS======================================================================*/


void DataProcessApp::on_actionLoad_Datasets_triggered()
{
    QStringList fileNames=QFileDialog::getOpenFileNames(this,"Load a file"); // Asking the user what file they want to load and where it is stored

    if(fileNames.isEmpty()){
        return;
    }

    bool has_any_file_been_loaded=false;
    bool has_repeat_loaded_file=false;

    this->invalid_file_name_list->clear();

    for (int i = 0; i < fileNames.size(); ++i) {

        if(this->file_name_list->contains(fileNames.at(i))){
            has_repeat_loaded_file=true;
            continue;
        }

        QFile loadFile(fileNames.at(i)); // Creating a new file with the file name given by the user
        loadFile.open(QIODevice::ReadOnly); // Openning the file for reading
        QTextStream input(&loadFile); // Defining the text stream to be "extracted" from the file
        QString read_text=input.readAll(); // finally doing the reading

        QStringList point_list=read_text.split("\r\n");
        QVector<Point> point_vec;

        if(!isFileFormatValid(point_list)){ //record invalid files which not conform to x.xx... , x.xx...
            this->invalid_file_name_list->append(fileNames.at(i));
        }else{ //if the file format is valid, process it further
            foreach (QString point_str, point_list) { //store the points in the dataset into vector
                QStringList point_coordinate_pair=point_str.split(",");
                Point point(point_coordinate_pair.at(0).toDouble(),point_coordinate_pair.at(1).toDouble());
                point_vec.push_back(point);
            }

            std::sort(point_vec.begin(),point_vec.end(),Point::sortByXCoordinate);//sort points by x coordinate value

            if(!isEveryXValueUnique(point_vec)){ //although format is valid, we need to check again the value in x coordinate
                this->invalid_file_name_list->append(fileNames.at(i));
            }else{
                has_any_file_been_loaded=true;
                this->file_name_list->append(fileNames.at(i)); //store the filename selected by the user
                this->fileName_dataset_map->insert(fileNames.at(i),point_vec); //map the file name with the datasets
            }
        }
        loadFile.close();
    }

    this->selectDialog->setFilesInList(*(this->file_name_list)); //list the loaded datasets for users to select in a new dialog

    //display error dialog to users if there are invalid files
    if(!this->invalid_file_name_list->isEmpty()){
        QString error_message="The file format of the following file(s) are incorrcect!\n\n";
        foreach (QString invalid_file_name, *(this->invalid_file_name_list)) {
            error_message.append(invalid_file_name).append(",\n\n");
        }
        error_message.append("Possible reasons are:\n1. The format of some rows are not in 'd.dd...(or d),d.dd...(or d)' format. The 'd' represents a digit and no space is allowed.\n2. A same x coordinate value exists repeatedly.\n\n");
        error_message.append("A correct example is shown below:\n1,2\n1.5,3.0\n3.0,6\n...");
        this->exceptionDialog->setDialogMessage(error_message);
        this->exceptionDialog->exec();
    }

    if(has_any_file_been_loaded){
        QMessageBox msgBox;
        if(!this->invalid_file_name_list->isEmpty()){
            msgBox.setText("The other valid datasets you selected has been loaded and you can select them in the tool bar now.");
            msgBox.exec();
        }else{
            msgBox.setText("All datasets you selected has been loaded successfully and you can select them in the tool bar now.");
            msgBox.exec();
        }
    }else{
        if(has_repeat_loaded_file&&this->invalid_file_name_list->isEmpty()){
            QMessageBox msgBox;
            msgBox.setText("Please don't repeat loading the same file.");
            msgBox.exec();
        }
    }

}

void DataProcessApp::on_actionSelect_Datasets_to_Plot_triggered()
{
    this->selectDialog->exec();

    //process the plot request only whrn the dialog is accepted
    if(this->selectDialog->getIsDialogAccepted()){
        ui->customPlot->clearGraphs();

        this->setSelected_datasets_list(this->selectDialog->getSelected_datasets_list());

        QVector<QVector<double>> dataset_key_domains_vec;
        QVector<QVector<double>> dataset_values_domains_vec;

        //plot selected datasets
        for (int i = 0; i < this->selected_datasets_list->size(); ++i) {
            QVector<Point> point_vec = this->fileName_dataset_map->value(this->selected_datasets_list->at(i));
            QVector<double> coordinate_x_vec;
            QVector<double> coordinate_y_vec;
            foreach (Point point, point_vec) {
                coordinate_x_vec.push_back(point.x);
                coordinate_y_vec.push_back(point.y);
            }
            dataset_key_domains_vec.push_back(coordinate_x_vec); //record the key domain of each dataset
            dataset_values_domains_vec.push_back(coordinate_y_vec); //record the value domain of each dataset

            ui->customPlot->addGraph();
            ui->customPlot->graph(i)->setName("Data "+QString::number(i+1));
            ui->customPlot->graph(i)->setData(coordinate_x_vec,coordinate_y_vec);
        }
        ui->customPlot->rescaleAxes();
        ui->customPlot->replot();

        *(this->selected_dataset_key_domains_vec)=dataset_key_domains_vec;
        *(this->selected_dataset_value_domains_vec)=dataset_values_domains_vec;

        //let the user write the function for datasets and plot the new one
        if(this->selected_datasets_list->size()>1){
            addGraphFromFunction(dataset_key_domains_vec,dataset_values_domains_vec); //includes replot();
        }
    }
}

void DataProcessApp::on_actionWrite_Function_for_Plots_triggered()
{
    addGraphFromFunction(*(this->selected_dataset_key_domains_vec),*(this->selected_dataset_value_domains_vec));
}

void DataProcessApp::on_actionDelete_All_Datasets_and_Graphs_triggered()
{
    resetFiles();
}

void DataProcessApp::on_actionRemove_the_Function_Graph_triggered()
{
    if(ui->customPlot->graphCount()>this->selected_datasets_list->size()){
        ui->customPlot->removeGraph(this->selected_datasets_list->size());
        ui->customPlot->rescaleAxes();
        ui->customPlot->replot();
    }else{
        QMessageBox msgBox;
        msgBox.setText("No functions have been plotted.");
        msgBox.exec();
    }
}

void DataProcessApp::on_actionClean_the_Screen_triggered()
{
    this->selected_datasets_list->clear();
    ui->customPlot->clearGraphs();
    ui->customPlot->replot();
}


/*
 * change color by double clicking the graph
*/
void DataProcessApp::graphDoubleClicked(QCPAbstractPlottable*)
{
    runColorDialog();
}

/*
 * change name of datasets by double clicking the legend
*/
void DataProcessApp::legendDoubleClick(QCPLegend *legend, QCPAbstractLegendItem *item)
{
      Q_UNUSED(legend)
      if (item) // only react if item was clicked (user could have clicked on border padding of legend where there is no item, then item is 0)
      {
        QCPPlottableLegendItem *plItem = qobject_cast<QCPPlottableLegendItem*>(item);
        bool ok;
        QString newName = QInputDialog::getText(this, "Edit", "New graph name:", QLineEdit::Normal, plItem->plottable()->name(), &ok);
        if (ok)
        {
          plItem->plottable()->setName(newName);
          ui->customPlot->replot();
        }
      }
}

void DataProcessApp::plotContextMenuRequest(QPoint pos)
{
    if(ui->customPlot->selectedGraphs().size()==0){
        this->contextMenu->popup(ui->customPlot->mapToGlobal(pos));
    }else{
        this->graph_selectedMenu->popup(ui->customPlot->mapToGlobal(pos));
    }

}

void DataProcessApp::on_actionChange_Graph_Color_triggered()
{
    runColorDialog();
}

void DataProcessApp::zoomGraphByWheel()
{
    // if an axis is selected, only allow the direction of that axis to be zoomed
    // if no axis is selected, both directions may be zoomed
    if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
        ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->xAxis->orientation());
      else if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
        ui->customPlot->axisRect()->setRangeZoom(ui->customPlot->yAxis->orientation());
      else
        ui->customPlot->axisRect()->setRangeZoom(Qt::Horizontal|Qt::Vertical);
}

void DataProcessApp::dragScreenByPress()
{
      // if an axis is selected, only allow the direction of that axis to be dragged
      // if no axis is selected, both directions may be dragged
      if (ui->customPlot->xAxis->selectedParts().testFlag(QCPAxis::spAxis))
        ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->xAxis->orientation());
      else if (ui->customPlot->yAxis->selectedParts().testFlag(QCPAxis::spAxis))
        ui->customPlot->axisRect()->setRangeDrag(ui->customPlot->yAxis->orientation());
      else
        ui->customPlot->axisRect()->setRangeDrag(Qt::Horizontal|Qt::Vertical);
}
