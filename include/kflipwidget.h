#pragma once;

#include "kwidget.h"

class KFlipWidgetPrivate;

class KXFW_API KFlipWidget : public KWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KFlipWidget)
	Q_PROPERTY(bool example READ useExample WRITE setUseExample)
public:
	explicit KFlipWidget(QGraphicsItem* parent=NULL);
	~KFlipWidget();

	KWidget *frontPage();
	KWidget *backPage();

	void flip();
	void activate(bool bFront);

	bool useExample();
	void setUseExample(bool on);
signals:
	void activeFront();
	void activeBack();

protected:
	void resizeEvent(QGraphicsSceneResizeEvent *event);

private slots:
	void on_animation_stateChanged(QAbstractAnimation::State newState, QAbstractAnimation::State oldState);

private:
	void on_animation_start();
	void on_animation_stop();
};