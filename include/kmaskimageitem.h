#pragma once

#include "kwidget.h"

class KMaskImageItemPrivate;

class KXFW_API KMaskImageItem : public KWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(KMaskImageItem)
	Q_DECLARE_PRIVATE(KMaskImageItem)

	Q_PROPERTY(QString maskImage READ maskImage WRITE setMaskImage)
	Q_PROPERTY(QRect maskRect READ maskRect WRITE setMaskRect)
	Q_PROPERTY(QRect maskEllipse READ maskEllipse WRITE setMaskEllipse)
	Q_PROPERTY(QString imagePath READ imagePath WRITE setImagePath)
	Q_PROPERTY(bool maskMouse READ maskMouse WRITE setMaskMouse)
public:
	explicit KMaskImageItem(QGraphicsItem *parent = NULL);
	~KMaskImageItem();

	QString maskImage() const;
	void setMaskImage(const QString& path);

	QRect maskRect() const;
	void setMaskRect(const QRect& rt);

	QRect maskEllipse() const;
	void setMaskEllipse(const QRect& rt);

	QString imagePath() const;
	void setImagePath(const QString& path);

	bool maskMouse() const; 
	void setMaskMouse(bool bMask);
		
	QPixmap pixmap() const;
	void setPixmap(const QPixmap& pixmap);

	bool getBatchRandomPoint(QList<QPoint>& pts, int count, QGraphicsItem *toItem);
signals:
	void mouseEvent(QEvent *ev);
protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
	void hoverMoveEvent(QGraphicsSceneHoverEvent * event);
	void mousePressEvent( QGraphicsSceneMouseEvent * event );
	void mouseReleaseEvent( QGraphicsSceneMouseEvent * event );
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
private:
	bool filterMessage(QPointF pt);
};