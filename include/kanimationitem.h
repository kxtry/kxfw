#pragma once

#include "kwidget.h"

class KAnimationItemPrivate;

class KXFW_API KAnimationItem: public KWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(KAnimationItem)
	Q_DECLARE_PRIVATE(KAnimationItem)

public:
	enum PlayPolicy
	{
		NoDefine,
		PlayToVisible,
		PlayOnHover
	};

	Q_ENUMS(PlayPolicy)

	Q_PROPERTY(QString imagePath READ imagePath WRITE setImagePath)
	Q_PROPERTY(bool playLoop READ playLoop WRITE setPlayLoop)
	Q_PROPERTY(QColor borderColor READ borderColor WRITE setBorderColor)
	Q_PROPERTY(int borderPixel READ borderPixel WRITE setBorderPixel)
	Q_PROPERTY(PlayPolicy playPolicy READ playPolicy WRITE setPlayPolicy)
	Q_PROPERTY(QMargins playMargins READ playMargins WRITE setPlayMargins)
	Q_PROPERTY(bool stretch READ stretch WRITE setStretch)
	Q_PROPERTY(bool smooth READ smooth WRITE setSmooth)
	Q_PROPERTY(int roundPixel READ roundPixel WRITE setRoundPixel)
public:
	explicit KAnimationItem( QGraphicsItem* parent = NULL );
	
	void setImagePath( const QString &imageName );
	QString imagePath() const;

	bool playLoop() const;
	void setPlayLoop( bool isLoop );

	int borderPixel();
	void setBorderPixel(int pixel);

	int roundPixel();
	void setRoundPixel(int pixel);

	PlayPolicy playPolicy();
	void setPlayPolicy(PlayPolicy p);

	QMargins playMargins() const;
	void setPlayMargins(const QMargins& m);

	bool stretch();
	void setStretch(bool on);

	bool smooth();
	void setSmooth(bool on);

	QColor borderColor() const;
	void setBorderColor( const QColor &color );

    QSize getMovieSize(bool simple);

public:
	Q_INVOKABLE void start();
	Q_INVOKABLE void stop();

private slots:
	void on_frameChanged ( int frameNumber );
	void on_finished();

protected:
	void init();

	void resizeEvent( QGraphicsSceneResizeEvent * event );
	void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */ );
	void visibleEvent(bool bvis);
	void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);

private:
	QRect marginRect();
};

