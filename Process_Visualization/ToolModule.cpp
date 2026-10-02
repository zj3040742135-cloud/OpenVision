#include "ToolModule.h"

ToolModule::ToolModule()
{
	m_tool = new ToolBlockTool();

}

ToolModule::~ToolModule()
{}

void ToolModule::Show()
{
	ToolBlockControl* tool = new ToolBlockControl();
	tool->BindingTool(m_tool);
	tool->show();
}

void ToolModule::Run()
{
	m_tool->Run(RunSucesses);
}

