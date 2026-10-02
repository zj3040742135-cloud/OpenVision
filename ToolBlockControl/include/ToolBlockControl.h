#pragma once

#include "ToolBlockControl_global.h"
#include <QWidget>

class TOOLBLOCKCONTROL_EXPORT ToolBlockControl : public QWidget
{
	Q_OBJECT
public:
	explicit ToolBlockControl(QWidget* parent = nullptr);
	~ToolBlockControl() override;
};
