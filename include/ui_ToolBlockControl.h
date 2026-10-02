/********************************************************************************
** Form generated from reading UI file 'ToolBlockControl.ui'
**
** Created by: Qt User Interface Compiler version 6.7.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_TOOLBLOCKCONTROL_H
#define UI_TOOLBLOCKCONTROL_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDockWidget>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTreeView>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_ToolBlockControlClass
{
public:
    QWidget *centralWidget;
    QVBoxLayout *verticalLayout;
    QFrame *frame;
    QFrame *frame_2;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout;
    QTabWidget *tabWidget;
    QWidget *tab;
    QHBoxLayout *horizontalLayout_2;
    QTreeView *TooltreeView;
    QWidget *tab_2;
    QWidget *ImageDisplaywidget;
    QDockWidget *dockWidget;
    QWidget *dockWidgetContents_2;

    void setupUi(QMainWindow *ToolBlockControlClass)
    {
        if (ToolBlockControlClass->objectName().isEmpty())
            ToolBlockControlClass->setObjectName("ToolBlockControlClass");
        ToolBlockControlClass->resize(964, 797);
        centralWidget = new QWidget(ToolBlockControlClass);
        centralWidget->setObjectName("centralWidget");
        verticalLayout = new QVBoxLayout(centralWidget);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName("verticalLayout");
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        frame = new QFrame(centralWidget);
        frame->setObjectName("frame");
        frame->setMinimumSize(QSize(0, 20));
        frame->setStyleSheet(QString::fromUtf8("background-color: rgb(218, 218, 218);"));
        frame->setFrameShape(QFrame::Shape::StyledPanel);
        frame->setFrameShadow(QFrame::Shadow::Raised);

        verticalLayout->addWidget(frame);

        frame_2 = new QFrame(centralWidget);
        frame_2->setObjectName("frame_2");
        frame_2->setStyleSheet(QString::fromUtf8("background-color: rgb(229, 229, 229);"));
        frame_2->setFrameShape(QFrame::Shape::StyledPanel);
        frame_2->setFrameShadow(QFrame::Shadow::Raised);
        verticalLayout_2 = new QVBoxLayout(frame_2);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName("verticalLayout_2");
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(0);
        horizontalLayout->setObjectName("horizontalLayout");
        tabWidget = new QTabWidget(frame_2);
        tabWidget->setObjectName("tabWidget");
        tabWidget->setMinimumSize(QSize(300, 0));
        tab = new QWidget();
        tab->setObjectName("tab");
        horizontalLayout_2 = new QHBoxLayout(tab);
        horizontalLayout_2->setSpacing(0);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName("horizontalLayout_2");
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        TooltreeView = new QTreeView(tab);
        TooltreeView->setObjectName("TooltreeView");

        horizontalLayout_2->addWidget(TooltreeView);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName("tab_2");
        tabWidget->addTab(tab_2, QString());

        horizontalLayout->addWidget(tabWidget);

        ImageDisplaywidget = new QWidget(frame_2);
        ImageDisplaywidget->setObjectName("ImageDisplaywidget");

        horizontalLayout->addWidget(ImageDisplaywidget);

        horizontalLayout->setStretch(1, 1);

        verticalLayout_2->addLayout(horizontalLayout);


        verticalLayout->addWidget(frame_2);

        verticalLayout->setStretch(1, 1);
        ToolBlockControlClass->setCentralWidget(centralWidget);
        dockWidget = new QDockWidget(ToolBlockControlClass);
        dockWidget->setObjectName("dockWidget");
        dockWidgetContents_2 = new QWidget();
        dockWidgetContents_2->setObjectName("dockWidgetContents_2");
        dockWidget->setWidget(dockWidgetContents_2);
        ToolBlockControlClass->addDockWidget(Qt::DockWidgetArea::LeftDockWidgetArea, dockWidget);

        retranslateUi(ToolBlockControlClass);

        tabWidget->setCurrentIndex(0);


        QMetaObject::connectSlotsByName(ToolBlockControlClass);
    } // setupUi

    void retranslateUi(QMainWindow *ToolBlockControlClass)
    {
        ToolBlockControlClass->setWindowTitle(QCoreApplication::translate("ToolBlockControlClass", "ToolBlockControl", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab), QCoreApplication::translate("ToolBlockControlClass", "Tab 1", nullptr));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QCoreApplication::translate("ToolBlockControlClass", "Tab 2", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ToolBlockControlClass: public Ui_ToolBlockControlClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_TOOLBLOCKCONTROL_H
