#include "stdafx.h"
#include "ksplitwidgetext.h"
#include "ksplitwidgetext_p.h"
#include "kimageitem.h"
#include "ktheme.h"
#include "kxmlui.h"

KX_WIDGET_CREATOR_GLOBAL_STATIC(KSplitWidgetExt)

KSplitWidgetExt::KSplitWidgetExt( QGraphicsItem *parent /*= NULL*/ )
: KWidget((*new KSplitWidgetExtPrivate()), parent)
{
	Q_D(KSplitWidgetExt);
	d->init();
	setAcceptedMouseButtons(Qt::LeftButton);
	setDragPolicy( KWidget::Nothing );
	setFlag(QGraphicsItem::ItemHasNoContents);
}

KSplitWidgetExt::KSplitWidgetExt( Qt::Orientation orientation, QGraphicsItem *parent /*= NULL*/ )
: KWidget((*new KSplitWidgetExtPrivate()), parent)
{
	Q_D(KSplitWidgetExt);
	d->init();
	d->setOrientation(orientation);
	setAcceptedMouseButtons(Qt::LeftButton);
	setDragPolicy( KWidget::Nothing );
}

KSplitWidgetExt::~KSplitWidgetExt()
{
}


void KSplitWidgetExt::setOrientation( Qt::Orientation orientation )
{
	Q_D(KSplitWidgetExt);
	d->setOrientation(orientation);
}

Qt::Orientation KSplitWidgetExt::orientation() const
{
	Q_D( const KSplitWidgetExt );
	return d->orientation();
}

void KSplitWidgetExt::insertItem( KWidget* item, int index /*= -1 */ )
{
	Q_D(KSplitWidgetExt);
	int ret = d->insertItem(item, index);
	if (ret != -1)
	{
		connect(item, SIGNAL(destroyed(QObject *)), SLOT(onItemDestroyed(QObject*)));
	}
}


void KSplitWidgetExt::removeItem( KWidget* item )
{
	Q_D(KSplitWidgetExt);
	d->removeItem(item);
}

void KSplitWidgetExt::removeItem( int index )
{
	Q_D(KSplitWidgetExt);
	d->removeItem(index);
}

void KSplitWidgetExt::resizeEvent( QGraphicsSceneResizeEvent *event )
{
	Q_D(KSplitWidgetExt);
	d->resizeEvent(event);
}

void KSplitWidgetExt::showEvent( QShowEvent *event )
{
	Q_D(KSplitWidgetExt);
	d->showEvent(event);
}

void KSplitWidgetExt::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
	if (Qt::LeftButton != event->button())
	{
		return;
	}
	Q_D(KSplitWidgetExt);
	d->mousePressEvent(event);
}

void KSplitWidgetExt::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
	if (Qt::LeftButton != event->button())
	{
		return;
	}
	Q_D(KSplitWidgetExt);
	d->mouseReleaseEvent(event);
}

void KSplitWidgetExt::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
	Q_D(KSplitWidgetExt);
	d->mouseMoveEvent(event);
}

void KSplitWidgetExt::addItem( KWidget* item )
{
	insertItem(item, -1);
}

KWidget * KSplitWidgetExt::itemByIndex( int index ) const
{
	Q_D( const KSplitWidgetExt );
	return d->itemByIndex(index);
}

int KSplitWidgetExt::itemIndex( KWidget *item )
{
	Q_D(KSplitWidgetExt);
	return d->itemIndex(item);
}

bool KSplitWidgetExt::setItemPrefferredSize( KWidget* item, qreal prefferredSize )
{
	Q_D(KSplitWidgetExt);
	return d->setItemPrefferredSize(item, prefferredSize);
}

bool KSplitWidgetExt::getSpliterPreferredSize(int index , int& w, int& h)
{
    Q_D(KSplitWidgetExt);
    return d->getSpliterPreferredSize(index, w, h);
}

void KSplitWidgetExt::relayout()
{
    Q_D(KSplitWidgetExt);
    return d->relayout(size());
}

void KSplitWidgetExt::onItemDestroyed(QObject *obj)
{
	KWidget *w = static_cast<KWidget*>(obj);
	removeItem(w);
}

QString KSplitWidgetExt::splitBarImage() const
{
	Q_D(const KSplitWidgetExt);
	return d->splitbarImage;
}

void KSplitWidgetExt::setSplitBarImage( const QString& img )
{
	Q_D(KSplitWidgetExt);
	d->splitbarImage = img;
}

int KSplitWidgetExt::splitBarSize() const
{
	Q_D(const KSplitWidgetExt);
	return d->splitbarSize;
}

void KSplitWidgetExt::setSplitBarSize( int s )
{
	Q_D(KSplitWidgetExt);
	d->splitbarSize = s;
}
