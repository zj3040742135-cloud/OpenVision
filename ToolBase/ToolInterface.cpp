#include "pch.h"
#include "ToolInterface.h"

ToolInterface::ToolInterface()
{
}

ToolInterface::~ToolInterface()
{
}

void ToolInterface::SetToolSDK(AlgorithmType sdkType)
{
}
ToolFactory& ToolFactory::Instance()
{
	static ToolFactory factory;
	return factory;
}