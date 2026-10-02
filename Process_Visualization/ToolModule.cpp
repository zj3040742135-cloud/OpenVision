#include "ToolModule.h"

ToolModule::ToolModule()
{}

ToolModule::~ToolModule()
{}

void ToolModule::Show()
{
	ToolBlockControl* tool = new ToolBlockControl();
	tool->show();
}

