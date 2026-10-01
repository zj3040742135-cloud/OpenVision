#include "OpenVision.h"
#include<qdebug.h>
OpenVision::OpenVision(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    tool = new ImageSourceTool();
    
     t = new  ImageSourceTool();
     t->AddInput("Height", &t->OutImage.height, VarType::TYPE_INT);
     t->Inputs["Height"]->Binding = true;
     t->Inputs["Height"]->BindingInfo = tool->Outputs["OutImage"];
    connect(ui.pushButton, &QPushButton::clicked, this, &OpenVision::text);
}

OpenVision::~OpenVision()
{}
void OpenVision::text()
{
    ToolResult r;
    tool->Run(r);

    int width = t->Inputs["Height"]->Get<int>();
    t->Inputs["Height"]->Set(tool->Outputs["OutImage"]->Get<int>());
    qDebug() << "t.Height (bound to tool.width) =" << width;
    qDebug() << "t.Height (bound to tool.height) =" << t->OutImage.height;
}


