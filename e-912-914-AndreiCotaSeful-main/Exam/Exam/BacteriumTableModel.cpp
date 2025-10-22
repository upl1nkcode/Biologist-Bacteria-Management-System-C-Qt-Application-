#include "BacteriumTableModel.h"
#include "DataService.h"

BacteriumTableModel::BacteriumTableModel(QObject* parent)
    : QAbstractTableModel(parent)
{
    connect(&DataService::instance(), &DataService::bacteriumAdded,
            this, &BacteriumTableModel::onBacteriumAdded);
    connect(&DataService::instance(), &DataService::bacteriumUpdated,
            this, &BacteriumTableModel::onBacteriumUpdated);
}

int BacteriumTableModel::rowCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return m_bacteria.size();
}

int BacteriumTableModel::columnCount(const QModelIndex& parent) const
{
    Q_UNUSED(parent)
    return ColumnCount;
}

QVariant BacteriumTableModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= m_bacteria.size())
        return QVariant();
    
    const Bacterium& bacterium = m_bacteria[index.row()];
    
    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        switch (index.column()) {
        case NameColumn:
            return bacterium.getName();
        case SpeciesColumn:
            return bacterium.getSpecies();
        case SizeColumn:
            return bacterium.getSize();
        case DiseasesColumn:
            return bacterium.getDiseases().join(", ");
        }
    }
    
    return QVariant();
}

QVariant BacteriumTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch (section) {
        case NameColumn:
            return "Name";
        case SpeciesColumn:
            return "Species";
        case SizeColumn:
            return "Size";
        case DiseasesColumn:
            return "Diseases";
        }
    }
    
    return QVariant();
}

bool BacteriumTableModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || index.row() >= m_bacteria.size() || role != Qt::EditRole)
        return false;
    
    Bacterium& bacterium = m_bacteria[index.row()];
    
    switch (index.column()) {
    case NameColumn:
        bacterium.setName(value.toString());
        break;
    case SpeciesColumn:
        bacterium.setSpecies(value.toString());
        break;
    case SizeColumn:
        bacterium.setSize(value.toInt());
        break;
    case DiseasesColumn: {
        QStringList diseases = value.toString().split(",");
        for (QString& disease : diseases) {
            disease = disease.trimmed();
        }
        bacterium.setDiseases(diseases);
        break;
    }
    default:
        return false;
    }
    
    DataService::instance().updateBacterium(bacterium);
    emit dataChanged(index, index);
    ///emits a signal which once again is wierd trough tableview to keep updated
    return true;
}

Qt::ItemFlags BacteriumTableModel::flags(const QModelIndex& index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    
    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

void BacteriumTableModel::setBacteria(const QList<Bacterium>& bacteria)
{
    beginResetModel();
    m_bacteria = bacteria;
    endResetModel();
}

Bacterium BacteriumTableModel::getBacterium(int row) const
{
    if (row >= 0 && row < m_bacteria.size()) {
        return m_bacteria[row];
    }
    return Bacterium();
}

void BacteriumTableModel::updateBacterium(const Bacterium& bacterium)
{
    int row = findBacteriumRow(bacterium);
    if (row >= 0) {
        m_bacteria[row] = bacterium;
        emit dataChanged(index(row, 0), index(row, ColumnCount - 1));
    }
}

void BacteriumTableModel::onBacteriumAdded(const Bacterium& bacterium)
{
    // Check if this bacterium should be visible in current view
    // This will be handled by the window that manages filtering
}

void BacteriumTableModel::onBacteriumUpdated(const Bacterium& bacterium)
{
    updateBacterium(bacterium);
}

int BacteriumTableModel::findBacteriumRow(const Bacterium& bacterium) const
{
    for (int i = 0; i < m_bacteria.size(); ++i) {
        if (m_bacteria[i].getKey() == bacterium.getKey()) {
            return i;
        }
    }
    return -1;
}