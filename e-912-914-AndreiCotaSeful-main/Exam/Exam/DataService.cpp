#include "DataService.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>

DataService& DataService::instance()
{
    static DataService instance;
    return instance;
}

bool DataService::loadBiologists(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open file for biologists:" << filename;
        return false;
    }
    
    QTextStream in(&file);
    m_biologists.clear();
    
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        
        QStringList parts = line.split(':');
        if (parts.size() >= 2) {
            QString name = parts[0].trimmed();
            QStringList species = parts[1].split(',');
            for (QString& s : species) {
                s = s.trimmed();
            }
            m_biologists.append(Biologist(name, species));
        }
    }
    
    return true;
}

bool DataService::loadBacteria(const QString& filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        qDebug() << "Failed to open bacteria file:" << filename;
        return false;
    }
    
    QTextStream in(&file);
    m_bacteria.clear();
    
    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;
        
        QStringList parts = line.split('|');//separating with |
        if (parts.size() >= 4) {
            QString name = parts[0].trimmed();
            QString species = parts[1].trimmed();
            int size = parts[2].toInt();
            QStringList diseases = parts[3].split(',');//separate with , for diseases
            for (QString& d : diseases) {
                d = d.trimmed();
            }
            
            Bacterium bacterium(name, species, size, diseases);
            m_bacteria[bacterium.getKey()] = bacterium;
        }
    }
    
    return true;
}

bool DataService::saveBacteria(const QString& filename)
{   // savin the bacteria to a file
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        qDebug() << "Cannot save bacteria file:" << filename;
        return false;
    }
    
    QTextStream out(&file);
    for (const Bacterium& bacterium : m_bacteria.values()) {
        out << bacterium.getName() << "|"
            << bacterium.getSpecies() << "|"
            << bacterium.getSize() << "|"
            << bacterium.getDiseases().join(",") << "\n";
    }
    
    return true;
}

QList<Bacterium> DataService::getBacteriaForBiologist(const QString& biologistName) const
{
    QList<Bacterium> result;
    
    // Find the biologist 
    Biologist targetBiologist;
    bool found = false;
    for (const Biologist& bio : m_biologists) {
        if (bio.getName() == biologistName) {
            targetBiologist = bio;
            found = true;
            break;
        }
    }
    
    if (!found) return result;
    
    // Get bacteria for species studied by this biologist
    for (const Bacterium& bacterium : m_bacteria.values()) {
        if (targetBiologist.studiesSpecies(bacterium.getSpecies())) {
            result.append(bacterium);
        }
    }
    
    return result;
}

QList<Bacterium> DataService::getBacteriaBySpecies(const QString& species) const
{
    QList<Bacterium> result;
    for (const Bacterium& bacterium : m_bacteria.values()) {
        if (bacterium.getSpecies() == species) {
            result.append(bacterium);
        }
    }
    return result;
}

QStringList DataService::getAllSpecies() const
{
    QStringList species;
    for (const Bacterium& bacterium : m_bacteria.values()) {
        if (!species.contains(bacterium.getSpecies())) {
            species.append(bacterium.getSpecies());
        }
    }
    species.sort();
    return species;
}

bool DataService::addBacterium(const Bacterium& bacterium)
{
    QString key = bacterium.getKey();
    if (m_bacteria.contains(key)) {
        return false; // Already exists
    }
    
    m_bacteria[key] = bacterium;
    emit bacteriumAdded(bacterium);/// this like the other methods emits the signal so all windows stay updated
    return true;
}

bool DataService::updateBacterium(const Bacterium& bacterium)
{
    QString key = bacterium.getKey();
    if (!m_bacteria.contains(key)) {
        return false; // Doesn't exist
    }
    
    m_bacteria[key] = bacterium;
    emit bacteriumUpdated(bacterium);
    return true;
}

bool DataService::bacteriumExists(const QString& name, const QString& species) const
{
    QString key = name + "|" + species;
    return m_bacteria.contains(key);
}