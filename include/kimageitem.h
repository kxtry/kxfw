#pragma once;

#include "kwidget.h"

class KImageItemPrivate;

class KXFW_API KImageItem : public KWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KImageItem)
public:
	Q_PROPERTY(QString imagePath READ imagePath WRITE setImagePath)
	Q_PROPERTY(qreal scale READ scale WRITE setScale)
	Q_PROPERTY(bool autoAdjust READ autoAdjust WRITE setAutoAdjust)
	Q_PROPERTY(bool handHover READ handHover WRITE setHandHover)
	Q_PROPERTY(bool acceptClick READ acceptClick WRITE setAcceptClick)
	Q_PROPERTY(QMargins imageMargins READ imageMargins WRITE setImageMargins);
	Q_PROPERTY(Qt::Alignment imageAlignment READ imageAlignment WRITE setImageAlignment)
	Q_PROPERTY(QColor effectColor READ effectColor WRITE setEffectColor)
	Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
	Q_PROPERTY(int borderPixel READ borderPixel WRITE setBorderPixel)
	Q_PROPERTY(qreal rotation READ rotation WRITE setRotation)
	Q_PROPERTY(QSize pixmapSize READ pixmapSize)
public:
	explicit KImageItem(QGraphicsItem *parent = NULL);
	~KImageItem();
	
	QString imagePath() const;
	void setImagePath(const QString& file);

	qreal scale() const;
	void setScale(qreal factor);

	qreal rotation() const;
	void setRotation(qreal val);

	int borderPixel();
	void setBorderPixel(int pixel);

	bool autoAdjust() const;
	void setAutoAdjust(bool bAdjust);

	QMargins imageMargins() const;
	void setImageMargins(const QMargins &margin);

	bool handHover() const;
	void setHandHover(bool hover); 

	bool acceptClick() const;
	void setAcceptClick(bool bClick);

	QPixmap pixmap() const;
	void setPixmap(const QPixmap& pixmap);

	QSize pixmapSize();

	Qt::Alignment imageAlignment() const;
	void setImageAlignment(Qt::Alignment align);

	void setEffectColor( const QColor& color );
	QColor effectColor() const;

	QColor borderColor() const;
	void setBorderColor( const QColor &color );

signals:
	void clicked();
	void pressed( const QPointF &point );
	void released( const QPointF &point );
	void enter();
	void leave();

private:
	void mousePressEvent ( QGraphicsSceneMouseEvent * event );
	void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
	void hoverEnterEvent( QGraphicsSceneHoverEvent * event );
	void hoverLeaveEvent( QGraphicsSceneHoverEvent * event );
	void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */ );
	void drawBorder(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void drawTopLeft(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void drawTopRight(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void drawBotLeft(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void drawBotRight(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void drawHCenter(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void drawVCenter(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void drawHVCenter(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void drawBHCenter(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
	void drawRVCenter(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);

protected:
	void doThemeChange();

private:
	QSizeF drawScaleSize(int border);
	int borderWidth();
	QRect marginRect();

};