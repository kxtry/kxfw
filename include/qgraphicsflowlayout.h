#pragma once;

#include <QGraphicsLayout>

class KXFW_API QGraphicsFlowLayout : public QGraphicsLayout
{
public:
	explicit QGraphicsFlowLayout(QGraphicsLayoutItem *parent = NULL);
	void addItem(QGraphicsLayoutItem * item);
	void removeItem(QGraphicsLayoutItem *item);
	void setContentsMargins(const QMargins &margins);
	int heightForWidth(int width) const;
	void setSpacing(bool bVertical, qreal val);
	qreal spacing(bool bVertical) const;
protected:
	virtual int	count() const;
	virtual QGraphicsLayoutItem *itemAt(int i) const;
	virtual void removeAt(int index);
	virtual void getContentsMargins(qreal * left, qreal * top, qreal * right, qreal * bottom) const;
	virtual void updateGeometry();
	virtual void widgetEvent(QEvent * e);
	virtual void invalidate();
	virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF & constraint = QSizeF()) const;
private:
	QSizeF minimumSize() const;
	qreal doLayout(const QRectF &rect, bool testOnly) const;
	int smartSpacing(QStyle::PixelMetric pm) const;
private:
	QList<QGraphicsLayoutItem *> m_itemList;
	int m_hSpace;
	int m_vSpace;
	QMargins m_margins;
};