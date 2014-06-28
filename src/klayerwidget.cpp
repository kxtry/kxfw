#include "stdafx.h"
#include "klayerwidget.h"
#include "kxmlui.h"
#include "kwidget_p.h"


KX_WIDGET_CREATOR_GLOBAL_STATIC(KLayerWidget)

class KLayerWidgetPrivate: public KWidgetPrivate
{
public:
	QList<KWidget*> lstLayers;
};

KLayerWidget::KLayerWidget( QGraphicsItem* parent /*= NULL*/ )
: KWidget(*new KLayerWidgetPrivate(), parent)
{

}

KLayerWidget::~KLayerWidget()
{

}

void KLayerWidget::addItem( KWidget* item )
{
	if(bylayout())
	{
		__super::addItem(item);
		return;
	}
	if(!d_func()->lstLayers.contains(item))
	{
		QRectF geom = geometry();
		d_func()->lstLayers.push_back(item);
		item->setGeometry(geom);
	}
}

void KLayerWidget::removeItem( KWidget *item )
{
	__super::removeItem(item);
	d_func()->lstLayers.removeOne(item);
}

void KLayerWidget::construct()
{
	QList<QObject*> childs = children();
	QGraphicsLayout *layout = QGraphicsWidget::layout();
	QRectF geom = geometry();
	for(int i = 0; i < childs.count(); i++)
	{
		KWidget *item = qobject_cast<KWidget*>(childs[i]);
		if(!item->bylayout())
		{
			item->setByLayout(false);
			item->setParentItem(parentItem());
			item->setParent(parent());
			if(!d_func()->lstLayers.contains(item))
			{
				d_func()->lstLayers.push_back(item);
			}
			item->setGeometry(geom);
		}
	}
}

QVariant KLayerWidget::itemChange( GraphicsItemChange change, const QVariant &value )
{
	if(change == QGraphicsItem::ItemParentChange)
	{
		reparentItem(value.value<QGraphicsItem*>());
	}
	return __super::itemChange(change, value);
}

void KLayerWidget::reparentItem( QGraphicsItem* itemParent )
{
	QObject *objParent = parent();
	if(objParent)
	{
		objParent->removeEventFilter(this);
	}
	if(itemParent)
	{
		QObject *objParent = itemParent->toGraphicsObject();
		objParent->installEventFilter(this);
	}
	for(int i = 0; i < d_func()->lstLayers.count(); i++)
	{
		KWidget *widget = d_func()->lstLayers[i];
		widget->setParentItem(itemParent);
		widget->setParent(itemParent->toGraphicsObject());
	}
}

void KLayerWidget::resizeEvent( QGraphicsSceneResizeEvent * event )
{
	__super::resizeEvent(event);
	QRectF geom = geometry();
	for(int i = 0; i < d_func()->lstLayers.count(); i++)
	{
		KWidget *widget = d_func()->lstLayers[i];
		widget->setFixSize(geom.size());
		widget->setGeometry(geom);
	}
}

bool KLayerWidget::eventFilter( QObject * watched, QEvent * event )
{
	if(event->type() == QEvent::ChildRemoved)
	{
		QChildEvent *e = (QChildEvent*)event;
		KWidget *item = qobject_cast<KWidget*>(e->child());
		removeItem(item);
	}
	return false;
}
