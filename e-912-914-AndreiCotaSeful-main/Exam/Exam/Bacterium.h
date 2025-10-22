#ifndef BACTERIUM_H
#define BACTERIUM_H

#include <QString>
#include <QStringList>

class Bacterium
{
public:
    Bacterium() = default;
    Bacterium(const QString& name, const QString& species, int size, const QStringList& diseases);
    
    QString getName() const { return m_name; }
    void setName(const QString& name) { m_name = name; }
    
    QString getSpecies() const { return m_species; }
    void setSpecies(const QString& species) { m_species = species; }
    
    int getSize() const { return m_size; }
    void setSize(int size) { m_size = size; }
    
    QStringList getDiseases() const { return m_diseases; }
    void setDiseases(const QStringList& diseases) { m_diseases = diseases; }
    void addDisease(const QString& disease);
    void removeDisease(const QString& disease);
    
    QString getKey() const { return m_name + "|" + m_species; }// key will be name|species

    
private:
    QString m_name;
    QString m_species;
    int m_size = 0;
    QStringList m_diseases;
};

#endif 