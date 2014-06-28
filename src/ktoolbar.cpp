#include "stdafx.h"
#include "ktoolbar.h"
#include "kxmlui.h"
#include "kwidget_p.h"

KX_WIDGET_CREATOR_GLOBAL_STATIC(KToolBar)

class KToolBarPrivate : public KWidgetPrivate
{
public:
	KToolBarPrivate()
	{

	}

	struct WidgetInfo
	{
		KWidget *widget;
		qreal itemSpacing;
		qreal itemWidth;
	};
	QList<WidgetInfo> lstWidgetInfo;
};

KToolBar::KToolBar( QGraphicsItem *parent /*= NULL*/ )
    : KWidget(*new KToolBarPrivate(), parent)
{
	setFlags(QGraphicsItem::ItemHasNoContents);
}

KToolBar::~KToolBar()
{

}

KWidget * KToolBar::findButton( const QString& objectName )
{
	return findChild<KWidget*>(objectName);
}

void KToolBar::showButton( const QString& objectName )
{
	Q_D(KToolBar);

	KWidget *item = findButton(objectName);
	if(item == NULL)
		return;
	if(item->isVisible())
		return;
	for(int i = 0; i < d->lstWidgetInfo.count(); i++)
	{
		const KToolBarPrivate::WidgetInfo &wi = d->lstWidgetInfo.at(i);
		if(wi.widget != item)
			continue;
		item->setFixWidth(wi.itemWidth);
		item->setItemSpacing(wi.itemSpacing);
		item->show();
		trimRightItemSpacing();
		layout()->activate();
		break;
	}
}

void KToolBar::hideButton( const QString& objectName )
{
	KWidget *item = findButton(objectName);
	if(item == NULL)
		return;
	if(!item->isVisible())
		return;
	item->setFixWidth(0);
	item->setItemSpacing(0);
	item->hide();
	trimRightItemSpacing();
	layout()->activate();
}

void KToolBar::childEvent( QChildEvent *event )
{
	__super::childEvent(event);
	return;
	/*当删除对象时，即是*/
	if(event->type() == QEvent::ChildRemoved)
	{
		KWidget *wobj = qobject_cast<KWidget*>(event->child());
		disconnect(wobj, SIGNAL(visibleChanged()), this, SLOT(on_child_visibleChanged()));
	}
}

void KToolBar::construct()
{
	Q_D(KToolBar);
	if(!(layoutType() == VBox || layoutType() == HBox))
		return;
	QGraphicsLinearLayout *layout = (QGraphicsLinearLayout *)QGraphicsWidget::layout();
	for(int i = 0; i < layout->count(); i++)
	{
		QGraphicsLayoutItem *layoutItem = layout->itemAt(i);
		QGraphicsItem *baseItem = layoutItem->graphicsItem();
		if(baseItem == NULL)
			continue;
		QGraphicsObject *wobj = baseItem->toGraphicsObject();
		if(wobj == NULL)
			continue;
		KToolBarPrivate::WidgetInfo wi;
		wi.itemSpacing = layout->itemSpacing(i);
		wi.itemWidth = layoutItem->preferredWidth();
		wi.widget = qobject_cast<KWidget*>(wobj);
		d->lstWidgetInfo.push_back(wi);
		if(!baseItem->isVisible())
		{
			wobj->setProperty("fixWidth", 0);
			wobj->setProperty("itemSpacing", 0);
		}
		bool ok = connect(wobj, SIGNAL(visibleChanged()), this, SLOT(on_child_visibleChanged()));
	}
	layout->activate();
}

void KToolBar::on_child_visibleChanged()
{
	QGraphicsObject *obj = (QGraphicsObject*)(sender());
	if(obj->isVisible())
	{
		showButton(obj->objectName());
	}
	else
	{
		hideButton(obj->objectName());
	}
}

void KToolBar::connectSignalToSlot( const QObject* wobj )
{
	Q_D(KToolBar);

	const QMetaObject *mo = wobj->metaObject();
	for(int i = 0; i < d->lstWidgetInfo.count(); i++)
	{
		const KToolBarPrivate::WidgetInfo &wi = d->lstWidgetInfo.at(i);
		const QString & name = wi.widget->objectName();
		const QString& slot = "1on_" + name + "_triggered()";
		const std::string aslot = slot.toStdString();
		const char *pslot = aslot.data();
		pslot++;
		int idx = mo->indexOfMethod(pslot);
		if(idx > 0)
		{

			bool bOk = QObject::connect(wi.widget, "2clicked()", wobj, aslot.data());
			Q_ASSERT_X(bOk, __FUNCTION__, QString("the %1 item of %2 ToolBar had bad connection.").arg(name).arg(objectName()).toStdString().data());
			Q_UNUSED(bOk);
		}
	}
}

void KToolBar::trimRightItemSpacing()
{
	QGraphicsLinearLayout *layout = (QGraphicsLinearLayout *)QGraphicsWidget::layout();
	KWidget *widgetLast = NULL;
	for(int i = 0; i < layout->count(); i++)
	{
		QGraphicsLayoutItem *layoutItem = layout->itemAt(i);
		QGraphicsItem *baseItem = layoutItem->graphicsItem();
		if(baseItem == NULL)
			continue;
		QGraphicsObject *wobj = baseItem->toGraphicsObject();
		if(wobj == NULL)
			continue;
		if(wobj->isVisible())
		{
			widgetLast = qobject_cast<KWidget*>(wobj);
		}
	}
	if(widgetLast)
	{
		widgetLast->setItemSpacing(0);
	}
}
