#include "pch.h"
#include "ImageSourceTool.h"

ImageSourceTool::ImageSourceTool()
{
}

ImageSourceTool::~ImageSourceTool()
{
}

bool ImageSourceTool::Run(ToolResult& _toolresult)
{
	return false;
}

void ImageSourceTool::SaveToVpp(std::string file)
{
}

void ImageSourceTool::LoadFromVpp(std::string file)
{
}

void ImageSourceTool::Save()
{
}

void ImageSourceTool::Load()
{
}

bool ImageSourceTool::AddInput(std::string VarName, ToolInOutVar var)
{
	return false;
}

bool ImageSourceTool::AddOutput(std::string VarName, ToolInOutVar var)
{
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
