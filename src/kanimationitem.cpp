#include "stdafx.h"
#include "kanimationitem.h"

#include <QMovie>

#include "kwidget_p.h"
#include "kresource.h"
#include "kxmlui.h"

class KAnimationItemPrivate : public KWidgetPrivate
{
public:
	Q_DECLARE_PUBLIC(KAnimationItem);

	KAnimationItemPrivate(void)
		: movieShow( NULL )
		, isPlayLoop( true )
		, playPolicy(KAnimationItem::NoDefine)
		, smooth(false)
		, stretch(false)
		, borderPixel(0)
		, roundPixel(0)
	{
	}

	~KAnimationItemPrivate(void)
	{

	}

private:
	bool				isPlayLoop;
	QPointer<QMovie>	movieShow;		//播放器
	QColor				borderColor;
	int					borderPixel;
	int					roundPixel;
	QByteArray			data;
	QBuffer				buf;
	KAnimationItem::PlayPolicy	playPolicy;
	QMargins			margins;
	bool				smooth;
	bool				stretch;
};

//////////////////////////////////////////////////////////////////////////

KX_WIDGET_CREATOR_GLOBAL_STATIC(KAnimationItem)

KAnimationItem::KAnimationItem( QGraphicsItem* parent ) 
: KWidget(*new KAnimationItemPrivate(), parent)
{
	init();
}

void KAnimationItem::init()
{
	Q_D(KAnimationItem);
	setFlag(QGraphicsItem::ItemClipsToShape);
	d->movieShow = new QMovie(this);
	connect(d->movieShow, SIGNAL(frameChanged(int)), this, SLOT(on_frameChanged(int)));
}

void KAnimationItem::on_frameChanged ( int frameNumber )
{
	Q_UNUSED(frameNumber);
	Q_D(KAnimationItem);

	if (!d->isPlayLoop && frameNumber == d->movieShow->frameCount() - 1 )
	{
		stop();
	}
	update();
}

QString KAnimationItem::imagePath() const
{
	return d_func()->movieShow->fileName();
}

#define FIX_LIST_ITEM_QUICK_SCROL_BUG

void KAnimationItem::setImagePath( const QString &imageName )
{
	Q_D(KAnimationItem);
	
	QMovie::MovieState ms = d->movieShow->state();
	if(ms != QMovie::NotRunning)
	{
		d->movieShow->stop();
	}

	if ( imageName.isEmpty() )
	{
		d->data.clear();
		d->buf.close();
		d->movieShow->setDevice( NULL );
	}
	else
	{
		d->data = KResource::loadRawData(imageName);

		d->buf.close();
		d->buf.setData( d->data );
#ifdef FIX_LIST_ITEM_QUICK_SCROL_BUG
		if(d->movieShow->isValid())
		{
			/*
			以下清除是为解决，在列表中快速滚动时，而造成重新设置动画图片而失效问题。
			一般情况下，没有以下清除动作，是不会出现问题的，但在列表中快速滚动时，却出现了，别无它法。
			*/
			qDeleteLater(d->movieShow);
			d->movieShow = new QMovie(this);
			QObject::connect(d->movieShow, SIGNAL(frameChanged(int)), this, SLOT(on_frameChanged(int)));
		}
#endif
		d->movieShow->setDevice( &d->buf );
		d->movieShow->jumpToFrame(0);

		if(ms == QMovie::Paused)
		{
			d->movieShow->start();
			d->movieShow->setPaused(true);
		}
		else if(ms == QMovie::Running)
		{
			d->movieShow->start();
		}
	}
	update();
}

void KAnimationItem::start()
{
	Q_D(KAnimationItem);
	d->movieShow->start();
	update();
}

void KAnimationItem::stop()
{
	Q_D(KAnimationItem);
	d->movieShow->stop();
	d->movieShow->jumpToFrame(0);
	update();
}

void KAnimationItem::resizeEvent ( QGraphicsSceneResizeEvent * event )
{
	__super::resizeEvent(event);
}

void KAnimationItem::on_finished()
{
	Q_D(KAnimationItem);

	if ( !d->isPlayLoop )
	{
		stop();
	}
}

void KAnimationItem::setPlayLoop( bool isLoop )
{
	Q_D(KAnimationItem);
	d->isPlayLoop = isLoop;
}

bool KAnimationItem::playLoop() const
{
	Q_D(const KAnimationItem);
	return d->isPlayLoop;
}

QColor KAnimationItem::borderColor() const
{
	return d_func()->borderColor;	
}

void KAnimationItem::setBorderColor( const QColor &color )
{
	Q_D(KAnimationItem);

	d->borderColor = color;
}

void KAnimationItem::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */ )
{
	Q_D(KAnimationItem);

	/*绘制动画*/
	if(!d->data.isEmpty())
	{
		bool onold = painter->testRenderHint(QPainter::SmoothPixmapTransform);
		painter->setRenderHint(QPainter::SmoothPixmapTransform, d->smooth);
		QPixmap pix = d->movieShow->currentPixmap();
		QRectF marginRt = rect();
		QSizeF drawSize = pix.size();
		marginRt.adjust(d->margins.left(), d->margins.top(), -d->margins.right(), -d->margins.bottom());
		if(d->stretch)
		{
			qreal HScale = (marginRt.width() - d->borderPixel * 2) / pix.width();
			qreal VScale = (marginRt.height() - d->borderPixel * 2) / pix.height();
			drawSize = pix.size() * qMin(HScale, VScale);
		}
		QRectF drawRt = QRectF(d->margins.left(), d->margins.top(), drawSize.width(), drawSize.height());
		drawRt.translate((marginRt.width() - drawSize.width()) / 2, (marginRt.height() - drawSize.height()) / 2);
		painter->drawPixmap(drawRt, pix, QRectF(0, 0, pix.width(), pix.height()));
		painter->setRenderHint(QPainter::SmoothPixmapTransform, onold);
	}

	/*绘制边框*/
	if ( d->borderColor.isValid() && d->borderPixel > 0 )
	{
		QPainterPath outerPath;
		QRect outerRect = rect().toRect().adjusted(d->borderPixel, d->borderPixel, -d->borderPixel, -d->borderPixel);
		if(d->roundPixel > 0)
		{
			outerPath.addRoundedRect(outerRect, d->roundPixel, d->roundPixel);
		}
		else
		{
			outerPath.addRect(outerRect);
		}
		QPen penOld = painter->pen();
		QPen pen = penOld;
		pen.setWidth(d->borderPixel);
		pen.setColor(d->borderColor);
		painter->setPen(pen);
		painter->drawPath(outerPath);
		painter->setPen(penOld);
	}
}

QSize KAnimationItem::getMovieSize(bool simple )
{
    Q_D(KAnimationItem);
    int w = 0;
    int h = 0;
    if(d->movieShow)
    {
		if(simple)
		{
			d->movieShow->jumpToFrame(0);
			QPixmap pix = d->movieShow->currentPixmap();
			return pix.size();
		}
		int fcount = d->movieShow->frameCount();
		for (int i=0;i<fcount;++i)
		{
			d->movieShow->jumpToFrame(i);
			QImage  img =d->movieShow->currentImage();
			if(!img.isNull())
			{
				if(img.rect().width()>w)
				{
					w = img.rect().width();
				}
				if(img.rect().height()>h)
				{
					h = img.rect().height();
				}
			}
		}		
		d->movieShow->jumpToFrame(0);
		return QSize(w, h);
    }

	return QSize(0,0);
}


void KAnimationItem::visibleEvent( bool bvis )
{
	Q_D(KAnimationItem);
	if(bvis)
	{
		if(d->playPolicy == PlayToVisible)
		{
			start();
		}

	}
	else
	{
		stop();
	}
}

KAnimationItem::PlayPolicy KAnimationItem::playPolicy()
{
	Q_D(KAnimationItem);
	return d->playPolicy;
}

void KAnimationItem::setPlayPolicy( PlayPolicy p )
{
	Q_D(KAnimationItem);
	d->playPolicy = p;
	if(p == PlayOnHover)
	{
		setAcceptHoverEvents(true);
	}
	else
	{
		setAcceptHoverEvents(false);
	}
}

void KAnimationItem::hoverEnterEvent( QGraphicsSceneHoverEvent * event )
{
	Q_D(KAnimationItem);
	if(d->playPolicy == PlayOnHover)
	{
		start();
	}
}

void KAnimationItem::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
{
	Q_D(KAnimationItem);
	if(d->playPolicy == PlayOnHover)
	{
		stop();
	}
}

QMargins KAnimationItem::playMargins() const
{
	Q_D(const KAnimationItem);
	return d->margins;
}

void KAnimationItem::setPlayMargins( const QMargins& m )
{
	Q_D(KAnimationItem);
	d->margins = m;
}

bool KAnimationItem::stretch()
{
	Q_D(KAnimationItem);
	return d->stretch;
}

void KAnimationItem::setStretch( bool on )
{
	Q_D(KAnimationItem);
	d->stretch = on;
}

bool KAnimationItem::smooth()
{
	Q_D(KAnimationItem);
	return d->smooth;
}

void KAnimationItem::setSmooth( bool on )
{
	Q_D(KAnimationItem);
	d->smooth = on;
}

int KAnimationItem::borderPixel()
{
	Q_D(KAnimationItem);
	return d->borderPixel;
}

void KAnimationItem::setBorderPixel( int pixel )
{
	Q_D(KAnimationItem);
	d->borderPixel = pixel;
}

QRect KAnimationItem::marginRect()
{
	Q_D(KAnimationItem);
	QRect winRt = rect().toRect();
	return winRt.adjusted(d->margins.left(), d->margins.top(), -d->margins.right(), -d->margins.bottom());
}

int KAnimationItem::roundPixel()
{
	Q_D(KAnimationItem);
	return d->roundPixel;
}

void KAnimationItem::setRoundPixel( int pixel )
{
	Q_D(KAnimationItem);
	d->roundPixel = pixel;
}
