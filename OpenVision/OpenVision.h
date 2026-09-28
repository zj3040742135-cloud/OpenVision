#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OpenVision.h"

class OpenVision : public QMainWindow
{
    Q_OBJECT

public:
    OpenVision(QWidget *parent = nullptr);
    ~OpenVision();

private:
    Ui::OpenVisionClass ui;
};

