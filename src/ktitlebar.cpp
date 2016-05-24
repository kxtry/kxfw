#include "stdafx.h"
#include "ktitlebar.h"
#include "klabel.h"
#include "kpushbutton.h"
#include "kimageitem.h"
#include "kpopupwindow.h"
#include "kxmlui.h"
#include "kwidget_p.h"
#include "ktooltip.h"

#include <QMetaObject>

KX_WIDGET_CREATOR_GLOBAL_STATIC(KTitlebar)

class KTitlebarPrivate : public KWidgetPrivate
{
public:
	KPushButton *btnOrgMax;
	KPushButton *btnMin;
	KPushButton *btnClose;
	KLabel *title;
	KImageItem *icon;
	QString szOrgImage, szMaxImage;
	bool bIconClick;
	QPointer<KToolTip> tooltip;
	bool bDbClick;
public:
	KTitlebarPrivate()
	{
		bDbClick = true;
	}
};

KTitlebar::KTitlebar( QGraphicsItem *parent /*= NULL*/ ) 
: KWidget(*new KTitlebarPrivate(), parent)
{
	Q_D(KTitlebar);

	setDragPolicy(KWidget::WindowMove);
	setAcceptedMouseButtons(Qt::LeftButton);
	setFlag(QGraphicsItem::ItemHasNoContents);
	setLayoutType(KWidget::HBox);
	
	d->icon = new KImageItem(this);
	d->icon->setImageAlignment(Qt::AlignLeft|Qt::AlignVCenter);
	d->icon->setImageMargins(QMargins(2,2,2,2));
	d->icon->setFixWidth(0);
	//d->icon->setBackgroundBrush(QColor(125,125,0));
	addItem(d->icon);
	d->title = new KLabel(this);
	d->title->setTextAlignment(Qt::AlignLeft|Qt::AlignVCenter);
	//d->title->setBackgroundBrush(QColor(255,125,0));
	addItem(d->title);
	d->btnMin = new KPushButton(this);
	d->btnMin->setFixWidth(0);
	addItem(d->btnMin);
	d->btnOrgMax = new KPushButton(this);
	d->btnOrgMax->setFixWidth(0);
	addItem(d->btnOrgMax);
	d->btnClose = new KPushButton(this);
	d->btnClose->setFixWidth(0);
	addItem(d->btnClose);
	d->btnClose->setToolTip("关闭窗口");

	d->bIconClick = false;
	setClosePath(":/image/theme/frameclose_normal.png|:/image/theme/frameclose_hover.png|:/image/theme/frameclose_down.png");
	setMinimumPath(":/image/theme/framemin_normal.png|:/image/theme/framemin_hover.png|:/image/theme/framemin_down.png");
	setMaximumPath(":/image/theme/framemax_normal.png|:/image/theme/framemax_hover.png|:/image/theme/framemax_down.png");
	setRestorePath(":/image/theme/framerestore_normal.png|:/image/theme/framerestore_hover.png|:/image/theme/framerestore_down.png");
	setIconPath(":/image/theme/titleicon.png");

	setTitlebarHeight(20);


	QObject::connect(d->btnClose, SIGNAL(clicked()), this, SLOT(buttonClicked()));
	QObject::connect(d->btnOrgMax, SIGNAL(clicked()), this, SLOT(buttonClicked()));
	QObject::connect(d->btnMin, SIGNAL(clicked()), this, SLOT(buttonClicked()));

	KToolTip *tip = KToolTip::textTipInstance();
	d->tooltip = tip;
	tip->addTarget(d->btnClose);
}

KTitlebar::~KTitlebar()
{
	Q_D(KTitlebar);
}

QString KTitlebar::iconPath() const
{
	return d_func()->icon->imagePath();
}

void KTitlebar::setIconPath( const QString& f )
{
	Q_D(KTitlebar);
	d->icon->setImagePath(f);
	d->icon->setFixWidth(height());
	d->icon->setAutoAdjust(true);
}

QString KTitlebar::maximumPath() const
{
	return d_func()->szMaxImage;
}

void KTitlebar::setMaximumPath( const QString& f )
{
	d_func()->szMaxImage = f;
	updateTitelbar();
}

QString KTitlebar::minimumPath() const
{
	return d_func()->btnMin->imagePath();
}

void KTitlebar::setMinimumPath( const QString& f )
{
	d_func()->btnMin->setImagePath(f);
	updateTitelbar();
}

QString KTitlebar::closePath() const
{
	return d_func()->btnClose->imagePath();
}

void KTitlebar::setClosePath( const QString& f )
{
	d_func()->btnClose->setImagePath(f);
	d_func()->btnClose->setSizeToFrameImage();
}

QString KTitlebar::title() const
{
	return d_func()->title->text();
}

void KTitlebar::setTitle( const QString& t )
{
	d_func()->title->setText(t);
}

void KTitlebar::appendButton( KWidget *btn )
{
	insertLayoutItem(btn, 2);
	addChild(btn);
}

void KTitlebar::buttonClicked()
{
	KPushButton *btn = (KPushButton*)(sender());
	QGraphicsView *view = this->view();
	HWND hWnd = view->winId();
	/*
	不能使用Qt的Maximize和Minimize的操作，主要是由于Win7系统上由于启用AeroSnap功能后，会造成Qt内部Maximize和Minimize操作会判断错误。
	*/
	if(btn == d_func()->btnClose)
	{
		QMetaObject::invokeMethod(view, "onCloseClick");
	}
	else if(btn == d_func()->btnOrgMax)
	{
		if(::IsMaximized(hWnd))
		{
			::ShowWindow(hWnd, SW_RESTORE);
			setMaximumPath(":/image/theme/framemax_normal.png|:/image/theme/framemax_hover.png|:/image/theme/framemax_down.png");
		}
		else
		{
			::ShowWindow(hWnd, SW_MAXIMIZE);
			setRestorePath(":/image/theme/framerestore_normal.png|:/image/theme/framerestore_hover.png|:/image/theme/framerestore_down.png");
		}
	}
	else if(btn == d_func()->btnMin)
	{
		::ShowWindow(hWnd, SW_MINIMIZE);
	}
}

QString KTitlebar::restorePath() const
{
	return d_func()->szOrgImage;
}

void KTitlebar::setRestorePath( const QString& f )
{
	d_func()->szOrgImage = f;
	updateTitelbar();
}

void KTitlebar::resizeEvent( QGraphicsSceneResizeEvent * event )
{
	__super::resizeEvent(event);
	updateTitelbar();
}

void KTitlebar::updateTitelbar()
{
	Q_D(KTitlebar);

	KPopupWindow *gv = qobject_cast<KPopupWindow*>(view());
	if(gv == NULL)
		return;

	HWND hWnd = gv->winId();

	DWORD dwStyle = GetWindowLong(gv->winId(), GWL_STYLE);
	if(gv->minimumButton())
	{
		d->btnMin->setSizeToFrameImage();
	}
	else
	{
		d->btnMin->setFixWidth(0);
	}
	if(gv->maximumButton())
	{
		if(::IsMaximized(hWnd))
		{
			d->btnOrgMax->setImagePath(d->szOrgImage);
		}
		else
		{
			d->btnOrgMax->setImagePath(d->szMaxImage);
		}
		d->btnOrgMax->setSizeToFrameImage();
	}
	else
	{
		d->btnOrgMax->setFixWidth(0);
	}
	QSize ico = d->icon->pixmapSize();
	if(ico.width() > 0 && ico.height() > 0)
	{
		d->icon->setFixWidth(height());
	}
	else
	{
		d->icon->setFixWidth(0);
	}
	d->title->setFixHeight(height());
}

bool KTitlebar::iconClickExit() const
{
	return d_func()->bIconClick;
}

void KTitlebar::setIconClickExit( bool bExit )
{
	d_func()->bIconClick = bExit;
}

int KTitlebar::titlebarHeight()
{
	return preferredHeight();
}

void KTitlebar::setTitlebarHeight( int height )
{
	setFixHeight(height);
}

void KTitlebar::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event )
{
	Q_D(KTitlebar);
	if(!d->bDbClick)
		return;
	QGraphicsView *gv = view();
	QVariant var = gv->property("resizable");
	if(var.isValid())
	{
		if(!var.toBool())
			return;
	}
	HWND hWnd = gv->winId();
	if(::IsMaximized(hWnd))
	{
		::ShowWindow(hWnd, SW_RESTORE);
	}
	else
	{
		::ShowWindow(hWnd, SW_MAXIMIZE);
	}
}

QColor KTitlebar::titleColor() const
{
	Q_D(const KTitlebar);
	return d->title->textColor();
}

void KTitlebar::setTitleColor( const QColor& clr )
{
	Q_D(KTitlebar);
	d->title->setTextColor(clr);
}

bool KTitlebar::titleBold()
{
	Q_D(KTitlebar);
	return d->title->bold();
}

void KTitlebar::setTitleBold( bool on )
{
	Q_D(KTitlebar);
	d->title->setBold(on);
}

void KTitlebar::setAcceptMouseDoubleClick( bool on )
{
	Q_D(KTitlebar);
	d->bDbClick = on;
}

bool KTitlebar::acceptDoubleClicked()
{
	Q_D(KTitlebar);
	return d->bDbClick;
}
