#include "stdafx.h"
#include "kicon.h"
#include "kwidget_p.h"
#include "kxmlui.h"

KX_WIDGET_CREATOR_GLOBAL_STATIC(KIcon)
class KIconPrivate: public KWidgetPrivate
{
public:
	KIconPrivate()
		: mode(KIcon::Normal)
		, state(KIcon::Off)
		, iconAlignment(Qt::AlignCenter)
	{

	}
	QIcon icon;
	KIcon::Mode mode;
	KIcon::State state;
	Qt::Alignment iconAlignment;
};

KIcon::KIcon( QGraphicsItem *parent /*= NULL*/ )
: KWidget(*new KIconPrivate(), parent)
{

}

KIcon::~KIcon()
{

}

QIcon KIcon::icon() const
{
	return d_func()->icon;
}

void KIcon::setIcon( const QIcon& ico )
{
	d_func()->icon = ico;
	update();
}

QString KIcon::iconPath() const
{
	return d_func()->icon.name();
}

void KIcon::setIconPath( const QString& file )
{
	d_func()->icon = QIcon(file);
	update();
}

KIcon::Mode KIcon::mode() const
{
	return d_func()->mode;
}

void KIcon::setMode( KIcon::Mode m )
{
	d_func()->mode = m;
}

KIcon::State KIcon::state() const
{
	return d_func()->state;
}

void KIcon::setState( KIcon::State s )
{
	d_func()->state = s;
}

Qt::Alignment KIcon::iconAlignment() const
{
	return d_func()->iconAlignment;
}

void KIcon::setIconAlignment( const Qt::Alignment align )
{
	d_func()->iconAlignment = align;
}

void KIcon::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget /*= 0*/ )
{
	Q_D(KIcon);
	QIcon::Mode mode = QIcon::Normal;
	switch(d_func()->mode)
	{
	case Disabled:
		mode = QIcon::Disabled;
		break;
	case Active:
		mode = QIcon::Active;
		break;
	case Selected:
		mode = QIcon::Selected;
		break;
	}
	QIcon::State state = d->state == Off ? QIcon::Off : QIcon::On;
	d->icon.paint(painter, rect().toRect(), d->iconAlignment, mode, state);
}

QSize KIcon::actualSize( const QSize & size/*=QSize()*/, Mode mode /*= Normal*/, State state /*= Off */ ) const
{
	Q_D(const KIcon);
	return d->icon.actualSize(size, (QIcon::Mode)mode, (QIcon::State)state);
}
