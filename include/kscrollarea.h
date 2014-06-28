#pragma once;

#include "kwidget.h"
class KScrollBar;
class KSpacer;

class KXFW_API KScrollArea : public KWidget
{
	Q_OBJECT
public:
	Q_PROPERTY(QPoint scrollPos READ scrollPos WRITE scrollTo)
	Q_PROPERTY(bool widgetResizable READ widgetResizable WRITE setWidgetResizable)
	Q_PROPERTY(bool contentUnderBar READ contentUnderBar WRITE setContentUnderBar)
	Q_PROPERTY(qreal barOpacity READ barOpacity WRITE setBarOpacity)
	Q_PROPERTY(Qt::ScrollBarPolicy hbarPolicy READ hscrollBarPolicy WRITE setHScrollBarPolicy)
	Q_PROPERTY(Qt::ScrollBarPolicy vbarPolicy READ vscrollBarPolicy WRITE setVScrollBarPolicy)
	Q_PROPERTY(int barThickness READ barThickness WRITE setBarThickness)
	Q_PROPERTY(bool dragContent READ dragContent WRITE setDragContent)
	Q_PROPERTY(QObject* vscroll READ vScrollBar)
	Q_PROPERTY(QObject* hscroll READ hScrollBar)
public:
	explicit KScrollArea(QGraphicsItem *parent = NULL);
	~KScrollArea();
	
	void scrollTo(int x, int y);
	void scrollTo(const QPoint val);
	QPoint scrollPos() const;
	int hscrollPos() const;
	int vscrollPos() const;
	
	void setWidgetResizable(bool bResize);
	bool widgetResizable();

	bool contentUnderBar() const;
	void setContentUnderBar(bool on);

	bool dragContent() const;
	void setDragContent(bool on);

	qreal barOpacity() const;
	void setBarOpacity(qreal o);

	Q_INVOKABLE QObject* setContentWidget(QObject* w);
	Q_INVOKABLE QObject* contentWidget();

	KWidget* viewport();

	QObject *hScrollBar();
	QObject *vScrollBar();

	void setScrollBarPolicy(Qt::Orientation orientation, Qt::ScrollBarPolicy policy);
	Qt::ScrollBarPolicy scrollBarPolicy(Qt::Orientation orientation);

	void setHScrollBarPolicy(Qt::ScrollBarPolicy policy);
	Qt::ScrollBarPolicy hscrollBarPolicy() const;

	void setVScrollBarPolicy(Qt::ScrollBarPolicy policy);
	Qt::ScrollBarPolicy vscrollBarPolicy() const;

	int barThickness() const;
	void setBarThickness(int iPixel);

	void scrollToEnd();
	void scrollToStart();
public slots:
	void updateScroll();
	void on_vscroll_value_changed(qreal val);
	void on_hscroll_value_changed(qreal val);
protected:
	virtual void resizeEvent(QGraphicsSceneResizeEvent *e);
	virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);
	virtual void mouseMoveEvent(QGraphicsSceneMouseEvent * event);
	virtual void wheelEvent(QGraphicsSceneWheelEvent * e);
private:
	void updateScrollArea();
	void resizeContent();
	bool needHScrollBar();
	bool needVScrollBar();

	Qt::ScrollBarPolicy _vscrollPolicy();
	Qt::ScrollBarPolicy _hscrollPolicy();
protected:
	KScrollBar *m_hbar;
	KScrollBar *m_vbar;
	KSpacer *m_viewport;
	KWidget *m_content;
	Qt::ScrollBarPolicy m_hbarPolicy;
	Qt::ScrollBarPolicy m_vbarPolicy;
	bool m_bContentUnderBar;
	bool m_bDragContent;
	int m_barThickness;
	QSizeF m_contentSize;
	bool m_bWidgetResizable;
};


class KXFW_API KScrollWidget : public KScrollArea
{
	Q_OBJECT
public:
	explicit KScrollWidget(QGraphicsItem *parent=NULL);
	~KScrollWidget();
protected:
	void addItem(KWidget *item);
	void removeItem(KWidget *item);
	KWidget::LayoutType layoutType();
	void setLayoutType(KWidget::LayoutType type);
	void resizeEvent(QGraphicsSceneResizeEvent *e);
	void timerEvent(QTimerEvent *t);
protected:
	KWidget *m_content;
	QBasicTimer m_timer;
};

