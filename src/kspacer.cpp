#include "stdafx.h"
#include "kspacer.h"
#include "kxmlui.h"


KX_WIDGET_CREATOR_GLOBAL_STATIC(KSpacer)

KSpacer::KSpacer( QGraphicsItem *parent ) 
: KWidget(parent)
{
	setFlag(QGraphicsItem::ItemHasNoContents);
}

KSpacer::~KSpacer()
{

}

bool KSpacer::acceptClick() const
{
	return acceptedMouseButtons();
}

void KSpacer::setAcceptClick( bool bClick )
{
	setAcceptedMouseButtons(bClick ? (Qt::LeftButton | Qt::RightButton) : (0));
}

bool KSpacer::acceptHover() const
{
	return acceptHoverEvents();
}

void KSpacer::setAcceptHover( bool bHover )
{
	setAcceptHoverEvents(bHover);
}

void KSpacer::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	emit mouseEvent(event);
}

void KSpacer::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
	emit mouseEvent(event);
}

void KSpacer::hoverEnterEvent( QGraphicsSceneHoverEvent * event )
{
	emit mouseEvent(event);
}

void KSpacer::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
{
	emit mouseEvent(event);
}

void KSpacer::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event )
{
	emit mouseEvent(event);
}

void KSpacer::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
    emit mouseEvent(event);
}