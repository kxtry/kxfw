#pragma once;

#include "kwidget.h"

class KFrameItem;
class KPushButton;
class KImageItem;
class KIcon;
//
class KXFW_API KScrollBar : public KWidget
{
    Q_OBJECT
	Q_DISABLE_COPY(KScrollBar)

    Q_PROPERTY(qreal value READ value WRITE setValue NOTIFY valueChanged USER true)
    Q_PROPERTY(qreal minimum READ minimum WRITE setMinimum)
    Q_PROPERTY(qreal maximum READ maximum WRITE setMaximum)
    Q_PROPERTY(qreal singleStep READ singleStep WRITE setSingleStep)
    Q_PROPERTY(qreal pageStep READ pageStep WRITE setPageStep)
	Q_PROPERTY(qreal deltaFactor READ deltaFactor WRITE setDeltaFactor)
    Q_PROPERTY(Qt::Orientation orientation READ orientation WRITE setOrientation)
	Q_PROPERTY(qreal buttonHeight READ buttonHeight WRITE setButtonHeight)
	Q_PROPERTY(qreal sliderMinimumHeight READ sliderMinimumHeight WRITE setSliderMinimumHeight)
	Q_PROPERTY(QString lineAddImage READ lineAddImage WRITE setLineAddImage)
	Q_PROPERTY(QString lineSubImage READ lineSubImage WRITE setLineSubImage)
	Q_PROPERTY(QString thumbImage READ thumbImage WRITE setThumbImage)
	Q_PROPERTY(QString thumbIcon READ thumbIcon WRITE setThumbIcon)
	Q_PROPERTY(QPointF thumbIconOffset READ thumbIconOffset WRITE setThumbIconOffset)
	Q_PROPERTY(QString backgroundImage READ backgroundImage WRITE setBackgroundImage)
	Q_PROPERTY(qreal frameThickness READ frameThickness WRITE setFrameThickness)
	Q_PROPERTY(bool showFullOnHover READ showFullOnHover WRITE setShowFullOnHover)
	Q_PROPERTY(QObject *lineAdd READ lineAdd)
	Q_PROPERTY(QObject *lineSub READ lineSub)
	Q_PROPERTY(QObject *icon READ sliderIcon)
	Q_PROPERTY(QObject *slider READ slider)
public:
	explicit KScrollBar(QGraphicsItem *parent = 0);
    virtual ~KScrollBar();

    qreal value() const;
    
    qreal minimum() const;
    void setMinimum(qreal minimum);

    qreal maximum() const;
    void setMaximum(qreal maximum);

	void setRange(qreal min, qreal max);
    
    qreal singleStep() const;
    void setSingleStep(qreal step);

    qreal pageStep() const;
    void setPageStep(qreal step);

	qreal deltaFactor() const;
	void setDeltaFactor(qreal v);

    Qt::Orientation orientation() const;
    void setOrientation(Qt::Orientation orientation);

	qreal buttonHeight();
	void setButtonHeight(qreal height);

	qreal sliderMinimumHeight();
	void setSliderMinimumHeight(qreal height);

	QString lineAddImage() const;
	void setLineAddImage(const QString& file);

	QString lineSubImage() const;
	void setLineSubImage(const QString& file);

	QString thumbImage() const;
	void setThumbImage(const QString& file);

	QString thumbIcon() const;
	void setThumbIcon(const QString& ico);

	QPointF thumbIconOffset() const;
	void setThumbIconOffset(const QPointF& pt);

	QString backgroundImage() const;
	void setBackgroundImage(const QString& file);
    
	qreal frameThickness() const;
	void setFrameThickness(qreal val);

	bool showFullOnHover();
	void setShowFullOnHover(bool on);

	QObject *lineAdd();
	QObject *lineSub();
	QObject *sliderIcon();
	QObject *slider();

public Q_SLOTS:
    void setValue(qreal value);
    void increase();
    void decrease();
	void handleMouseWheelEvent(QGraphicsSceneWheelEvent *event);
    
Q_SIGNALS:
    void valueChanged(qreal value);
    void minimumChanged(qreal minimum);
    void maximumChanged(qreal maximum);
	void thicknessChanged();

protected:
	QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint) const;
	void resizeEvent(QGraphicsSceneResizeEvent *event);
    void timerEvent(QTimerEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

protected:

private slots:
	void on_linebutton_clicked();
	void on_value_changed(qreal value);
private:
	QRect subControlRect(QStyle::SubControl sc);
	QStyle::SubControl hitTestComplexControl(const QPoint &pt);

	void performSubControlAction(QStyle::SubControl control);

	int sliderPositionFromValue(int min, int max, int val, int space,
		bool upsideDown = false);
	int sliderValueFromPosition(int min, int max, int pos, int space,
		bool upsideDown = false);

	int sliderPixels();
	void resetLayout();
	void updateSlider();
	void updateSliderPosition();
	void displayFullOnHover(bool isHover);
	
private:
	qreal m_value;
	qreal m_minimum;
	qreal m_maximum;

	qreal m_singleStep;
	qreal m_pageStep;

	Qt::Orientation m_orientation;

	qreal m_buttonHeight;
	qreal m_sliderMinimumHeight;

	qreal m_frameThickness;

	qreal m_pressedValue;
	qreal m_tagetValue;

	qreal m_deltaFactor;

	QStyle::SubControl m_hoveredControl;
	QStyle::SubControl m_pressedControl;

	QBasicTimer m_initialDelayTimer;
	QBasicTimer m_repeatActionTimer;

	int m_initialDelay;
	int m_repeatFrequency;

	QPointF m_ptThumbIconOffset;

	/*各控件的位置*/
	KFrameItem *m_background;
	KFrameItem *m_thumb;
	KIcon *m_thumbIcon;
	KPushButton *m_lineAddButton;
	KPushButton *m_lineSubButton;

	bool m_bShowFullOnHover;

	QBasicTimer m_timer;

	friend class KScrollArea;
};