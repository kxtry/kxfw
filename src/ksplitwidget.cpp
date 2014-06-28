#include "stdafx.h"
#include "ksplitwidget.h"
#include "kwidget_p.h"
#include "kxmlui.h"
#include "kframeitem.h"


KX_WIDGET_CREATOR_GLOBAL_STATIC(KSplitWidget)

class KSplitWidgetPrivate: public KWidgetPrivate
{
	Q_DECLARE_PUBLIC(KSplitWidget)
public:
	KSplitWidgetPrivate()
		: leftTopWidget(NULL)
		, rightBotWidget(NULL)
		, seperatorThinkness(3)
		, ltMinSize(0)
		, ltMaxSize(-1)
		, rbMinSize(0)
		, rbMaxSize(-1)
		, value(-1)
		, orientation(Qt::Vertical)
	{
	}

	void init()
	{
		Q_Q(KSplitWidget);
		seperator = new KFrameItem(q);
		seperator->setBackgroundColor(QColor(125,125,125));
		seperator->setZValue(100);
		seperator->setDragPolicy(KWidget::Nothing);
		Qt::CursorShape hitCursor = orientation == Qt::Horizontal ? Qt::SplitHCursor : Qt::SplitVCursor;
		seperator->setCursor(hitCursor);
	}

	KFrameItem *seperator;
	QPointer<KWidget> leftTopWidget;
	QPointer<KWidget> rightBotWidget;

	int seperatorThinkness;

	Qt::Orientation orientation;

	int ltMinSize,ltMaxSize;
	int rbMinSize, rbMaxSize;
	int value;

	QRectF splitBarRt;
	QColor splitBarColor;

	QBasicTimer layoutTimer;
};

KSplitWidget::KSplitWidget( QGraphicsItem *parent/*=NULL*/ )
:KWidget(*new KSplitWidgetPrivate(), parent)
{
	Q_D(KSplitWidget);
	d->init();
	setFlag(QGraphicsItem::ItemHasNoContents);
	setAcceptedMouseButtons(Qt::LeftButton);
	setAcceptHoverEvents(true);
}

KSplitWidget::~KSplitWidget()
{

}

KWidget::LayoutType KSplitWidget::layoutType()
{
	return None;
}

void KSplitWidget::setLayoutType( KWidget::LayoutType type )
{

}

void KSplitWidget::addItem( KWidget *item )
{
	Q_D(KSplitWidget);
	if(d->leftTopWidget == NULL)
	{
		d->leftTopWidget = item;
		item->setParent(this);
	}
	else 
	{
		if(d->leftTopWidget == item)
			return;
		if(d->rightBotWidget == NULL)
		{
			d->rightBotWidget = item;
			item->setParent(this);
		}
		else
		{
			if(d->rightBotWidget == item)
				return;
			Q_ASSERT_X(item==NULL, __FUNCTION__, "SplitWidget only support two widget.");
		}
	}
}

void KSplitWidget::removeItem( KWidget *item )
{
	Q_D(KSplitWidget);

}

void KSplitWidget::construct()
{
	
}

int KSplitWidget::leftTopMinSize()
{
	Q_D(KSplitWidget);
	return d->ltMinSize;
}

void KSplitWidget::setLeftTopMinSize( int s )
{
	Q_D(KSplitWidget);
	d->ltMinSize = s;
}

int KSplitWidget::leftTopMaxSize()
{
	Q_D(KSplitWidget);
	return d->ltMaxSize;
}

void KSplitWidget::setLeftTopMaxSize( int s )
{
	Q_D(KSplitWidget);
	d->ltMaxSize = s;
}

int KSplitWidget::rightBotMinSize()
{
	Q_D(KSplitWidget);
	return d->rbMinSize;
}

void KSplitWidget::setRightBotMinSize( int s )
{
	Q_D(KSplitWidget);
	d->rbMinSize = s;
}

int KSplitWidget::rightBotMaxSize()
{
	Q_D(KSplitWidget);
	return d->rbMaxSize;
}

void KSplitWidget::setRightBotMaxSize( int s )
{
	Q_D(KSplitWidget);
	d->rbMaxSize = s;
}

Qt::Orientation KSplitWidget::orientation()
{
	Q_D(KSplitWidget);
	return d->orientation;
}

void KSplitWidget::setOrientation( Qt::Orientation o )
{
	Q_D(KSplitWidget);
	if(d->orientation == 0)
		return;
	d->orientation = o;
	Qt::CursorShape hitCursor = d->orientation == Qt::Horizontal ? Qt::SplitHCursor : Qt::SplitVCursor;
	d->seperator->setCursor(hitCursor);
	updateLayout();
}

int KSplitWidget::value()
{
	Q_D(KSplitWidget);
	return d->value;
}

void KSplitWidget::setValue( int v )
{
	Q_D(KSplitWidget);
	d->value = v;
	updateLayout();
}

void KSplitWidget::resizeEvent( QGraphicsSceneResizeEvent *event )
{
	Q_D(KSplitWidget);
	updateLayout();
}

void KSplitWidget::updateLayout()
{
	Q_D(KSplitWidget);
	if (!d->layoutTimer.isActive())
		d->layoutTimer.start(0, this);
}

void KSplitWidget::_updateLayout()
{
	Q_D(KSplitWidget);
	
	d->layoutTimer.stop();

	int minSize = d->ltMinSize + d->rbMinSize + d->seperatorThinkness;
	int maxFreeSize = height() - minSize;
	if(d->orientation == Qt::Horizontal)
		maxFreeSize = width() - minSize;
	QRectF ltRt, barRt, rbRt;
	if(maxFreeSize < 0)
	{
		if(d->orientation == Qt::Vertical)
		{
			ltRt.setRect(0, 0, width(), d->ltMinSize);
			barRt.setRect(0, d->ltMinSize, width(), d->seperatorThinkness);
			rbRt.setRect(0, d->ltMinSize+d->seperatorThinkness, width(), height()-d->ltMinSize-d->seperatorThinkness);
		}
		else
		{
			ltRt.setRect(0, 0, d->ltMinSize, height());
			barRt.setRect(d->ltMinSize, 0, d->seperatorThinkness, height());
			rbRt.setRect(d->ltMinSize+d->seperatorThinkness, 0, width()-d->ltMinSize-d->seperatorThinkness, height());
		}
		d->value = d->ltMinSize;
	}
	else if(d->ltMaxSize > 0)
	{
		if(d->orientation == Qt::Vertical)
		{
			if(d->value < 0)
			{
				d->value = height() / 2;
			}
			if(d->value > d->ltMaxSize)
			{
				d->value = d->ltMaxSize;
			}
			if(d->value < d->ltMinSize)
			{
				d->value = d->ltMinSize;
			}
			if(d->value > height() - d->rbMinSize - d->seperatorThinkness)
			{
				d->value = height() - d->rbMinSize - d->seperatorThinkness;
			}

			ltRt.setRect(0, 0, width(), d->value);
			barRt.setRect(0, d->value, width(), d->seperatorThinkness);
			rbRt.setRect(0, d->value+d->seperatorThinkness, width(), height()-d->value-d->seperatorThinkness);
		}
		else
		{
			if(d->value < 0)
			{
				d->value = width() / 2;
			}
			if(d->value > d->ltMaxSize)
			{
				d->value = d->ltMaxSize;
			}
			if(d->value < d->ltMinSize)
			{
				d->value = d->ltMinSize;
			}
			if(d->value > width() - d->rbMinSize - d->seperatorThinkness)
			{
				d->value = width() - d->rbMinSize - d->seperatorThinkness;
			}

			ltRt.setRect(0, 0, d->value, height());
			barRt.setRect(d->value, 0, d->seperatorThinkness, height());
			rbRt.setRect(d->value+d->seperatorThinkness, 0, width()-d->value-d->seperatorThinkness, height());
		}
	}
	else if(d->rbMaxSize > 0)
	{
		if(d->orientation == Qt::Vertical)
		{
			if(d->value < 0)
			{
				d->value = height() / 2;
			}
			if(d->value < height() - d->rbMaxSize - d->seperatorThinkness)
			{
				d->value = height() - d->rbMaxSize - d->seperatorThinkness;
			}
			if(d->value < d->ltMinSize)
			{
				d->value = d->ltMinSize;
			}
			if(d->value > height() - d->rbMinSize - d->seperatorThinkness)
			{
				d->value = height() - d->rbMinSize - d->seperatorThinkness;
			}

			ltRt.setRect(0, 0, width(), d->value);
			barRt.setRect(0, d->value, width(), d->seperatorThinkness);
			rbRt.setRect(0, d->value+d->seperatorThinkness, width(), height()-d->value-d->seperatorThinkness);
		}
		else
		{
			if(d->value < 0)
			{
				d->value = width() / 2;
			}
			if(d->value < width() - d->rbMaxSize - d->seperatorThinkness)
			{
				d->value = width() - d->rbMaxSize - d->seperatorThinkness;
			}
			if(d->value < d->ltMinSize)
			{
				d->value = d->ltMinSize;
			}
			if(d->value > width() - d->rbMinSize - d->seperatorThinkness)
			{
				d->value = width() - d->rbMinSize - d->seperatorThinkness;
			}

			ltRt.setRect(0, 0, d->value, height());
			barRt.setRect(d->value, 0, d->seperatorThinkness, height());
			rbRt.setRect(d->value+d->seperatorThinkness, 0, width()-d->value-d->seperatorThinkness, height());
		}
	}
	else
	{
		if(d->orientation == Qt::Vertical)
		{
			if(d->value < 0)
			{
				d->value = height() / 2;
			}
			if(d->value < d->ltMinSize)
			{
				d->value = d->ltMinSize;
			}
			if(d->value > height() - d->rbMinSize - d->seperatorThinkness)
			{
				d->value = height() - d->rbMinSize - d->seperatorThinkness;
			}
			ltRt.setRect(0, 0, width(), d->value);
			barRt.setRect(0, d->value, width(), d->seperatorThinkness);
			rbRt.setRect(0, d->value+d->seperatorThinkness, width(), height()-d->value-d->seperatorThinkness);
		}
		else
		{
			if(d->value < 0)
			{
				d->value = width() / 2;
			}
			if(d->value < d->ltMinSize)
			{
				d->value = d->ltMinSize;
			}
			if(d->value > width() - d->rbMinSize - d->seperatorThinkness)
			{
				d->value = width() - d->rbMinSize - d->seperatorThinkness;
			}

			ltRt.setRect(0, 0, d->value, height());
			barRt.setRect(d->value, 0, d->seperatorThinkness, height());
			rbRt.setRect(d->value+d->seperatorThinkness, 0, width()-d->value-d->seperatorThinkness, height());
		}
	}

	if(d->leftTopWidget)
	{
		d->leftTopWidget->setGeometry(ltRt);
		d->leftTopWidget->setFixSize(ltRt.size());
	}
	if(d->rightBotWidget)
	{
		d->rightBotWidget->setGeometry(rbRt);
		d->rightBotWidget->setFixSize(rbRt.size());
	}
	d->seperator->setGeometry(barRt);
	d->splitBarRt = barRt;
}

void KSplitWidget::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	Q_D(KSplitWidget);
	if(d->splitBarRt.contains(event->pos()))
	{
		event->setAccepted(true);
	}
	else
	{
		unsetCursor();
		event->setAccepted(false);
	}
}

void KSplitWidget::mouseReleaseEvent( QGraphicsSceneMouseEvent * event )
{
	Q_D(KSplitWidget);
	unsetCursor();
}

void KSplitWidget::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
	Q_D(KSplitWidget);
	if(d->orientation == Qt::Horizontal)
	{
		d->value = event->pos().x();
	}
	else
	{
		d->value = event->pos().y();
	}
	if(d->value < 0)
		d->value = 0;
	else if(d->value > height())
		d->value = height();
	updateLayout();
}

QObject* KSplitWidget::splitbar()
{
	Q_D(KSplitWidget);
	return d->seperator;
}

int KSplitWidget::splitBarSize()
{
	Q_D(KSplitWidget);
	return d->seperatorThinkness;
}

void KSplitWidget::setSplitBarSize( int s )
{
	Q_D(KSplitWidget);
	d->seperatorThinkness = s;
	updateLayout();
}

void KSplitWidget::timerEvent( QTimerEvent * event )
{
	Q_D(KSplitWidget);
	if (event->type() == QEvent::Timer)
	{
		if(static_cast<QTimerEvent*>(event)->timerId() == d->layoutTimer.timerId())
		{
			_updateLayout();
		}
	}
	__super::timerEvent(event);
}
