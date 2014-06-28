#include "stdafx.h"
#include "kpopupwindow.h"
#include "kpopupwindow_p.h"
#include "ktheme.h"
#include "MonitorHelp.h"
#include "kwidget.h"
#include "kspacer.h"
#include "kpushbutton.h"
#include "kimageitem.h"
#include "kxmlui.h"
#include "kutility.h"

#include "shlwapi.h"
#pragma comment(lib, "shlwapi.lib")

KX_WINDOW_CREATOR_GLOBAL_STATIC(KPopupWindow)

#define BODER_WIDTH			(3)

KPopupWindowPrivate::KPopupWindowPrivate() 
: nBorderPixel(BODER_WIDTH) 
, bResizable(false) 
, closeOnEscape(true) 
, closeOnF4(true)
, bMaxButton(false) 
, bMinButton(false) 
, dragPosition(-1,-1) 
, bDragWindow(false)
, bEnableDragWindow(true)
, hasFrame(true)
{

}

KPopupWindow::KPopupWindow( QWidget *parent/*=NULL*/ ) 
: KWindow(*new KPopupWindowPrivate(), parent)
{
	init();
}

KPopupWindow::KPopupWindow( KPopupWindowPrivate &dd, QWidget *parent /* = NULL*/ )
: KWindow(dd, parent)
{
	init();
}

KPopupWindow::~KPopupWindow()
{

}

void KPopupWindow::drawBackground( QPainter * painter, const QRectF & rect )
{
	Q_D(KPopupWindow);
	if(d->backgroundColor.isValid())
	{
		drawBackgroundColor(painter);
	}
	else if(!d->customImage.isNull())
	{
		drawBackgroundImage(painter);
	}
}

void KPopupWindow::drawForeground( QPainter * painter, const QRectF & rect )
{
	Q_D(KPopupWindow);
	if(d->hasFrame)
	{
		drawFrameImage(painter);
	}
}

void KPopupWindow::drawBackgroundImage( QPainter *painter)
{
	Q_D(KPopupWindow);

	QRectF dstRt = rect();
	dstRt.adjust(d->maskMargins.left(), d->maskMargins.top(), -d->maskMargins.right(), -d->maskMargins.bottom());
	painter->save();
	painter->setClipRegion(d->maskRegion, Qt::IntersectClip);
	painter->drawPixmap(0, 0, d->customImage);
	painter->restore();
}

void KPopupWindow::drawBackgroundColor(QPainter *painter)
{
	Q_D(KPopupWindow);
	QRectF dstRt = rect();
	dstRt.adjust(d->maskMargins.left(), d->maskMargins.top(), -d->maskMargins.right(), -d->maskMargins.bottom());
	painter->save();
	painter->setClipRegion(d->maskRegion, Qt::IntersectClip);
	painter->fillRect(dstRt, d->backgroundColor);
	painter->restore();
}

void KPopupWindow::drawFrameImage( QPainter *painter )
{
	Q_D(KPopupWindow);

	QRectF dstRt = rect();
	QRectF fromRt = QRectF(0, 0, d->frameImage.width(), d->frameImage.height());
	QRectF blockRt;
	QRectF drawRt;
	/*左上角*/
	blockRt.setRect(fromRt.left(), fromRt.top(), fromRt.width() / 2, fromRt.height() / 2);
	drawRt.setRect(dstRt.left(), dstRt.top(), blockRt.width(), blockRt.height());
	painter->drawPixmap(drawRt, d->frameImage, blockRt);
	/*上部中间*/
	blockRt.setRect(fromRt.left() + fromRt.width() / 2, fromRt.top(), 1, fromRt.height() / 2);
	drawRt.setRect(dstRt.left() + fromRt.width() / 2, dstRt.top(), dstRt.width() - fromRt.width(), blockRt.height());
	painter->drawPixmap(drawRt, d->frameImage, blockRt);
	/*右上角*/
	blockRt.setRect(fromRt.left() + fromRt.width() / 2, fromRt.top(), fromRt.width() - fromRt.width() / 2, fromRt.height() / 2);
	drawRt.setRect(dstRt.right() - fromRt.width() / 2, dstRt.top(), blockRt.width(), blockRt.height());
	painter->drawPixmap(drawRt, d->frameImage, blockRt);
	/*左边中部*/
	blockRt.setRect(fromRt.left(), fromRt.top() + fromRt.height() / 2, fromRt.width() / 2, 1);
	drawRt.setRect(dstRt.left(), dstRt.top() + fromRt.height() / 2, blockRt.width(), dstRt.height() - fromRt.height());
	painter->drawPixmap(drawRt, d->frameImage, blockRt);
	/*右边中部*/
	blockRt.setRect(fromRt.left() + fromRt.width() / 2, fromRt.top() + fromRt.height() / 2, fromRt.width() - fromRt.width() / 2, 1);
	drawRt.setRect(dstRt.right() - blockRt.width(), dstRt.top() + fromRt.height() / 2, blockRt.width(), dstRt.height() - fromRt.height());
	painter->drawPixmap(drawRt, d->frameImage, blockRt);
	/*左下角*/
	blockRt.setRect(fromRt.left(), fromRt.top() + fromRt.height() / 2, fromRt.width() / 2, fromRt.height() - fromRt.height() / 2);
	drawRt.setRect(dstRt.left(), dstRt.bottom() - blockRt.height(), blockRt.width(), blockRt.height());
	painter->drawPixmap(drawRt, d->frameImage, blockRt);
	/*右下中部*/
	blockRt.setRect(fromRt.left() + fromRt.width() / 2, fromRt.top() + fromRt.height() / 2, 1, fromRt.height() - fromRt.height() / 2);
	drawRt.setRect(dstRt.left() + fromRt.width() / 2, dstRt.bottom() - blockRt.height(), dstRt.width() - fromRt.width(), blockRt.height());
	painter->drawPixmap(drawRt, d->frameImage, blockRt);
	/*右下角*/
	blockRt.setRect(fromRt.left() + fromRt.width() / 2, fromRt.top() + fromRt.height() / 2, fromRt.width() - fromRt.width() / 2, fromRt.height() - fromRt.height() / 2);
	drawRt.setRect(dstRt.right() - blockRt.width(), dstRt.bottom() - blockRt.height(), blockRt.width(), blockRt.height());
	painter->drawPixmap(drawRt, d->frameImage, blockRt);
}

void KPopupWindow::resizeEvent( QResizeEvent *event )
{
	__super::resizeEvent(event);
	createWindowRegion(rect());
	setWindowShape();
}

void KPopupWindow::setFrameImage( const QString& frame )
{
	Q_D(KPopupWindow);
	d->szFrameImage = frame;
}

QString KPopupWindow::frameImage() const
{
	return d_func()->szFrameImage;
}

QMargins KPopupWindow::maskMargins() const
{
	return d_func()->maskMarginsCache;
}

void KPopupWindow::setMaskMargins( const QMargins& margins )
{
	d_func()->maskMarginsCache = margins;
}

void KPopupWindow::setMaskImage( const QString& mask )
{
	Q_D(KPopupWindow);
	d->szMaskImage = mask;
}

QString KPopupWindow::maskImage() const
{
	return d_func()->szMaskImage;
}

QString KPopupWindow::backgroundImage() const
{
	return d_func()->szCustomImage;
}

void KPopupWindow::setBackgroundImage( const QString& custom )
{
	Q_D(KPopupWindow);
	d->szCustomImage = custom;
	d->backgroundColor = QColor();
	d->customImage = KTheme::getPixmap(custom);
	update();
}

QColor KPopupWindow::backgroundColor() const
{
	Q_D(const KPopupWindow);
	return d->backgroundColor;
}

void KPopupWindow::setBackgroundColor(const QColor& clr)
{
	Q_D(KPopupWindow);
	d->backgroundColor = clr;
	update();
}

void KPopupWindow::createFourAngleRegion()
{
	Q_D(KPopupWindow);
	QRegion maskRgn(d->maskImage);
	int width = d->maskImage.width();
	int height = d->maskImage.height();
	d->rgnLeftTop = maskRgn.intersected(QRect(0, 0, width / 2, height / 2));
	d->rgnRightTop = maskRgn.intersected(QRect(width / 2, 0, width - width / 2, height / 2));
	d->rgnLeftBot = maskRgn.intersected(QRect(0, height / 2, width / 2, height - height / 2));
	d->rgnRightBot = maskRgn.intersected(QRect(width / 2, height / 2, width - width / 2, height - height / 2));
}

void KPopupWindow::createWindowRegion(const QRect& winRt )
{
	Q_D(KPopupWindow);

	if(d->hasFrame)
	{
		int width = d->maskImage.width();
		int height = d->maskImage.height();
		QRect r = winRt.adjusted(width / 2, height / 2, -width + width / 2, -height + height / 2);

		d->maskRegion = QRegion(r);
		QRect contentRt = winRt.adjusted(d->maskMargins.left(), d->maskMargins.top(), -d->maskMargins.right(), -d->maskMargins.bottom());
		QRect leftRt(0, r.top(), r.left(), r.height());
		leftRt = leftRt.intersect(contentRt);
		d->maskRegion += leftRt;
		QRect topRt(r.left(), 0, r.width(), r.top());
		topRt = topRt.intersect(contentRt);
		d->maskRegion += topRt;
		QRect rightRt(r.right(), r.top(), winRt.width() - r.right(), r.height());
		rightRt = rightRt.intersect(contentRt);
		d->maskRegion += rightRt;
		QRect botRt(r.left(), r.bottom(), r.width(), winRt.height() - r.bottom());
		botRt = botRt.intersect(contentRt);
		d->maskRegion += botRt;

		d->maskRegion += d->rgnLeftTop;

		d->rgnRightTop.translate(r.width(), 0);
		d->maskRegion += d->rgnRightTop;
		d->rgnRightTop.translate(-r.width(), 0);

		d->rgnLeftBot.translate(0, r.height());
		d->maskRegion += d->rgnLeftBot;
		d->rgnLeftBot.translate(0, -r.height());

		d->rgnRightBot.translate(r.width(), r.height());
		d->maskRegion += d->rgnRightBot;
		d->rgnRightBot.translate(-r.width(), -r.height());
	}
	else
	{
		d->maskRegion = winRt;
	}
}

void KPopupWindow::setWindowShape()
{
	Q_D(KPopupWindow);
	quint32 exstyle = GetWindowLong(winId(), GWL_EXSTYLE);
	if(testAttribute(Qt::WA_TranslucentBackground))
	{
		setStyleSheet("QGraphicsView { border-style: none; background: transparent}");
		SetWindowLong(winId(), GWL_EXSTYLE, exstyle | WS_EX_LAYERED);
		setMask(QRegion());		
	}
	else
	{
		/*
		以下三行代码顺序不能错乱，否则会到导致黑色背景闪一下。用于解决由半透明切回不透明时，其界面不能更新的问题。
		*/
		setMask(d->maskRegion);
		setStyleSheet("QGraphicsView { border-style: none}");
		SetWindowLong(winId(), GWL_EXSTYLE, exstyle & ~WS_EX_LAYERED);
	}
}

bool KPopupWindow::winEvent( MSG * pMsg, long * result )
{
	//务必调用KWindow的winEvent，因为在KWindow中处理了Qt的中文输入法BUG。
	bool bres = __super::winEvent(pMsg, result);
	
	switch(pMsg->message)
	{
	case WM_SHOWWINDOW:
		updateWindowStyle();
		return false;
	case WM_NCHITTEST:
		return hitTestEvent(pMsg, result);
	case WM_LBUTTONDOWN:
		return leftButtonDownEvent(pMsg, result);
	case WM_MOUSEMOVE:
		return mouseMoveEvent(pMsg, result);
	case WM_LBUTTONUP:
		return leftButtonUpEvent(pMsg, result);
	case WM_LBUTTONDBLCLK:
		return leftButtonDBClickEvent(pMsg, result);
	case WM_SETCURSOR:
		return setCursorEvent(pMsg, result);
	case WM_NCLBUTTONDOWN:
		return ncLeftButtonDownEvent(pMsg, result);
	case WM_GETMINMAXINFO:
		return getMinMaxInfoEvent(pMsg, result);
	case WM_KEYDOWN:
		return keyDownEvent(pMsg, result);
	case WM_KEYUP:
		return keyUpEvent(pMsg, result);
	case WM_SYSCOMMAND:
		return systemCommandEvent(pMsg, result);
	}
	return bres;
}

bool KPopupWindow::systemCommandEvent(MSG *pMsg, long *result)
{
	Q_D(KPopupWindow);
	if(pMsg->wParam == SC_CLOSE)
	{
		if(!d->closeOnF4)
		{
			*result = 0;
			return true;
		}
	}
	return false;
}

bool KPopupWindow::keyDownEvent(MSG *pMsg, long *result)
{
	return false;
}

bool KPopupWindow::keyUpEvent(MSG *pMsg, long *result)
{
	return false;
}

bool KPopupWindow::leftButtonDownEvent(MSG *pMsg, long *result)
{
	Q_D(KPopupWindow);

	QPoint pt(GET_X_LPARAM(pMsg->lParam), GET_Y_LPARAM(pMsg->lParam));
	if(hitTestClient(pt) == HTCAPTION)
	{
		if(!isMaximized())
		{
			QPoint screenPt = mapToGlobal(pt);
			d->dragPosition = screenPt - frameGeometry().topLeft();
			d->bDragWindow = true;
			*result = 0;

			/*该处不能使用Qt的grabMouse，因为其封装多了一个HOOKER，导致了WM_LBUTTONDLCLK无法发送出来。*/
			SetCapture(winId());
			//grabMouse();
			return false;
		}
	}
	return false;
}

bool KPopupWindow::leftButtonDBClickEvent(MSG *pMsg, long *result)
{
	*result = 0;
	return false;
}

bool KPopupWindow::leftButtonUpEvent(MSG *pMsg, long *result)
{
	Q_D(KPopupWindow);

	if(d->bDragWindow)
	{
		d->dragPosition = QPoint(-1, -1);
		d->bDragWindow = false;
		*result = 0;
		ReleaseCapture();
		//releaseMouse();
		return false;
	}
	return false;
}

bool KPopupWindow::mouseMoveEvent(MSG *pMsg, long *result)
{
	Q_D(KPopupWindow);

	if(d->bDragWindow && (pMsg->wParam & MK_LBUTTON))
	{
		QPoint pt(GET_X_LPARAM(pMsg->lParam), GET_Y_LPARAM(pMsg->lParam));
		QPoint screenPt = mapToGlobal(pt);
		move(screenPt - d->dragPosition);
		*result = 0;
		return true;
	}

	return false;
}

bool KPopupWindow::ncCalcSizeEvent( MSG *pMsg, long *result )
{
	NCCALCSIZE_PARAMS* pParams = NULL;
	RECT* pRect = NULL;

	BOOL bValue = static_cast<BOOL>(pMsg->wParam);
	if(bValue) 
	{
		pParams = reinterpret_cast<NCCALCSIZE_PARAMS*>(pMsg->lParam);
	}
	else 
	{
		pRect = reinterpret_cast<RECT*>(pMsg->lParam);
	}

	if(bValue)
	{
		pRect = &pParams->rgrc[0];
	}
	if(bValue) 
	{
		pRect->left = pRect->left + 0;
		pRect->top = pRect->top + 0;
		pRect->right = pRect->right - 0;
		pRect->bottom = pRect->bottom - 0;

		pParams->rgrc[1] = pParams->rgrc[0];
	}
	*result = true;
	return true;
}

bool KPopupWindow::getMinMaxInfoEvent( MSG *pMsg, long *result )
{
	Q_D(KPopupWindow);

	MINMAXINFO *pMinMaxInfo = (MINMAXINFO*)pMsg->lParam;
	MONITORINFO info;
	MonitorHelp::GetMonitorInfo( winId(), &info, MONITOR_DEFAULTTONEAREST );
	pMinMaxInfo->ptMinTrackSize.x = (d->maskMargins.left() + d->maskMargins.right()) * 5;
	pMinMaxInfo->ptMinTrackSize.y = (d->maskMargins.top() + d->maskMargins.bottom()) * 3;
	if(d->sizeFrameMin.width() > 0 && pMinMaxInfo->ptMinTrackSize.x < d->sizeFrameMin.width())
	{
		pMinMaxInfo->ptMinTrackSize.x = d->sizeFrameMin.width();
	}
	if(d->sizeFrameMin.height() > 0 && pMinMaxInfo->ptMinTrackSize.y < d->sizeFrameMin.height())
	{
		pMinMaxInfo->ptMinTrackSize.y = d->sizeFrameMin.height();
	}
	//这里将边界去掉
	if ( info.dwFlags & MONITORINFOF_PRIMARY )
	{
		pMinMaxInfo->ptMaxPosition.x = info.rcWork.left - d->maskMargins.left()  - d->nBorderPixel;
		pMinMaxInfo->ptMaxPosition.y = info.rcWork.top - d->maskMargins.top() - d->nBorderPixel;
		pMinMaxInfo->ptMaxSize.x = info.rcWork.right - info.rcWork.left + ( d->maskMargins.left() + d->maskMargins.right() + d->nBorderPixel * 2) ;
		pMinMaxInfo->ptMaxSize.y = info.rcWork.bottom - info.rcWork.top + ( d->maskMargins.top() + d->maskMargins.bottom() + d->nBorderPixel * 2);
	}
	else
	{
		pMinMaxInfo->ptMaxPosition.x = -d->maskMargins.left() - d->nBorderPixel;
		pMinMaxInfo->ptMaxPosition.y = -d->maskMargins.top() - d->nBorderPixel;
		pMinMaxInfo->ptMaxSize.x += ( d->maskMargins.left() + d->maskMargins.right() + d->nBorderPixel * 2) ;
		pMinMaxInfo->ptMaxSize.y += ( d->maskMargins.top() + d->maskMargins.bottom() + d->nBorderPixel * 2);
	}
	if(d->sizeFrameMax.width() > 0 && pMinMaxInfo->ptMaxSize.x > d->sizeFrameMax.width())
	{
		pMinMaxInfo->ptMaxSize.x = d->sizeFrameMax.width();
	}
	if(d->sizeFrameMax.height() > 0 && pMinMaxInfo->ptMaxSize.y > d->sizeFrameMax.height())
	{
		pMinMaxInfo->ptMaxSize.y = d->sizeFrameMax.height();
	}
	pMinMaxInfo->ptMaxTrackSize = pMinMaxInfo->ptMaxSize;
	*result = 0;
	return true;
}

bool KPopupWindow::hitTestEvent( MSG *pMsg, long *result )
{
	Q_D(KPopupWindow);

	if(!d->bEnableDragWindow)
		return HTCLIENT;

	int x = LOWORD(pMsg->lParam);
	int y = HIWORD(pMsg->lParam);
	QPoint pt = mapFromGlobal(QPoint(x, y));
	QRect frameRt = rect();
	int borderSize = 0;
	*result = HTCLIENT;
	if(!resizable())
	{
		return true;
	}
	borderSize = d->nBorderPixel;
	frameRt.adjust(d->maskMargins.left(), d->maskMargins.top(), -d->maskMargins.right(), -d->maskMargins.bottom());
	*result = hitTestFrame(pt, frameRt, QMargins(borderSize, borderSize, borderSize, borderSize), 0);
	return true;
}

long KPopupWindow::hitTestFrame( const QPoint& pt, const QRect& frameRt, const QMargins& border, int captionHeight )
{	
	bool bNotZoom = !::IsZoomed(winId());

	QRect captionRt;
	captionRt.setRect(frameRt.left() + border.left(), frameRt.top() + border.top(), frameRt.width() - border.left() - border.right(), captionHeight);
	if(captionRt.contains(pt))
		return HTCAPTION;
	QRect topLeftRt;
	topLeftRt.setRect(frameRt.left(), frameRt.top(), border.left() * 2, border.top() * 2);
	if(topLeftRt.contains(pt) && bNotZoom)
		return HTTOPLEFT;
	QRect topRightRt;
	topRightRt.setRect(frameRt.right() - border.right() * 2, frameRt.top(), border.right() * 2, border.top() * 2);
	if(topRightRt.contains(pt) && bNotZoom)
		return HTTOPRIGHT;
	QRect botLeftRt;
	botLeftRt.setRect(frameRt.left(), frameRt.bottom() - border.bottom() * 2, border.left() * 2, border.bottom() * 2);
	if(botLeftRt.contains(pt) && bNotZoom)
		return HTBOTTOMLEFT;
	QRect botRightRt;
	botRightRt.setRect(frameRt.right() - border.right() * 2, frameRt.bottom() - border.bottom() * 2, border.right() * 2, border.bottom() * 2);
	if(botRightRt.contains(pt) && bNotZoom)
		return HTBOTTOMRIGHT;
	QRect topRt;
	topRt.setRect(frameRt.left() + border.left(), frameRt.top(), frameRt.width() - border.left() - border.right(), border.top());
	if(topRt.contains(pt) && bNotZoom)
		return HTTOP;
	QRect leftRt;
	leftRt.setRect(frameRt.left(), frameRt.top() + border.top(), border.left(), frameRt.height() - border.top() - border.bottom() * 2);
	if(leftRt.contains(pt) && bNotZoom)
		return HTLEFT;
	QRect rightRt;
	rightRt.setRect(frameRt.right() - border.right(), frameRt.top() + border.top(), border.right(), frameRt.height() - border.top() - border.bottom());
	if(rightRt.contains(pt) && bNotZoom)
		return HTRIGHT;
	QRect botRt;
	botRt.setCoords(frameRt.left() + border.left(), frameRt.bottom() - border.bottom(), frameRt.right() - border.right(), frameRt.bottom());
	if(botRt.contains(pt) && bNotZoom)
		return HTBOTTOM;
	return HTCLIENT;
}

long HitTestWidget(KWidget *item, const QPoint& pt)
{
	if(item == NULL)
		return HTCLIENT;
	if(item->hitTest(pt) == KWidget::Nothing)
	{
		return HTCLIENT;
	}
	else if(item->hitTest(pt) == KWidget::WindowMove)
	{
		return HTCAPTION;
	}
	else if(item->hitTest(pt) == KWidget::AskParent)
	{
		item = qobject_cast<KWidget*>(item->parentWidget());
		if(item == NULL)
			return HTCAPTION;
		return HitTestWidget(item, pt);
	}
	return HTCLIENT;
}

long KPopupWindow::hitTestClient( const QPoint& pt )
{
	Q_D(KPopupWindow);

	if(!d->bEnableDragWindow)
		return HTCLIENT;

	QWidget *childWidget = childAt(pt);
	if( childWidget && childWidget != viewport() )
	{
		return HTCLIENT;
	}

	QGraphicsItem *item = scene()->itemAt(pt);
	if ( item != NULL )
	{
		KWidget *widget = qobject_cast<KWidget*>( item->toGraphicsObject() );
		if(widget != NULL)
		{
			return HitTestWidget(widget, pt);
		}
		else
		{
			do 
			{
				item = item->parentItem();
				widget = qobject_cast<KWidget*>(item->toGraphicsObject());
				if(widget != NULL)
				{
					return HitTestWidget(widget, pt);
				}
			}while(item != NULL);
		}
	}
	return HTCLIENT;
}

bool KPopupWindow::setCursorEvent( MSG *pMsg, long *result )
{
	int nHitTest = LOWORD(pMsg->lParam);
	if(nHitTest == HTCAPTION || nHitTest == HTSYSMENU || nHitTest == HTMENU || nHitTest == HTCLIENT)
	{
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)));
		return false;
	}
	else if(nHitTest == HTTOP || nHitTest == HTBOTTOM)
	{
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS)));
	}
	else if(nHitTest == HTLEFT || nHitTest == HTRIGHT)
	{
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE)));
	}
	else if(nHitTest == HTTOPLEFT || nHitTest == HTBOTTOMRIGHT)
	{
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE)));
	}
	else if(nHitTest == HTTOPRIGHT || nHitTest == HTBOTTOMLEFT)
	{
		SetCursor(LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW)));
	}
	return true;
}

bool KPopupWindow::ncLeftButtonDownEvent( MSG *pMsg, long *result )
{
	int nHitTest = pMsg->wParam;
	if(nHitTest == HTTOP)
	{
		SendMessage(winId(), WM_SYSCOMMAND, SC_SIZE | WMSZ_TOP, pMsg->lParam );
	}
	else if(nHitTest == HTBOTTOM)
	{
		SendMessage(winId(), WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOM, pMsg->lParam);
	}
	else if(nHitTest == HTLEFT)
	{
		SendMessage(winId(), WM_SYSCOMMAND, SC_SIZE | WMSZ_LEFT, pMsg->lParam);
	}
	else if(nHitTest == HTRIGHT)
	{
		SendMessage(winId(), WM_SYSCOMMAND, SC_SIZE | WMSZ_RIGHT, pMsg->lParam);
	}
	else if(nHitTest == HTTOPLEFT)
	{
		SendMessage(winId(), WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPLEFT, pMsg->lParam);
	}
	else if(nHitTest == HTTOPRIGHT)
	{
		SendMessage(winId(), WM_SYSCOMMAND, SC_SIZE | WMSZ_TOPRIGHT, pMsg->lParam);
	}
	else if(nHitTest == HTBOTTOMLEFT)
	{
		SendMessage(winId(), WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMLEFT, pMsg->lParam);
	}
	else if(nHitTest == HTBOTTOMRIGHT)
	{
		SendMessage(winId(), WM_SYSCOMMAND, SC_SIZE | WMSZ_BOTTOMRIGHT, pMsg->lParam);
	}
	else if (nHitTest == HTCAPTION)
	{
		return false;
		SendMessage(winId(), WM_SYSCOMMAND, SC_MOVE | HTCAPTION , pMsg->lParam);
	}
	else
	{
		return false;
	}
	return true;
}

void KPopupWindow::keyPressEvent( QKeyEvent *event )
{
	Q_D(KPopupWindow);

	switch(event->key())
	{
	case Qt::Key_Return:
	case Qt::Key_Enter:
		{
			//当为Key_Enter时，会有一个keypadmodifier标志，表示是小键盘的回车键
			Qt::KeyboardModifiers modifier = event->modifiers();
			if ( modifier.testFlag(Qt::KeypadModifier) )
			{
				modifier ^= Qt::KeypadModifier;
			}
			onEnterPressed(modifier);
			break;
		}
	case Qt::Key_Escape:
		{
			Qt::KeyboardModifiers modifier = event->modifiers();
			if ( modifier.testFlag(Qt::KeypadModifier) )
			{
				modifier ^= Qt::KeypadModifier;
			}				
			onEscPressed(modifier);		
			break;
		}

	default:
		break;
	}

	QGraphicsView::keyPressEvent(event);
}

void KPopupWindow::onEnterPressed( Qt::KeyboardModifiers modifiers )
{
	emit enterPressed(modifiers);
}

void KPopupWindow::updateWindowStyle()
{
	if(!isWindow())
		return;
	DWORD style = 0;
	if(minimumButton())
	{
		style |= WS_MINIMIZEBOX;
	}
	if(maximumButton())
	{
		style |= WS_MAXIMIZEBOX;
	}
	DWORD dwStyle = GetWindowLong(winId(), GWL_STYLE);
	if((dwStyle & style) == style)
		return;
	dwStyle |= style;
	SetWindowLong(winId(), GWL_STYLE, dwStyle);
}

QSize KPopupWindow::minimumSize() const
{
	Q_D(const KPopupWindow);
	return d->sizeFrameMin;
}

void KPopupWindow::setMinimumSize( const QSize& s )
{
	Q_D(KPopupWindow);
	d->sizeFrameMin = s;
	__super::setMinimumSize(s);
}

void KPopupWindow::setMaximumSize( const QSize& s )
{
	Q_D(KPopupWindow);
	d->sizeFrameMax = s;
	__super::setMaximumSize(s);
}

QSize KPopupWindow::maximumSize() const
{
	Q_D(const KPopupWindow);
	return d->sizeFrameMax;
}

QSize KPopupWindow::fixSize() const
{
	return minimumSize();
}

void KPopupWindow::setFixSize(const QSize& s)
{
	setMinimumSize(s);
	setMaximumSize(s);
}

void KPopupWindow::showEvent( QShowEvent * event )
{
	__super::showEvent(event);
	updateRequest();
}

void KPopupWindow::updateRequest()
{
	QApplication::postEvent(this, new QEvent(QEvent::UpdateRequest), Qt::LowEventPriority);	
}

int KPopupWindow::borderPixel() const
{
	return d_func()->nBorderPixel;
}

void KPopupWindow::setBorderPixel( int pixel )
{
	d_func()->nBorderPixel = pixel;
}

bool KPopupWindow::translucent() const
{
	return testAttribute(Qt::WA_TranslucentBackground);
}

void KPopupWindow::setTranslucent( bool bTranslucent /*= true*/ )
{
	setAttribute(Qt::WA_TranslucentBackground, bTranslucent);
	setWindowShape();
}


bool KPopupWindow::systemMenu() const
{
	Qt::WindowFlags flag = windowFlags();
	return !flag.testFlag(Qt::Tool);
}

void KPopupWindow::setSystemMenu( bool on )
{
	Qt::WindowFlags flag = windowFlags();
	if(on)
	{
		flag |= Qt::WindowSystemMenuHint;
		flag &=~ Qt::Tool;
	}
	else
	{
		flag &= ~(Qt::WindowSystemMenuHint);
		flag |= Qt::Tool;
	}
	/*补充回Window属性，确保它仍是一个弹出式窗口。*/
	flag |= Qt::Window;
	setWindowFlags(flag);
}

bool KPopupWindow::resizable() const
{
	return d_func()->bResizable;
}

void KPopupWindow::setResizable( bool on )
{
	d_func()->bResizable = on;
}

QSize KPopupWindow::frameSize() const
{
	return size();
}

void KPopupWindow::setFrameSize( const QSize& s )
{
	resize(s);
}

bool KPopupWindow::maximumButton() const
{
	return d_func()->bMaxButton;
}

void KPopupWindow::setMaximumButton( bool bMax )
{
	d_func()->bMaxButton = bMax;
	updateWindowStyle();
}

bool KPopupWindow::minimumButton() const
{
	return d_func()->bMinButton;
}

void KPopupWindow::setMinimumButton( bool bMin )
{
	d_func()->bMinButton = bMin;
	updateWindowStyle();
}

bool KPopupWindow::toolWindow() const
{
	DWORD dwExStyle = GetWindowLong(winId(), GWL_EXSTYLE);
	return dwExStyle & WS_EX_TOOLWINDOW;
}

void KPopupWindow::setToolWindow( bool on )
{
	Qt::WindowFlags flags = windowFlags();
	if(on)
	{
		flags |= Qt::Tool;
	}
	else
	{
		flags &= ~Qt::Tool;
	}
	/*补充回Window属性，确保它仍是一个弹出式窗口。*/
	flags |= Qt::Window;
	setWindowFlags(flags);	
}

bool KPopupWindow::closeOnEscape() const
{
	return d_func()->closeOnEscape;
}

void KPopupWindow::setCloseOnEscape( bool on )
{
	d_func()->closeOnEscape = on;
}


bool KPopupWindow::closeOnF4() const
{
	return d_func()->closeOnF4;
}

void KPopupWindow::setCloseOnF4( bool on )
{
	d_func()->closeOnF4 = on;
}

bool KPopupWindow::dragWindow() const
{
	Q_D(const KPopupWindow);
	return d->bEnableDragWindow;
}

void KPopupWindow::setDragWindow( bool on )
{
	Q_D(KPopupWindow);
	d->bEnableDragWindow = on;
}

bool KPopupWindow::quitAppOnClose() const
{
	return testAttribute(Qt::WA_QuitOnClose);
}

void KPopupWindow::setQuitAppOnClose( bool on )
{
	setAttribute(Qt::WA_QuitOnClose, on);
}

void KPopupWindow::init()
{
	setMaskImage(":/image/theme/maskimage.png");
	setFrameImage(":/image/theme/frameimage.png");
	setBackgroundColor(KTheme::themeColor());
	setMaskMargins(QMargins(8, 8, 8, 8));

	setBackgroundColor(KTheme::themeColor());
	/*
	只要具有Window属性，就一定是弹出窗口,它不能与WA_NativeWindow一起使用。在Qt中的概念widget为子窗口，Window为弹出窗口。
	而NativeWindow是强调Widget窗口为子窗口，且NativeWindow会导致该窗口很容易就重复的创建。
	*/
	setWindowFlags(Qt::FramelessWindowHint|Qt::Window|Qt::CustomizeWindowHint);
	setAttribute(Qt::WA_DeleteOnClose);
}

KPopupWindow::WindowStyles KPopupWindow::windowStyle() const
{
	WindowStyles style = CloseButton;
	if(resizable())
		style |= Resizable;
	if(minimumButton())
		style |= MinButton;
	if(maximumButton())
		style |= MaxButton;
	if(quitAppOnClose())
		style |= AppWindow;
	if(toolWindow())
		style |= ToolWindow;
	if(systemMenu())
		style |= SystemMenu;
	if(hasFrame())
	{
		style &= ~NoFrame;
	}
	else
	{
		style |= NoFrame;
	}
	return style;
}

void KPopupWindow::setWindowStyle( WindowStyles style )
{
	setResizable(style & Resizable);
	setMaximumButton(style & MaxButton);
	setMinimumButton(style & MinButton);
	setSystemMenu(style & SystemMenu);
	setQuitAppOnClose(style & AppWindow);
	setToolWindow(style & ToolWindow);
	setHasFrame(!(style & NoFrame));
}

void KPopupWindow::moveCenter( KWidget *center, bool bAsyn )
{
	QRect wRt;
	if(center == NULL)
	{
		QWidget *widgetParent = qobject_cast<QWidget*>(parent());
		if(widgetParent)
		{
			/*窗口中心*/
			wRt = widgetParent->geometry();
		}
		else
		{
			/*桌面中心*/
			wRt = QApplication::desktop()->availableGeometry();
		}
	}
	else
	{
		wRt = KUtility::mapWidgetToScreen(center);
	}
	QPoint pt = wRt.center();
	if(bAsyn)
	{
		QMetaObject::invokeMethod(this, "moveCenter", Qt::QueuedConnection, Q_ARG(QPoint,pt));
	}
	else
	{
		moveCenter(pt);
	}
}

void KPopupWindow::moveCenter( QPoint pt )
{
	QSize minS = minimumSize();
	QSize maxS = maximumSize();
	QRect winRt = QRect(QPoint(0, 0), size());
	if(minS == maxS && minS != QSize(-1,-1))
	{
		winRt = QRect(QPoint(0, 0), minS);
	}
	winRt.moveCenter(pt);
	setGeometry(winRt);
}

void KPopupWindow::onEscPressed( Qt::KeyboardModifiers moidifier )
{
	Q_D(KPopupWindow);

	if(d->closeOnEscape)
	{
		close();
	}
}

void KPopupWindow::closeEvent( QCloseEvent * event )
{
	__super::closeEvent(event);
}

void KPopupWindow::onCloseClick( )
{
	close();
}

void KPopupWindow::themeChange( KThemeChangeEvent *event )
{
	setBackgroundColor(KTheme::themeColor());
	__super::themeChange(event);
}

bool KPopupWindow::hasFrame() const
{
	Q_D(const KPopupWindow);
	return d->hasFrame;
}

void KPopupWindow::setHasFrame( bool on )
{
	Q_D(KPopupWindow);
	d->hasFrame = on;
}

void KPopupWindow::resetFrame()
{
	Q_D(KPopupWindow);
	if(hasFrame())
	{
		//Mask
		QPixmap maskPixmap = KTheme::getPixmap(d->szMaskImage);
		d->maskImage = maskPixmap.mask();
		createFourAngleRegion();

		//Frame
		d->frameImage = KTheme::getPixmap(d->szFrameImage);

		//MaskMargin

		d->maskMargins = d->maskMarginsCache;
		setSceneMargins(d->maskMargins);
	}
	else
	{
		//mask
		d->rgnLeftTop = QRegion();
		d->rgnLeftBot = QRegion();
		d->rgnRightBot = QRegion();
		d->rgnRightTop = QRegion();

		//frame
		d->frameImage = QPixmap();


		//MaskMargin
		d->maskMargins = QMargins(0,0,0,0);
		setSceneMargins(d->maskMargins);
	}

	createWindowRegion(rect());
	setWindowShape();
}

void KPopupWindow::construct()
{
	__super::construct();
	resetFrame();
}

void KPopupWindow::setWindowIcon( HINSTANCE hInst, int resid )
{
	HICON ico = LoadIcon(hInst, MAKEINTRESOURCE(resid));
	SendMessage(winId(), WM_SETICON, 0 /* ICON_SMALL */, (LPARAM)ico);
	SendMessage(winId(), WM_SETICON, 1 /* ICON_BIG */, (LPARAM)ico);
}

void KPopupWindow::setWindowIcon( HINSTANCE hInst, const char* myico )
{
	HICON ico = LoadIconA(hInst,myico);
	SendMessage(winId(), WM_SETICON, 0 /* ICON_SMALL */, (LPARAM)ico);
	SendMessage(winId(), WM_SETICON, 1 /* ICON_BIG */, (LPARAM)ico);
}

void KPopupWindow::setWindowIcon( const QIcon& ico )
{
	__super::setWindowIcon(ico);
}
