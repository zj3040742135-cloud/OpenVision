#include "pch.h"
#include "ToolBlockTool.h"
#include<fstream>
REGISTER_TOOL(ToolBlockTool, ToolBlockTool);
ToolBlockTool::ToolBlockTool()
{
	ToolName = "ToolBlock";
	this->m_toolType = ToolType::ToolBlockTool;
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
}
bool ToolBlockTool::AddInput(std::string VarName, ToolInOutVar var)
{
	if (Inputs.contains(VarName))
		return false;
	ToolInOutVar input;
	input.BindingInfo = &var;
	input.Binding = true;
	input.ToolVarType = var.ToolVarType;
	input.BindingName = var.BindingName;
	Inputs[VarName] = &input;
	return true;
}
bool ToolBlockTool::AddOutput(std::string VarName, ToolInOutVar var)
{
	if (Outputs.contains(VarName))
		return false;
	ToolInOutVar Output;
	Output.BindingInfo = &var;
	Output.Binding = true;
	Output.ToolVarType = var.ToolVarType;
	Output.BindingName = var.BindingName;
	Inputs[VarName] = &Output;
	return true;
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
	return true;
}
bool ToolBlockTool::RemoveTool(std::string const _toolName)
{
	if (!Tools.contains(_toolName))
		return false;
	Tools.erase(_toolName);
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
	bool isConnection = HasConnection(conn);
	if (isConnection)
	{
		ToolConnections.push_back(conn);
		TargetTool->Inputs[ToToolNode]->BindingInfo = SourceTool->Outputs[FromToolNode];
		TargetTool->Inputs[ToToolNode]->Binding = true;
	}

}
bool ToolBlockTool::HasConnection(const ConnectionsInfo info)
{
	for (ConnectionsInfo item : ToolConnections)
	{
		if (item.FromToolName == info.FromToolName && item.FormToolNodeName == info.FormToolNodeName && item.ToToolName == info.ToToolName && item.ToToolNodeName == info.ToToolNodeName)
		{
			return false;
		}
	}
	return true;
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