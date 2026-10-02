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
	 void Show() override;
};

