#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QList>
#include "BiologistWindow.h"
#include "Biologist.h"

class MainController : public QObject
{
    Q_OBJECT
    
public:
    explicit MainController(QObject* parent = nullptr);
    ~MainController();
    
    void initialize();
    
private:
    void loadData();
    void createBiologistWindows();
    void saveDataOnExit();
    
    QList<BiologistWindow*> m_biologistWindows;
};

#endif // MAINCONTROLLER_H