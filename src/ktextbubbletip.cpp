#include "stdafx.h"
#include "ktextbubbletip.h"
#include "klabel.h"
#include "ktextitem.h"


class KTextBubbleTipPrivate
{
public:
	QPointer<KTextItem> text;
	QPointer<KLabel> label;
	qreal textWidth;
	int elapsetm;
	QTimer timer;
public:
	KTextBubbleTipPrivate()
		: textWidth(0.0)
		, elapsetm(0)
	{

	}

	~KTextBubbleTipPrivate()
	{

	}

};

KTextBubbleTip::KTextBubbleTip( QWidget *parent/*=NULL*/ )
: KBubbleTip(parent)
, d(new KTextBubbleTipPrivate())
{
	KWidget *root = rootWidget();
	KLabel *label = new KLabel(root);
	KTextItem *text = new KTextItem(root);
	label->setFixHeight(20);
	root->addItem(text);
	root->addItem(label);
	d->text = text;
	d->label = label;
	label->setText("关闭");
	label->setTextColor("blue");
	label->setTextAlignment(Qt::AlignVCenter|Qt::AlignRight);
	label->setAcceptClick(true);
	label->setUnderlineOnHover(true);
	label->setTextMargins(QMargins(10,0,10,0));
	QObject::connect(label, SIGNAL(clicked()), this, SLOT(on_clicked()));
	QObject::connect(&d->timer, SIGNAL(timeout()), this, SLOT(on_elapse_timeout()));
}

KTextBubbleTip::~KTextBubbleTip()
{

}

QString KTextBubbleTip::text() const
{
	return d->text->toText();
}

void KTextBubbleTip::setText( const QString& txt )
{
	d->text->setText(txt);
	updateTextLayout();
}

Qt::Orientation KTextBubbleTip::orientation()
{
	KWidget *root = rootWidget();
	QGraphicsLinearLayout *layout = dynamic_cast<QGraphicsLinearLayout*>(root->layout());
	return layout->orientation();
}

void KTextBubbleTip::setOrientation( Qt::Orientation o )
{
	KWidget *root = rootWidget();
	QGraphicsLinearLayout *layout = dynamic_cast<QGraphicsLinearLayout*>(root->layout());
	layout->setOrientation(o);
	if(o == Qt::Horizontal)
	{
		d->label->setAutoAdjust(true);
		d->label->setFixHeight(-1);
	}
	else
	{
		d->label->setAutoAdjust(false);
		d->label->setFixHeight(20);
	}
}

qreal KTextBubbleTip::textWidth()
{
	return d->textWidth;
}

void KTextBubbleTip::setTextWidth( qreal v )
{
	d->textWidth = v;
	updateTextLayout();
}

void KTextBubbleTip::updateTextLayout()
{
	if(qFuzzyCompare(d->textWidth, 0.0))
	{
		QSizeF okSize = d->text->idealSize();
		d->text->setFixSize(okSize);
	}
	else
	{
		QSizeF size = d->text->hitTextSize(d->textWidth);
		d->text->setFixSize(size);
	}
}

void KTextBubbleTip::on_clicked()
{
	emit clicked();
	close();
}

KLabel * KTextBubbleTip::labelObject()
{
	return d->label;
}

KTextItem * KTextBubbleTip::textObject()
{
	return d->text;
}

int KTextBubbleTip::elapseTime()
{
	return d->elapsetm;
}

void KTextBubbleTip::setElapseTime( int t )
{
	d->elapsetm = t;
}

void KTextBubbleTip::showEvent( QShowEvent *event )
{
	if(d->elapsetm > 0)
	{
		d->timer.start(1000);
		d->label->setText(QString("关闭(%1)").arg(d->elapsetm));
	}
	else
	{
		d->label->setText("关闭");
	}
	QSizeF s = calcWindowPreferSize();
	setWindowSize(s.toSize());
	resetGeometry();
}

void KTextBubbleTip::hideEvent( QShowEvent *event )
{
	d->timer.stop();
	d->elapsetm = 0;
}

void KTextBubbleTip::on_elapse_timeout()
{
	d->elapsetm--;
	if(d->elapsetm <= 0)
	{
		d->timer.stop();
		close();
	}
	d->label->setText(QString("关闭(%1)").arg(d->elapsetm));
}