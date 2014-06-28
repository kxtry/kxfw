#pragma once;


#include "kwidget.h"

class KTableWidgetPrivate;

class KXFW_API KTableWidget : public KWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KTableWidget)
	Q_PROPERTY(QMargins margins READ margins WRITE setMargins)
	Q_PROPERTY(QSizeF spacing READ spacing WRITE setSpacing)
	Q_PROPERTY(qreal hspacing READ hspacing WRITE setHSpacing)
	Q_PROPERTY(qreal vspacing READ vspacing WRITE setVSpacing)
public:
	explicit KTableWidget(QGraphicsItem *parent=NULL);
	~KTableWidget();
	
	QSizeF spacing() const;
	void setSpacing(const QSizeF& s);

	qreal hspacing();
	void setHSpacing(qreal v);

	qreal vspacing();
	void setVSpacing(qreal v);
	
	QMargins margins() const;
	void setMargins(const QMargins& m);

	int itemCount();
	void removeAll();
	
	void updateLayout();
	int calcMinimumHeight(bool reset=true);
protected:
	void addItem(KWidget *item);
	void removeItem(KWidget *item);
	KWidget::LayoutType layoutType();
	void setLayoutType(KWidget::LayoutType type);
	void resizeEvent(QGraphicsSceneResizeEvent *e);
private:
	void doLayout(bool reset = true);
};