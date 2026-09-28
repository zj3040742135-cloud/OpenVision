#pragma once
#ifdef TOOLBASE_EXPORTS
#define TOOLBASE_API __declspec(dllexport)
#else
#define TOOLBASE_API __declspec(dllimport)
#endif
#include<string>
#include<map>
#include"ToolImage.h"
#include"ToolResult.h"
#include<functional>
#include"nlohmann/json.hpp"
enum AlgorithmType
{
	OPENCV,
	HALCON
};
enum VarType
{
	TYPE_UNDEFINED,
	TYPE_STRING,
	TYPE_BOOL,
	TYPE_INT,
	TYPE_DOUBLE,
	TYPE_IMAGE,
	TYPE_REGION,
	TYPE_LINE,
	TYPE_POINT,
	TYPE_LIST_STRING,
	TYPE_LIST_BOOL,
	TYPE_LIST_INT,
	TYPE_LIST_DOUBLE,
	TYPE_LIST_REGION,
	TYPE_LIST_LINE,
	TYPE_LIST_POINT
};
enum ToolType
{
	UNDEFINED = 0,
	ImageSourceTool = 1,
	FixtureTool = 2,
	BlobTool = 3,
	ToolBlockTool = 4
};
struct ToolInOutVar
{
	std::string BindingName = "";
	void* value = nullptr;
	bool Binding = false;
	VarType ToolVarType = TYPE_UNDEFINED;
	ToolInOutVar* BindingInfo = nullptr;
};

class TOOLBASE_API ToolInterface
{
public:
	std::string ToolName;
	ToolInterface* OWner;
	ToolType m_toolType = UNDEFINED; 
	AlgorithmType algorithmType = AlgorithmType::OPENCV;
	std::map<std::string, ToolInOutVar*> Inputs;
	std::map<std::string, ToolInOutVar*> Outputs;
	ToolResult Result;
	ToolImage InputImage;
public:
	virtual bool  Run(ToolResult& _toolresult) = 0;
	virtual void SaveToVpp(std::string file) = 0;
	virtual void LoadFromVpp(std::string file) = 0;
	virtual void Save() = 0;
	virtual void Load() = 0;
	virtual bool AddInput(std::string VarName, ToolInOutVar var) = 0;
	virtual bool AddOutput(std::string VarName, ToolInOutVar var) = 0;
	virtual nlohmann::json ToJson() const = 0;
	virtual bool FromJson(const nlohmann::json& j) = 0;
public:
	ToolInterface();
	virtual ~ToolInterface();
	void SetToolSDK(AlgorithmType sdkType);
};


using ToolCreator = std::function<ToolInterface* ()>;

class TOOLBASE_API ToolFactory
{
public:
	static ToolFactory& Instance();

	void Register(ToolType type, ToolCreator creator)
	{
		m_typeMap[type] = creator;
	}

	ToolInterface* Create(ToolType type)
	{
		auto it = m_typeMap.find(type);
		if (it == m_typeMap.end()) return nullptr;
		return it->second();
	}

private:
	std::map<ToolType, ToolCreator> m_typeMap;
	ToolFactory() = default;
};

#define REGISTER_TOOL(TOOLTYPE_ENUM, TOOL_CLASS) \
    static bool reg_##TOOL_CLASS = [](){ \
        ToolFactory::Instance().Register(TOOLTYPE_ENUM, [](){ \
            return new class TOOL_CLASS(); \
        }); \
        return true; \
    }();

