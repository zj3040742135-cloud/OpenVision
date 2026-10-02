#pragma once

#include <QObject>
#include"ModuleBase.h"
#include"ToolBlockControl.h"
#include"ToolBlockTool.h"
class ToolModule  : public ModuleBase
{
	Q_OBJECT

public:
	ToolModule();
	~ToolModule();
public:
	ToolBlockTool* m_tool = nullptr;
	ToolResult RunSucesses;
public:
	 void Show() override;
	 void Run()override;
};

