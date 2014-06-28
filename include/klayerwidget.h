#pragma once;
#include "kwidget.h"

class KLayerWidgetPrivate;
class KLayerWidget : public KWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KLayerWidget)
public:
	explicit KLayerWidget(QGraphicsItem* parent = NULL);
	~KLayerWidget();

	void addItem(KWidget* item);
	void removeItem(KWidget *item);
	void construct();
private:
	void resizeEvent(QGraphicsSceneResizeEvent * event);
	QVariant itemChange(GraphicsItemChange change, const QVariant &value);
	void reparentItem(QGraphicsItem* itemParent);
	bool eventFilter(QObject * watched, QEvent * event);
private:
	Q_DISABLE_COPY(KLayerWidget);
};