#pragma once;

#include "kwidget.h"

class KColorTablePrivate;

class KXFW_API KColorTable : public KWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KColorTable)
	Q_PROPERTY(QColor color READ color WRITE setColor)
public:
	explicit KColorTable(QGraphicsItem *parent = NULL);
	~KColorTable();

	QColor color() const;
	void setColor(const QColor& clr);

	Q_INVOKABLE void connectToTheme();
signals:
	void colorChanged(const QColor& clr);

public slots:
	void on_colorChanged(const QColor& clr);

protected:
	void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */ );
	void resizeEvent(QGraphicsSceneResizeEvent *event);
	void mousePressEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent * event);

private:
	QRectF hueLightRect();
	QRectF satuationRect();
};