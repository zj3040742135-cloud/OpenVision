#pragma once

#include <QMainWindow>
#include <QStandardItemModel>
#include "ToolBlockControl_global.h"
#include "ui_ToolBlockControl.h"

class TOOLBLOCKCONTROL_EXPORT ToolBlockControl : public QMainWindow
{
	Q_OBJECT

public:
	ToolBlockControl(QWidget *parent = nullptr);
	~ToolBlockControl();

private:
	Ui::ToolBlockControlClass ui;
	QStandardItemModel* m_treeModel = nullptr;
};

