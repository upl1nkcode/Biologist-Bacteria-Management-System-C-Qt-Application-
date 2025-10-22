#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_Exam.h"

class Exam : public QMainWindow
{
    Q_OBJECT

public:
    Exam(QWidget *parent = nullptr);
    ~Exam();

private:
    Ui::ExamClass ui;
};

