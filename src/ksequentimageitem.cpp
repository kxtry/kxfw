#include "stdafx.h"
#include "ksequentimageitem.h"

#include "ktheme.h"
#include "kwidget_p.h"
#include "kxmlui.h"

KX_WIDGET_CREATOR_GLOBAL_STATIC(KSequentImageItem)

class KSequentImageItemPrivate : public KWidgetPrivate
{
public:
	KSequentImageItemPrivate()
		: row(1)
		, column(1)
		, index(0)
		, fps(24)
		, bAutoStart(true)
		, bPlayLoop(true)
	{

	}

	QPixmap matrixPixmap;
	QString imagePath;
	int row;
	int column;
	bool bAutoStart;
	bool bPlayLoop;
	int index;
	int fps;
	QTimer timer;
};

KSequentImageItem::KSequentImageItem( QGraphicsItem *parent/*=NULL*/ )
: KWidget(*new KSequentImageItemPrivate(), parent)
{
	Q_D(KSequentImageItem);
	QObject::connect(&d->timer, SIGNAL(timeout()), this, SLOT(on_timeout()));
	setFlag(QGraphicsItem::ItemClipsToShape);
}

KSequentImageItem::~KSequentImageItem()
{

}

QString KSequentImageItem::imagePath() const
{
	Q_D(const KSequentImageItem);
	return d->imagePath;
}

void KSequentImageItem::setImagePath( const QString& imgs )
{
	Q_D(KSequentImageItem);
	d->imagePath = imgs;
	d->matrixPixmap = KTheme::getPixmap(imgs);
}

QSize KSequentImageItem::rowcolumn() const
{
	Q_D(const KSequentImageItem);
	return QSize(d->row, d->column);
}

void KSequentImageItem::setRowColumn( const QSize& rc )
{
	setRowColumn(rc.width(), rc.height());
}

void KSequentImageItem::setRowColumn( int row, int col )
{
	Q_D(KSequentImageItem);
	d->row = row;
	d->column = col;
}

void KSequentImageItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /*= 0*/ )
{
	Q_D(KSequentImageItem);
	painter->drawPixmap(drawAnimationRect(), d->matrixPixmap, sourcePixmapRect(d->index));
}

void KSequentImageItem::start()
{
	Q_D(KSequentImageItem);
	d->index = 0;
	d->timer.start(1000 / d->fps);
}

void KSequentImageItem::stop()
{
	Q_D(KSequentImageItem);
	d->timer.stop();
}

bool KSequentImageItem::autoStart() const
{
	Q_D(const KSequentImageItem);
	return d->bAutoStart;
}

void KSequentImageItem::setAutoStart( bool on )
{
	Q_D(KSequentImageItem);
	d->bAutoStart = on;
	if(isVisible() && on)
	{
		start();
	}
}

bool KSequentImageItem::playLoop() const
{
	Q_D(const KSequentImageItem);
	return d->bPlayLoop;
}

void KSequentImageItem::setPlayLoop( bool on )
{
	Q_D(KSequentImageItem);
	d->bPlayLoop = on;
}

int KSequentImageItem::fps()
{
	Q_D(KSequentImageItem);
	return d->fps;
}

void KSequentImageItem::setFps( int f )
{
	Q_D(KSequentImageItem);
	d->fps = f;	
}



void KSequentImageItem::on_timeout()
{
	Q_D(KSequentImageItem);
	QRectF updateRt = drawAnimationRect();
	update(updateRt);
	d->index++;
	if(d->index >= frameCount())
	{
		if(d->bPlayLoop)
		{
			d->index = 0;
		}
		else
		{
			d->timer.stop();
		}
	}
}

int KSequentImageItem::frameCount()
{
	Q_D(KSequentImageItem);
	return d->row * d->column;
}

QSize KSequentImageItem::frameSize()
{
	Q_D(KSequentImageItem);
	int w = d->matrixPixmap.width() / d->column;
	int h = d->matrixPixmap.height() / d->row;
	return QSize(w, h);
}


QRectF KSequentImageItem::drawAnimationRect()
{
	Q_D(KSequentImageItem);
	QSize s = frameSize();
	return QRectF((width() - s.width()) / 2, (height() - s.height()) / 2, s.width(), s.height());
}

QRectF KSequentImageItem::sourcePixmapRect( int index )
{
	Q_D(KSequentImageItem);
	if(index >= frameCount())
		index = 0;
	int row = index / d->column;
	int col = index % d->column;
	QSize s = frameSize();
	QRectF srcRt(0, 0, s.width(), s.height());
	srcRt.translate(col * s.width(), row * s.height());
	return srcRt;
}

bool KSequentImageItem::example()
{
	return false;
}

void KSequentImageItem::playExample( bool on )
{
	if(on)
	{
		setImagePath(":/image/theme/anijuhua.png");
		setRowColumn(1,8);
	}
	else
	{
		setImagePath("");
		setRowColumn(1,1);
	}
}

void KSequentImageItem::visibleEvent( bool bvis )
{
	Q_D(KSequentImageItem);
	if(bvis)
	{
		if(d->bAutoStart)
		{
			start();
		}
	}
	else
	{
		if(d->bAutoStart)
		{
			stop();
		}
	}
}