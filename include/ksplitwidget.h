#pragma once;

#include "kwidget.h"

class KSplitWidgetPrivate;

class KXFW_API KSplitWidget : public KWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KSplitWidget)
	Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation)
	Q_PROPERTY(int leftTopMinSize READ leftTopMinSize WRITE setLeftTopMinSize)
	Q_PROPERTY(int leftTopMaxSize READ leftTopMaxSize WRITE setLeftTopMaxSize)
	Q_PROPERTY(int rightBotMinSize READ rightBotMinSize WRITE setRightBotMinSize)
	Q_PROPERTY(int rightBotMaxSize READ rightBotMaxSize WRITE setRightBotMaxSize)
	Q_PROPERTY(int splitbarSize READ splitBarSize WRITE setSplitBarSize)
	Q_PROPERTY(int value READ value WRITE setValue)
	Q_PROPERTY(QObject* splitbar READ splitbar)
public:
	explicit KSplitWidget(QGraphicsItem *parent=NULL);
	~KSplitWidget();

	Qt::Orientation orientation();
	void setOrientation(Qt::Orientation o);

	int leftTopMinSize();
	void setLeftTopMinSize(int s);

	int leftTopMaxSize();
	void setLeftTopMaxSize(int s);

	int rightBotMinSize();
	void setRightBotMinSize(int s);

	int rightBotMaxSize();
	void setRightBotMaxSize(int s);

	int value();
	void setValue(int v);

	int splitBarSize();
	void setSplitBarSize(int s);

	QObject* splitbar();
protected:
	virtual void resizeEvent(QGraphicsSceneResizeEvent *event);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
	virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
	virtual void timerEvent(QTimerEvent * event);

	virtual LayoutType layoutType();
	virtual void setLayoutType(LayoutType type);
	virtual void addItem(KWidget *item);
	virtual void removeItem(KWidget *item);
	virtual void construct();

private:
	void updateLayout();
	void _updateLayout();
};