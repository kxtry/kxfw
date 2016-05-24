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
		: movieFrameIndex( 0 )
		, isPlayLoop( true )
		, playPolicy(KAnimationItem::PlayToVisible)
		, smooth(false)
		, stretch(false)
		, borderPixel(0)
		, roundPixel(0)
	{
	}

	~KAnimationItemPrivate(void)
	{

	}

	void extractMovie(const QString& imageName)
	{
		QMovie mov(imageName);
		if(!mov.isValid())
		{
			movieFrameIndex = 0;
			movieFrames = QList<QPixmap>();
			return;
		}
		mov.start();
		int ms = mov.nextFrameDelay();
		if(ms <= 0)
		{
			ms = 200;
		}
		movieTimer.setInterval(ms);
		if(movieTimer.isActive())
		{
			movieTimer.start();
		}
		for(int i = 0; i < mov.frameCount(); i++)
		{
			mov.jumpToFrame(i);
			QPixmap img = mov.currentPixmap();
			movieFrames.append(img);
		}
	}

private:
	int					movieFrameIndex;
	QTimer				movieTimer;
	QList<QPixmap>		movieFrames;
	bool				isPlayLoop;
	QColor				borderColor;
	int					borderPixel;
	int					roundPixel;
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

	QObject::connect(&d->movieTimer, SIGNAL(timeout()), this, SLOT(on_frameChanged()));
	d->movieTimer.setInterval(200);
}

void KAnimationItem::on_frameChanged( )
{
	Q_D(KAnimationItem);
	
	if(d->movieFrames.count())
	{
		int idx = (d->movieFrameIndex + 1) % d->movieFrames.count();
		d->movieFrameIndex = idx;
	}
	
	update();
}

QString KAnimationItem::imagePath() const
{
	return "";
}

void KAnimationItem::setImagePath( const QString &imageName )
{
	Q_D(KAnimationItem);
	
	d->extractMovie(imageName);
	update();
}

void KAnimationItem::start()
{
	Q_D(KAnimationItem);
	
	
	d->movieTimer.start();
	update();
	
}

void KAnimationItem::stop()
{
	Q_D(KAnimationItem);
	d->movieTimer.stop();
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

	/*»æÖÆ¶¯»­*/
	if(d->movieFrames.count())
	{
		bool onold = painter->testRenderHint(QPainter::SmoothPixmapTransform);
		painter->setRenderHint(QPainter::SmoothPixmapTransform, d->smooth);
		QPixmap pix = d->movieFrames[d->movieFrameIndex];
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

	/*»æÖÆ±ß¿ò*/
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
    if(d->movieFrames.count() > 0)
    {
		if(simple)
		{
			QPixmap pix = d->movieFrames[0];
			return pix.size();
		}
		for (int i=0;i<d->movieFrames.count();++i)
		{
			QPixmap  img = d->movieFrames[i];
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
