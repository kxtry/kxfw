#pragma once

#include "kwidget.h"

class KSliderPrivate;

class KXFW_API KSlider : public KWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(KSlider)
	Q_DECLARE_PRIVATE(KSlider)

	Q_PROPERTY(QString thumbImage READ thumbImage WRITE setThumbImage)
	Q_PROPERTY(QString frameImage READ frameImage WRITE setFrameImage)
	Q_PROPERTY(QString selectImage READ selectImage WRITE setSelectImage)
	Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation)
	Q_PROPERTY(QSizeF thumbSize READ thumbSize WRITE setThumbSize)
	Q_PROPERTY(qreal value READ value WRITE setValue)
	Q_PROPERTY(QSizeF range READ range WRITE setRange)
	Q_PROPERTY(qreal minimum READ minimum WRITE setMinimum)
	Q_PROPERTY(qreal maximum READ maximum WRITE setMaximum)
public:
	explicit KSlider(QGraphicsItem *parent = NULL);
	explicit KSlider(KWidgetPrivate &dd, QGraphicsItem *parent = NULL);
	~KSlider();

	qreal value() const;
	bool setValue(qreal v);

	QSizeF range() const;
	void setRange(const QSizeF& s);
	void setRange(qreal minval, qreal maxval);

	Qt::Orientation orientation() const;
	void setOrientation(Qt::Orientation o);

	QString thumbImage() const;
	void setThumbImage( const QString &thumbImage );

	QString frameImage() const;
	void setFrameImage( const QString &backImage );

	QString selectImage() const;
	void setSelectImage( const QString &selectImage );

	qreal minimum();
	void setMinimum(qreal v);

	qreal maximum();
	void setMaximum(qreal v);

	QSizeF thumbSize() const;
	void setThumbSize(const QSizeF& s);
signals:
	void valueChanged(qreal value);
	void mouseRelease();

protected:
	void mousePressEvent(QGraphicsSceneMouseEvent * event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
	void mouseMoveEvent(QGraphicsSceneMouseEvent * event);

	void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

	void resizeEvent( QGraphicsSceneResizeEvent * event );
	void timerEvent(QTimerEvent *event);
private:
	void init();
	void relayout();

	qreal _value() const;
	bool _setValue(qreal v);

	void _relayout();
};