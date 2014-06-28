#include "stdafx.h"
#include "ktooltip.h"
#include "kwidget.h"
#include "kframeitem.h"
#include "ktextitem.h"

#include "kfollowwindow_p.h"

#define SHOW_TIP_OFFSET	(3)
#define HIDE_TIP_OFFSET	(8)

class KToolTipPrivate : public KFollowWindowPrivate
{
	Q_DECLARE_PUBLIC(KToolTip)
public:
	QList<QPointer<KWidget>> targets;
	QTimer timerForShowTip;
	QTimer timerForHideTip;
	QTimer timerForHover;
	KToolTip::TipStyle tipStyle;
	KToolTip::TipTrigger tipTrigger;
	QPoint showTipPt;
	QPointer<KFrameItem> tipFrame;
	QPointer<KTextItem> tipText;
public:
	KToolTipPrivate(KToolTip::TipStyle style)
		: tipStyle(style)
		, tipTrigger(KToolTip::CursorStopAtTarget)
	{
		
	}

	~KToolTipPrivate()
	{

	}

	void init()
	{
		Q_Q(KToolTip);

		tipTrigger = KToolTip::CursorStopAtTarget;

		QObject::connect(&timerForHideTip, SIGNAL(timeout()), q, SLOT(on_tipTimeout()));
		QObject::connect(&timerForShowTip, SIGNAL(timeout()), q, SLOT(on_tipTimeout()));
		QObject::connect(&timerForHover, SIGNAL(timeout()), q, SLOT(on_tipTimeout()));

		timerForShowTip.setInterval(200);
		timerForHideTip.setInterval(100);
		timerForHover.setInterval(100);

		q->setDirection(KFollowWindow::UnderCursor);
		q->setTranslucent(true);
		q->setAutoDeleteWhenAppQuit(true);

		if(tipStyle == KToolTip::SimpleText)
		{
			KWidget *root = q->rootWidget();
			tipFrame = new KFrameItem(root);
			root->addItem(tipFrame);
			tipFrame->setImagePath(":/image/theme/lineedit.png");
			tipFrame->setFrameType(KFrameItem::GridFour);
			tipFrame->setLayoutType(KWidget::VBox);
			tipFrame->setMargins(QMargins(2,1,2,2));
			tipText = new KTextItem(tipFrame);
			tipFrame->addItem(tipText);
		}
	}

	void initContent()
	{
		Q_Q(KToolTip);
		if(tipStyle == KToolTip::SimpleText)
		{
			QString txt = target->toolTip();
			tipText->setText(txt);
			QSizeF s = tipText->idealSize();
			tipText->setFixSize(s);

			QSizeF sf = q->calcWindowPreferSize();
			q->setWindowSize(sf.toSize());
		}
	}

	void on_tipTimeoutForShowTip()
	{
		Q_Q(KToolTip);

		timerForShowTip.stop();
		if(target == NULL)
		{
			return;
		}

		QPoint ptNow = QCursor::pos();
		int offsetx = qAbs(ptNow.x() - showTipPt.x());
		int offsety = qAbs(ptNow.y() - showTipPt.y());
		if( offsetx > SHOW_TIP_OFFSET || offsety > SHOW_TIP_OFFSET)
		{
			return;
		}
		emit q->aboutToShow();
		q->resetGeometry();
		q->show();
		timerForHideTip.start();
	}

	void on_tipTimeoutForHover()
	{
		Q_Q(KToolTip);

		QPoint ptNow = QCursor::pos();
		int offsetx = qAbs(ptNow.x() - showTipPt.x());
		int offsety = qAbs(ptNow.y() - showTipPt.y());
		if( offsetx > SHOW_TIP_OFFSET || offsety > SHOW_TIP_OFFSET)
		{
			showTipPt = ptNow;
			return;
		}
		/*显示提示*/
		timerForHover.stop();
		timerForShowTip.start();
	}

	void on_tipTimeoutForHideTip()
	{
		Q_Q(KToolTip);

		QPoint ptNow = QCursor::pos();
		int offsetx = qAbs(ptNow.x() - showTipPt.x());
		int offsety = qAbs(ptNow.y() - showTipPt.y());
		if( offsetx < HIDE_TIP_OFFSET && offsety < HIDE_TIP_OFFSET)
		{
			return;
		}
		/*隐藏提示*/
		showTipPt = QPoint();
		q->hide();
		timerForHideTip.stop();
	}
};

KToolTip::KToolTip(TipStyle style)
: KFollowWindow(*new KToolTipPrivate(style), NULL)
{
	Q_D(KToolTip);
	d->init();
}

KToolTip::~KToolTip()
{
	removeAllTarget();
}

bool KToolTip::addTarget( KWidget *w )
{
	Q_D(KToolTip);
	if(d->targets.contains(w))
		return false;
	if(w->parentItem() == NULL)
		return false;
	listenEvent(w, tipFlag);
	d->targets.push_back(w);
	return true;
}

void KToolTip::removeTarget( KWidget *w )
{
	Q_D(KToolTip);
	if(!d->targets.contains(w))
		return;
	listenEvent(w, abortAll);
	d->targets.removeOne(w);
}

void KToolTip::removeAllTarget()
{
	Q_D(KToolTip);
	for(QList<QPointer<KWidget>>::iterator iter = d->targets.begin(); iter != d->targets.end(); iter++)
	{
		KWidget *w = *iter;
		if(w)
		{
			listenEvent(w, abortAll);
		}
	}
	d->targets.clear();
}


bool KToolTip::eventFilter( QObject *obj, QEvent *e )
{
	Q_D(KToolTip);
	KWidget* wobj = qobject_cast<KWidget*>(obj);
	if(d->targets.contains(wobj))
	{
		switch(e->type())
		{
		case QEvent::GraphicsSceneHoverEnter:
			{
				d->target = wobj;
				hide();
				if(d->tipTrigger == CursorStopAtTarget)
				{
					d->timerForHideTip.stop();
					d->timerForShowTip.stop();
					d->timerForHover.stop();
					d->showTipPt = QCursor::pos();
					d->timerForHover.start();
				}
				else if(d->tipTrigger == CursorInTarget)
				{
					emit aboutToShow();
					resetGeometry();
					show();
				}
			}
			break;
		case QEvent::GraphicsSceneHoverLeave:
			{
				hide();
				if(d->tipTrigger == CursorStopAtTarget)
				{
					d->timerForHideTip.stop();
					d->timerForShowTip.stop();
					d->timerForHover.stop();
					d->showTipPt = QPoint();
				}
			}
			break;
		}
		return false;
	}
	return __super::eventFilter(obj, e);
}

void KToolTip::setTrigger( TipTrigger trigger )
{
	Q_D(KToolTip);
	d->tipTrigger = trigger;
}

void KToolTip::on_tipTimeout()
{
	Q_D(KToolTip);

	QTimer *timer = qobject_cast<QTimer*>(sender());
	if(timer == &d->timerForHideTip)
	{
		d->on_tipTimeoutForHideTip();
	}
	else if(timer == &d->timerForShowTip)
	{
		d->on_tipTimeoutForShowTip();
	}
	else if(timer == &d->timerForHover)
	{
		d->on_tipTimeoutForHover();
	}
}

void KToolTip::resetGeometry()
{
	Q_D(KToolTip);
	Direction tmpDirection = d->direction;

	d->initContent();

	if(tmpDirection == UnderCursor)
	{
		bool bHandled = false;
		emit aboutToAdjust(this, &bHandled);
		if(bHandled)
		{
			return;
		}
		QPoint pt = QCursor::pos();
		QSizeF winSize = d->winSize;
		QRectF winRt(pt.x(), pt.y(), winSize.width(), winSize.height());
		winRt.translate(0, 20);
		setGeometry(winRt.toRect());
		return;
	}
	__super::resetGeometry();
}

KToolTip * KToolTip::textTipInstance()
{
	static QPointer<KToolTip> sToolTip;
	if(sToolTip)
		return sToolTip;
	sToolTip = new KToolTip();
	return sToolTip;
}
