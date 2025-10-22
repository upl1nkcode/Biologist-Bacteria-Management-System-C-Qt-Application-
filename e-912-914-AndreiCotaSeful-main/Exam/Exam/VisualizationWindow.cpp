#include "VisualizationWindow.h"
#include "DataService.h"
#include <QScrollArea>
#include <QVBoxLayout>
#include <QPaintEvent>
#include <QFontMetrics>
#include <QRandomGenerator>

VisualizationWidget::VisualizationWidget(const Biologist& biologist, QWidget* parent)
    : QWidget(parent), m_biologist(biologist)
{
    // Group bacteria by species
    QList<Bacterium> bacteria = DataService::instance().getBacteriaForBiologist(m_biologist.getName());
    for (const Bacterium& bacterium : bacteria) {
        m_bacteriaBySpecies[bacterium.getSpecies()].append(bacterium);
    }
    
    setMinimumSize(800, 600);
}

void VisualizationWidget::paintEvent(QPaintEvent* event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    QRect availableRect = rect().adjusted(20, 20, -20, -20);
    int speciesCount = m_bacteriaBySpecies.size();
    
    if (speciesCount == 0) {
        painter.drawText(rect(), Qt::AlignCenter, "No bacteria to display");
        return;
    }
    
    // Calculate layout
    int cols = qMax(1, (int)qSqrt(speciesCount));
    int rows = (speciesCount + cols - 1) / cols;
    
    int speciesWidth = availableRect.width() / cols;
    int speciesHeight = availableRect.height() / rows;
    
    int speciesIndex = 0;
    for (auto it = m_bacteriaBySpecies.begin(); it != m_bacteriaBySpecies.end(); ++it) {
        int row = speciesIndex / cols;
        int col = speciesIndex % cols;
        
        QRect speciesRect(
            availableRect.x() + col * speciesWidth,
            availableRect.y() + row * speciesHeight,
            speciesWidth,
            speciesHeight
        );
        
        drawSpeciesGroup(painter, it.key(), it.value(), speciesRect);
        speciesIndex++;
    }
}

void VisualizationWidget::drawSpeciesGroup(QPainter& painter, const QString& species, 
                                         const QList<Bacterium>& bacteria, QRect& rect)
{
    // Draw species title
    painter.save();
    QFont titleFont = painter.font();
    titleFont.setBold(true);
    titleFont.setPointSize(12);
    painter.setFont(titleFont);
    
    QFontMetrics fm(titleFont);
    int titleHeight = fm.height() + 10;
    
    QRect titleRect = rect;
    titleRect.setHeight(titleHeight);
    
    painter.fillRect(titleRect, QColor(26, 28, 28));
    painter.drawRect(titleRect);
    painter.drawText(titleRect, Qt::AlignCenter, species);
    
    // Draw bacteria
    QRect bacteriaRect = rect;
    bacteriaRect.setTop(rect.top() + titleHeight);
    
    painter.setFont(QFont()); // Reset font
    
    // Calculate positions for bacteria
    int bacteriaCount = bacteria.size();
    if (bacteriaCount > 0) {
        int cols = qMax(1, (int)qSqrt(bacteriaCount));
        int rows = (bacteriaCount + cols - 1) / cols;
        
        int cellWidth = bacteriaRect.width() / cols;
        int cellHeight = bacteriaRect.height() / rows;
        
        for (int i = 0; i < bacteriaCount; ++i) {
            int row = i / cols;
            int col = i % cols;
            
            QPoint center(
                bacteriaRect.x() + col * cellWidth + cellWidth / 2,
                bacteriaRect.y() + row * cellHeight + cellHeight / 2
            );
            
            drawBacterium(painter, bacteria[i], center);
        }
    }
    
    painter.restore();
}

void VisualizationWidget::drawBacterium(QPainter& painter, const Bacterium& bacterium, const QPoint& center)
{
    painter.save();
    
    
    uint hash = qHash(bacterium.getName());
    bool isCircle = (hash % 2) == 0;// randomly choosing if it
    //is a circle or no based on the hash genereated from its name
    
    // Size based on bacterium size (scaled for display)
    int displaySize = qMax(20, qMin(60, bacterium.getSize() / 10));
    
    // Color based on species hash
    uint speciesHash = qHash(bacterium.getSpecies());//genrate a hash based on the species name to have random colors
    QColor color = QColor::fromHsv((speciesHash % 360), 150, 200);
    
    painter.setBrush(color);
    painter.setPen(QPen(Qt::black, 2));
    
    if (isCircle) {
        painter.drawEllipse(center, displaySize/2, displaySize/2);
    } else {
        QRect rect(center.x() - displaySize/2, center.y() - displaySize/2, displaySize, displaySize);
        painter.drawRect(rect);
    }
    
    // Draw bacterium name
    painter.setPen(Qt::white);
    QFont font = painter.font();
    font.setPointSize(8);
    painter.setFont(font);
    
    QRect textRect(center.x() - 50, center.y() + displaySize/2 + 5, 100, 20);
    painter.drawText(textRect, Qt::AlignCenter, bacterium.getName());
    
    painter.restore();
}

VisualizationWindow::VisualizationWindow(const Biologist& biologist, QWidget* parent)
    : QMainWindow(parent)
{
    setWindowTitle(QString("Bacteria Visualization - %1").arg(biologist.getName()));
    
    QScrollArea* scrollArea = new QScrollArea;
    m_visualizationWidget = new VisualizationWidget(biologist);
    scrollArea->setWidget(m_visualizationWidget);
    scrollArea->setWidgetResizable(true);
    
    setCentralWidget(scrollArea);
    resize(1000, 700);
}