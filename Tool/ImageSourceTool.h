#pragma once
#ifdef TOOL_EXPORTS
#define TOOL_API __declspec(dllexport)//dllimport
#else
#define IMAGESOURCETOOL_API __declspec(dllexport)//dllexport
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
	void SaveToVpp(std::string file) override;
	void LoadFromVpp(std::string file) override;
	void Save() override;
	void Load() override;
	bool AddInput(std::string VarName, ToolInOutVar var) override;
	bool AddOutput(std::string VarName, ToolInOutVar var) override;
	nlohmann::json ToJson() const override;
	bool FromJson(const nlohmann::json& j) override;
};

