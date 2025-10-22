#include "MainController.h"
#include "DataService.h"
#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QFile>
MainController::MainController(QObject* parent)
    : QObject(parent)
{
    // Connect to application quit signal to save data
    connect(qApp, &QApplication::aboutToQuit, this, &MainController::saveDataOnExit);
}

MainController::~MainController()
{
    qDeleteAll(m_biologistWindows);//destructor 
}

void MainController::initialize()
{
    loadData();
    createBiologistWindows();//creates the windows for my biologists
}

void MainController::loadData()
{
    DataService& dataService = DataService::instance();
    
    // Load biologists
    if (!dataService.loadBiologists("biologists.txt")) {
        qDebug() << "Could not load biologists file. Creating sample data.";
        // Create sample data file
        QFile file("biologists.txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << "Dr. Kurt Cobain: Salmonella\n";
            out << "Dr. M. Shadows: Streptococcus, Staphylococcus\n";
            out << "Dr. House: E.coli, Bacillus, Streptococcus, Staphylococcus\n";
        }
        dataService.loadBiologists("biologists.txt");
    }
    
    // Load bacteria
    if (!dataService.loadBacteria("bacteria.txt")) {
        qDebug() << "Could not load bacteria file. Creating sample data.";
        // Create sample data file
        QFile file("bacteria.txt");
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            QTextStream out(&file);
            out << "E.coli O157|E.coli|200|Food poisoning, Diarrhea\n";
            out << "E.coli K12|E.coli|180|Laboratory strain\n";
            out << "Salmonella enterica|Salmonella|300|Typhoid fever, Gastroenteritis\n";
            out << "Streptococcus pyogenes|Streptococcus|100|Strep throat, Scarlet fever\n";
            out << "Staphylococcus aureus|Staphylococcus|120|Skin infections, Food poisoning\n";
            out << "Bacillus subtilis|Bacillus|250|Generally harmless\n";
        }
        dataService.loadBacteria("bacteria.txt");
    }
}

void MainController::createBiologistWindows()
{
    QList<Biologist> biologists = DataService::instance().getBiologists();
    
    if (biologists.isEmpty()) {
        QMessageBox::warning(nullptr, "No Data", "No biologists found in the data file.");
        return;
    }
    
    for (const Biologist& biologist : biologists) {
        BiologistWindow* window = new BiologistWindow(biologist);
        window->show();
        m_biologistWindows.append(window);
    }
}

void MainController::saveDataOnExit()
{
    DataService::instance().saveBacteria("bacteria.txt");
    qDebug() << "Data saved successfully.";
}