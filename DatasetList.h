#ifndef DATASETLIST_H
#define DATASETLIST_H

#include <QWidget>
#include <QAbstractListModel>

namespace Ui {
class DatasetList;
}

class DatasetList : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit DatasetList(QWidget *parent = nullptr);
    ~DatasetList();

private:
    Ui::DatasetList *ui;
};

#endif // DATASETLIST_H
