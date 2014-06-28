#include "stdafx.h"
#include "kprogressbar.h"

#include "kframeitem.h"
#include "klabel.h"
#include "kwidget_p.h"
#include "ktheme.h"
#include "kxmlui.h"

class KProgressBarPrivate :public KWidgetPrivate
{	
public:
	Q_DECLARE_PUBLIC(KProgressBar);

	KProgressBarPrivate(void);
	~KProgressBarPrivate(void);

private:
	KFrameItem *m_frame;	// 背景
	KFrameItem *m_processItem;
	KLabel *m_progressText;

	int m_value;			// 当前进度位置
	int m_minimum;
	int m_maximum;
	int m_nMinSize;
	bool m_isTextVisible;
	KProgressBar::ProgressNumberStyle m_numberTexStyle;
	Qt::Orientation m_orientation;
	QColor m_outTextColor;
	QColor m_inTextColor;

	void update();
};

KProgressBarPrivate::KProgressBarPrivate( void )
: m_frame( NULL )
, m_processItem( NULL )
, m_progressText( NULL )
, m_value( 0 )
, m_minimum( 0 )
, m_maximum( 100 )
, m_nMinSize(2)
, m_isTextVisible( true )
, m_numberTexStyle(KProgressBar::NUMBER_PERCENTAGE)
, m_orientation(Qt::Horizontal)
, m_outTextColor(QColor(0,0,0))
, m_inTextColor(QColor(0,0,0))
{

}

KProgressBarPrivate::~KProgressBarPrivate( void )
{

}

void KProgressBarPrivate::update()
{	
	Q_Q(KProgressBar);

	int newSizeValue = 0;
	if ( ( m_maximum - m_minimum ) > 0 )
	{
		if (m_orientation != Qt::Vertical)
		{
			newSizeValue =  ( m_value * m_frame->width() ) / ( m_maximum - m_minimum );
		}
		else
		{
			newSizeValue =  ( m_value * m_frame->height() ) / ( m_maximum - m_minimum );
		}
	}
	else
	{
		Q_ASSERT_X(false, __FUNCTION__, QString("can not divide zero.").toLatin1().constData());
	}

	if ( m_value != 0 && newSizeValue < m_nMinSize )
		newSizeValue = m_nMinSize;

	QRectF processItemRect;
	if (m_orientation != Qt::Vertical)
	{
		processItemRect = QRectF( 0, 0, newSizeValue, m_frame->height() );
	}
	else
	{
		processItemRect = QRectF( 0, m_frame->height()-newSizeValue, m_frame->width(), newSizeValue);
	}
	
	m_processItem->setGeometry( processItemRect );

	if (m_isTextVisible)
	{
		QString progressText;
		
		if(m_progressText == NULL)
		{
			m_progressText = new KLabel(q);
			m_progressText->setZValue(10.0);
			m_progressText->setTextAlignment(Qt::AlignCenter);
		}
		m_progressText->setGeometry(m_frame->rect());
		if (m_numberTexStyle == KProgressBar::JUST_VALUE)
		{
			progressText = QString::number(m_value);
		}
		else if (m_numberTexStyle = KProgressBar::PERCENTAGE_NO_SIGN)
		{
			progressText = QString::number( m_value * 100 / ( m_maximum - m_minimum ) );
		}
		else 
		{
			progressText = QString::number( m_value * 100 / ( m_maximum - m_minimum ) );
			progressText += "%";
		} 
		m_progressText->setText(progressText);

		m_progressText->show();
	}
	else
	{
		if(m_progressText)
			m_progressText->hide();
	}
}

//////////////////////////////////////////////////////////////////////////

KX_WIDGET_CREATOR_GLOBAL_STATIC(KProgressBar)

KProgressBar::KProgressBar(QGraphicsItem* parent) : 
KWidget(*(new KProgressBarPrivate()), parent )
{
	//setFlag(QGraphicsItem::ItemClipsChildrenToShape);
	init();
}

void KProgressBar::setValue(int value)
{
	Q_D(KProgressBar);

	d->m_value = value;

	d->update();

	emit valueChanged( value );
}

void KProgressBar::init()
{
	Q_D(KProgressBar);
	d->m_frame = new KFrameItem(":/image/theme/hslider_bkg.png", KFrameItem::GridFour , this);	
	d->m_processItem = new KFrameItem( ":/image/theme/slider_select.png", KFrameItem::GridFour, this);
	d->m_frame->setGeometry( 0, 0, 210, 9 );
	d->m_processItem->setGeometry( d->m_frame->rect() );
}

void KProgressBar::setProgressImage( const QString& imgs )
{
	Q_D(KProgressBar);
	d->m_processItem->setImagePath(imgs);
}

QString KProgressBar::progressImage() const
{
	Q_D(const KProgressBar);
	return d->m_processItem->imagePath();
}

void KProgressBar::setRange( int minimum, int maximum )
{
	if ( maximum <= minimum || maximum == 0 )
	{
		Q_ASSERT(false);
		return;
	}
	Q_D(KProgressBar);
	if(d->m_minimum == minimum && d->m_maximum == maximum)
		return;
	d->m_minimum = minimum;
	d->m_maximum = maximum;
	d->update();
}

void KProgressBar::reset()
{
	d_func()->m_value = d_func()->m_minimum;
	d_func()->update();
}

void KProgressBar::setMaximum( int maximum )
{
	Q_D(KProgressBar);
	if(d->m_maximum == maximum)
		return;
	d->m_maximum = maximum;
	d->update();
}

void KProgressBar::setMinimum( int minimum )
{
	Q_D(KProgressBar);
	if(d->m_minimum == minimum)
		return;
	d->m_minimum = minimum;
	d->update();
}

int KProgressBar::minSize() const
{
	return d_func()->m_nMinSize;
}

void KProgressBar::setMinSize(int size)
{
	d_func()->m_nMinSize = size;
	d_func()->update();
}

int KProgressBar::value() const
{
	return d_func()->m_value;
}

int KProgressBar::maximum() const
{
	return d_func()->m_maximum;
}

int KProgressBar::minimum() const
{
	return d_func()->m_minimum;
}

bool KProgressBar::isTextVisible() const
{
	return d_func()->m_isTextVisible;
}

void KProgressBar::setTextVisible( bool visible )
{
	d_func()->m_isTextVisible = visible;
}

void KProgressBar::resizeEvent( QGraphicsSceneResizeEvent * event )
{
	Q_D(KProgressBar);

	d->m_frame->setGeometry( QRectF( 0, 0, event->newSize().width(), event->newSize().height() ));
	d->update();
}

int KProgressBar::numberTextStyle() const
{
	return d_func()->m_numberTexStyle;
}

void KProgressBar::setNumberTextStyle(int style)
{
	d_func()->m_numberTexStyle = (KProgressBar::ProgressNumberStyle)style;
}

Qt::Orientation KProgressBar::orientation() const
{
	return d_func()->m_orientation;
}

void KProgressBar::setOrientation(Qt::Orientation orientation)
{
	Q_D(KProgressBar);
	if(d->m_orientation == orientation)
		return;
	d->m_orientation = orientation;
	if(orientation == Qt::Horizontal)
	{
		d->m_frame->setImagePath(":/image/theme/hslider_bkg.png");
		d->m_processItem->setImagePath(":/image/theme/slider_select.png");
	}
	else
	{
		d->m_frame->setImagePath(":/image/theme/vslider_bkg.png");
		d->m_processItem->setImagePath(":/image/theme/slider_select.png");
	}
}

void KProgressBar::setTextColor(const QColor &outsideColor, const QColor &insideColor)
{
	d_func()->m_outTextColor = outsideColor;
	d_func()->m_inTextColor = insideColor;
}

void KProgressBar::setFrameImage( const QString& imgs )
{
	Q_D(KProgressBar);
	d->m_frame->setImagePath(imgs);
}

QString KProgressBar::frameImage() const
{
	Q_D(const KProgressBar);
	return d->m_frame->imagePath();	
}
