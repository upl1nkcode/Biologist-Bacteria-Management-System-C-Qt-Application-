#include "Biologist.h"

Biologist::Biologist(const QString& name, const QStringList& species)
    : m_name(name), m_species(species)//constructor for the biologist
{
}

void Biologist::addSpecies(const QString& species)
{
    if (!m_species.contains(species)) {
        m_species.append(species);// implementation for addiing a species for biologist to analize

    }
}

bool Biologist::studiesSpecies(const QString& species) const
{
    return m_species.contains(species);///what species should be studied
}