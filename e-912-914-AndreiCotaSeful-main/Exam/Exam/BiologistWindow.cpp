#include "BiologistWindow.h"
#include "AddBacteriumDialog.h"
#include "VisualizationWindow.h"
#include "DataService.h"
#include <QHeaderView>
#include <QMessageBox>
#include <QInputDialog>
#include <QLabel>
#include <QGroupBox>

BiologistWindow::BiologistWindow(const Biologist& biologist, QWidget* parent)
    : QMainWindow(parent), m_biologist(biologist)
{
    setWindowTitle(m_biologist.getName());
    setupUI();
    refreshBacteriaList();
    
    // Connect to data service signals
    connect(&DataService::instance(), &DataService::bacteriumAdded,
            this, &BiologistWindow::refreshBacteriaList);
    connect(&DataService::instance(), &DataService::bacteriumUpdated,
            this, &BiologistWindow::refreshBacteriaList);
}

void BiologistWindow::setupUI()
{
    m_centralWidget = new QWidget;
    setCentralWidget(m_centralWidget);
    
    // Main layout
    QVBoxLayout* mainLayout = new QVBoxLayout(m_centralWidget);
    
    // Top controls
    QHBoxLayout* controlsLayout = new QHBoxLayout;
    
    controlsLayout->addWidget(new QLabel("Filter by Species:"));
    m_speciesFilter = new QComboBox;
    m_speciesFilter->addItem("All Species");
    QStringList species = DataService::instance().getAllSpecies();
    for (const QString& s : species) {
        if (m_biologist.studiesSpecies(s)) {
            m_speciesFilter->addItem(s);
        }
    }
    connect(m_speciesFilter, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &BiologistWindow::onSpeciesFilterChanged);
    controlsLayout->addWidget(m_speciesFilter);
    
    controlsLayout->addStretch();
    
    m_addBacteriumBtn = new QPushButton("Add Bacterium");
    connect(m_addBacteriumBtn, &QPushButton::clicked,
            this, &BiologistWindow::onAddBacteriumClicked);
    controlsLayout->addWidget(m_addBacteriumBtn);
    
    m_viewBtn = new QPushButton("View");
    connect(m_viewBtn, &QPushButton::clicked,
            this, &BiologistWindow::onViewClicked);
    controlsLayout->addWidget(m_viewBtn);
    
    mainLayout->addLayout(controlsLayout);
    
    // Main content splitter
    m_mainSplitter = new QSplitter(Qt::Horizontal);
    
    // Bacteria table
    m_tableModel = new BacteriumTableModel(this);
    m_bacteriaTable = new QTableView;
    m_bacteriaTable->setModel(m_tableModel);
    m_bacteriaTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    m_bacteriaTable->horizontalHeader()->setStretchLastSection(true);
    connect(m_bacteriaTable->selectionModel(), &QItemSelectionModel::currentRowChanged,
            this, &BiologistWindow::onBacteriumSelectionChanged);
    
    m_mainSplitter->addWidget(m_bacteriaTable);
    
    // Diseases panel
    QGroupBox* diseasesGroup = new QGroupBox("Diseases");
    QVBoxLayout* diseasesLayout = new QVBoxLayout(diseasesGroup);
    
    m_diseasesList = new QListWidget;
    diseasesLayout->addWidget(m_diseasesList);
    
    QHBoxLayout* diseaseButtonsLayout = new QHBoxLayout;
    m_addDiseaseBtn = new QPushButton("Add Disease");
    m_removeDiseaseBtn = new QPushButton("Remove Disease");
    connect(m_addDiseaseBtn, &QPushButton::clicked,
            this, &BiologistWindow::onAddDiseaseClicked);
    connect(m_removeDiseaseBtn, &QPushButton::clicked,
            this, &BiologistWindow::onRemoveDiseaseClicked);
    
    diseaseButtonsLayout->addWidget(m_addDiseaseBtn);
    diseaseButtonsLayout->addWidget(m_removeDiseaseBtn);
    diseasesLayout->addLayout(diseaseButtonsLayout);
    
    m_mainSplitter->addWidget(diseasesGroup);
    m_mainSplitter->setSizes({600, 300});
    
    mainLayout->addWidget(m_mainSplitter);
    
    resize(900, 600);
}

void BiologistWindow::onSpeciesFilterChanged()
{
    m_currentSpeciesFilter = m_speciesFilter->currentText();
    updateBacteriaDisplay();
}

void BiologistWindow::onAddBacteriumClicked()
{
    AddBacteriumDialog dialog(m_biologist, this);
    if (dialog.exec() == QDialog::Accepted) {
        refreshBacteriaList();
    }
}

void BiologistWindow::onViewClicked()
{
    VisualizationWindow* vizWindow = new VisualizationWindow(m_biologist);
    vizWindow->setAttribute(Qt::WA_DeleteOnClose);
    vizWindow->show();
}

void BiologistWindow::onBacteriumSelectionChanged()
{
    updateDiseasesList();
}

void BiologistWindow::onAddDiseaseClicked()
{
    QModelIndexList selection = m_bacteriaTable->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::information(this, "No Selection", "Please select a bacterium first.");
        return;
    }
    
    bool ok;
    QString disease = QInputDialog::getText(this, "Add Disease", "Disease name:", QLineEdit::Normal, "", &ok);
    if (ok && !disease.isEmpty()) {
        int row = selection.first().row();
        Bacterium bacterium = m_tableModel->getBacterium(row);
        bacterium.addDisease(disease);
        DataService::instance().updateBacterium(bacterium);
        updateDiseasesList();
    }
}

void BiologistWindow::onRemoveDiseaseClicked()
{
    QModelIndexList selection = m_bacteriaTable->selectionModel()->selectedRows();
    if (selection.isEmpty()) {
        QMessageBox::information(this, "No Selection", "Please select a bacterium first.");
        return;
    }
    
    QListWidgetItem* diseaseItem = m_diseasesList->currentItem();
    if (!diseaseItem) {
        QMessageBox::information(this, "No Disease Selected", "Please select a disease to remove.");
        return;
    }
    
    int row = selection.first().row();
    Bacterium bacterium = m_tableModel->getBacterium(row);
    bacterium.removeDisease(diseaseItem->text());
    DataService::instance().updateBacterium(bacterium);
    updateDiseasesList();
}

void BiologistWindow::refreshBacteriaList()
{
    updateBacteriaDisplay();
}

void BiologistWindow::updateBacteriaDisplay()
{
    QList<Bacterium> bacteria;
    
    if (m_currentSpeciesFilter == "All Species" || m_currentSpeciesFilter.isEmpty()) {
        bacteria = DataService::instance().getBacteriaForBiologist(m_biologist.getName());
    } else {
        QList<Bacterium> allBacteria = DataService::instance().getBacteriaForBiologist(m_biologist.getName());
        for (const Bacterium& b : allBacteria) {
            if (b.getSpecies() == m_currentSpeciesFilter) {
                bacteria.append(b);
            }
        }
    }
    
    // Sort by name
    std::sort(bacteria.begin(), bacteria.end(), [](const Bacterium& a, const Bacterium& b) {
        return a.getName() < b.getName();
    });
    
    m_tableModel->setBacteria(bacteria);
}

void BiologistWindow::updateDiseasesList()
{
    m_diseasesList->clear();
    
    QModelIndexList selection = m_bacteriaTable->selectionModel()->selectedRows();
    if (!selection.isEmpty()) {
        int row = selection.first().row();
        Bacterium bacterium = m_tableModel->getBacterium(row);
        
        for (const QString& disease : bacterium.getDiseases()) {
            m_diseasesList->addItem(disease);
        }
    }
}