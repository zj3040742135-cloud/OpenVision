#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_OpenVision.h"
#include"ImageSourceTool.h"

class OpenVision : public QMainWindow
{
    Q_OBJECT

public:
    OpenVision(QWidget *parent = nullptr);
    ~OpenVision();
    ImageSourceTool* tool;
    ImageSourceTool* t ;
private:
    Ui::OpenVisionClass ui;
    void text();
};

