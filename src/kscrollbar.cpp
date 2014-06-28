#include "stdafx.h"
#include "kscrollbar.h"

#include "kframeitem.h"
#include "kpushbutton.h"
#include "kimageitem.h"
#include "kicon.h"
#include "kxmlui.h"

KX_WIDGET_CREATOR_GLOBAL_STATIC(KScrollBar)

/*!
 */
KScrollBar::KScrollBar(QGraphicsItem *parent)
    : KWidget(parent)
	, m_value(0)
	, m_minimum(0)
	, m_maximum(1)
	, m_deltaFactor(1.0)
	, m_singleStep(1)
	, m_pageStep(1)
	, m_orientation(Qt::Horizontal)
	, m_pressedValue(0)
	, m_hoveredControl(QStyle::SC_None)
	, m_pressedControl(QStyle::SC_None)
	, m_initialDelay(250)
	, m_repeatFrequency(50)
	, m_buttonHeight(14)
	, m_frameThickness(15)
	, m_sliderMinimumHeight(16)
	, m_ptThumbIconOffset(0,0)
	, m_bShowFullOnHover(false)
{
    setAcceptHoverEvents(true);
	setAcceptedMouseButtons(Qt::LeftButton);

	m_lineAddButton = new KPushButton(this);
	m_lineAddButton->setZValue(1);
	QObject::connect(m_lineAddButton, SIGNAL(clicked()), this, SLOT(on_linebutton_clicked()));
	addChild(m_lineAddButton);
	m_lineSubButton = new KPushButton(this);
	m_lineSubButton->setZValue(1);
	QObject::connect(m_lineSubButton, SIGNAL(clicked()), this, SLOT(on_linebutton_clicked()));
	addChild(m_lineSubButton);
	m_thumb = new KFrameItem(this);
	m_thumb->setFrameType(KFrameItem::GridFour);
	m_thumb->setZValue(1000);
	addChild(m_thumb);
	m_thumbIcon = new KIcon(this);
	m_thumbIcon->setZValue(2000);
	addChild(m_thumbIcon);
	m_background = new KFrameItem(this);
	m_background->setFrameType(KFrameItem::GridFour);
	m_background->setZValue(0);
	addChild(m_background);

	QObject::connect(this, SIGNAL(valueChanged(qreal)), this, SLOT(on_value_changed(qreal)));

	setOrientation(Qt::Vertical);

	displayFullOnHover(m_bShowFullOnHover ? false : true);
}

/*!
 */
KScrollBar::~KScrollBar()
{
}

/*!
  Returns the value of the scrollbar.
  \sa setValue()
 */
qreal KScrollBar::value() const
{
    return m_value;
}

/*!
  Sets the value of the scrollbar to be the given \a value.
  \sa value()
 */
void KScrollBar::setValue(qreal value)
{
    value = qBound(m_minimum, value, m_maximum);
    if (value != m_value) 
	{
        m_value = value;
        emit valueChanged(value);
    }
}

/*!
  Returns the minimum value of the scrollbar.
  \sa setMinimum()
 */
qreal KScrollBar::minimum() const
{
    return m_minimum;
}

/*!
  Sets the minimum value of the scrollbar to the given \a minimum.
  \sa minimum()
 */
void KScrollBar::setMinimum(qreal minimum)
{
    if (minimum != m_minimum) 
	{
        m_minimum = minimum;
        emit minimumChanged(minimum);
        setValue(m_value);
		updateSlider();
    }
}

/*!
  Returns the maximum value of the scrollbar.
  \sa setMaximum()
 */
qreal KScrollBar::maximum() const
{
    return m_maximum;
}

/*!
  Sets the maximum value of the scrollbar to the given \a maximum.
  \sa maximum()
 */
void KScrollBar::setMaximum(qreal maximum)
{
    if (maximum != m_maximum) 
	{
        m_maximum = maximum;
        emit maximumChanged(maximum);
        setValue(m_value);
		updateSlider();
    }
}

/*!
  Returns the single step of the scrollbar.
 */
qreal KScrollBar::singleStep() const
{
    return m_singleStep;
}

/*!
  Sets the single step of the scrollbar to the given \a step.
  \sa singleStep()
 */
void KScrollBar::setSingleStep(qreal step)
{
    m_singleStep = step;
	updateSlider();
}

/*!
  Returns the page step of the scrollbar.
  \sa setPageStep()
 */
qreal KScrollBar::pageStep() const
{
    return m_pageStep;
}

/*!
  Sets the page step of the scrollbar to be the given \a step.
  \sa setPageStep
 */
void KScrollBar::setPageStep(qreal step)
{
    m_pageStep = step;
	updateSlider();
}

/*!
  Returns the orientation of the scrollbar.
  \sa setOrientation()
 */
Qt::Orientation KScrollBar::orientation() const
{
    return m_orientation;
}

/*!
  Sets the orientation of the scrollbar to the given \a orientation.
  \sa orientation()
 */
void KScrollBar::setOrientation(Qt::Orientation orientation)
{
	if(orientation == m_orientation)
		return;
    m_orientation = orientation;
	if(orientation == Qt::Vertical)
	{
		setLineAddImage(":/image/theme/vscrollbar_lineadd_normal.png|:/image/theme/vscrollbar_lineadd_hover.png|:/image/theme/vscrollbar_lineadd_down.png");
		setLineSubImage(":/image/theme/vscrollbar_linesub_normal.png|:/image/theme/vscrollbar_linesub_hover.png|:/image/theme/vscrollbar_linesub_down.png");
		setBackgroundImage(":/image/theme/vscrollbar_bkg.png");
		setThumbImage(":/image/theme/vscrollbar_thumb_normal.png|:/image/theme/vscrollbar_thumb_hover.png|:/image/theme/vscrollbar_thumb_down.png");
		setThumbIcon(":/image/theme/vscrollbar_thumbico.png");
		setThumbIconOffset(QPointF(-1,1));
	}
	else
	{
		setLineAddImage(":/image/theme/hscrollbar_lineadd_normal.png|:/image/theme/hscrollbar_lineadd_hover.png|:/image/theme/hscrollbar_lineadd_down.png");
		setLineSubImage(":/image/theme/hscrollbar_linesub_normal.png|:/image/theme/hscrollbar_linesub_hover.png|:/image/theme/hscrollbar_linesub_down.png");
		setBackgroundImage(":/image/theme/hscrollbar_bkg.png");
		setThumbImage(":/image/theme/hscrollbar_thumb_normal.png|:/image/theme/hscrollbar_thumb_hover.png|:/image/theme/hscrollbar_thumb_down.png");
		setThumbIcon(":/image/theme/hscrollbar_thumbico.png");
		setThumbIconOffset(QPointF(1,-1));
	}
}

/*!
  \reimp
 */
QSizeF KScrollBar::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    if (which == Qt::MinimumSize || which == Qt::PreferredSize) 
	{
        QSize contents;
        if (orientation() == Qt::Vertical)
            contents = QSize(m_frameThickness, m_buttonHeight * 2 + m_sliderMinimumHeight);
        else
            contents = QSize(m_buttonHeight * 2 + m_sliderMinimumHeight, m_frameThickness);
        
        return contents;
    }
    return QGraphicsWidget::sizeHint(which, constraint);
}

/*!
 */
void KScrollBar::increase()
{
    setValue(value() + singleStep());
}

/*!
 */
void KScrollBar::decrease()
{
    setValue(value() - singleStep());
}

/*!
  \reimp
 */
void KScrollBar::timerEvent(QTimerEvent *event)
{
    if (m_initialDelayTimer.timerId() == event->timerId()) 
	{
        m_repeatActionTimer.start(m_repeatFrequency, this);
    } 
	else if (m_repeatActionTimer.timerId() == event->timerId()) 
	{
        performSubControlAction(m_pressedControl);
    }
	else if(m_timer.timerId() == event->timerId())
	{
		m_timer.stop();
		updateSliderPosition();
	}
}

/*!
  \reimp
 */
void KScrollBar::keyPressEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
}

/*!
  \reimp
 */
void KScrollBar::keyReleaseEvent(QKeyEvent *event)
{
    Q_UNUSED(event);
}

/*!
  \reimp
 */
void KScrollBar::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QStyle::SubControl control = hitTestComplexControl(event->pos().toPoint());
    m_pressedControl = control;
	if(control == QStyle::SC_ScrollBarAddPage  || control == QStyle::SC_ScrollBarSubPage)
	{
		int pos = m_orientation == Qt::Vertical ? event->pos().y() : event->pos().x();

		int sbextent = m_buttonHeight;
		int maxlen = ((m_orientation == Qt::Horizontal) ? width() : height()) - (sbextent * 2);
		int sliderlen = sliderPixels();
		m_tagetValue = sliderValueFromPosition(m_minimum, m_maximum, pos - sbextent, maxlen - sliderlen, false);
	}
    performSubControlAction(control);
    if ((control != QStyle::SC_ScrollBarSlider) &&
        !(m_initialDelayTimer.isActive() || m_repeatActionTimer.isActive()))
	{
		m_initialDelayTimer.start(m_initialDelay, this);
	}
	if(m_pressedControl == QStyle::SC_ScrollBarSlider)
	{
		m_thumb->setDown();
	}
}

/*!
  \reimp
 */
void KScrollBar::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_pressedValue == -1 || m_initialDelayTimer.isActive() || m_repeatActionTimer.isActive())
        return;
 
    QRect rect = subControlRect(QStyle::SC_ScrollBarGroove);
	QPointF pos = event->pos();
	QPointF downPos = event->buttonDownPos(Qt::LeftButton);
    QPointF delta = event->pos() - event->buttonDownPos(Qt::LeftButton);
    qreal range = maximum() - minimum();

    if (orientation() == Qt::Vertical) 
	{
        qreal scale = rect.height() / (range + pageStep());
        setValue((delta.y() / scale) + m_pressedValue);
    }
	else 
	{
        qreal scale = rect.width() / (range + pageStep());
        setValue((delta.x() / scale) + m_pressedValue);
    }
}

/*!
  \reimp
 */
void KScrollBar::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (m_initialDelayTimer.isActive())
        m_initialDelayTimer.stop();
    if (m_repeatActionTimer.isActive())
        m_repeatActionTimer.stop();
	if(m_pressedControl == QStyle::SC_ScrollBarSlider)
	{
		m_thumb->setNormal();
	}
	m_pressedControl = QStyle::SC_None;
}


/*!
  \reimp
 */
void KScrollBar::wheelEvent(QGraphicsSceneWheelEvent *event)
{
	qreal delta = event->delta() * m_deltaFactor;
    setValue(value() - delta / 120);
}

/*!
  \reimp
 */
void KScrollBar::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    QStyle::SubControl control = hitTestComplexControl(event->pos().toPoint());
    m_hoveredControl = control;
	if(control == QStyle::SC_ScrollBarSlider)
	{
		m_thumb->setHover();
	}

	displayFullOnHover(true);
}

/*!
  \reimp
 */
void KScrollBar::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
    QStyle::SubControl control = hitTestComplexControl(event->pos().toPoint());
    m_hoveredControl = control;
}

/*!
  \reimp
 */
void KScrollBar::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
	if(m_hoveredControl == QStyle::SC_ScrollBarSlider)
	{
		m_thumb->setNormal();
	}
    m_hoveredControl = QStyle::SC_None;
	displayFullOnHover(false);
}

int KScrollBar::sliderPositionFromValue(int min, int max, int logicalValue, int span, bool upsideDown)
{
    if (span <= 0 || logicalValue < min || max <= min)
        return 0;
    if (logicalValue > max)
        return upsideDown ? span : min;

    uint range = max - min;
    uint p = upsideDown ? max - logicalValue : logicalValue - min;

    if (range > (uint)INT_MAX/4096) {
        double dpos = (double(p))/(double(range)/span);
        return int(dpos);
    } else if (range > (uint)span) {
        return (2 * p * span + range) / (2*range);
    } else {
        uint div = span / range;
        uint mod = span % range;
        return p * div + (2 * p * mod + range) / (2 * range);
    }
    // equiv. to (p * span) / range + 0.5
    // no overflow because of this implicit assumption:
    // span <= 4096
}

/*!
    \fn int QStyle::sliderValueFromPosition(int min, int max, int position, int span, bool upsideDown)

    Converts the given pixel \a position to a logical value. 0 maps to
    the \a min parameter, \a span maps to \a max and other values are
    distributed evenly in-between.

    This function can handle the entire integer range without
    overflow.

    By default, this function assumes that the maximum value is on the
    right for horizontal items and on the bottom for vertical
    items. Set the \a upsideDown parameter to true to reverse this
    behavior.

    \sa sliderPositionFromValue()
*/

int KScrollBar::sliderValueFromPosition(int min, int max, int pos, int span, bool upsideDown)
{
    if (span <= 0 || pos <= 0)
        return upsideDown ? max : min;
    if (pos >= span)
        return upsideDown ? min : max;

    uint range = max - min;

    if ((uint)span > range) {
        int tmp = (2 * pos * range + span) / (2 * span);
        return upsideDown ? max - tmp : tmp + min;
    } else {
        uint div = range / span;
        uint mod = range % span;
        int tmp = pos * div + (2 * pos * mod + span) / (2 * span);
        return upsideDown ? max - tmp : tmp + min;
    }
    // equiv. to min + (pos*range)/span + 0.5
    // no overflow because of this implicit assumption:
    // pos <= span < sqrt(INT_MAX+0.0625)+0.25 ~ sqrt(INT_MAX)
}

QRect KScrollBar::subControlRect(QStyle::SubControl sc)
{
	QRect ret;
	const QRect scrollBarRect = QRect(QPoint(0, 0), size().toSize());
	int sbextent = m_buttonHeight;
	switch (sc)
	{
	case QStyle::SC_ScrollBarSubLine:            // top/left button
		if (m_orientation == Qt::Horizontal) 
		{
			int buttonWidth = qMin(scrollBarRect.width() / 2, sbextent);
			ret.setRect(0, 0, buttonWidth, scrollBarRect.height());
		} 
		else 
		{
			int buttonHeight = qMin(scrollBarRect.height() / 2, sbextent);
			ret.setRect(0, 0, scrollBarRect.width(), buttonHeight);
		}
		return ret;
	case QStyle::SC_ScrollBarAddLine:            // bottom/right button
		if (m_orientation == Qt::Horizontal) 
		{
			int buttonWidth = qMin(scrollBarRect.width()/2, sbextent);
			ret.setRect(scrollBarRect.width() - buttonWidth, 0, buttonWidth, scrollBarRect.height());
		} 
		else 
		{
			int buttonHeight = qMin(scrollBarRect.height()/2, sbextent);
			ret.setRect(0, scrollBarRect.height() - buttonHeight, scrollBarRect.width(), buttonHeight);
		}
		return ret;
	}
	int maxlen = ((m_orientation == Qt::Horizontal) ? width() : height()) - (sbextent * 2);
	int sliderlen = sliderPixels();
	int sliderstart = sbextent + sliderPositionFromValue(m_minimum, m_maximum, m_value, maxlen - sliderlen, false);
	switch (sc)
	{
	case QStyle::SC_ScrollBarSubPage:            // between top/left button and slider
		if (m_orientation == Qt::Horizontal)
			ret.setRect(sbextent, 0, sliderstart - sbextent, scrollBarRect.height());
		else
			ret.setRect(0, sbextent, scrollBarRect.width(), sliderstart - sbextent);
		break;
	case QStyle::SC_ScrollBarAddPage:            // between bottom/right button and slider
		if (m_orientation == Qt::Horizontal)
			ret.setRect(sliderstart + sliderlen, 0, maxlen - sliderstart - sliderlen + sbextent, scrollBarRect.height());
		else
			ret.setRect(0, sliderstart + sliderlen, scrollBarRect.width(), maxlen - sliderstart - sliderlen + sbextent);
		break;
	case QStyle::SC_ScrollBarGroove:
		if (m_orientation == Qt::Horizontal)
			ret.setRect(sbextent, 0, scrollBarRect.width() - sbextent * 2, scrollBarRect.height());
		else
			ret.setRect(0, sbextent, scrollBarRect.width(), scrollBarRect.height() - sbextent * 2);
		break;
	case QStyle::SC_ScrollBarSlider:
		if (m_orientation == Qt::Horizontal)
			ret.setRect(sliderstart, 0, sliderlen, scrollBarRect.height());
		else
			ret.setRect(0, sliderstart, scrollBarRect.width(), sliderlen);
		break;
	default:
		break;
	}
	return ret;
}

QStyle::SubControl KScrollBar::hitTestComplexControl(const QPoint &pt)
{
	QStyle::SubControl sc = QStyle::SC_None;
	QRect r;
	uint ctrl = QStyle::SC_ScrollBarAddLine;
	while (ctrl <= QStyle::SC_ScrollBarGroove) 
	{
		r = subControlRect(QStyle::SubControl(ctrl));
		if (r.isValid() && r.contains(pt)) 
		{
			sc = QStyle::SubControl(ctrl);
			break;
		}
		ctrl <<= 1;
	}
	return sc;
}

void KScrollBar::performSubControlAction( QStyle::SubControl control )
{
	switch (control) 
	{
	case QStyle::SC_ScrollBarSubLine:
		setValue(m_value - m_singleStep);
		break;
	case QStyle::SC_ScrollBarAddLine:
		setValue(m_value + m_singleStep);
		break;
	case QStyle::SC_ScrollBarSubPage:
		{
			qreal value = m_value - m_pageStep;
			if(value < m_tagetValue)
			{
				value = m_tagetValue;
				m_pressedControl = QStyle::SC_None;
			}
			setValue(m_value - m_pageStep);
		}
		break;
	case QStyle::SC_ScrollBarAddPage:
		{
			qreal value = m_value + m_pageStep;
			if(value > m_tagetValue)
			{
				value = m_tagetValue;
				m_pressedControl = QStyle::SC_None;
			}
			setValue(value);
		}
		break;
	case QStyle::SC_ScrollBarSlider:
		m_pressedValue = m_value;
		break;
	default:
		m_pressedValue = -1;
		return;
	}
}

int KScrollBar::sliderPixels()
{
	int sliderlen = 0;
	int sbextent = m_buttonHeight;
	int maxlen = ((m_orientation == Qt::Horizontal) ? width() : height()) - (sbextent * 2);

	if (m_maximum != m_minimum) 
	{
		uint range = m_maximum - m_minimum;
		sliderlen = (qint64(m_pageStep) * maxlen) / (range + m_pageStep);

		int slidermin = m_sliderMinimumHeight;
		if (sliderlen < slidermin || range > INT_MAX / 2)
			sliderlen = slidermin;
		if (sliderlen > maxlen)
			sliderlen = maxlen;
	} 
	else 
	{
		sliderlen = maxlen;
	}
	return sliderlen;
}

qreal KScrollBar::buttonHeight()
{
	return m_buttonHeight;
}

void KScrollBar::setButtonHeight( qreal height )
{
	if(m_buttonHeight == height)
		return;
	m_buttonHeight = height;
	resetLayout();
}

qreal KScrollBar::sliderMinimumHeight()
{
	return m_sliderMinimumHeight;
}

void KScrollBar::setSliderMinimumHeight( qreal height )
{
	if(m_sliderMinimumHeight == height)
		return;
	m_sliderMinimumHeight = height;
	resetLayout();
}

void KScrollBar::resetLayout()
{
	QRect ctrlRt;
	ctrlRt = subControlRect(QStyle::SC_ScrollBarAddLine);
	if(ctrlRt != m_lineAddButton->geometry())
	{
		m_lineAddButton->setGeometry(ctrlRt);
	}
	ctrlRt = subControlRect(QStyle::SC_ScrollBarSubLine);
	if(ctrlRt != m_lineSubButton->geometry())
	{
		m_lineSubButton->setGeometry(ctrlRt);
	}
	ctrlRt = subControlRect(QStyle::SC_ScrollBarSlider);
	if(ctrlRt != m_thumb->geometry())
	{
		m_thumb->setGeometry(ctrlRt);
		QSizeF s = m_thumbIcon->actualSize(ctrlRt.size());
		QPointF lt = ctrlRt.center();
		QRectF iconRt(lt.x() - s.width() / 2, lt.y() - s.height() / 2, s.width(), s.height());
		iconRt.translate(m_ptThumbIconOffset);
		m_thumbIcon->setGeometry(iconRt);
	}
	ctrlRt = rect().toRect();
	if(ctrlRt != m_background->geometry())
	{
		m_background->setGeometry(rect());
	}
}

void KScrollBar::resizeEvent( QGraphicsSceneResizeEvent *event )
{
	__super::resizeEvent(event);
	resetLayout();
}

QString KScrollBar::lineAddImage() const
{
	return m_lineAddButton->imagePath();
}

void KScrollBar::setLineAddImage( const QString& file )
{
	m_lineAddButton->setImagePath(file);
}

QString KScrollBar::lineSubImage() const
{
	return m_lineSubButton->imagePath();
}

void KScrollBar::setLineSubImage( const QString& file )
{
	m_lineSubButton->setImagePath(file);
}

QString KScrollBar::thumbImage() const
{
	return m_thumb->imagePath();
}

void KScrollBar::setThumbImage( const QString& file )
{
	m_thumb->setImagePath(file);
}

QString KScrollBar::backgroundImage() const
{
	return m_background->imagePath();
}

void KScrollBar::setBackgroundImage( const QString& file )
{
	m_background->setImagePath(file);
}

void KScrollBar::on_linebutton_clicked()
{
	if(m_lineAddButton == sender())
	{
		performSubControlAction(QStyle::SC_ScrollBarAddLine);
	}
	else
	{
		performSubControlAction(QStyle::SC_ScrollBarSubLine);
	}
}

void KScrollBar::on_value_changed(qreal value)
{
	//resetLayout();
	updateSlider();
}

void KScrollBar::updateSlider()
{
	//m_timer.start(30, this);
	updateSliderPosition();
}

void KScrollBar::setRange( qreal min, qreal max )
{
	bool bChanged = false;
	if(m_minimum != min)
	{
		m_minimum = min;
		emit minimumChanged(min);
		bChanged = true;
	}
	if(m_maximum != max)
	{
		m_maximum = max;
		emit minimumChanged(max);
		bChanged = true;
	}
	if(bChanged)
	{
		setValue(m_value);
		updateSlider();
	}
}

qreal KScrollBar::frameThickness() const
{
	return m_frameThickness;
}

void KScrollBar::setFrameThickness( qreal val )
{
	if(m_frameThickness != val)
	{
		m_frameThickness = val;
		emit thicknessChanged();
	}
}

QObject * KScrollBar::lineAdd()
{
	return m_lineAddButton;
}

QObject * KScrollBar::lineSub()
{
	return m_lineSubButton;
}

void KScrollBar::updateSliderPosition()
{
	QRect ctrlRt = subControlRect(QStyle::SC_ScrollBarSlider);
	if(ctrlRt != m_thumb->geometry())
	{
		m_thumb->setGeometry(ctrlRt);
		QSizeF s = m_thumbIcon->actualSize(ctrlRt.size());
		QPointF lt = ctrlRt.center();
		QRectF iconRt(lt.x() - s.width() / 2, lt.y() - s.height() / 2, s.width(), s.height());
		iconRt.translate(m_ptThumbIconOffset);
		m_thumbIcon->setGeometry(iconRt);
	}
}

QString KScrollBar::thumbIcon() const
{
	return m_thumbIcon->iconPath();
}

void KScrollBar::setThumbIcon( const QString& ico )
{
	m_thumbIcon->setIconPath(ico);
}

QPointF KScrollBar::thumbIconOffset() const
{
	return m_ptThumbIconOffset;
}

void KScrollBar::setThumbIconOffset( const QPointF& pt )
{
	m_ptThumbIconOffset = pt;
}

bool KScrollBar::showFullOnHover()
{
	return m_bShowFullOnHover;
}

void KScrollBar::setShowFullOnHover( bool on )
{
	m_bShowFullOnHover = on;
}

void KScrollBar::displayFullOnHover( bool isHover )
{
	if(!m_bShowFullOnHover)
		return ;
	if(isHover)
	{
		m_background->show();
		m_lineAddButton->show();
		m_lineSubButton->show();
	}
	else
	{
		m_background->hide();
		m_lineAddButton->hide();
		m_lineSubButton->hide();
	}
}

QObject * KScrollBar::sliderIcon()
{
	return m_thumbIcon;
}

QObject * KScrollBar::slider()
{
	return m_thumb;
}

qreal KScrollBar::deltaFactor() const
{
	return m_deltaFactor;
}

void KScrollBar::setDeltaFactor( qreal v )
{
	m_deltaFactor = v;
}

void KScrollBar::handleMouseWheelEvent( QGraphicsSceneWheelEvent *event )
{
	wheelEvent(event);
}
