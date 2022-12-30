#include "DataProcessApp.h"
#include "ui_DataProcessApp.h"

DataProcessApp::DataProcessApp(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::DataProcessApp)
{
    ui->setupUi(this);
    setCentralWidget(ui->customPlot);
    ui->customPlot->legend->setVisible(true);
    ui->customPlot->xAxis->setLabel("x");
    ui->customPlot->yAxis->setLabel("y");
}

DataProcessApp::~DataProcessApp()
{
    delete ui;
    delete file_name_list;
    delete fileName_dataset_map;
    delete selectDialog;
    delete selected_datasets_list;
    delete dataset_domains_vec_attr;
    delete dataset_y_values_vec_attr;
    delete invalid_file_name_list;
    delete exceptionDialog;
    delete functionDialog;
}

bool DataProcessApp::isFileFormatValid(QStringList file_content_list)
{
    bool is_format_valid = true;
    QRegularExpression re("\s*(([+-]?[1-9]\d*\.?\d*)|([+-]?0\.\d*))\s*,\s*(([+-]?[1-9]\d*\.?\d*)|([+-]?0\.\d*))\s*");
    //QRegularExpression re("([+-]?\d+\.?\d+)\s*,\s*([+-]?\d+\.?\d+)");
    QRegularExpressionMatch match;
    foreach (QString point_str, file_content_list) {
        match=re.match(point_str);
        if(!match.hasMatch()){
            is_format_valid=false;
        }
    }
    return is_format_valid;
}

/* check whether the domains of selected datasets to draw a new function graph are identical
 * if not identical, the function written by the user can't be applied on these datasets
*/
bool DataProcessApp::areDomainsIdentical(QVector<QVector<double>> dataset_domains_vec)
{
    QVector<double> current_domain_vec;
    QVector<double> previous_domain_vec;
    for (int i = 0; i < dataset_domains_vec.size(); ++i) {
        current_domain_vec=dataset_domains_vec.at(i);
        std::sort(current_domain_vec.begin(),current_domain_vec.end());
        std::sort(previous_domain_vec.begin(),previous_domain_vec.end());
        if(i>0){
            if(current_domain_vec.size()!=previous_domain_vec.size()){//compare size first
                return false;
            }
            if(current_domain_vec!=previous_domain_vec){//then compare content
                return false;
            }
        }
        if(i<dataset_domains_vec.size()-1){
            //traverse
            previous_domain_vec=current_domain_vec;
            current_domain_vec=dataset_domains_vec.at(i+1);
        }
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
void DataProcessApp::addGraphFromFunction(QVector<QVector<double> > dataset_domains_vec, QVector<QVector<double> > dataset_y_values_vec)
{
    runFunctionDialog();
    if(ui->customPlot->graphCount()>this->selected_datasets_list->size()){
        ui->customPlot->removeGraph(this->selected_datasets_list->size()); //clear the graph drawn by the previous written function
    }

    //assign the data of the new graph according to the function written by the user
    QVector<double> new_y_value_vec;
    if(!(this->functionDialog->getSelectedFunction().isEmpty())){
        if(areDomainsIdentical(dataset_domains_vec)){
            if(this->functionDialog->getSelectedFunction()=="product"){
                for (int i = 0; i < dataset_y_values_vec.at(0).size(); ++i) {
                    double new_y_value=1;
                    foreach (QVector<double> y_value_vec, dataset_y_values_vec) {
                        new_y_value=new_y_value*y_value_vec.at(i);
                    }
                    new_y_value_vec.push_back(new_y_value);
                }
            }else if(this->functionDialog->getSelectedFunction()=="sum"){
                for (int i = 0; i < dataset_y_values_vec.at(0).size(); ++i) {
                    double new_y_value=0;
                    foreach (QVector<double> y_value_vec, dataset_y_values_vec) {
                        new_y_value=new_y_value+y_value_vec.at(i);
                    }
                    new_y_value_vec.push_back(new_y_value);
                }
            }else if(this->functionDialog->getSelectedFunction()=="square sum"){
                for (int i = 0; i < dataset_y_values_vec.at(0).size(); ++i) {
                    double new_y_value=0;
                    foreach (QVector<double> y_value_vec, dataset_y_values_vec) {
                        new_y_value=new_y_value+std::pow(y_value_vec.at(i),2);
                    }
                    new_y_value_vec.push_back(new_y_value);
                }
            }else if(this->functionDialog->getSelectedFunction()=="root sum"){
                for (int i = 0; i < dataset_y_values_vec.at(0).size(); ++i) {
                    double new_y_value=0;
                    foreach (QVector<double> y_value_vec, dataset_y_values_vec) {
                        new_y_value=new_y_value+std::sqrt(y_value_vec.at(i));
                    }
                    new_y_value_vec.push_back(new_y_value);
                }
            }
            ui->customPlot->addGraph();
            ui->customPlot->graph(this->selected_datasets_list->size())->setData(dataset_domains_vec.at(0),new_y_value_vec);
            ui->customPlot->graph(this->selected_datasets_list->size())->rescaleAxes(true);
        }else{
            this->exceptionDialog->setDialogMessage("Function failed! Please ensure the domain of datasets are identical.");
            this->exceptionDialog->exec();
        }
    }
    ui->customPlot->replot();
}

void DataProcessApp::runFunctionDialog()
{
    QStringList available_func_for_user;
    if(this->selected_datasets_list->size()==2){
        available_func_for_user.append("product");
        available_func_for_user.append("sum");
    }else if(this->selected_datasets_list->size()>=3){
        available_func_for_user.append("product");
        available_func_for_user.append("sum");
        available_func_for_user.append("square sum");
        available_func_for_user.append("root sum");
    }
    if(this->selected_datasets_list->size()>=2){
        this->functionDialog->setFunctionList(available_func_for_user);
        this->functionDialog->setUserHint(available_func_for_user, this->selected_datasets_list->size());
    }else{
        this->functionDialog->clearFunctionList();
        this->functionDialog->remindUserToSelectDatasets();
    }
    this->functionDialog->clearLineEdit();
    this->functionDialog->exec();
}

void DataProcessApp::on_actionLoad_Datasets_triggered()
{
    //preparation for loading datasets
    bool is_file_valid = true;

    QStringList fileNames=QFileDialog::getOpenFileNames(this,"Load a file"); // Asking the user what file they want to load and where it is stored

    if(!fileNames.isEmpty()){
        ui->customPlot->clearGraphs();
        ui->customPlot->replot();
        this->file_name_list->clear();
        this->fileName_dataset_map->clear();
        this->selectDialog->clearFilesInList();
        this->invalid_file_name_list->clear();
    }else{
        return;
    }

    for (int i = 0; i < fileNames.size(); ++i) {
        QFile loadFile(fileNames.at(i)); // Creating a new file with the file name given by the user
        loadFile.open(QIODevice::ReadOnly); // Openning the file for reading
        QTextStream input(&loadFile); // Defining the text stream to be "extracted" from the file
        QString read_text=input.readAll(); // finally doing the reading

        QStringList point_list=read_text.split("\r\n");
        QVector<Point> point_vec;

        if(!isFileFormatValid(point_list)){ //record invalid files
            is_file_valid=false;
            this->invalid_file_name_list->append(fileNames.at(i));
        }else{ //if the file is valid, process it as normal
            foreach (QString point_str, point_list) { //store the points in the dataset into vector
                QStringList point_coordinate_pair=point_str.split(",");
                Point* point = new Point(point_coordinate_pair.at(0).toDouble(),point_coordinate_pair.at(1).toDouble());
                point_vec.push_back(*point);
                delete point;
                point=nullptr;
            }

            this->file_name_list->append(fileNames.at(i)); //store the filename selected by the user
            this->fileName_dataset_map->insert(fileNames.at(i),point_vec); //map the file name with the datasets
            this->selectDialog->setFilesInList(*(this->file_name_list)); //list the loaded datasets for users to select in a new dialog
        }
        loadFile.close();
    }

    //display error dialog to users if there are invalid files
    if(!is_file_valid){
        QString error_dlg_label="The file format of:\r\n";
        foreach (QString invalid_file_name, *(this->invalid_file_name_list)) {
            error_dlg_label.append(invalid_file_name).append(",\r\n");
        }
        error_dlg_label.append("are incorrect!");
        this->exceptionDialog->setDialogMessage(error_dlg_label);
        this->exceptionDialog->exec();
    }
}


void DataProcessApp::on_actionSelect_Datasets_to_Plot_triggered()
{
    this->selectDialog->exec();

    //process the plot request only whrn the dialog is accepted
    if(this->selectDialog->getIsDialogAccepted()){
        ui->customPlot->clearGraphs();

        this->setSelected_datasets_list(this->selectDialog->getSelected_datasets_list());

        QVector<QVector<double>> dataset_domains_vec;
        QVector<QVector<double>> dataset_y_values_vec;

        //plot selected datasets
        for (int i = 0; i < this->selected_datasets_list->size(); ++i) {
            QVector<Point> point_vec = this->fileName_dataset_map->value(this->selected_datasets_list->at(i));
            QVector<double> coordinate_x_vec(point_vec.size());
            QVector<double> coordinate_y_vec(point_vec.size());
            foreach (Point point, point_vec) {
                coordinate_x_vec.push_back(point.x);
                coordinate_y_vec.push_back(point.y);
            }
            dataset_domains_vec.push_back(coordinate_x_vec); //record the domain of definition of each dataset
            dataset_y_values_vec.push_back(coordinate_y_vec); //record the value field of definition of each dataset

            ui->customPlot->addGraph();
            ui->customPlot->graph(i)->setData(coordinate_x_vec,coordinate_y_vec);
            if(i==0){
                ui->customPlot->graph(i)->rescaleAxes(); // let the ranges scale themselves so graph fits perfectly in the visible area:
            }else{
                ui->customPlot->graph(i)->rescaleAxes(true);
            }
        }
        *(this->dataset_domains_vec_attr)=dataset_domains_vec;
        *(this->dataset_y_values_vec_attr)=dataset_y_values_vec;

        //let the user write the function for datasets and plot the new one
        if(this->selected_datasets_list->size()>1){
            addGraphFromFunction(dataset_domains_vec,dataset_y_values_vec); //includes replot();
        }else{
            ui->customPlot->replot();
        }
    }
}


void DataProcessApp::on_actionWrite_Function_for_Plots_triggered()
{
    addGraphFromFunction(*(this->dataset_domains_vec_attr),*(this->dataset_y_values_vec_attr));
}

