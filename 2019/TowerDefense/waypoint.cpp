#include "waypoint.h"
#include <QPainter>
#include <QColor>

WayPoint::WayPoint(QPoint pos)
	: m_pos(pos)
	, m_nextWayPoint(NULL)
{
}

void WayPoint::setNextWayPoint(WayPoint *nextPoint)
{
	m_nextWayPoint = nextPoint;
}

WayPoint* WayPoint::nextWayPoint() const
{
	return m_nextWayPoint;
}

const QPoint WayPoint::pos() const
{
	return m_pos;
}


