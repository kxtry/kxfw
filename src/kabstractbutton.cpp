#include "stdafx.h"
#include "kabstractbutton.h"
#include "kabstractbutton_p.h"

KAbstractButtonPrivate::KAbstractButtonPrivate(KAbstractButton::ButtonType type)
: typeButton(type)
{
	statesButton = KAbstractButton::StateNormal;
}


void KAbstractButtonPrivate::init()
{
	Q_Q(KAbstractButton);
	q->setAcceptHoverEvents(true);
	q->setAcceptedMouseButtons(Qt::LeftButton);
	q->setDragPolicy(KWidget::Nothing);
}

KAbstractButton::KAbstractButton( QGraphicsItem *parent, ButtonType type ) 
: KWidget(*new KAbstractButtonPrivate(type), parent)
{
	d_func()->init();
}

KAbstractButton::KAbstractButton( KAbstractButtonPrivate& dd, QGraphicsItem *parent /*= NULL*/)
: KWidget(dd, parent)

{
	d_func()->init();
}

KAbstractButton::~KAbstractButton()
{

}

void KAbstractButton::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	event->accept();
	switch(d_func()->typeButton)
	{
	case CheckButton:
		{
			d_func()->statesButton &= CheckMask;
			d_func()->statesButton |= StatePressed;
		}
		break;
	case PushButton:
		{
			d_func()->statesButton = StatePressed;
		}
		break;
	}
	updateState(d_func()->statesButton);
	emit pressed(event->pos());
}

void KAbstractButton::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
	event->accept();

	switch(d_func()->typeButton)
	{
	case CheckButton:
		{
			d_func()->statesButton &= CheckMask;
			d_func()->statesButton |= StateNormal;
			d_func()->statesButton ^= StateChecked;
		}
		break;
	case PushButton:
		{
			d_func()->statesButton = StateNormal;
		}
		break;
	}
	updateState(d_func()->statesButton);
	emit released( event->pos() );
	emit clicked( );
}

void KAbstractButton::hoverEnterEvent( QGraphicsSceneHoverEvent * event )
{
	event->accept();

	switch(d_func()->typeButton)
	{
	case CheckButton:
		{
			d_func()->statesButton &= CheckMask;
			d_func()->statesButton |= StateHover;
		}
		break;
	case PushButton:
		{
			d_func()->statesButton = StateHover;
		}
		break;
	}
	updateState(d_func()->statesButton);
}


void KAbstractButton::hoverMoveEvent( QGraphicsSceneHoverEvent *event )
{
	event->accept();

	switch(d_func()->typeButton)
	{
	case CheckButton:
		{
			d_func()->statesButton &= CheckMask;
			d_func()->statesButton |= StateHover;
		}
		break;
	case PushButton:
		{
			d_func()->statesButton = StateHover;
		}
		break;
	}
	updateState(d_func()->statesButton);
}

void KAbstractButton::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
{
	event->accept();

	switch(d_func()->typeButton)
	{
	case CheckButton:
		{
			d_func()->statesButton &= CheckMask;
			d_func()->statesButton |= StateNormal;
		}
		break;
	case PushButton:
		{
			d_func()->statesButton = StateNormal;
		}
		break;
	}
	updateState(d_func()->statesButton);
}

void KAbstractButton::resizeEvent( QGraphicsSceneResizeEvent * event )
{
	updateLayout();
}

bool KAbstractButton::enabled() const
{
	return !d_func()->statesButton.testFlag(StateDisable);
}

void KAbstractButton::setEnabled( bool on )
{
	Q_D(KAbstractButton);
	if(on)
	{
		d->statesButton &= ~StateDisable;
	}
	else
	{
		d->statesButton |= StateDisable;
	}
	__super::setEnabled(on);
	updateState(d->statesButton);
}

bool KAbstractButton::checked() const
{
	return d_func()->statesButton & StateChecked;
}

void KAbstractButton::setChecked( bool on )
{
	Q_D(KAbstractButton);
	KAbstractButton::ButtonStates bsold = d->statesButton;
	if(on)
	{
		d->statesButton |= StateChecked;
	}
	else
	{
		d->statesButton &= ~StateChecked;
	}
	if(bsold == d->statesButton)
		return;
	updateState(d->statesButton);
}
