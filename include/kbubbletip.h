#pragma once;

#include "kfollowwindow.h"

class KBubbleTipPrivate;

class KXFW_API KBubbleTip : public KFollowWindow
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KBubbleTip)
public:
	Q_PROPERTY(bool shadow READ shadowEnabled WRITE setShadowEnable)
	Q_PROPERTY(qreal shadowBlur READ shadowBlurRadius WRITE setShadowBlurRadius)
	Q_PROPERTY(QPointF shadowOffset READ shadowOffset WRITE setShadowOffset)
	Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
	Q_PROPERTY(QColor brushColor READ brushColor WRITE setBrushColor)
	Q_PROPERTY(QColor penColor READ penColor WRITE setPenColor)

public:
	explicit KBubbleTip(QWidget *parent=NULL);
	~KBubbleTip();

	void setTarget(KWidget *w);

	QColor brushColor() const;
	void setBrushColor(const QColor& clr);

	QColor backgroundColor() const;
	void setBackgroundColor(const QColor& clr);

	QColor penColor() const;
	void setPenColor(const QColor& clr);

	void setPen(const QPen& pen);
	void setBrush(const QBrush& br);

	qreal shadowBlurRadius();
	void setShadowBlurRadius(qreal v);

	QPointF shadowOffset();
	void setShadowOffset(QPointF v);

	bool shadowEnabled();
	void setShadowEnable(bool on);

protected:
	virtual void construct();
	virtual void resetGeometry();
	virtual void drawBackground( QPainter * painter, const QRectF & rect );
	virtual void drawForeground( QPainter * painter, const QRectF & rect );
};