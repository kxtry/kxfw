#pragma once;

#include "kwidget.h"

class KLoadingItemPrivate;

class KXFW_API KLoadingItem: public KWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(KLoadingItem)
	Q_DECLARE_PRIVATE(KLoadingItem)

	Q_PROPERTY(QSizeF animationSize READ animationSize WRITE setAnimationSize)
	Q_PROPERTY(qreal ratio READ ratio WRITE setRatio)
	Q_PROPERTY(qreal fps READ fps WRITE setFps)
	Q_PROPERTY(QColor color READ color WRITE setColor)

public:
	explicit KLoadingItem( QGraphicsItem* parent );

	QSizeF animationSize() const;
	void setAnimationSize(const QSizeF& s);

	qreal ratio() const;
	void setRatio(const qreal &r);

	qreal fps() const;
	void setFps(const qreal &f);

	QColor color() const;
	void setColor(const QColor &color);

public:
	Q_INVOKABLE void start();
	Q_INVOKABLE void stop();
protected:
	void visibleEvent(bool bvis);
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void timerEvent(QTimerEvent *event);

};