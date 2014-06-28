#pragma once;

#include "kwidget.h"

/*
±³¾°Í¼ÊÇKFrameItem,Í¨¹ý×´Ì¬²»¶ÏÐÞ¸ÄÆäÍ¼Æ¬×´Ì¬¡£
*/
class KAbstractButtonPrivate;

class KXFW_API KAbstractButton : public KWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KAbstractButton)
	Q_ENUMS(ButtonState)
	Q_ENUMS(ButtonStates)
	Q_PROPERTY(bool check READ checked WRITE setChecked)
	Q_PROPERTY(bool enable READ enabled WRITE setEnabled)
public:
	enum ButtonType
	{
		CheckButton = 0,
		PushButton
	};
	enum ButtonState
	{
		StateNormal = 1,
		StateHover = 2,
		StatePressed = 4,

		StateChecked = 0x1 << 8,
		StateDisable = 0x1 << 9,
		CheckMask = 0xFF00
	};
	Q_DECLARE_FLAGS(ButtonStates, ButtonState)
public:
	KAbstractButton(QGraphicsItem *parent = NULL, ButtonType type = PushButton);
	KAbstractButton(KAbstractButtonPrivate& dd, QGraphicsItem *parent = NULL);
	~KAbstractButton();

	bool enabled() const;
	void setEnabled(bool enabled);

	bool checked() const;
	void setChecked(bool checked);

signals:
	void pressed( QPointF& pt );
	void released( QPointF& pt );
	void clicked();
	
protected:
	virtual void updateLayout() = 0;
	virtual void updateState(ButtonStates states) = 0;
private:
	void mousePressEvent ( QGraphicsSceneMouseEvent * event );
	void mouseReleaseEvent ( QGraphicsSceneMouseEvent * event );
	void hoverEnterEvent( QGraphicsSceneHoverEvent * event );
	void hoverLeaveEvent( QGraphicsSceneHoverEvent * event );
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void resizeEvent(QGraphicsSceneResizeEvent * event);
};