#ifndef ADDBACTERIUMDIALOG_H
#define ADDBACTERIUMDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QComboBox>
#include <QSpinBox>
#include <QTextEdit>
#include <QPushButton>
#include "Biologist.h"

class AddBacteriumDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AddBacteriumDialog(const Biologist& biologist, QWidget* parent = nullptr);
    
private slots:
    void onAccept();
    
private:
    void setupUI();
    
    Biologist m_biologist;
    
    QLineEdit* m_nameEdit;
    QComboBox* m_speciesCombo;
    QSpinBox* m_sizeSpin;
    QTextEdit* m_diseasesEdit;
    QPushButton* m_okBtn;
    QPushButton* m_cancelBtn;
};

#endif // ADDBACTERIUMDIALOG_H