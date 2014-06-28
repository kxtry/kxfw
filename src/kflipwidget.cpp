#include "stdafx.h"
#include "kflipwidget.h"
#include "kwidget_p.h"
#include "kxmlui.h"

#include "klabel.h"

#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QVector3D>
#include <QSignalTransition>

KX_WIDGET_CREATOR_GLOBAL_STATIC(KFlipWidget)

class KFlipWidgetPrivate : public KWidgetPrivate
{
	Q_DECLARE_PUBLIC(KFlipWidget)
public:
	QPointer<QGraphicsRotation> flipRotation;
	QPointer<QGraphicsRotation> xRotation;
	QPointer<QGraphicsRotation> yRotation;
	QPointer<KWidget> front;
	QPointer<KWidget> back;
	QPointer<QPropertyAnimation> smoothFlipRotation;
	QPointer<QPropertyAnimation> smoothFlipScale;
	QPointer<QPropertyAnimation> smoothFlipXRotation;
	QPointer<QPropertyAnimation> smoothFlipYRotation;
	QPointer<QParallelAnimationGroup> flipAnimation;

	QPointer<QState> frontState;
	QPointer<QState> backState;
	QPointer<QStateMachine> stateMachine;
	QPointer<QSignalTransition> frontTransition;
	QPointer<QSignalTransition> backTransition;

	int vum;
	QPainter::RenderHints hints;
public:
	KFlipWidgetPrivate()
	{
		vum = -1;
	}

	~KFlipWidgetPrivate()
	{

	}

	void init()
	{
		Q_Q(KFlipWidget);
		flipRotation = new QGraphicsRotation(q);
		xRotation = new QGraphicsRotation(q);
		yRotation = new QGraphicsRotation(q);

		flipRotation->setAxis(Qt::YAxis);
		xRotation->setAxis(Qt::YAxis);
		yRotation->setAxis(Qt::XAxis);
		q->setTransformations(QList<QGraphicsTransform *>() << flipRotation << xRotation << yRotation);

		front = new KWidget(q);
		back = new KWidget(q);

		QSizeF s = q->size();
		back->setTransform(QTransform().rotate(180, Qt::YAxis).translate(-s.width(), 0));

		smoothFlipRotation = new QPropertyAnimation(flipRotation, "angle");
		smoothFlipScale = new QPropertyAnimation(q, "scale");
		smoothFlipXRotation = new QPropertyAnimation(xRotation, "angle");
		smoothFlipYRotation = new QPropertyAnimation(yRotation, "angle");
		flipAnimation = new QParallelAnimationGroup(q);

		smoothFlipScale->setDuration(500);
		smoothFlipRotation->setDuration(500);
		smoothFlipXRotation->setDuration(500);
		smoothFlipYRotation->setDuration(500);
		smoothFlipScale->setEasingCurve(QEasingCurve::InOutQuad);
		smoothFlipRotation->setEasingCurve(QEasingCurve::InOutQuad);
		smoothFlipXRotation->setEasingCurve(QEasingCurve::InOutQuad);
		smoothFlipYRotation->setEasingCurve(QEasingCurve::InOutQuad);
		smoothFlipScale->setKeyValueAt(0, qVariantValue<qreal>(1.0));
		smoothFlipScale->setKeyValueAt(0.5, qVariantValue<qreal>(0.7));
		smoothFlipScale->setKeyValueAt(1, qVariantValue<qreal>(1.0));

		flipAnimation->addAnimation(smoothFlipRotation);
		flipAnimation->addAnimation(smoothFlipScale);
		flipAnimation->addAnimation(smoothFlipXRotation);
		flipAnimation->addAnimation(smoothFlipYRotation);


		// Flip animation delayed property assignment
		QSequentialAnimationGroup *setVariablesSequence = new QSequentialAnimationGroup;
		QPropertyAnimation *setBackItemVisibleAnimation = new QPropertyAnimation(back, "visible");
		QPropertyAnimation *setSelectionItemVisibleAnimation = new QPropertyAnimation(front, "visible");
		setBackItemVisibleAnimation->setDuration(0);
		setSelectionItemVisibleAnimation->setDuration(0);
		setVariablesSequence->addPause(250);
		setVariablesSequence->addAnimation(setBackItemVisibleAnimation);
		setVariablesSequence->addAnimation(setSelectionItemVisibleAnimation);
		flipAnimation->addAnimation(setVariablesSequence);

		// Build the state machine
		stateMachine = new QStateMachine(q);
		frontState = new QState(stateMachine);
		backState = new QState(stateMachine);

		frontState->assignProperty(back, "visible", false);
		frontState->assignProperty(flipRotation, "angle", qVariantValue<qreal>(0.0));
		frontState->assignProperty(front, "visible", true);

		backState->assignProperty(back, "visible", true);
		backState->assignProperty(xRotation, "angle", qVariantValue<qreal>(0.0));
		backState->assignProperty(yRotation, "angle", qVariantValue<qreal>(0.0));
		backState->assignProperty(flipRotation, "angle", qVariantValue<qreal>(180.0));
		backState->assignProperty(front, "visible", false);

		stateMachine->setInitialState(frontState);

		frontTransition = new QSignalTransition(q, SIGNAL(activeBack()), frontState);
		backTransition = new QSignalTransition(q, SIGNAL(activeFront()), backState);
		frontTransition->addAnimation(flipAnimation);
		backTransition->addAnimation(flipAnimation);
		frontTransition->setTargetState(backState);
		backTransition->setTargetState(frontState);

		bool bok = QObject::connect(flipAnimation, SIGNAL(stateChanged(QAbstractAnimation::State,QAbstractAnimation::State)), q, SLOT(on_animation_stateChanged(QAbstractAnimation::State,QAbstractAnimation::State)));
		bool bok1 = QObject::connect(flipAnimation, SIGNAL(stateChanged(QAbstractAnimation::State,QAbstractAnimation::State)), q, SLOT(on_animation_stateChanged(QAbstractAnimation::State,QAbstractAnimation::State)));

		stateMachine->start();
	}

	void autoTest()
	{
		front->setLayoutType(KWidget::VBox);
		front->setMargins(QMargins(15,15,15,15));
		back->setLayoutType(KWidget::VBox);
		back->setMargins(QMargins(15,15,15,15));
		KLabel *label1 = new KLabel(front);
		KLabel *label2 = new KLabel(back);
		front->addItem(label1);
		back->addItem(label2);
		label1->setText("front-front-front");
		label1->setBackgroundColor(255,0,0,50);
		label2->setText("back-back-back");
		label2->setBackgroundColor(255,0,255,50);
	}

	KWidget *frontPage()
	{
		return front;
	}

	KWidget *backPage()
	{
		return back;
	}

	void resize(QSizeF s)
	{
		Q_Q(KFlipWidget);
		front->setGeometry(0,0,s.width(),s.height());
		back->setGeometry(0,0,s.width(), s.height());
		back->setTransform(QTransform().rotate(180, Qt::YAxis).translate(-s.width(), 0));

		flipRotation->setOrigin(QVector3D(QPointF(s.width()/2, s.height()/2)));
		xRotation->setOrigin(QVector3D(QPointF(s.width()/2, s.height()/2)));
		yRotation->setOrigin(QVector3D(QPointF(s.width()/2, s.height()/2)));
	}

	void activate(bool bFront)
	{
		stateMachine->stop();
		stateMachine->setInitialState(bFront ? frontState : backState);
		stateMachine->start();
	}

	void flip()
	{
		Q_Q(KFlipWidget);
		if(front->isVisible())
		{
			q->activeBack();
		}
		else
		{
			q->activeFront();
		}
	}

};

KFlipWidget::KFlipWidget( QGraphicsItem* parent/*=NULL*/ )
 : KWidget(*new KFlipWidgetPrivate(), parent)
{
	Q_D(KFlipWidget);
	d->init();
}

KFlipWidget::~KFlipWidget()
{

}

KWidget * KFlipWidget::frontPage()
{
	Q_D(KFlipWidget);
	return d->frontPage();
}

KWidget * KFlipWidget::backPage()
{
	Q_D(KFlipWidget);
	return d->backPage();
}

void KFlipWidget::resizeEvent( QGraphicsSceneResizeEvent *event )
{
	Q_D(KFlipWidget);
	d->resize(size());
}

bool KFlipWidget::useExample()
{
	return false;
}

void KFlipWidget::setUseExample( bool on )
{
	Q_D(KFlipWidget);
	d->autoTest();
}

void KFlipWidget::activate( bool bFront )
{
	Q_D(KFlipWidget);
	d->activate(bFront);
}

void KFlipWidget::flip()
{
	Q_D(KFlipWidget);
	d->flip();
}

void KFlipWidget::on_animation_start()
{
	Q_D(KFlipWidget);
	QGraphicsView *gv = view();
	QGraphicsView::ViewportUpdateMode vum = gv->viewportUpdateMode();
	if(vum != QGraphicsView::FullViewportUpdate)
	{
		d->vum = vum;
		d->hints = gv->renderHints();
		gv->setRenderHints(QPainter::Antialiasing|QPainter::TextAntialiasing|QPainter::SmoothPixmapTransform|QPainter::HighQualityAntialiasing);
		gv->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
	}
}

void KFlipWidget::on_animation_stop()
{
	Q_D(KFlipWidget);
	if(d->vum >= 0)
	{
		QGraphicsView *gv = view();
		gv->setRenderHints(d->hints);
		gv->setViewportUpdateMode((QGraphicsView::ViewportUpdateMode)d->vum);
		d->vum = -1;
	}
}

void KFlipWidget::on_animation_stateChanged( QAbstractAnimation::State newState, QAbstractAnimation::State oldState )
{
	if(newState == QAbstractAnimation::Running)
	{
		on_animation_start();
	}
	else
	{
		on_animation_stop();
	}
}
