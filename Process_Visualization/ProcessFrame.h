#pragma once
#include <QObject>
#include<QtWidgets/qframe.h>
#include<string>
#include"ModuleBase.h"
#include<map>
#include<vector>
#include<memory>
#include<QtWidgets/qmainwindow.h>
#include <QtWidgets/QMenu>
#include <QVector>
#include <QPointF>

struct ModuldeConnection
{
	std::string SourceModule;
	int SourceOutPortIndex;
	std::string TargetModule;
	int TargetInPortIndex;
	QVector<QPointF> TurnPoints;   // 转折点（折线: start -> T1 -> T2 -> ... -> end）
};

class ProcessFrame :public QFrame
{
	Q_OBJECT
public:
	ProcessFrame(QWidget* parent);
	~ProcessFrame();
public:
	std::vector<ModuldeConnection> connections;
	std::map<std::string, std::unique_ptr<ModuleBase>> Modules;
	std::string ProcessName;
private:
	QPoint ClickLocation;
	ModuleBase* m_pSelectedModule = nullptr;
	QPoint m_DragOffset;
	// 连线选中
	int m_selectedConnIndex = -1;   // 选中连线在 connections 中的下标，-1 表示未选中
	bool m_bConnecting = false;
	std::string m_pendingSourceModule;
	int m_pendingOutPortIdx = -1;
	QPoint m_mousePos;
	int m_toolCounter = 0;   // 生成唯一模块名
	QMenu m_connMenu;        // 连线右键菜单
	QMenu m_moduleMenu;      // 模块右键菜单
	QMenu menu;
protected:
	void contextMenuEvent(QContextMenuEvent* event) override;
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void paintEvent(QPaintEvent* event) override;
private:
	void AddToolModule();
	void AddStartlModule();

	// 端口命中检测：返回是否命中、所在模块名、是否输出端口、端口索引
	bool HitTestPort(const QPoint& pos, std::string& outModuleName, bool& outIsOutPort, int& outPortIdx) const;
	// 连线命中检测：返回连线下标，未命中返回 -1
	int HitTestConnection(const QPoint& pos) const;
	// 点到线段距离
	static qreal PointToSegmentDist(const QPointF& p, const QPointF& a, const QPointF& b);
	// 检查模块能否放置到 newTopLeft（不越界、不与其他模块重叠）
	bool CanPlaceModule(const ModuleBase* mod, const QPoint& newTopLeft) const;
	// 连接规则校验
	bool CanConnect(const std::string& srcModule, int outIdx, const std::string& dstModule, int inIdx) const;
	// 计算转折点（避让模块主体）
	void RouteConnection(ModuldeConnection& conn) const;
	// 模块移动后重路由所有相关连线
	void UpdateAllConnections();
	// 添加一条连线（校验 + 路由）
	bool AddConnection(const std::string& srcModule, int outIdx, const std::string& dstModule, int inIdx);
	// 删除选中的连线
	void DeleteSelectedConnection();
	// 删除选中的模块（同时清理相关连线）
	void DeleteSelectedModule();
};
