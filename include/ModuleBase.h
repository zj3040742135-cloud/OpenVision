#pragma once
#include <QObject>
#include<qrect.h>
#include "process_visualization_global.h"
#include<vector>
#include<string>

struct ModuleInOutPortArrow
{
	float ArrowHeight;
	float ArrowWidth;
	QPoint ArrowLocation;
};


class PROCESS_VISUALIZATION_EXPORT ModuleBase : public QObject
{
	Q_OBJECT
public:
	ModuleBase(QObject* parent=nullptr);
	~ModuleBase();
private:
	double MainRectWidth = 80;
	double MainRectHeight = 40;
public:
	std::string ModuleName;
	int InPort = 1;
	int OutPort = 1;
	QRect mainRect;
	QList< ModuleInOutPortArrow> InPorts;
	QList< ModuleInOutPortArrow> OutPorts;
	bool RunStatus = false;
public:
	void CreateModule(QPoint point, int InNum, int OutNum);
	void Move(QPoint point);
	void AddInport();
	void AddOutPort();
public:
	virtual void Show();
};
