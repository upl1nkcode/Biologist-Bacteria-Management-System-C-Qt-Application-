#include "AddBacteriumDialog.h"
#include "DataService.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QMessageBox>

AddBacteriumDialog::AddBacteriumDialog(const Biologist& biologist, QWidget* parent)
    : QDialog(parent), m_biologist(biologist)
{
    setWindowTitle("Add New Bacterium");
    setupUI();
}

void AddBacteriumDialog::setupUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    // Form layout
    QFormLayout* formLayout = new QFormLayout;
    
    m_nameEdit = new QLineEdit;
    formLayout->addRow("Name:", m_nameEdit);
    
    m_speciesCombo = new QComboBox;
    m_speciesCombo->setEditable(true);
    for (const QString& species : m_biologist.getSpecies()) {
        m_speciesCombo->addItem(species);
    }
    formLayout->addRow("Species:", m_speciesCombo);
    
    m_sizeSpin = new QSpinBox;
    m_sizeSpin->setRange(1, 10000);
    m_sizeSpin->setSuffix(" nm");
    formLayout->addRow("Size:", m_sizeSpin);
    
    m_diseasesEdit = new QTextEdit;
    m_diseasesEdit->setMaximumHeight(100);
    m_diseasesEdit->setPlaceholderText("Enter diseases separated by commas");
    formLayout->addRow("Diseases:", m_diseasesEdit);
    
    mainLayout->addLayout(formLayout);
    
    // Buttons
    QHBoxLayout* buttonLayout = new QHBoxLayout;
    buttonLayout->addStretch();
    
    m_cancelBtn = new QPushButton("Cancel");
    connect(m_cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    buttonLayout->addWidget(m_cancelBtn);
    
    m_okBtn = new QPushButton("Add Bacterium");
    m_okBtn->setDefault(true);
    connect(m_okBtn, &QPushButton::clicked, this, &AddBacteriumDialog::onAccept);
    buttonLayout->addWidget(m_okBtn);
    
    mainLayout->addLayout(buttonLayout);
    
    resize(400, 300);
}

void AddBacteriumDialog::onAccept()
{
    QString name = m_nameEdit->text().trimmed();
    QString species = m_speciesCombo->currentText().trimmed();
    int size = m_sizeSpin->value();
    
    if (name.isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a bacterium name.");
        return;
    }
    
    if (species.isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "Please enter a species.");
        return;
    }
    
    // Check if bacterium already exists
    if (DataService::instance().bacteriumExists(name, species)) {
        QMessageBox::warning(this, "Duplicate Bacterium", 
                           "A bacterium like this already exists.");
        return;
    }
    
    // Check if biologist studies this species
    if (!m_biologist.studiesSpecies(species)) {
        QMessageBox::warning(this, "Invalid Species", 
                           "This biologist doesn't study this.");
        return;
    }
    
    
    QStringList diseases;
    QString diseasesText = m_diseasesEdit->toPlainText();
    if (!diseasesText.isEmpty()) {
        diseases = diseasesText.split(',');
        for (QString& disease : diseases) {
            disease = disease.trimmed();
        }
        diseases.removeAll("");
    }
    
    
    Bacterium bacterium(name, species, size, diseases);
    if (DataService::instance().addBacterium(bacterium)) {
        accept();
    } else {
        QMessageBox::critical(this, "Error", "Failed to add!");
    }
}