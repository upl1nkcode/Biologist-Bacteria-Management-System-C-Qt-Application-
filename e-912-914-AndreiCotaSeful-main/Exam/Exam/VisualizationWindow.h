#ifndef VISUALIZATIONWINDOW_H
#define VISUALIZATIONWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QPainter>
#include <QMap>
#include "Biologist.h"
#include "Bacterium.h"

class VisualizationWidget : public QWidget
{
    Q_OBJECT
    
public:
    explicit VisualizationWidget(const Biologist& biologist, QWidget* parent = nullptr);
    
protected:
    void paintEvent(QPaintEvent* event) override;
    
private:
    void drawSpeciesGroup(QPainter& painter, const QString& species, 
                         const QList<Bacterium>& bacteria, QRect& rect);
    void drawBacterium(QPainter& painter, const Bacterium& bacterium, const QPoint& center);
    
    Biologist m_biologist;
    QMap<QString, QList<Bacterium>> m_bacteriaBySpecies;
};

class VisualizationWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit VisualizationWindow(const Biologist& biologist, QWidget* parent = nullptr);
    
private:
    VisualizationWidget* m_visualizationWidget;
};

#endif 