#pragma once

#include "kwidget.h"

class KProgressBarPrivate;

class KXFW_API KProgressBar : public KWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(KProgressBar)
	Q_DECLARE_PRIVATE(KProgressBar)

	Q_PROPERTY(int minimum READ minimum WRITE setMinimum)
	Q_PROPERTY(int maximum READ maximum WRITE setMaximum)
	Q_PROPERTY(int value READ value WRITE setValue NOTIFY valueChanged)
	Q_PROPERTY(bool textVisible READ isTextVisible WRITE setTextVisible)
	Q_PROPERTY(QString frameImage READ frameImage WRITE setFrameImage)
	Q_PROPERTY(QString progressImage READ progressImage WRITE setProgressImage)
	Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation)
	Q_PROPERTY(int numberTextStyle READ numberTextStyle WRITE setNumberTextStyle)
	Q_PROPERTY(int minSize READ minSize WRITE setMinSize)
	
public:
	enum ProgressNumberStyle
	{
		NUMBER_PERCENTAGE = 0,
		PERCENTAGE_NO_SIGN,
		JUST_VALUE,		
	};

public:
	explicit KProgressBar( QGraphicsItem* parent = NULL );

	void setValue( int value );
	int	value() const;

	void setRange( int minimum, int maximum );

	void setMaximum ( int maximum );
	int	maximum() const;

	void setMinimum ( int minimum );
	int minimum() const;

	bool isTextVisible () const;
	void setTextVisible ( bool visible );

	void reset();

	void setProgressImage(const QString& imgs);
	QString progressImage() const;

	void setFrameImage(const QString& imgs);
	QString frameImage() const;

	int numberTextStyle() const;
	void setNumberTextStyle(int style);

	Qt::Orientation orientation() const;
	void setOrientation(Qt::Orientation orientation);

	int minSize() const;
	void setMinSize(int size);

	void setTextColor(const QColor &outsideColor, const QColor &insideColor);

signals:
	void valueChanged ( int value );

protected:
	void init();
	void resizeEvent( QGraphicsSceneResizeEvent * event );
};


