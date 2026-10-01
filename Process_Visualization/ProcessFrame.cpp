#include "ProcessFrame.h"
#include <QPainter>
#include <QMouseEvent>
#include <QAction>
#include <QDebug>
#include <QPolygonF>
#include <QLineF>
#include <algorithm>

ProcessFrame::ProcessFrame(QWidget* parent)
	: QFrame(parent)
{
	connections.clear();
	Modules.clear();
	AddStartlModule();
	// 只创建一次action，不要每次弹出菜单add
	QAction* addToolAct = new QAction("添加工具", this);
	connect(addToolAct, &QAction::triggered, this, &ProcessFrame::AddToolModule);
	menu.addAction(addToolAct);

	// 连线右键菜单
	QAction* delConnAct = new QAction("删除连线", this);
	connect(delConnAct, &QAction::triggered, this, &ProcessFrame::DeleteSelectedConnection);
	m_connMenu.addAction(delConnAct);

	// 模块右键菜单
	QAction* delModAct = new QAction("删除工具", this);
	connect(delModAct, &QAction::triggered, this, &ProcessFrame::DeleteSelectedModule);
	m_moduleMenu.addAction(delModAct);
}

ProcessFrame::~ProcessFrame()
{
}

void ProcessFrame::contextMenuEvent(QContextMenuEvent* event)
{
	// 右键命中连线：选中并弹出"删除连线"菜单
	int connIdx = HitTestConnection(event->pos());
	if (connIdx >= 0)
	{
		m_selectedConnIndex = connIdx;
		m_pSelectedModule = nullptr;
		this->update();
		m_connMenu.exec(event->globalPos());
		event->accept();
		return;
	}

	// 右键命中模块：选中并弹出"删除工具"菜单
	ModuleBase* hitMod = nullptr;
	for (auto& pair : Modules)
	{
		if (pair.second->mainRect.contains(event->pos()))
		{
			hitMod = pair.second.get();
			break;
		}
	}
	if (hitMod)
	{
		m_pSelectedModule = hitMod;
		m_selectedConnIndex = -1;
		this->update();
		m_moduleMenu.exec(event->globalPos());
		event->accept();
		return;
	}

	// 否则弹出添加工具菜单
	ClickLocation = event->pos();
	menu.exec(event->globalPos());
	event->accept();
}

void ProcessFrame::mousePressEvent(QMouseEvent* event)
{
	QFrame::mousePressEvent(event);

	if (event->button() != Qt::LeftButton)
		return;

	QPoint pos = event->pos();

	// 1) 优先检测是否点中端口：点中 OutPort 则开始连线
	std::string hitModule;
	bool hitIsOut = false;
	int hitIdx = -1;
	if (HitTestPort(pos, hitModule, hitIsOut, hitIdx))
	{
		if (hitIsOut)
		{
			// 规则1: 只能从 OutPort 发起连线
			m_bConnecting = true;
			m_pendingSourceModule = hitModule;
			m_pendingOutPortIdx = hitIdx;
			m_mousePos = pos;
			m_pSelectedModule = nullptr;
			m_selectedConnIndex = -1;
			this->update();
		}
		// 点中 InPort 不做任何事（InPort 只能作为目标）
		return;
	}

	// 2) 点中连线：选中连线（高亮），取消模块选中
	int connIdx = HitTestConnection(pos);
	if (connIdx >= 0)
	{
		m_selectedConnIndex = connIdx;
		m_pSelectedModule = nullptr;
		this->update();
		return;
	}

	// 3) 点中模块主体：选中用于拖拽，取消连线选中
	m_selectedConnIndex = -1;
	m_pSelectedModule = nullptr;
	for (auto& pair : Modules)
	{
		if (pair.second->mainRect.contains(pos))
		{
			m_pSelectedModule = pair.second.get();
			m_DragOffset = pos - m_pSelectedModule->mainRect.topLeft();
			break;
		}
	}
	this->update();
}

void ProcessFrame::mouseReleaseEvent(QMouseEvent* event)
{
	QFrame::mouseReleaseEvent(event);

	if (event->button() != Qt::LeftButton)
		return;

	// 结束连线拖拽
	if (m_bConnecting)
	{
		std::string hitModule;
		bool hitIsOut = false;
		int hitIdx = -1;
		bool ok = false;
		if (HitTestPort(event->pos(), hitModule, hitIsOut, hitIdx) && !hitIsOut)
		{
			// 释放在 InPort 上，尝试建立连接
			ok = AddConnection(m_pendingSourceModule, m_pendingOutPortIdx, hitModule, hitIdx);
		}
		m_bConnecting = false;
		m_pendingSourceModule.clear();
		m_pendingOutPortIdx = -1;
		this->update();
		return;
	}

	if (m_pSelectedModule)
	{
		m_pSelectedModule = nullptr;
	}
}

void ProcessFrame::mouseDoubleClickEvent(QMouseEvent* event)
{
	QFrame::mouseDoubleClickEvent(event);
}

void ProcessFrame::mouseMoveEvent(QMouseEvent* event)
{
	QFrame::mouseMoveEvent(event);

	// 连线拖拽中：更新临时线终点
	if (m_bConnecting)
	{
		m_mousePos = event->pos();
		this->update();
		return;
	}

	// 模块拖拽
	if (m_pSelectedModule && (event->buttons() & Qt::LeftButton))
	{
		QPoint newTopLeft = event->pos() - m_DragOffset;
		if (CanPlaceModule(m_pSelectedModule, newTopLeft))
		{
			m_pSelectedModule->Move(newTopLeft);
			UpdateAllConnections();   // 模块移动后重路由所有连线
		}
		this->update();
	}
}

void ProcessFrame::paintEvent(QPaintEvent* event)
{
	QFrame::paintEvent(event);
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing);

	// === 先画连线（在模块下方） ===
	painter.setBrush(Qt::NoBrush);

	for (size_t i = 0; i < connections.size(); i++)
	{
		auto& conn = connections[i];
		auto srcIt = Modules.find(conn.SourceModule);
		auto dstIt = Modules.find(conn.TargetModule);
		if (srcIt == Modules.end() || dstIt == Modules.end())
			continue;
		if (conn.SourceOutPortIndex >= srcIt->second->OutPorts.size() ||
			conn.TargetInPortIndex >= dstIt->second->InPorts.size())
			continue;

		QPointF start = srcIt->second->OutPorts[conn.SourceOutPortIndex].ArrowLocation;
		QPointF end = dstIt->second->InPorts[conn.TargetInPortIndex].ArrowLocation;

		QPolygonF poly;
		poly << start;
		for (auto& tp : conn.TurnPoints)
			poly << tp;
		poly << end;

		// 选中连线：蓝色加粗；未选中：深灰细线
		if ((int)i == m_selectedConnIndex)
		{
			QPen selPen(QColor(0, 120, 215));
			selPen.setWidth(4);
			painter.setPen(selPen);
		}
		else
		{
			QPen normPen(QColor(60, 60, 60));
			normPen.setWidth(2);
			painter.setPen(normPen);
		}
		painter.drawPolyline(poly);
	}

	// 拖拽中的临时连线
	if (m_bConnecting)
	{
		auto srcIt = Modules.find(m_pendingSourceModule);
		if (srcIt != Modules.end() && m_pendingOutPortIdx < srcIt->second->OutPorts.size())
		{
			QPointF start = srcIt->second->OutPorts[m_pendingOutPortIdx].ArrowLocation;
			QPen tempPen(QColor(0, 120, 215));
			tempPen.setWidth(2);
			tempPen.setStyle(Qt::DashLine);
			painter.setPen(tempPen);
			painter.drawLine(start, QPointF(m_mousePos));
		}
	}

	// === 再画模块与端口 ===
	for (auto& pair : Modules)
	{
		auto& modulePtr = pair.second;

		painter.setBrush(QBrush(QColor(255, 0, 0)));
		if (modulePtr.get() == m_pSelectedModule)
		{
			QPen selPen(QColor(0, 120, 215));
			selPen.setWidth(3);
			painter.setPen(selPen);
		}
		else
		{
			painter.setPen(QPen(Qt::black));
		}
		painter.drawRoundedRect(modulePtr->mainRect, 6, 6);

		painter.setPen(QPen(Qt::black));
		painter.drawText(modulePtr->mainRect, Qt::AlignCenter,
			QString::fromStdString(modulePtr->ModuleName));

		painter.setBrush(QBrush(QColor(0, 255, 0)));
		for (auto& port : modulePtr->InPorts)
		{
			QPointF c = port.ArrowLocation;
			qreal w = port.ArrowWidth;
			qreal h = port.ArrowHeight;
			QPolygonF arrow;
			arrow << QPointF(c.x() - w / 2, c.y() - h / 2)
			      << QPointF(c.x() + w / 2, c.y() - h / 2)
			      << QPointF(c.x(), c.y() + h / 2);
			painter.drawPolygon(arrow);
		}
		for (auto& port : modulePtr->OutPorts)
		{
			QPointF c = port.ArrowLocation;
			qreal w = port.ArrowWidth;
			qreal h = port.ArrowHeight;
			QPolygonF arrow;
			arrow << QPointF(c.x() - w / 2, c.y() - h / 2)
			      << QPointF(c.x() + w / 2, c.y() - h / 2)
			      << QPointF(c.x(), c.y() + h / 2);
			painter.drawPolygon(arrow);
		}
	}
}

void ProcessFrame::AddToolModule()
{
	ModuleBase* m = new ModuleBase();
	m->CreateModule(ClickLocation, 1, 2);
	m_toolCounter++;
	std::string name = "debug_" + std::to_string(m_toolCounter);
	m->ModuleName = name;

	// 确保新模块不越界、不与已有模块重叠；若冲突则向右下偏移寻找空位
	QPoint pos = ClickLocation;
	QSize sz = m->mainRect.size();
	for (int attempt = 0; attempt < 50; attempt++)
	{
		if (CanPlaceModule(m, pos))
			break;
		pos += QPoint(sz.width() + 10, 0);
		// 若横向越界，换到下一行
		if (pos.x() + sz.width() > this->rect().right())
			pos = QPoint(ClickLocation.x(), pos.y() + sz.height() + 10);
	}
	m->Move(pos);

	std::unique_ptr<ModuleBase> t(m);
	Modules[name] = std::move(t);
	this->update();
}

void ProcessFrame::AddStartlModule()
{
	QPoint p = QPoint(100, 100);
	ModuleBase* m = new ModuleBase();
	m->CreateModule(p, 0, 1);
	std::string name = "start";
	m->ModuleName = name;
	std::unique_ptr<ModuleBase> t(m);
	Modules[name] = std::move(t);
	this->update();
}

// ==================== 连线辅助方法 ====================

bool ProcessFrame::HitTestPort(const QPoint& pos, std::string& outModuleName, bool& outIsOutPort, int& outPortIdx) const
{
	const int pad = 3;   // 命中区域比箭头稍大，便于点击
	for (auto& pair : Modules)
	{
		ModuleBase* mod = pair.second.get();
		for (int i = 0; i < mod->OutPorts.size(); i++)
		{
			auto& p = mod->OutPorts[i];
			QRectF r(p.ArrowLocation.x() - p.ArrowWidth / 2 - pad,
				p.ArrowLocation.y() - p.ArrowHeight / 2 - pad,
				p.ArrowWidth + pad * 2, p.ArrowHeight + pad * 2);
			if (r.contains(QPointF(pos)))
			{
				outModuleName = pair.first;
				outIsOutPort = true;
				outPortIdx = i;
				return true;
			}
		}
		for (int i = 0; i < mod->InPorts.size(); i++)
		{
			auto& p = mod->InPorts[i];
			QRectF r(p.ArrowLocation.x() - p.ArrowWidth / 2 - pad,
				p.ArrowLocation.y() - p.ArrowHeight / 2 - pad,
				p.ArrowWidth + pad * 2, p.ArrowHeight + pad * 2);
			if (r.contains(QPointF(pos)))
			{
				outModuleName = pair.first;
				outIsOutPort = false;
				outPortIdx = i;
				return true;
			}
		}
	}
	return false;
}

qreal ProcessFrame::PointToSegmentDist(const QPointF& p, const QPointF& a, const QPointF& b)
{
	QPointF ab = b - a;
	qreal len2 = ab.x() * ab.x() + ab.y() * ab.y();
	if (len2 == 0.0)
		return QLineF(p, a).length();
	qreal t = ((p.x() - a.x()) * ab.x() + (p.y() - a.y()) * ab.y()) / len2;
	t = std::max(0.0, std::min(1.0, t));
	QPointF proj(a.x() + t * ab.x(), a.y() + t * ab.y());
	return QLineF(p, proj).length();
}

int ProcessFrame::HitTestConnection(const QPoint& pos) const
{
	const qreal hitRadius = 6.0;   // 命中容差（像素）
	QPointF p(pos);
	for (size_t i = 0; i < connections.size(); i++)
	{
		auto& conn = connections[i];
		auto srcIt = Modules.find(conn.SourceModule);
		auto dstIt = Modules.find(conn.TargetModule);
		if (srcIt == Modules.end() || dstIt == Modules.end())
			continue;
		if (conn.SourceOutPortIndex >= srcIt->second->OutPorts.size() ||
			conn.TargetInPortIndex >= dstIt->second->InPorts.size())
			continue;

		// 构建折线点集
		QVector<QPointF> pts;
		pts.push_back(srcIt->second->OutPorts[conn.SourceOutPortIndex].ArrowLocation);
		for (auto& tp : conn.TurnPoints)
			pts.push_back(tp);
		pts.push_back(dstIt->second->InPorts[conn.TargetInPortIndex].ArrowLocation);

		// 任一线段距离小于阈值即命中
		for (int j = 0; j < pts.size() - 1; j++)
		{
			if (PointToSegmentDist(p, pts[j], pts[j + 1]) <= hitRadius)
				return (int)i;
		}
	}
	return -1;
}

void ProcessFrame::DeleteSelectedConnection()
{
	if (m_selectedConnIndex < 0 || m_selectedConnIndex >= (int)connections.size())
		return;
	connections.erase(connections.begin() + m_selectedConnIndex);
	m_selectedConnIndex = -1;
	this->update();
}

void ProcessFrame::DeleteSelectedModule()
{
	if (!m_pSelectedModule)
		return;

	// 找到模块名
	std::string name;
	for (auto& pair : Modules)
	{
		if (pair.second.get() == m_pSelectedModule)
		{
			name = pair.first;
			break;
		}
	}
	if (name.empty())
		return;

	// 清理所有与该模块相关的连线（作为源或目标）
	connections.erase(
		std::remove_if(connections.begin(), connections.end(),
			[&name](const ModuldeConnection& c) {
				return c.SourceModule == name || c.TargetModule == name;
			}),
		connections.end());

	// 从模块表中移除
	Modules.erase(name);
	m_pSelectedModule = nullptr;
	m_selectedConnIndex = -1;
	this->update();
}

bool ProcessFrame::CanPlaceModule(const ModuleBase* mod, const QPoint& newTopLeft) const
{
	// 预测模块新位置的矩形
	QRect newRect = mod->mainRect;
	newRect.moveTopLeft(newTopLeft);

	// 1) 画布边界约束：模块不能超出 ProcessFrame 可视区域
	QRect canvas = this->rect();
	if (newRect.left() < canvas.left() || newRect.top() < canvas.top() ||
		newRect.right() > canvas.right() || newRect.bottom() > canvas.bottom())
	{
		return false;
	}

	// 2) 模块间不重叠：AABB 相交检测（排除自身）
	for (auto& pair : Modules)
	{
		ModuleBase* other = pair.second.get();
		if (other == mod)
			continue;
		if (newRect.intersects(other->mainRect))
			return false;
	}

	return true;
}

bool ProcessFrame::CanConnect(const std::string& srcModule, int outIdx, const std::string& dstModule, int inIdx) const
{
	// 规则2: 同一模块不能自连
	if (srcModule == dstModule)
		return false;

	auto srcIt = Modules.find(srcModule);
	auto dstIt = Modules.find(dstModule);
	if (srcIt == Modules.end() || dstIt == Modules.end())
		return false;
	if (outIdx < 0 || outIdx >= srcIt->second->OutPorts.size())
		return false;
	if (inIdx < 0 || inIdx >= dstIt->second->InPorts.size())
		return false;

	// 规则1: OutPort -> InPort（由交互层保证发起端是 OutPort、目标端是 InPort）
	// 规则3: OutPort 若已有连接，由 AddConnection 删除旧连接后再新建
	return true;
}

void ProcessFrame::RouteConnection(ModuldeConnection& conn) const
{
	auto srcIt = Modules.find(conn.SourceModule);
	auto dstIt = Modules.find(conn.TargetModule);
	if (srcIt == Modules.end() || dstIt == Modules.end())
		return;

	QPointF start = srcIt->second->OutPorts[conn.SourceOutPortIndex].ArrowLocation;
	QPointF end = dstIt->second->InPorts[conn.TargetInPortIndex].ArrowLocation;
	QRect srcRect = srcIt->second->mainRect;
	QRect dstRect = dstIt->second->mainRect;

	const qreal vGap = 15.0;   // 末段转折点距 InPort 的上方距离
	const qreal hGap = 20.0;   // 水平段距模块底部/侧边的距离
	const qreal sidePad = 10.0; // 绕行时距模块右侧的间距

	qreal yAbove = end.y() - vGap;   // 末段水平段的 Y（InPort 正上方）
	conn.TurnPoints.clear();

	bool srcAboveDst = srcRect.bottom() <= dstRect.top();   // 源在目标正上方且有间隙

	if (srcAboveDst)
	{
		// 常规布局（源在上、目标在下）：在两模块间隙中走线，3 个转折点
		qreal yHoriz = srcRect.bottom() + hGap;
		if (yHoriz >= dstRect.top())
			yHoriz = (srcRect.bottom() + dstRect.top()) / 2.0;

		conn.TurnPoints.push_back(QPointF(start.x(), yHoriz));   // T1: 垂直下
		conn.TurnPoints.push_back(QPointF(end.x(), yHoriz));     // T2: 水平到 InPort X
		conn.TurnPoints.push_back(QPointF(end.x(), yAbove));      // T3: 垂直到 InPort 上方
		// T3 -> end: 垂直向下接入 InPort
	}
	else
	{
		// 源在目标下方/重叠：需绕目标侧边上升，从上方接入。
		// 分别计算绕目标左侧与右侧的路径总长度，取最短。
		qreal yBot = std::max(srcRect.bottom(), dstRect.bottom()) + hGap;
		qreal xLeft = dstRect.left() - sidePad;
		qreal xRight = dstRect.right() + sidePad;

		auto routeLen = [&](qreal xSide) -> qreal {
			qreal seg1 = yBot - start.y();                 // OutPort 垂直下
			qreal seg2 = std::abs(xSide - start.x());       // 水平到侧边
			qreal seg3 = yBot - yAbove;                      // 沿侧边上升
			qreal seg4 = std::abs(end.x() - xSide);          // 水平到 InPort 正上方
			qreal seg5 = end.y() - yAbove;                   // 垂直接入 InPort
			return seg1 + seg2 + seg3 + seg4 + seg5;
		};

		qreal xSide = (routeLen(xLeft) <= routeLen(xRight)) ? xLeft : xRight;

		conn.TurnPoints.push_back(QPointF(start.x(), yBot));      // T1: 垂直下到两模块下方
		conn.TurnPoints.push_back(QPointF(xSide, yBot));          // T2: 水平到较近侧边
		conn.TurnPoints.push_back(QPointF(xSide, yAbove));        // T3: 垂直上升到 InPort 上方
		conn.TurnPoints.push_back(QPointF(end.x(), yAbove));      // T4: 水平到 InPort 正上方
		// T4 -> end: 垂直向下接入 InPort
	}
}

void ProcessFrame::UpdateAllConnections()
{
	for (auto& conn : connections)
	{
		RouteConnection(conn);
	}
}

bool ProcessFrame::AddConnection(const std::string& srcModule, int outIdx, const std::string& dstModule, int inIdx)
{
	if (!CanConnect(srcModule, outIdx, dstModule, inIdx))
		return false;

	// 规则3: 一个 OutPort 只能连一个 InPort —— 若已有连接则删除旧连接
	connections.erase(
		std::remove_if(connections.begin(), connections.end(),
			[&](const ModuldeConnection& c) {
				return c.SourceModule == srcModule && c.SourceOutPortIndex == outIdx;
			}),
		connections.end());

	ModuldeConnection conn;
	conn.SourceModule = srcModule;
	conn.SourceOutPortIndex = outIdx;
	conn.TargetModule = dstModule;
	conn.TargetInPortIndex = inIdx;
	RouteConnection(conn);
	connections.push_back(conn);
	return true;
}
