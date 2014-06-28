#pragma once;

#include "kwindow_p.h"
#include "kpopupwindow.h"

class KXFW_API KPopupWindowPrivate : public KWindowPrivate
{
	Q_DECLARE_PUBLIC(KPopupWindow)
public:
	KPopupWindowPrivate();

	QPixmap frameImage;
	QString szFrameImage;
	QBitmap maskImage;
	QString szMaskImage;
	QMargins maskMargins;
	QMargins maskMarginsCache;
	bool hasFrame;
	QPixmap customImage; //将修改为专用的KBackgroundItem但不是GraphicsItem的那种，防止过度刷新进行处理该绘制。
	QString szCustomImage;
	QColor backgroundColor;
	QRegion maskRegion;
	QRegion rgnLeftTop, rgnRightTop, rgnLeftBot, rgnRightBot;

	bool bResizable;
	QSize sizeFrameMin, sizeFrameMax;
	QPoint dragPosition;
	bool bDragWindow;
	int nBorderPixel;
	bool bEnableDragWindow;

	bool bMinButton;
	bool bMaxButton;

	bool closeOnEscape;
	bool closeOnF4;
};