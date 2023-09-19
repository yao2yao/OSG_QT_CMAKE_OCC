#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_QTTEST.h"

class QTTEST : public QMainWindow
{
    Q_OBJECT

public:
    QTTEST(QWidget *parent = Q_NULLPTR);

public:
    Ui::QTTESTClass ui;
};
