#pragma once;

#include "kwidget.h"

class KStackedWidgetPrivate;

class KXFW_API KStackedWidget : public KWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(KStackedWidget)
	Q_DECLARE_PRIVATE(KStackedWidget)

public:
	enum EffectType
	{
		NoEffect,
		SlideInOut
	};
	Q_ENUMS(EffectType)
	Q_PROPERTY(EffectType effect READ effectType WRITE setEffectType)
	Q_PROPERTY(int interval READ interval WRITE setInterval)
	Q_PROPERTY(bool autoSwitch READ autoSwitch WRITE setAutoSwitch)
	Q_PROPERTY(int index READ currentIndex WRITE setCurrentIndex)

public:
	explicit KStackedWidget( QGraphicsItem *parent = NULL );
	~KStackedWidget();

	EffectType effectType() const;
	void setEffectType(EffectType effect);

	int interval();
	void setInterval(int v);

	bool autoSwitch();
	void setAutoSwitch(bool on);

public:
	int count() const;
	int currentIndex() const;
	void setCurrentWidget( const KWidget *widget );
	KWidget *widgetAt(int index) const;
	KWidget *widgetAtByName( const QString& objectName ) const;
	int widgetIndex( const KWidget* widget );

public slots:
	void onAnimationStateChanged(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);
	void onAnimationFinished();
	void on_autoswitch_page();
	void setCurrentIndex(int index);

protected:
	KStackedWidget( KStackedWidgetPrivate &dd, QGraphicsItem* parent = NULL);
	void addItem( KWidget* item );
	void removeItem( KWidget* item );
	void removeItem( int index );
	void resizeEvent(QGraphicsSceneResizeEvent *event);
	QGraphicsLayout *layout() const;
	void setLayout( QGraphicsLayout *layout );
	void construct();
	void visibleEvent(bool bvis);
private:
	int insertWidget(int index, KWidget *view);
	void _resetLayout();
	void doAutoSwitch();
};