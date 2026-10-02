#include "pch.h"
#include "ToolBlockTool.h"
#include<fstream>
REGISTER_TOOL(ToolType::Tool_ToolBlockTool, ToolBlockTool);
ToolBlockTool::ToolBlockTool()
{
	ToolName = "ToolBlock";
	this->m_toolType = ToolType::Tool_ToolBlockTool;
	OWner = nullptr;
	Inputs.clear();
	Outputs.clear();
	Tools.clear();
	ToolConnections.clear();

}
ToolBlockTool::~ToolBlockTool()
{
	OWner = nullptr;
	Inputs.clear();
	Outputs.clear();
	Tools.clear();
	ToolConnections.clear();
}
bool ToolBlockTool::Run(ToolResult& _toolresult)
{
	ToolResult result;
	bool isSucesses = true;
	for (const auto& item : Tools)
	{
		const auto& tool = item.second;
		bool ToolisSucesses = tool->Run(result);
		isSucesses = ToolisSucesses && isSucesses;
		this->Result.message += result.message;
		this->Result.runTime += result.runTime;
		this->Result.result = isSucesses ? ResultType::TYPE_SUCESSES : ResultType::TYPE_ERROR;
	}
	TriggerEvent();
	return isSucesses;
}
void ToolBlockTool::SaveToVpp(std::string filePath)
{

	std::ofstream ofs{ (filePath) };
	if (!ofs.is_open())
	{
		return;
	}
	ofs << ToJson().dump(4);
	ofs.close();
}
void ToolBlockTool::LoadFromVpp(std::string file)
{
	Tools.clear();
	TriggerEvent();
}
void ToolBlockTool::Save()
{
	std::wstring filePath;
	std::ofstream ofs{ (filePath) };
	if (!ofs.is_open())
	{
		return;
	}
	ofs << ToJson().dump(4);
	ofs.close();
}
void ToolBlockTool::Load()
{
	TriggerEvent();
}
bool ToolBlockTool::AddInput(std::string VarName, void* var, VarType type)
{
	TriggerEvent();
	return true;
}
bool ToolBlockTool::AddOutput(std::string VarName, void* var, VarType type)
{
	TriggerEvent();
	return true;
}
void ToolBlockTool::RegistCallBack(CallBack cb)
{
	m_cb = std::move(cb);
}
void ToolBlockTool::ClearCallBack()
{
	m_cb = nullptr;
}
void ToolBlockTool::TriggerEvent()
{
	if (m_cb)
		m_cb();
}
nlohmann::json ToolBlockTool::ToJson() const
{
	nlohmann::json j;
	nlohmann::json arrTools = nlohmann::json::array();
	for (const auto& item : Tools)
	{
		const std::string& toolKey = item.first;
		const auto& pTool = item.second;
		nlohmann::json toolJson = pTool->ToJson();
		toolJson["MapKey"] = toolKey;
		arrTools.push_back(toolJson);
	}
	j["Tools"] = arrTools;
	j["Connections"] = this->ToolConnections;
	return j;
}
bool ToolBlockTool::FromJson(const nlohmann::json& j)
{
	auto arrTools = j["Tools"];
	for (auto& elem : arrTools)
	{
		std::string key = elem["MapKey"].get<std::string>();
		ToolType tType = static_cast<ToolType>(elem["ToolType"].get<int>());
		ToolInterface* pTool = ToolFactory::Instance().Create(tType);
		if (!pTool) continue;
		std::unique_ptr< ToolInterface> t(pTool);
		pTool->FromJson(elem);
		Tools[key] = std::move(t);
	}
	ToolConnections = j["Connections"].get<decltype(ToolConnections)>();
	return false;
}
bool ToolBlockTool::AddTool(ToolType _type)
{
	ToolInterface* pTool = ToolFactory::Instance().Create(_type);
	if (!pTool)
		return false;
	pTool->ToolName = CanToolName(pTool->ToolName, CreateToolName(_type));
	std::unique_ptr< ToolInterface> t(pTool);
	Tools[pTool->ToolName] = std::move(t);
	TriggerEvent();
	return true;
}
bool ToolBlockTool::RemoveTool(std::string const _toolName)
{
	if (!Tools.contains(_toolName))
		return false;
	Tools.erase(_toolName);
	TriggerEvent();
	return true;
}
bool ToolBlockTool::AddConnection(std::string FromTool, std::string FromToolNode, std::string ToTool, std::string ToToolNode)
{
	if (!Tools.contains(FromTool) || !Tools.contains(ToTool))
		return false;
	const auto& SourceTool = Tools[FromTool];
	const auto& TargetTool = Tools[ToTool];
	if (!SourceTool->Outputs.contains(FromToolNode) || !TargetTool->Inputs.contains(ToToolNode))
		return false;
	ConnectionsInfo conn;
	conn.FromToolName = FromTool;
	conn.FormToolNodeName = FromToolNode;
	conn.ToToolName = ToTool;
	conn.ToToolNodeName = ToToolNode;
	bool isDuplicate = HasConnection(conn);
	if (isDuplicate)
		return false;
	ToolConnections.push_back(conn);
	TargetTool->Inputs[ToToolNode]->BindingInfo = SourceTool->Outputs[FromToolNode];
	TargetTool->Inputs[ToToolNode]->Binding = true;
	TriggerEvent();
	return true;
}
bool ToolBlockTool::HasConnection(const ConnectionsInfo info)
{
	for (const ConnectionsInfo& item : ToolConnections)
	{
		if (item.FromToolName == info.FromToolName && item.FormToolNodeName == info.FormToolNodeName && item.ToToolName == info.ToToolName && item.ToToolNodeName == info.ToToolNodeName)
		{
			return true;
		}
	}
	return false;
}
bool ToolBlockTool::RemoveConnection(std::string const _toolName)
{
	/*std::vector<ConnectionsInfo> toolConnections;
	for (ConnectionsInfo item : ToolConnections)
	{
	if (item.FromToolName == _toolName || item.ToToolName == _toolName)
	{
	toolConnections.push_back(item);
	}
	}*/
	for (auto it = ToolConnections.begin(); it != ToolConnections.end(); )
	{
		if (it->FromToolName == _toolName || it->ToToolName == _toolName)
		{
			it = ToolConnections.erase(it);
		}
		else
		{
			++it;
		}
	}
	TriggerEvent();
	return true;
}
int ToolBlockTool::CreateToolName(ToolType _type)
{
	int i = 1;
	std::string name;
	for (const auto& item : Tools)
	{
		const auto& tool = item.second;
		if (tool->m_toolType == _type)
		{
			i++;
		}
	}
	return i;
}
std::string ToolBlockTool::CanToolName(std::string name, int i)
{
	std::string toolname = name + std::to_string(i);
	if (Tools.contains(toolname))
	{
		i++;
		CanToolName(name, i);
	}
	return toolname;
}