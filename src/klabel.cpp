#include "stdafx.h"
#include "klabel.h"
#include "kxmlui.h"
#include "kwidget_p.h"
#include "klineedit.h"
#include "ktheme.h"

KX_WIDGET_CREATOR_GLOBAL_STATIC(KLabel)
class KLabelPrivate: public KWidgetPrivate
{
public:
	void init()
	{
		textFlags = Qt::AlignLeft|Qt::AlignVCenter;
		elideMode = Qt::ElideNone;
		bAdjustText = false;
		bAcceptClick = false;
		bUnderlineOnHover = false;
		bHandCursorOnHover = false;
		shadowEffect = NULL;
		bShadowEffect = false;
		shadowBlurRadius = 1.0;
		shadowColor = QColor(Qt::gray);
		antialiase = false;
	}
public:
	QString fullText;
	QString elideText;
	quint32 textFlags;
	QMargins textMargins;
	QColor textColor;
	Qt::TextElideMode elideMode;
	bool bAdjustText;
	bool bAcceptClick;
	bool bUnderlineOnHover;
	bool bHandCursorOnHover;
	bool bShadowEffect;
	QGraphicsDropShadowEffect *shadowEffect;

	QString linkUrl;
	QColor shadowColor;
	QPointF shadowOffset;
	qreal shadowBlurRadius;
	bool antialiase;
};

#define EDIT_MARGIN		(3)
KLabel::KLabel( QGraphicsItem* parent ) 
: KWidget(*new KLabelPrivate(), parent )
{
	d_func()->init();
}

KLabel::~KLabel(void)
{
}

QString KLabel::text() const
{
	return d_func()->fullText;
}

void KLabel::setText(const QString& text)
{
	Q_D(KLabel);

	if(d->fullText == text)
		return;

	d->fullText = text;
	updateElideText();
	update();
}

bool KLabel::autoAdjust() const
{
	return d_func()->bAdjustText;
}

void KLabel::setAutoAdjust(bool bAdjust)
{
	Q_D(KLabel);
	if(d->bAdjustText == bAdjust)
		return;
	d->bAdjustText = bAdjust;
	updateElideText();
}

void KLabel::setLinkUrl( const QString& url )
{
	d_func()->linkUrl = url;
}

QString KLabel::linkUrl() const
{
	return d_func()->linkUrl;
}

void KLabel::setTextColor( const QColor &textColor )
{
	Q_D(KLabel);
	if(d->textColor == textColor)
		return;
	d->textColor = textColor;
	update();
}

QColor KLabel::textColor( void ) const
{
	return d_func()->textColor;
}

void KLabel::setFontSize( int fs )
{
	QFont &f = font();
	f.setPixelSize(fs);
	setFont(f);
	updateElideText();
}

int KLabel::fontSize() const
{
	QFont &f = font();
	return f.pixelSize();
}

QString KLabel::fontName() const
{
	QFont &f = font();
	return f.family();
}

void KLabel::setFontName( const QString& name )
{
	QFont &f = font();
	f.setFamily(name);
	setFont(f);
	updateElideText();
}


void KLabel::setAcceptClick( bool enabled )
{
	d_func()->bAcceptClick = enabled;
	if(enabled)
	{
		setAcceptedMouseButtons(Qt::LeftButton);
	}
	else
	{
		setAcceptedMouseButtons(0);
	}
}

bool KLabel::acceptClick() const
{
	return d_func()->bAcceptClick;
}

void KLabel::setUnderlineOnHover( bool on )
{
	d_func()->bUnderlineOnHover = on;
	setAcceptHoverEvents(d_func()->bUnderlineOnHover);
}

bool KLabel::underlineOnHover() const
{
	return d_func()->bUnderlineOnHover;
}

bool KLabel::handCursorOnHover()
{
	Q_D(KLabel);
	return d->bHandCursorOnHover;
}

void KLabel::setHandCursorOnHover(bool on)
{
	Q_D(KLabel);
	d->bHandCursorOnHover = on;
}

void KLabel::setBold( bool enable )
{
	QFont &f = font();

	if(f.bold() == enable)
		return;

	f.setBold(enable);
	setFont(f);
	updateElideText();
}

bool KLabel::bold() const
{
	return font().bold();
}


bool KLabel::antialiase()
{
	Q_D(KLabel);
	return d->antialiase;
}

void KLabel::setAntialiase( bool on )
{
	Q_D(KLabel);
	if(d->antialiase == on)
		return;
	d->antialiase = on;
	update();
}

bool KLabel::dropShadow() const
{
	return d_func()->bShadowEffect;
}

void KLabel::setDropShadow( bool shadow )
{
	if(shadow)
	{
		d_func()->shadowEffect = new QGraphicsDropShadowEffect(this);
		d_func()->shadowEffect->setBlurRadius(d_func()->shadowBlurRadius);
		d_func()->shadowEffect->setColor(d_func()->shadowColor);
		d_func()->shadowEffect->setOffset(d_func()->shadowOffset);
		setGraphicsEffect(d_func()->shadowEffect);
		d_func()->bShadowEffect = true;
	}
	else
	{
		d_func()->bShadowEffect = false;
		setGraphicsEffect(NULL);
	}
}

Qt::Alignment KLabel::textAlignment() const
{
	return d_func()->textFlags & (Qt::AlignHorizontal_Mask | Qt::AlignVertical_Mask);
}

void KLabel::setTextAlignment( Qt::Alignment align )
{
	if(textAlignment() == align)
		return;
	d_func()->textFlags &= ~(Qt::AlignHorizontal_Mask | Qt::AlignVertical_Mask);
	d_func()->textFlags |= align;
	updateElideText();
}

Qt::TextElideMode KLabel::textElideMode() const
{
	return d_func()->elideMode;
}

void KLabel::setTextElidedMode(Qt::TextElideMode elide)
{
	d_func()->elideMode = elide;
	updateElideText();
}

bool KLabel::wordWrap() const
{
	return d_func()->textFlags & Qt::TextWordWrap;
}

void KLabel::setWordWrap(bool wrap)
{
	if(wrap)
	{
		d_func()->textFlags |=  Qt::TextWordWrap;
	}
	else
	{
		d_func()->textFlags &= ~Qt::TextWordWrap;
	}
	updateElideText();
}

QColor KLabel::shadowColor() const
{
	return d_func()->shadowColor;
}

void KLabel::setShadowColor( const QColor &color )
{
	d_func()->shadowColor = color;
	if(d_func()->shadowEffect)
	{
		d_func()->shadowEffect->setColor(d_func()->shadowColor);
	}
}

QPointF KLabel::shadowOffset() const
{
	return d_func()->shadowOffset;
}

void KLabel::setShadowOffset( const QPointF &offset )
{
	d_func()->shadowOffset = offset;
	if(d_func()->shadowEffect)
	{
		d_func()->shadowEffect->setOffset(d_func()->shadowOffset);
	}
}

qreal KLabel::shadowBlurRadius() const
{
	return d_func()->shadowBlurRadius;
}

void KLabel::setShadowBlurRadius( qreal blurRadius )
{
	d_func()->shadowBlurRadius = blurRadius;
	if(d_func()->shadowEffect)
	{
		d_func()->shadowEffect->setBlurRadius(d_func()->shadowBlurRadius);
	}
}

void KLabel::setEnabled( bool enabled )
{
	if(isEnabled() == enabled)
		return;
	KWidget::setEnabled(enabled);
	if(enabled)
	{
		if(d_func()->bShadowEffect)
		{
			d_func()->shadowEffect = new QGraphicsDropShadowEffect(this);
			d_func()->shadowEffect->setBlurRadius(d_func()->shadowBlurRadius);
			d_func()->shadowEffect->setColor(d_func()->shadowColor);
			d_func()->shadowEffect->setOffset(d_func()->shadowOffset);
			setGraphicsEffect(d_func()->shadowEffect);
		}
	}
	else
	{
		QGraphicsColorizeEffect *effect = new QGraphicsColorizeEffect( this );
		effect->setColor(Qt::gray);
		setGraphicsEffect( effect );
	}
}

QMargins KLabel::textMargins() const
{
	return d_func()->textMargins;
}

void KLabel::setTextMargins( const QMargins &margins )
{
	Q_D(KLabel);
	if(d->textMargins == margins)
		return;
	d->textMargins = margins;
	updateElideText();
}



void KLabel::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
	Q_D(KLabel);
	event->setAccepted(d_func()->bAcceptClick);
}

void KLabel::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
	if(d_func()->bAcceptClick)
	{
		event->accept();
		if(!d_func()->linkUrl.isEmpty())
		{
			QDesktopServices::openUrl(d_func()->linkUrl);
		}
		emit clicked();
	}
}

void KLabel::resizeEvent( QGraphicsSceneResizeEvent * event )
{
	updateElideText();
}


void KLabel::mouseDoubleClickEvent( QGraphicsSceneMouseEvent * event )
{
	Q_D(KLabel);
	event->setAccepted(d_func()->bAcceptClick);
}

void KLabel::hoverMoveEvent( QGraphicsSceneHoverEvent *event )
{
	Q_D(KLabel);
	if(d->bUnderlineOnHover)
	{
		QFont &f = font();
		f.setUnderline(d->bUnderlineOnHover);
		setFont(f);
	}
	if(d->bHandCursorOnHover)
	{
		setCursor(Qt::PointingHandCursor);
	}
}

void KLabel::hoverLeaveEvent( QGraphicsSceneHoverEvent * event )
{
	Q_D(KLabel);
	if(d->bUnderlineOnHover)
	{
		QFont &f = font();
		f.setUnderline(false);
		setFont(f);
	}
	if(d->bHandCursorOnHover)
	{
		unsetCursor();
	}
}

void KLabel::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget /* = 0 */ )
{
	Q_D(KLabel);

	QRectF drawRt = rect();
	drawRt.adjust(d->textMargins.left(), d->textMargins.top(), -d->textMargins.right(), -d->textMargins.bottom());
	painter->save();
	painter->setRenderHint(QPainter::TextAntialiasing, d->antialiase);
	painter->setFont(font());
	QColor clr = hasTheme() ? KTheme::convertToThemeColor(d->textColor) : d->textColor;
	painter->setPen(clr);
	painter->drawText(drawRt, d->textFlags|Qt::TextSingleLine, d->elideText);
	painter->restore();
}

void KLabel::updateElideText()
{
	Q_D(KLabel);

	if(d->bAdjustText)
	{
		d->elideText = d->fullText;
		QFontMetrics fm(font());

		//int w = fm.boundingRect(QRect(), Qt::TextSingleLine | Qt::TextShowMnemonic, d->elideText).width();这是菜单的控制项。
		/*
		int text_flags = Qt::AlignVCenter | Qt::TextShowMnemonic | Qt::TextDontClip | Qt::TextSingleLine;
		p->drawText( vTextRect, text_flags, s.left(t) );
		*/
		int adjustWidth = fm.width(d->elideText) + d->textMargins.left() + d->textMargins.right();
		if(parentLayout())
		{
			if(adjustWidth == preferredWidth())
				return;
			setFixWidth(adjustWidth);
		}
		else
		{
			if(width() == adjustWidth)
				return;
			resize(adjustWidth, height());
		}
	}
	else
	{
		QRectF drawRt = rect();
		drawRt.adjust(d->textMargins.left(), d->textMargins.top(), -d->textMargins.right(), -d->textMargins.bottom());
		QFontMetrics fm(font());
		if(d->textFlags & Qt::TextWordWrap)
		{
			d->elideText = d->fullText;
		}
		else if(d->textFlags & Qt::AlignLeft)
		{
			d->elideText = fm.elidedText(d->fullText, d->elideMode, drawRt.width());
		}
		else if(d->textFlags & Qt::AlignHCenter)
		{
			d->elideText = d->fullText;
		}
		else if(d->textFlags & Qt::AlignRight)
		{
			d->elideText = fm.elidedText(d->fullText, d->elideMode, drawRt.width());
		}
	}
}

void KLabel::doSkinChange()
{

}
