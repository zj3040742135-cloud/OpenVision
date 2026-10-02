#include "ToolBlockControl.h"
#include <QStandardItem>

ToolBlockControl::ToolBlockControl(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	m_tool = new ToolBlockTool();
	// Build the tool tree with three top-level categories
	m_treeModel = new QStandardItemModel(this);
	m_treeModel->setHorizontalHeaderLabels(QStringList() << "Name");

	for (const auto& name : { "输入", "工具", "输出" })
	{
		QStandardItem* item = new QStandardItem(name);
		item->setEditable(false);
		m_treeModel->appendRow(item);
	}
	ui.TooltreeView->setModel(m_treeModel);
	ui.TooltreeView->setHeaderHidden(true);
}

ToolBlockControl::~ToolBlockControl()
{
	m_tool->ClearCallBack();
}

void ToolBlockControl::BindingTool(ToolBlockTool * tool)
{
	if (m_tool && m_tool != tool)
	{
		delete m_tool;
	}
	m_tool = tool;
	m_tool->RegistCallBack(std::bind(&ToolBlockControl::UpdateTreeView, this));
}

void ToolBlockControl::closeEvent(QCloseEvent* event)
{
	if (m_tool)
	{
		m_tool->ClearCallBack();
	}
	QMainWindow::closeEvent(event);
}

void ToolBlockControl::UpdateTreeView()
{
	//ui.TooltreeView->update();
	qDebug() << "回调触发" << "\r\n";
}

