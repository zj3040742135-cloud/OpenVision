#include "ToolBlockControl.h"
#include <QStandardItem>

ToolBlockControl::ToolBlockControl(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

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
{}

