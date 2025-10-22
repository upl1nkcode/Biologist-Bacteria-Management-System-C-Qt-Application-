#ifndef DATASERVICE_H
#define DATASERVICE_H

#include <QObject>
#include <QList>
#include <QMap>
#include "Biologist.h"
#include "Bacterium.h"

class DataService : public QObject
{
    Q_OBJECT
    
public:
    static DataService& instance();
    
    bool loadBiologists(const QString& filename);
    bool loadBacteria(const QString& filename);
    bool saveBacteria(const QString& filename);
    //methods for service, ties bacteria with the repssective studies of a biologist
    QList<Biologist> getBiologists() const { return m_biologists; }
    QList<Bacterium> getBacteria() const { return m_bacteria.values(); }

    QList<Bacterium> getBacteriaForBiologist(const QString& biologistName) const;
    QList<Bacterium> getBacteriaBySpecies(const QString& species) const;
    QStringList getAllSpecies() const;
    ///adding to qList every attriubute 

    bool addBacterium(const Bacterium& bacterium);
    bool updateBacterium(const Bacterium& bacterium);
    bool bacteriumExists(const QString& name, const QString& species) const;
    
signals://for live updating 
    void bacteriumAdded(const Bacterium& bacterium);
    void bacteriumUpdated(const Bacterium& bacterium);
    void bacteriumRemoved(const QString& name, const QString& species);
    
private:
    DataService() = default;
    QList<Biologist> m_biologists;
    QMap<QString, Bacterium> m_bacteria; // key: name|species
};

#endif 