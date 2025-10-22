#ifndef BIOLOGISTWINDOW_H
#define BIOLOGISTWINDOW_H

#include <QMainWindow>
#include <QTableView>
#include <QComboBox>
#include <QPushButton>
#include <QListWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSplitter>
#include "Biologist.h"
#include "BacteriumTableModel.h"

class BiologistWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit BiologistWindow(const Biologist& biologist, QWidget* parent = nullptr);
    
private slots:
    void onSpeciesFilterChanged();
    void onAddBacteriumClicked();
    void onViewClicked();
    void onBacteriumSelectionChanged();
    void onAddDiseaseClicked();
    void onRemoveDiseaseClicked();
    void refreshBacteriaList();
    
private:
    void setupUI();
    void updateBacteriaDisplay();
    void updateDiseasesList();
    
    Biologist m_biologist;
    
    // UI Components
    QWidget* m_centralWidget;
    QSplitter* m_mainSplitter;
    QTableView* m_bacteriaTable;
    BacteriumTableModel* m_tableModel;
    QComboBox* m_speciesFilter;
    QPushButton* m_addBacteriumBtn;
    QPushButton* m_viewBtn;
    QListWidget* m_diseasesList;
    QPushButton* m_addDiseaseBtn;
    QPushButton* m_removeDiseaseBtn;
    
    QString m_currentSpeciesFilter;
};

#endif // BIOLOGISTWINDOW_H