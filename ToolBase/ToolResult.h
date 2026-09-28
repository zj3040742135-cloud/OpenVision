#pragma once
#include<string>
enum ResultType
{
	TYPE_SUCESSES,
	TYPE_ERROR,
	TYPE_UNRUN
};
struct ToolResult
{
	ResultType result = TYPE_UNRUN;
	std::string message;
	double runTime;

};
