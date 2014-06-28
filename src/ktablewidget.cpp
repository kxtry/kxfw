#include "stdafx.h"
#include "ktablewidget.h"

#include "kwidget_p.h"
#include "kxmlui.h"

KX_WIDGET_CREATOR_GLOBAL_STATIC(KTableWidget)

class KTableWidgetPrivate : public KWidgetPrivate
{
	Q_DECLARE_PUBLIC(KTableWidget)
public:
	QList<KWidget*> itemList;
	QSizeF space;
	QMargins margin;
	qreal minTableHeight;
public:
	KTableWidgetPrivate()
		: space(0,0)
		, minTableHeight(0)
	{

	}

	~KTableWidgetPrivate()
	{

	}
};

KTableWidget::KTableWidget( QGraphicsItem *parent/*=NULL*/ )
: KWidget(*new KTableWidgetPrivate(), parent)
{

}

KTableWidget::~KTableWidget()
{

}

void KTableWidget::addItem( KWidget *item )
{
	Q_D(KTableWidget);

	if(d->itemList.contains(item))
		return;
	addGraphicsItem(item);
	addObjectItem(item);
	d->itemList.push_back(item);
}

void KTableWidget::removeItem( KWidget *item )
{
	Q_D(KTableWidget);
	removeGraphicsItem(item);
	removeObjectItem(item);
	d->itemList.removeOne(item);
}

KWidget::LayoutType KTableWidget::layoutType()
{
	return None;
}

void KTableWidget::setLayoutType( KWidget::LayoutType type )
{

}

void KTableWidget::resizeEvent( QGraphicsSceneResizeEvent *e )
{
	__super::resizeEvent(e);
	doLayout();
}

void KTableWidget::doLayout(bool reset)
{
	Q_D(KTableWidget);
	QRectF winRt = rect();
	QMargins m = margins();
	winRt.adjust(m.left(), m.top(), m.right(), m.bottom());
	int row = 0;
	int col = 0; 
	qreal rowHeight = 0;
	d->minTableHeight = 0;
	QPointF itemPt(winRt.topLeft());
	for(QList<KWidget*>::iterator iter = d->itemList.begin(); iter != d->itemList.end(); iter++)
	{
		KWidget *item = *iter;
		QSizeF itemSize = item->fixSize();
		if(col == 0 || itemPt.x() + itemSize.width() < winRt.right())
		{
			QRectF itemRt(itemPt.x(), itemPt.y(), itemSize.width(), itemSize.height());
			if(reset)
				item->setGeometry(itemRt);
			itemPt.setX(itemPt.x() + itemSize.width() + d->space.width());
		}
		else
		{
			itemPt.setX(winRt.left());
			itemPt.setY(itemPt.y() + rowHeight + d->space.height());
			d->minTableHeight += rowHeight + d->space.height();
			rowHeight = 0;
			QRectF itemRt(itemPt.x(), itemPt.y(), itemSize.width(), itemSize.height());
			if(reset)
				item->setGeometry(itemRt);
			itemPt.setX(itemPt.x() + itemSize.width() + d->space.width());
		}
		col++;
		if(itemSize.height() > rowHeight)
		{
			rowHeight = itemSize.height();
		}	
	}
	d->minTableHeight += rowHeight;
}

QSizeF KTableWidget::spacing() const
{
	Q_D(const KTableWidget);
	return d->space;
}

void KTableWidget::setSpacing( const QSizeF& s )
{
	Q_D(KTableWidget);
	d->space = s;
}

qreal KTableWidget::hspacing()
{
	Q_D(const KTableWidget);
	return d->space.width();
}

void KTableWidget::setHSpacing( qreal v )
{
	Q_D(KTableWidget);
	d->space.setWidth(v);
}

qreal KTableWidget::vspacing()
{
	Q_D(KTableWidget);

	return d->space.height();
}

void KTableWidget::setVSpacing( qreal v )
{
	Q_D(KTableWidget);
	d->space.setHeight(v);
}

QMargins KTableWidget::margins() const
{
	Q_D(const KTableWidget);
	return d->margin;
}

void KTableWidget::setMargins( const QMargins& m )
{
	Q_D(KTableWidget);
	d->margin = m;
}

int KTableWidget::itemCount()
{
	Q_D(KTableWidget);

	return d->itemList.count();
}

void KTableWidget::removeAll()
{
	Q_D(KTableWidget);
	for(QList<KWidget*>::iterator iter = d->itemList.begin(); iter != d->itemList.end(); iter++)
	{
		KWidget *item = *iter;
		delete item;
	}
	d->itemList.clear();
}

void KTableWidget::updateLayout()
{
	doLayout();
}

int KTableWidget::calcMinimumHeight(bool reset)
{
	Q_D(KTableWidget);
	doLayout(reset);
	return d->minTableHeight;
}
