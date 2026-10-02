#pragma once

#include <QMainWindow>
#include <QStandardItemModel>
#include <QCloseEvent>
#include "ToolBlockControl_global.h"
#include "ui_ToolBlockControl.h"
#include"ToolBlockTool.h"
class TOOLBLOCKCONTROL_EXPORT ToolBlockControl : public QMainWindow
{
	Q_OBJECT

public:
	ToolBlockControl(QWidget *parent = nullptr);
	~ToolBlockControl();
protected:
	// 关闭窗口时取消已注册的回调，避免工具持有悬空回调
	void closeEvent(QCloseEvent* event) override;
public:
	void BindingTool(ToolBlockTool* tool);
private:
	void UpdateTreeView();
private:
	Ui::ToolBlockControlClass ui;
	QStandardItemModel* m_treeModel = nullptr;
	ToolBlockTool* m_tool = nullptr;
};

