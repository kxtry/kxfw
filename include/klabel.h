#pragma once;

#include "kwidget.h"

class KLabelPrivate;

class KXFW_API KLabel : public KWidget
{
	Q_OBJECT

	Q_DISABLE_COPY(KLabel)
	Q_DECLARE_PRIVATE(KLabel)

	Q_PROPERTY( QString text READ text WRITE setText )
	Q_PROPERTY( bool autoAdjust READ autoAdjust WRITE setAutoAdjust )
	Q_PROPERTY( QString link READ linkUrl WRITE setLinkUrl )
	Q_PROPERTY( QColor textColor READ textColor WRITE setTextColor )
	Q_PROPERTY( int fontSize READ fontSize WRITE setFontSize )
	Q_PROPERTY( bool click READ acceptClick WRITE setAcceptClick )
	Q_PROPERTY( bool underlineOnHover READ underlineOnHover WRITE setUnderlineOnHover )
	Q_PROPERTY( bool handCursorOnHover READ handCursorOnHover WRITE setHandCursorOnHover)
	Q_PROPERTY( bool bold READ bold WRITE setBold )
	Q_PROPERTY( bool shadow READ dropShadow WRITE setDropShadow )
	Q_PROPERTY( Qt::Alignment textAlignment READ textAlignment WRITE setTextAlignment )
	Q_PROPERTY( Qt::TextElideMode elide READ textElideMode WRITE setTextElidedMode )
	Q_PROPERTY( bool wordWrap READ wordWrap WRITE setWordWrap )
	Q_PROPERTY( QColor shadowColor READ shadowColor WRITE setShadowColor )
	Q_PROPERTY( QPointF shadowOffset READ shadowOffset WRITE setShadowOffset )
	Q_PROPERTY( qreal shadowBlurRadius READ shadowBlurRadius WRITE setShadowBlurRadius )
	Q_PROPERTY( QMargins textMargins READ textMargins WRITE setTextMargins )
	Q_PROPERTY( QString fontName READ fontName WRITE setFontName )
	Q_PROPERTY( bool antialiase READ antialiase WRITE setAntialiase )

public:
	explicit KLabel( QGraphicsItem* parent = NULL );
	~KLabel( void );

	QString text() const;
	void setText( const QString& text );

	bool autoAdjust() const;
	void setAutoAdjust( bool bAdjust );

	void setLinkUrl( const QString& url );
	QString linkUrl() const;

	void setTextColor( const QColor &textColor );
	QColor textColor( void ) const;

	void setFontSize( int fs );
	int fontSize() const;

	void setAcceptClick(bool enabled);
	bool acceptClick() const;

	void setUnderlineOnHover( bool on );
	bool underlineOnHover() const;

	bool handCursorOnHover();
	void setHandCursorOnHover(bool on);

	void setBold( bool enable );
	bool bold() const;

	bool dropShadow() const;
	void setDropShadow(bool shadow);

	Qt::Alignment textAlignment() const;
	void setTextAlignment( Qt::Alignment align );

	Qt::TextElideMode textElideMode() const;
	void setTextElidedMode(Qt::TextElideMode elide);

	bool wordWrap() const;
	void setWordWrap(bool wrap);

	QColor shadowColor() const;
	void setShadowColor(const QColor &color);

	QPointF shadowOffset() const;
	void setShadowOffset(const QPointF &offset);

	qreal shadowBlurRadius() const;
	void setShadowBlurRadius(qreal blurRadius);

	void setEnabled( bool enabled );

	QMargins textMargins() const;
	void setTextMargins(const QMargins &margins);

	QString fontName() const;
	void setFontName(const QString& name);

	bool antialiase();
	void setAntialiase(bool on);

signals:
	void clicked();
	void editFinished();
protected:
	virtual void mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * event );
	virtual void mousePressEvent( QGraphicsSceneMouseEvent *event );
	virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
	virtual void resizeEvent ( QGraphicsSceneResizeEvent * event ); 
	virtual void hoverMoveEvent( QGraphicsSceneHoverEvent *event );
	virtual void hoverLeaveEvent ( QGraphicsSceneHoverEvent * event );	
	virtual void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */ );
	virtual void doSkinChange();
private:
	void updateElideText();
	QRectF editRect();
	void updateEdit();
};
