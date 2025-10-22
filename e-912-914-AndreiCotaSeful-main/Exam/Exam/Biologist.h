#ifndef BIOLOGIST_H
#define BIOLOGIST_H

#include <QString>
#include <QStringList>

class Biologist
{
public:
    Biologist() = default;
    Biologist(const QString& name, const QStringList& species);//constuctor
    
    QString getName() const { return m_name; }
    void setName(const QString& name) { m_name = name; }
    
    /// these are the stuff for ading species and stuff also the studies
    
    QStringList getSpecies() const { return m_species; }
    void setSpecies(const QStringList& species) { m_species = species; }
    void addSpecies(const QString& species);
    bool studiesSpecies(const QString& species) const;
    
private:
    QString m_name;
    QStringList m_species;//parameters
};

#endif 