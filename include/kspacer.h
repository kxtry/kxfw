#pragma once;

#include "kwidget.h"

class KXFW_API KSpacer : public KWidget
{
	Q_OBJECT
	Q_PROPERTY(bool acceptClick READ acceptClick WRITE setAcceptClick)
	Q_PROPERTY(bool acceptHover READ acceptHover WRITE setAcceptHover)
public:
	explicit KSpacer(QGraphicsItem *parent = NULL);
	~KSpacer();
	
	bool acceptClick() const;
	void setAcceptClick(bool bClick);

	bool acceptHover() const;
	void setAcceptHover(bool bHover);
signals:
	void mouseEvent(QEvent *event);
private:
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
	void mousePressEvent(QGraphicsSceneMouseEvent * event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
	void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
};