#pragma once
#ifdef TOOLBLOCKTOOL_EXPORTS
#define TOOLBLOCKTOOL_API __declspec(dllexport)
#else
#define TOOLBLOCKTOOL_API __declspec(dllimport)
#endif
#include"ToolInterface.h"
#include<map>
#include<string>
#include<vector>
#include<memory>
#include"nlohmann/json.hpp"
struct ConnectionsInfo
{
	std::string FromToolName;
	std::string FormToolNodeName;
	std::string ToToolName;
	std::string ToToolNodeName;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(ConnectionsInfo,
		FromToolName,
		FormToolNodeName,
		ToToolName,
		ToToolNodeName
	)
};

using ConnectionsList = std::vector<ConnectionsInfo>;
class TOOLBLOCKTOOL_API ToolBlockTool :public ToolInterface
{
public:
	ToolBlockTool();
	~ToolBlockTool();
	ToolBlockTool(const ToolBlockTool&) = delete;
	ToolBlockTool& operator=(const ToolBlockTool&) = delete;
	ToolBlockTool(ToolBlockTool&&) = default;
	ToolBlockTool& operator=(ToolBlockTool&&) = default;
public:
	std::map<std::string, std::unique_ptr <ToolInterface>> Tools;
	std::vector<ConnectionsInfo> ToolConnections;

public:
	bool Run(ToolResult& _toolresult) override;
	nlohmann::json ToJson() const override;
	bool FromJson(const nlohmann::json& j) override;
public:
	bool AddTool(ToolType _type);
	bool RemoveTool(std::string _toolName);
	bool AddConnection(std::string FromTool, std::string FromToolNode, std::string ToTool, std::string ToToolNode);
	bool HasConnection(const ConnectionsInfo info);
	bool RemoveConnection(std::string const _toolName);
	
private:
	int CreateToolName(ToolType _type);
	std::string CanToolName(std::string name, int i);
public:
	void SaveToVpp(std::string file);
	void LoadFromVpp(std::string file);
	void Save();
	void Load();
	bool AddInput(std::string VarName, void* var,VarType type);
	bool AddOutput(std::string VarName, void* var, VarType type);
public:
	using CallBack = std::function<void()>;
	void RegistCallBack(CallBack cb);
	void ClearCallBack();
private:
	CallBack m_cb;
	void TriggerEvent();

};
