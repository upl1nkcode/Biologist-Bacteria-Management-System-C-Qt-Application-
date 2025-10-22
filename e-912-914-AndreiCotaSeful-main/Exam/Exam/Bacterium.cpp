#include "Bacterium.h"

Bacterium::Bacterium(const QString& name, const QString& species, int size, const QStringList& diseases)
    : m_name(name), m_species(species), m_size(size), m_diseases(diseases)
{
}

void Bacterium::addDisease(const QString& disease)///apending to the field of disesases
{
    if (!m_diseases.contains(disease)) {
        m_diseases.append(disease);
    }
}

void Bacterium::removeDisease(const QString& disease)///removed all diseases
{
    m_diseases.removeAll(disease);
}