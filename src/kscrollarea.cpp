#include "stdafx.h"
#include "kscrollarea.h"
#include "kspacer.h"
#include "kscrollbar.h"
#include "kxmlui.h"

KX_WIDGET_CREATOR_GLOBAL_STATIC(KScrollArea)

KScrollArea::KScrollArea( QGraphicsItem *parent /*= NULL*/ ) 
: KWidget(parent)
,m_vbarPolicy(Qt::ScrollBarAsNeeded)
,m_hbarPolicy(Qt::ScrollBarAsNeeded)
,m_barThickness(15)
,m_bWidgetResizable(true)
,m_bContentUnderBar(false)
,m_content(NULL)
,m_bDragContent(true)
{
	setFlag(QGraphicsItem::ItemHasNoContents);
	setFlag(QGraphicsItem::ItemClipsChildrenToShape);
	m_viewport = new KSpacer(this);
	addObjectItem(m_viewport);
	m_viewport->setFlag(QGraphicsItem::ItemHasNoContents);
	m_viewport->setFlag(QGraphicsItem::ItemClipsChildrenToShape);
	m_viewport->setBackgroundColor(QColor(125,0,0));
	m_vbar = new KScrollBar(this);
	m_hbar = new KScrollBar(this);
	m_hbar->setOrientation(Qt::Horizontal);
	bool bOk = QObject::connect(m_hbar, SIGNAL(valueChanged(qreal)), this, SLOT(on_hscroll_value_changed(qreal)), Qt::QueuedConnection);
	bool bOk1 = QObject::connect(m_vbar, SIGNAL(valueChanged(qreal)), this, SLOT(on_vscroll_value_changed(qreal)), Qt::QueuedConnection);
	bool bOk2 = QObject::connect(m_viewport, SIGNAL(geometryChanged()), this, SLOT(updateScroll()), Qt::QueuedConnection);
	setAcceptedMouseButtons(Qt::LeftButton);
}

KScrollArea::~KScrollArea()
{

}

void KScrollArea::scrollTo( int x, int y )
{

}

void KScrollArea::scrollTo(const QPoint val )
{

}

QPoint KScrollArea::scrollPos() const
{
	return QPoint();
}

int KScrollArea::hscrollPos() const
{
	return 0;
}

int KScrollArea::vscrollPos() const
{
	return 0;
}


void KScrollArea::setWidgetResizable( bool bResize )
{
	if(m_bWidgetResizable == bResize)
		return;
	m_bWidgetResizable = bResize;
	resizeContent();
}

bool KScrollArea::widgetResizable()
{
	return m_bWidgetResizable;
}

QObject* KScrollArea::setContentWidget( QObject* w )
{
	KWidget *widget = qobject_cast<KWidget*>(w);
	if(widget == NULL)
		return m_content;
	if(m_content == widget)
	{
		QObject::disconnect(m_content, SIGNAL(geometryChanged()), this, SLOT(updateScroll()));
		return m_content;
	}
	KWidget *content = m_content;
	m_content = widget;
	m_content->setParentItem(m_viewport);
	m_content->setParent(m_viewport);
	QObject::connect(m_content, SIGNAL(geometryChanged()), this, SLOT(updateScroll()), Qt::QueuedConnection);
	m_content->setPos(0, 0);
	updateScrollArea();
	resizeContent();
	return content;
}

QObject* KScrollArea::contentWidget()
{
	return m_content;
}

void KScrollArea::setScrollBarPolicy( Qt::Orientation orientation, Qt::ScrollBarPolicy policy )
{
	if(orientation == Qt::Vertical)
	{
		m_vbarPolicy = policy;
	}
	else
	{
		m_hbarPolicy = policy;
	}
}

Qt::ScrollBarPolicy KScrollArea::scrollBarPolicy( Qt::Orientation orientation )
{
	if(orientation == Qt::Vertical)
	{
		return m_vbarPolicy;
	}
	else
	{
		return m_hbarPolicy;
	}
}

void KScrollArea::setHScrollBarPolicy( Qt::ScrollBarPolicy policy )
{
	if(m_hbarPolicy == policy)
		return;
	m_hbarPolicy = policy;
}

Qt::ScrollBarPolicy KScrollArea::hscrollBarPolicy() const
{
	return m_hbarPolicy;
}

void KScrollArea::setVScrollBarPolicy( Qt::ScrollBarPolicy policy )
{
	if(m_vbarPolicy == policy)
		return;
	m_vbarPolicy = policy;
}

Qt::ScrollBarPolicy KScrollArea::vscrollBarPolicy() const
{
	return m_vbarPolicy;
}

void KScrollArea::scrollToEnd()
{
	
}

void KScrollArea::scrollToStart()
{

}

bool KScrollArea::contentUnderBar() const
{
	return m_bContentUnderBar;
}

void KScrollArea::setContentUnderBar( bool on )
{
	m_bContentUnderBar = on;
	updateScrollArea();
}

void KScrollArea::resizeEvent( QGraphicsSceneResizeEvent *e )
{
	__super::resizeEvent(e);
	updateScrollArea();
}

int KScrollArea::barThickness() const
{
	return m_barThickness;
}

void KScrollArea::setBarThickness( int iPixel )
{
	m_barThickness = iPixel;
	m_hbar->setFixHeight(iPixel);
	m_vbar->setFixWidth(iPixel);
	updateScrollArea();
}

void KScrollArea::updateScrollArea()
{
	QRectF winRt = rect();
	int barPixels = barThickness();
	Qt::ScrollBarPolicy vbarPolicy = _vscrollPolicy();
	Qt::ScrollBarPolicy hbarPolicy = _hscrollPolicy();

	if(m_bContentUnderBar)
	{
		m_viewport->setFixSize(winRt.width(), winRt.height());
		m_hbar->setPageStep(winRt.width());
		m_vbar->setPageStep(winRt.height());
		if(vbarPolicy == Qt::ScrollBarAlwaysOn && hbarPolicy == Qt::ScrollBarAlwaysOn)
		{
			m_vbar->setFixSize(barPixels, winRt.height());
			m_vbar->setPos(winRt.right() - barPixels, 0);
			m_vbar->show();
			m_hbar->setFixSize(winRt.width() - barPixels, barPixels);
			m_hbar->setPos(0, winRt.height() - barPixels);
			m_hbar->show();		
		}
		else if(vbarPolicy == Qt::ScrollBarAlwaysOn)
		{
			m_vbar->setFixSize(barPixels, winRt.height());
			m_vbar->setPos(winRt.right() - barPixels, 0);
			m_vbar->show();
			m_hbar->setFixSize(0, 0);
			m_hbar->hide();
		}
		else if(hbarPolicy == Qt::ScrollBarAlwaysOn)
		{
			m_hbar->setFixSize(winRt.width(), barPixels);
			m_hbar->setPos(0, winRt.height() - barPixels);
			m_hbar->show();		
			m_vbar->setFixSize(0, 0);
			m_vbar->hide();
		}
		else
		{
			m_hbar->setFixSize(0, 0);
			m_hbar->hide();
			m_vbar->setFixSize(0, 0);
			m_vbar->hide();
		}
	}
	else
	{
		if(vbarPolicy == Qt::ScrollBarAlwaysOn && hbarPolicy == Qt::ScrollBarAlwaysOn)
		{
			m_viewport->setFixSize(winRt.width() - barPixels, winRt.height() - barPixels);
			m_vbar->setFixSize(barPixels, winRt.height());
			m_vbar->setPos(winRt.right() - barPixels, 0);
			m_vbar->show();
			m_hbar->setFixSize(winRt.width() - barPixels, barPixels);
			m_hbar->setPos(0, winRt.height() - barPixels);
			m_hbar->show();
			m_hbar->setPageStep(winRt.width() - barPixels);
			m_vbar->setPageStep(winRt.height() - barPixels);
		}
		else if(vbarPolicy == Qt::ScrollBarAlwaysOn)
		{
			m_viewport->setFixSize(winRt.width() - barPixels, winRt.height());
			m_vbar->setFixSize(barPixels, winRt.height());
			m_vbar->setPos(winRt.right() - barPixels, 0);
			m_vbar->setPageStep(winRt.height());
			m_vbar->show();
			m_hbar->setFixSize(0, 0);
			m_hbar->hide();
		}
		else if(hbarPolicy == Qt::ScrollBarAlwaysOn)
		{
			m_viewport->setFixSize(winRt.width(), winRt.height() - barPixels);
			m_hbar->setFixSize(winRt.width(), barPixels);
			m_hbar->setPos(0, winRt.height() - barPixels);
			m_hbar->setPageStep(winRt.width());
			m_hbar->show();		
			m_vbar->setFixSize(0, 0);
			m_vbar->hide();
		}
		else
		{
			m_viewport->setFixSize(winRt.width(), winRt.height());
			m_hbar->setFixSize(0, 0);
			m_hbar->hide();
			m_vbar->setFixSize(0, 0);
			m_vbar->hide();
		}
	}
	if(m_content)
	{
		QSizeF contentSize = m_content->size();
		QSizeF stepSize = m_viewport->size();
		QSizeF scrollSize = contentSize - stepSize;
		if(scrollSize.height() > 0)
		{
			m_vbar->setRange(0, scrollSize.height());
		}
		else
		{
			m_vbar->setRange(0, 1);
		}
		if(scrollSize.width() > 0)
		{
			m_hbar->setRange(0, scrollSize.width());
		}
		else
		{
			m_hbar->setRange(0, 1);
		}
	}

}

KWidget* KScrollArea::viewport()
{
	return m_viewport;
}

qreal KScrollArea::barOpacity() const
{
	return m_hbar->opacity();
}

void KScrollArea::setBarOpacity( qreal o )
{
	m_hbar->setOpacity(o);
	m_vbar->setOpacity(o);
}

bool KScrollArea::needVScrollBar()
{
	if(m_content == NULL)
		return false;
	return m_viewport->height() < m_content->height();
}

bool KScrollArea::needHScrollBar()
{
	if(m_content == NULL)
		return false;
	return m_viewport->width() < m_content->width();
}

QObject * KScrollArea::hScrollBar()
{
	return m_hbar;
}

QObject * KScrollArea::vScrollBar()
{
	return m_vbar;
}

void KScrollArea::updateScroll()
{
	QObject *obj = sender();
	if(obj == NULL)
	{
		updateScrollArea();
		resizeContent();
	}
	else if(obj == m_viewport)
	{
		updateScrollArea();
		resizeContent();
	}
	else
	{
		QSizeF size = m_content->size();
		if(size != m_contentSize)
		{
			updateScrollArea();
			resizeContent();
		}
		m_contentSize = size;
	}
}

void KScrollArea::on_vscroll_value_changed( qreal val )
{
	if(m_content == NULL)
		return;
	m_content->setY(-val);
}

void KScrollArea::on_hscroll_value_changed( qreal val )
{
	if(m_content == NULL)
		return;
	m_content->setX(-val);
}

Qt::ScrollBarPolicy KScrollArea::_vscrollPolicy()
{
	Qt::ScrollBarPolicy vbarPolicy = m_vbarPolicy;
	if(vbarPolicy == Qt::ScrollBarAsNeeded)
	{
		if(needVScrollBar())
		{
			vbarPolicy = Qt::ScrollBarAlwaysOn;
		}
		else
		{
			vbarPolicy = Qt::ScrollBarAlwaysOff;
		}
	}
	return vbarPolicy;
}

Qt::ScrollBarPolicy KScrollArea::_hscrollPolicy()
{
	Qt::ScrollBarPolicy hbarPolicy = m_hbarPolicy;
	if(hbarPolicy == Qt::ScrollBarAsNeeded)
	{
		if(needHScrollBar())
		{
			hbarPolicy = Qt::ScrollBarAlwaysOn;
		}
		else
		{
			hbarPolicy = Qt::ScrollBarAlwaysOff;
		}
	}
	return hbarPolicy;
}

void KScrollArea::mousePressEvent( QGraphicsSceneMouseEvent * event )
{
	event->setAccepted(m_bDragContent);
}

void KScrollArea::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
	Qt::MouseButtons mbs = event->buttons();
	if(mbs.testFlag(Qt::LeftButton))
	{
		QPointF val = event->pos() - event->lastPos();
		m_hbar->setValue(m_hbar->value() - val.x());
		m_vbar->setValue(m_vbar->value() - val.y());
	}
}

bool KScrollArea::dragContent() const
{
	return m_bDragContent;
}

void KScrollArea::setDragContent( bool on )
{
	m_bDragContent = on;
}

void KScrollArea::wheelEvent( QGraphicsSceneWheelEvent * e )
{
	if(_vscrollPolicy() == Qt::ScrollBarAlwaysOn)
	{
		m_vbar->wheelEvent(e);
	}
	else if(_hscrollPolicy() == Qt::ScrollBarAlwaysOn)
	{
		m_hbar->wheelEvent(e);
	}
}

void KScrollArea::resizeContent()
{
	if(!m_bWidgetResizable)
		return;
	if(m_hbarPolicy != Qt::ScrollBarAlwaysOff && m_vbarPolicy != Qt::ScrollBarAlwaysOff)
		return;
	QSizeF vportSize = m_viewport->size();
	if(m_vbarPolicy != Qt::ScrollBarAlwaysOff)
	{
		qreal contentHeight = m_content->height();
		m_content->resize(vportSize.width(), contentHeight);
	}
	else
	{
		qreal contentWidth = m_content->width();
		m_content->resize(contentWidth, vportSize.height());
	}
}

KX_WIDGET_CREATOR_GLOBAL_STATIC(KScrollWidget)

KScrollWidget::KScrollWidget( QGraphicsItem *parent/*=NULL*/ )
: KScrollArea(parent)
{
	m_content = new KSpacer(m_viewport);
	setContentWidget(m_content);
	setDragContent(false);
}

KScrollWidget::~KScrollWidget()
{

}

void KScrollWidget::addItem( KWidget *item )
{
	m_content->addItem(item);
}

void KScrollWidget::removeItem( KWidget *item )
{
	m_content->removeItem(item);
}

KWidget::LayoutType KScrollWidget::layoutType()
{
	return m_content->layoutType();
}

void KScrollWidget::setLayoutType( KWidget::LayoutType type )
{
	m_content->setLayoutType(type);
	switch(type)
	{
	case VBox:
		{
			setHScrollBarPolicy(Qt::ScrollBarAlwaysOff);
			m_content->setHeightToChild(true);
		}
		break;
	case HBox:
		{
			setVScrollBarPolicy(Qt::ScrollBarAlwaysOff);
			m_content->setWidthToChild(true);
		}
		break;
	}
}

void KScrollWidget::resizeEvent( QGraphicsSceneResizeEvent *e )
{
	__super::resizeEvent(e);
	//m_timer.start(0, this);
}

void KScrollWidget::timerEvent( QTimerEvent *t )
{
	if(t->timerId() == m_timer.timerId())
	{
		m_timer.stop();
		if(m_content->layoutType() == KWidget::VBox)
		{
			qreal w = m_viewport->width();
			m_content->setFixSize(w, -1);
		}
		else
		{
			qreal h = m_viewport->height();
			m_content->setFixSize(-1, h);
		}
	}
}
