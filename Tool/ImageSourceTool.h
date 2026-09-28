#pragma once
#ifdef TOOL_EXPORTS
#define TOOL_API __declspec(dllexport)
#else
#define TOOL_API __declspec(dllexport)
#endif
#include"ToolInterface.h"
#include"nlohmann/json.hpp"
#include"opencv2/opencv.hpp"
class TOOL_API ImageSourceTool:public ToolInterface
{
public:
	ImageSourceTool();
	~ImageSourceTool();
	ImageSourceTool(const ImageSourceTool&) = delete;
	ImageSourceTool& operator=(const ImageSourceTool&) = delete;
	ImageSourceTool(ImageSourceTool&&) = default;
	ImageSourceTool& operator=(ImageSourceTool&&) = default;
public:
	ToolImage OutImage;
public:
	bool Run(ToolResult& _toolresult) override;
	
	nlohmann::json ToJson() const override;
	bool FromJson(const nlohmann::json& j) override;
public:
	bool AddInput(std::string VarName, void* var, VarType type) ;
	bool AddOutput(std::string VarName, void* var, VarType type) ;
};

