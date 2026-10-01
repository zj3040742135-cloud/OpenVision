#include "ModuleBase.h"
ModuleBase::ModuleBase(QObject* parent)
	: QObject(parent)
{
	InPorts.clear();
	OutPorts.clear();
}
ModuleBase::~ModuleBase()
{
}
void ModuleBase::CreateModule(QPoint point, int InNum, int OutNum)
{
	mainRect.setRect(point.x(), point.y(), MainRectWidth, MainRectHeight);
	int inSpacing = (int)MainRectWidth / qMax(1, InNum);
	for (int i = 0; i < InNum; i++)
	{
		ModuleInOutPortArrow arrow;
		arrow.ArrowHeight = 8;
		arrow.ArrowWidth = 12;
		// ArrowLocation 是箭头中心点，居中于所在槽位
		arrow.ArrowLocation.setX(point.x() + inSpacing * i + inSpacing / 2);
		arrow.ArrowLocation.setY(point.y() - (int)arrow.ArrowHeight / 2);
		InPorts.append(arrow);
	}
	int outSpacing = (int)MainRectWidth / qMax(1, OutNum);
	for (int i = 0; i < OutNum; i++)
	{
		ModuleInOutPortArrow arrow;
		arrow.ArrowHeight = 8;
		arrow.ArrowWidth = 12;
		arrow.ArrowLocation.setX(point.x() + outSpacing * i + outSpacing / 2);
		arrow.ArrowLocation.setY(point.y() + (int)MainRectHeight +(int)arrow.ArrowHeight / 2);
		OutPorts.append(arrow);
	}
}
void ModuleBase::Move(QPoint point)
{
	mainRect.moveTopLeft(point);
	int inSpacing = (int)MainRectWidth / qMax(1, this->InPorts.count());
	for (int i = 0; i < InPorts.count(); i++)
	{
		InPorts[i].ArrowLocation.setX(point.x() + inSpacing * i + inSpacing / 2);
		InPorts[i].ArrowLocation.setY(point.y() - (int)InPorts[i].ArrowHeight / 2);
	}
	int outSpacing = (int)MainRectWidth / qMax(1, this->OutPorts.count());
	for (int i = 0; i < OutPorts.count(); i++)
	{
		OutPorts[i].ArrowLocation.setX(point.x() + outSpacing * i + outSpacing / 2);
		OutPorts[i].ArrowLocation.setY(point.y() + (int)MainRectHeight + (int)OutPorts[i].ArrowHeight / 2);
	}

}