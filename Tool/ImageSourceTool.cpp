#include "pch.h"
#include "ImageSourceTool.h"
#include<iostream>
ImageSourceTool::ImageSourceTool()
{

	AddInput("OutImage", &this->OutImage.width, VarType::TYPE_IMAGE);
	AddOutput("OutImage", &this->OutImage.width, VarType::TYPE_INT);
}

ImageSourceTool::~ImageSourceTool()
{
}

bool ImageSourceTool::Run(ToolResult& _toolresult)
{
	OutImage.width++;
	return false;
}

nlohmann::json ImageSourceTool::ToJson() const
{
	return nlohmann::json();
}

bool ImageSourceTool::FromJson(const nlohmann::json& j)
{
	return false;
}

bool ImageSourceTool::AddInput(std::string VarName, void* var,  VarType type)
{
	ToolInOutVar* input=new ToolInOutVar();
	input->value = var;
	input->Binding = false;
	input->ToolVarType = type;
	Inputs[VarName] = input;
	return true;
}

bool ImageSourceTool::AddOutput(std::string VarName, void* var, VarType type)
{
	ToolInOutVar* output = new ToolInOutVar();
	output->value = var;
	output->Binding = false;
	output->ToolVarType = type;
	Outputs[VarName] = output;
	return true;
}
