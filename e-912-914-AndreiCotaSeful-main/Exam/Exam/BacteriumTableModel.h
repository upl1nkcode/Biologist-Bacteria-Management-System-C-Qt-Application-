#ifndef BACTERIUMTABLEMODEL_H
#define BACTERIUMTABLEMODEL_H

#include <QAbstractTableModel>
#include "Bacterium.h"

class BacteriumTableModel : public QAbstractTableModel
{
    Q_OBJECT
    
public:
    enum Column {
        NameColumn = 0,
        SpeciesColumn,
        SizeColumn,
        DiseasesColumn,
        ColumnCount
    };
    
    explicit BacteriumTableModel(QObject* parent = nullptr);
    
    // QAbstractTableModel interface
    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;
    //Returns the data to display for a given cell 
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
    //extracting the header information
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
   //setting data for a given cell
    bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;
    //for selecting data and whatever, clickks..
    Qt::ItemFlags flags(const QModelIndex& index) const override;
    
    void setBacteria(const QList<Bacterium>& bacteria);
    Bacterium getBacterium(int row) const;
    void updateBacterium(const Bacterium& bacterium);
    
public slots:
    //connection for keeping live updated the bactrium list of inromation
    void onBacteriumAdded(const Bacterium& bacterium);
    void onBacteriumUpdated(const Bacterium& bacterium);
    
private:
    QList<Bacterium> m_bacteria;//data storage
    int findBacteriumRow(const Bacterium& bacterium) const;
};

#endif // BACTERIUMTABLEMODEL_H