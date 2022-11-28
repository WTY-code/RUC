#include "towerposition.h"
#include <QPainter>

const QSize TowerPosition::ms_fixedSize(44, 44);

TowerPosition::TowerPosition(QPoint pos, const QPixmap &sprite/* = QPixmap(":/image/open_spot.png")*/)
	: m_hasTower(false)
	, m_pos(pos)
	, m_sprite(sprite)
{
}

const QPoint TowerPosition::centerPos() const
{
	QPoint offsetPoint(ms_fixedSize.width() / 2, ms_fixedSize.height() / 2);
	return m_pos + offsetPoint;
}

bool TowerPosition::containPoint(const QPoint &pos) const
{
	bool isXInHere = m_pos.x() < pos.x() && pos.x() < (m_pos.x() + ms_fixedSize.width());
	bool isYInHere = m_pos.y() < pos.y() && pos.y() < (m_pos.y() + ms_fixedSize.height());
	return isXInHere && isYInHere;
}

bool TowerPosition::hasTower() const
{
	return m_hasTower;
}

void TowerPosition::setHasTower(bool hasTower/* = true*/)
{
	m_hasTower = hasTower;
}

void TowerPosition::draw(QPainter *painter) const
{
	painter->drawPixmap(m_pos.x(), m_pos.y(), m_sprite);
}
