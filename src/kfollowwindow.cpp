#include "stdafx.h"
#include "kfollowwindow.h"
#include "kfollowwindow_p.h"

#include "kwidget.h"
#include "kxmlui.h"

KX_WINDOW_CREATOR_GLOBAL_STATIC(KFollowWindow)

KFollowWindowPrivate::KFollowWindowPrivate()
	: direction(KFollowWindow::BottomCenter) 
	, winSize(100,50)
	, hideFlags(0)
{

}

KFollowWindowPrivate::~KFollowWindowPrivate()
{

}

void KFollowWindowPrivate::init()
{
	Q_Q(KFollowWindow);
	Qt::WindowFlags flags = Qt::FramelessWindowHint|Qt::Window|Qt::CustomizeWindowHint;
	if(q->parent())
	{
		flags |= Qt::Tool;
	}
	else
	{
		flags |= Qt::ToolTip;
	}
	q->setAttribute(Qt::WA_DeleteOnClose);
	q->setTranslucent(false);
	q->setWindowFlags(flags);
	q->setAttribute(Qt::WA_ShowWithoutActivating);
	q->setMinimumSize(10,10);
	q->setMaximumSize(500,500);
}


KFollowWindow::KFollowWindow( QWidget *parent/*=NULL*/ )
:KWindow(*new KFollowWindowPrivate(), parent) 
{
	Q_D(KFollowWindow);
	d->init();
	if(parent)
	{
		QObject::connect(parent, SIGNAL(posChanged()), this, SLOT(on_posChanged()));
	}
	viewport()->installEventFilter(this);
}

KFollowWindow::KFollowWindow( KWindowPrivate &dd, QWidget *parent /* = NULL*/ )
:KWindow(dd, parent)
{
	Q_D(KFollowWindow);
	d->init();
	if(parent)
	{
		QObject::connect(parent, SIGNAL(posChanged()), this, SLOT(on_posChanged()));
	}
	viewport()->installEventFilter(this);
}

KFollowWindow::~KFollowWindow()
{
	Q_D(KFollowWindow);
	viewport()->removeEventFilter(this);
}

void KFollowWindow::setTarget( KWidget *w )
{
	Q_D(KFollowWindow);
	if(d->target == w)
		return;
	d->target = w;
}

KWidget * KFollowWindow::target() const
{
	Q_D(const KFollowWindow);
	return d->target;
}


void KFollowWindow::listenEvent( KWidget *w, ListenFlags flags )
{
	Q_D(KFollowWindow);
	if(w == NULL)
		return;

	if(flags.testFlag(targetVisible))
	{
		QObject::connect(w, SIGNAL(visibleChanged()), this, SLOT(on_visibleChanged()), Qt::QueuedConnection);
	}
	else
	{
		QObject::disconnect(w, SIGNAL(visibleChanged()), this, SLOT(on_visibleChanged()));
	}

	if(flags.testFlag(targetWindowEvent))
	{
		QWidget *viewport = w->viewport();
		viewport->installEventFilter(this);
	}
	else
	{
		QWidget *viewport = w->viewport();
		viewport->removeEventFilter(this);
	}

	if(flags.testFlag(targetEvent))
	{
		w->installEventFilter(this);
	}
	else
	{
		w->removeEventFilter(this);
	}

	if(flags.testFlag(targetScenePosition))
	{
		w->setFlag(QGraphicsItem::ItemSendsScenePositionChanges);
		QObject::connect(w, SIGNAL(scenePosition(QPointF)), this, SLOT(on_scenePosition(QPointF)), Qt::QueuedConnection);
	}
	else
	{
		w->setFlag(QGraphicsItem::ItemSendsScenePositionChanges, false);
		QObject::disconnect(w, SIGNAL(scenePosition(QPointF)), this, SLOT(on_scenePosition(QPointF)));
	}
}

KFollowWindow::Direction KFollowWindow::direction()
{
	Q_D(KFollowWindow);
	return d->direction;
}

void KFollowWindow::setDirection( KFollowWindow::Direction dr )
{
	Q_D(KFollowWindow);
	d->direction = dr;
}

void KFollowWindow::on_scenePosition( const QPointF& pt )
{
	Q_D(KFollowWindow);
	if(sender() != d->target)
		return;
	if(d->targetPt == pt)
		return;
	if(!isVisible())
		return;
	d->targetPt = pt;
	resetGeometry();
}


void KFollowWindow::on_posChanged()
{
	if(!isVisible())
		return;
	resetGeometry();
}


void KFollowWindow::resetGeometry()
{
	Q_D(KFollowWindow);

	if(d->target == NULL)
		return;

	bool bHandled = false;
	emit aboutToAdjust(this, &bHandled);
	if(bHandled)
	{
		return;
	}
	QRectF targetRt = d->target->rectToScreen();
	QSizeF targetSize = targetRt.size();
	QSizeF winSize = d->winSize;
	QRectF winRt(0, 0, winSize.width(), winSize.height());
	QPointF winCenterPt = winRt.center();
	QPointF targetCenterPt = targetRt.center();		

	QSizeF hitDistance = (targetSize + winSize) / 2;
	Direction tmpDirection = d->direction;

	if(tmpDirection == LeftTop)
	{
		QPointF targetPt = QPointF(targetCenterPt.x(), targetRt.top());
		QPointF winPt = QPointF(winCenterPt.x(), winRt.top());
		QPointF offsetPt = targetPt - QPointF(hitDistance.width(), 0) - winPt;

		offsetPt += d->offsetPt;

		winRt.translate(offsetPt);
		setGeometry(winRt.toRect());
	}
	else if(tmpDirection == LeftCenter)
	{
		QPointF offsetPt = targetCenterPt - QPointF(hitDistance.width(), 0) - winCenterPt;

		offsetPt += d->offsetPt;

		winRt.translate(offsetPt);
		setGeometry(winRt.toRect());
	}
	else if(tmpDirection == LeftBottom)
	{
		QPointF targetPt = QPointF(targetCenterPt.x(), targetRt.bottom());
		QPointF winPt = QPointF(winCenterPt.x(), winRt.bottom());
		QPointF offsetPt = targetPt - QPointF(hitDistance.width(), 0) - winPt;

		offsetPt += d->offsetPt;

		winRt.translate(offsetPt);
		setGeometry(winRt.toRect());
	}
	else if(tmpDirection == RightTop)
	{
		QPointF targetPt = QPointF(targetCenterPt.x(), targetRt.top());
		QPointF winPt = QPointF(winCenterPt.x(), winRt.top());
		QPointF offsetPt = targetPt + QPointF(hitDistance.width(), 0) - winPt;

		offsetPt += d->offsetPt;

		winRt.translate(offsetPt);
		setGeometry(winRt.toRect());

	}
	else if(tmpDirection == RightCenter)
	{
		QPointF offsetPt = targetCenterPt + QPointF(hitDistance.width(), 0) - winCenterPt;
		offsetPt += d->offsetPt;

		winRt.translate(offsetPt);
		setGeometry(winRt.toRect());
	}
	else if(tmpDirection == RightBottom)
	{
		QPointF targetPt = QPointF(targetCenterPt.x(), targetRt.bottom());
		QPointF winPt = QPointF(winCenterPt.x(), winRt.bottom());
		QPointF offsetPt = targetPt + QPointF(hitDistance.width(), 0) - winPt;

		offsetPt += d->offsetPt;

		winRt.translate(offsetPt);
		setGeometry(winRt.toRect());
	}
	else if(tmpDirection == TopLeft)
	{
		QPointF targetPt = QPointF(targetRt.left(), targetCenterPt.y());
		QPointF winPt = QPointF(winRt.left(), winCenterPt.y());
		QPointF offsetPt = targetPt - QPointF(0, hitDistance.height()) - winPt;

		offsetPt += d->offsetPt;

		winRt.translate(offsetPt);

		setGeometry(winRt.toRect());

	}
	else if(tmpDirection == TopCenter)
	{
		QPointF offsetPt = targetCenterPt - QPointF(0, hitDistance.height()) - winCenterPt;

		offsetPt += d->offsetPt;

		winRt.translate(offsetPt);

		setGeometry(winRt.toRect());
	}
	else if(tmpDirection == TopRight)
	{
		QPointF targetPt = QPointF(targetRt.right(), targetCenterPt.y());
		QPointF winPt = QPointF(winRt.right(), winCenterPt.y());
		QPointF offsetPt = targetPt - QPointF(0, hitDistance.height()) - winPt;

		offsetPt += d->offsetPt;

		winRt.translate(offsetPt);

		setGeometry(winRt.toRect());
	}
	else if(tmpDirection == BottomLeft)
	{
		QPointF targetPt = QPointF(targetRt.left(), targetCenterPt.y());
		QPointF winPt = QPointF(winRt.left(), winCenterPt.y());
		QPointF offsetPt = targetPt + QPointF(0, hitDistance.height()) - winPt;

		offsetPt += d->offsetPt;

		winRt.translate(offsetPt);

		setGeometry(winRt.toRect());
	}
	else if(tmpDirection == BottomCenter)
	{
		QPointF winCenterPt = winRt.center();
		QPointF targetCenterPt = targetRt.center();		
		QPointF offsetPt = targetCenterPt + QPointF(0, hitDistance.height()) - winCenterPt;

		offsetPt += d->offsetPt;

		winRt.translate(offsetPt);
		setGeometry(winRt.toRect());
	}	
	else if(tmpDirection == BottomRight)
	{
		QPointF targetPt = QPointF(targetRt.right(), targetCenterPt.y());
		QPointF winPt = QPointF(winRt.right(), winCenterPt.y());
		QPointF offsetPt = targetPt + QPointF(0, hitDistance.height()) - winPt;

		offsetPt += d->offsetPt;

		winRt.translate(offsetPt);

		setGeometry(winRt.toRect());
	}

	update();
}

void KFollowWindow::on_visibleChanged()
{
	Q_D(KFollowWindow);
	if(d->target == NULL)
		return;
	if(!d->target->isVisible())
	{
		if(d->hideFlags & HideAsTargetHidden)
		{
			QMetaObject::invokeMethod(this, "hide", Qt::QueuedConnection);
		}
	}
}

void KFollowWindow::resizeEvent( QResizeEvent *event )
{
	__super::resizeEvent(event);
	resetGeometry();
}

bool KFollowWindow::eventFilter( QObject *obj, QEvent *e )
{
	Q_D(KFollowWindow);
	if(d->target == NULL)
		return false;
	if(obj == d->target->viewport())
	{
		switch(e->type())
		{
		case QEvent::WindowActivate:
			{
				int i = 0;
			}
			break;
		case QEvent::WindowDeactivate:
			{
				if(!isVisible())
				{
					return false;
				}
				if(d->hideFlags & TargetWindowInactive)
				{
					bool bAborted = false;
					emit aboutToHide(TargetWindowInactive, &bAborted);
					if(!bAborted)
					{
						QMetaObject::invokeMethod(this, "hide", Qt::QueuedConnection);
					}
				}
			}
			break;
		case QEvent::MouseButtonPress:
			{
				if(!isVisible())
				{
					return false;
				}
				if(d->target->isUnderMouse())
				{
					if(d->hideFlags & ClickAtTarget)
					{
						bool bAborted = false;
						emit aboutToHide(ClickAtTarget, &bAborted);
						if(!bAborted)
						{
							QMetaObject::invokeMethod(this, "hide", Qt::QueuedConnection);
						}
					}
				}
				else
				{
					if(d->hideFlags & ClickAtOther)
					{
						bool bAborted = false;
						emit aboutToHide(ClickAtOther, &bAborted);
						if(!bAborted)
						{
							QMetaObject::invokeMethod(this, "hide", Qt::QueuedConnection);
						}
					}
				}
			}
			break;
		case QEvent::Move:
			{
				if(!isVisible())
				{
					return false;
				}
				if(d->hideFlags & TargetWindowMoving)
				{
					bool bAborted = false;
					emit aboutToHide(TargetWindowMoving, &bAborted);
					if(!bAborted)
					{
						QMetaObject::invokeMethod(this, "hide", Qt::QueuedConnection);
					}
				}
			}
			break;
		case QEvent::Resize:
			{
				if(!isVisible())
				{
					return false;
				}
				if(d->hideFlags & TargetWindowResize)
				{
					bool bAborted = false;
					emit aboutToHide(TargetWindowResize, &bAborted);
					if(!bAborted)
					{
						QMetaObject::invokeMethod(this, "hide", Qt::QueuedConnection);
					}
				}
			}
			break;
		case QEvent::ParentChange:
			{

			}
			break;
		case QEvent::ZOrderChange:
			{

			}
			break;
		}
	}
	else if(obj == viewport())
	{
		switch(e->type())
		{
		case QEvent::MouseButtonPress:
			{
				if(!isVisible())
				{
					return false;
				}
				if(d->hideFlags & ClickAtMySelf)
				{
					bool bAborted = false;
					emit aboutToHide(ClickAtMySelf, &bAborted);
					if(!bAborted)
					{
						QMetaObject::invokeMethod(this, "hide", Qt::QueuedConnection);
					}
				}
			}
			break;
		case QEvent::WindowActivate:
			{
				int i = 0;
			}
			break;
		case QEvent::WindowDeactivate:
			{
				if(!isVisible())
				{
					return false;
				}
				if(d->hideFlags & MySelfWindowActiveToInactive)
				{
					bool bAborted = false;
					emit aboutToHide(MySelfWindowActiveToInactive, &bAborted);
					if(!bAborted)
					{
						QMetaObject::invokeMethod(this, "hide", Qt::QueuedConnection);
					}
				}
			}
			break;
		}
	}
	return false;
}

KFollowWindow::HideFlags KFollowWindow::hideFlags()
{
	Q_D(KFollowWindow);
	return d->hideFlags;
}

void KFollowWindow::setHideFlags( HideFlags flags )
{
	Q_D(KFollowWindow);
	d->hideFlags = flags;
}

bool KFollowWindow::translucent() const
{
	return testAttribute(Qt::WA_TranslucentBackground);
}

void KFollowWindow::setTranslucent( bool on )
{
	setAttribute(Qt::WA_TranslucentBackground, on);
	quint32 exstyle = GetWindowLong(winId(), GWL_EXSTYLE);
	if(testAttribute(Qt::WA_TranslucentBackground))
	{
		setStyleSheet("QGraphicsView { border-style: none; background: transparent}");
		SetWindowLong(winId(), GWL_EXSTYLE, exstyle | WS_EX_LAYERED);
	}
	else
	{
		/*
		以下三行代码顺序不能错乱，否则会到导致黑色背景闪一下。用于解决由半透明切回不透明时，其界面不能更新的问题。
		*/
		setStyleSheet("QGraphicsView { border-style: none}");
		SetWindowLong(winId(), GWL_EXSTYLE, exstyle & ~WS_EX_LAYERED);
	}
}

QPointF KFollowWindow::posOffset() const
{
	Q_D(const KFollowWindow);
	return d->offsetPt;
}

void KFollowWindow::setPosOffset( const QPointF& pt )
{
	Q_D(KFollowWindow);
	d->offsetPt = pt;
}

void KFollowWindow::setPosOffset( int x, int y )
{
	Q_D(KFollowWindow);
	d->offsetPt = QPointF(x, y);
}

void KFollowWindow::setWindowSize( const QSize& s )
{
	Q_D(KFollowWindow);
	d->winSize = s;
	resize(s);
}

void KFollowWindow::setWindowSize( int w, int h )
{
	Q_D(KFollowWindow);
	d->winSize = QSize(w,h);
	resize(d->winSize);
}

void KFollowWindow::adjustGeometry()
{
	resetGeometry();
}

