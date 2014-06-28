#include "stdafx.h"
#include "qgraphicsflowlayout.h"

QGraphicsFlowLayout::QGraphicsFlowLayout( QGraphicsLayoutItem *parent /*= NULL*/ )
: QGraphicsLayout(parent)
, m_vSpace(0)
, m_hSpace(0)
{

}

void QGraphicsFlowLayout::addItem( QGraphicsLayoutItem * item )
{
	if(m_itemList.contains(item))
		return;
	m_itemList.push_back(item);
}

void QGraphicsFlowLayout::removeItem( QGraphicsLayoutItem *item )
{
	m_itemList.removeOne(item);
}

int QGraphicsFlowLayout::count() const
{
	return m_itemList.count();
}

QGraphicsLayoutItem * QGraphicsFlowLayout::itemAt( int i ) const
{
	if(i < 0 || i >= m_itemList.count())
		return NULL;
	return m_itemList.at(i);
}

void QGraphicsFlowLayout::removeAt( int index )
{
	m_itemList.removeAt(index);
}

void QGraphicsFlowLayout::getContentsMargins( qreal * left, qreal * top, qreal * right, qreal * bottom ) const
{
	*left = m_margins.left();
	*top = m_margins.top();
	*right = m_margins.right();
	*bottom = m_margins.bottom();
}

void QGraphicsFlowLayout::updateGeometry()
{
	__super::updateGeometry();
	doLayout(geometry(), false);
}

void QGraphicsFlowLayout::widgetEvent( QEvent * e )
{
	__super::widgetEvent(e);
}

void QGraphicsFlowLayout::invalidate()
{
	__super::invalidate();
}

QSizeF QGraphicsFlowLayout::sizeHint( Qt::SizeHint which, const QSizeF & constraint /*= QSizeF()*/ ) const
{
	return minimumSize();
}

QSizeF QGraphicsFlowLayout::minimumSize() const
{
	QSizeF size;
	QGraphicsLayoutItem *item;
	foreach (item, m_itemList)
		size = size.expandedTo(item->minimumSize());

	size += QSizeF(m_margins.left() + m_margins.right(), m_margins.top() + m_margins.bottom());
	return size;
}

void QGraphicsFlowLayout::setContentsMargins( const QMargins &margins )
{
	m_margins = margins;
}

qreal QGraphicsFlowLayout::doLayout(const QRectF &rect, bool testOnly) const
{
	qreal left, top, right, bottom;
	getContentsMargins(&left, &top, &right, &bottom);
	QRectF effectiveRect = rect.adjusted(+left, +top, -right, -bottom);
	qreal x = effectiveRect.x();
	qreal y = effectiveRect.y();
	qreal lineHeight = 0;
	//! [9]

	//! [10]
	QGraphicsLayoutItem *item;
	foreach (item, m_itemList)
	{
		QGraphicsItem *wid = item->graphicsItem();
		qreal spaceX = spacing(false);
		if (spaceX == -1)
		{

		}

		qreal spaceY = spacing(true);
		if (spaceY == -1)
		{

		}
		//! [10]
		//! [11]
		int nextX = x + item->geometry().width() + spaceX;
		if (nextX - spaceX > effectiveRect.right() && lineHeight > 0) 
		{
			x = effectiveRect.x();
			y = y + lineHeight + spaceY;
			nextX = x + item->geometry().width() + spaceX;
			lineHeight = 0;
		}

		if (!testOnly)
			item->setGeometry(QRectF(QPointF(x, y), item->geometry().size()));

		x = nextX;
		lineHeight = qMax(lineHeight, item->geometry().height());
	}
	return y + lineHeight - rect.y() + bottom;
}

int QGraphicsFlowLayout::heightForWidth( int width ) const
{
	int height = doLayout(QRectF(0, 0, width, 0), true);
	return height;
}

void QGraphicsFlowLayout::setSpacing( bool bVertical, qreal val )
{
	if(bVertical)
	{
		m_vSpace = val;
	}
	else
	{
		m_hSpace = val;
	}
}

qreal QGraphicsFlowLayout::spacing( bool bVertical ) const
{
	if(bVertical)
	{
		if(m_vSpace > 0)
			return m_vSpace;
		return smartSpacing(QStyle::PM_LayoutVerticalSpacing);
	}

	if(m_hSpace > 0)
		return m_hSpace;
	return smartSpacing(QStyle::PM_LayoutHorizontalSpacing);
}

int QGraphicsFlowLayout::smartSpacing( QStyle::PixelMetric pm ) const
{
	return 0;
	/*QObject *parent = this->parent();
	if (!parent) {
		return -1;
	} else if (parent->isWidgetType()) {
		QWidget *pw = static_cast<QWidget *>(parent);
		return pw->style()->pixelMetric(pm, 0, pw);
	} else {
		return static_cast<QLayout *>(parent)->spacing();
	}*/
}

