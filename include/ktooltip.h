#pragma once;

#include "kfollowwindow.h"

class KToolTipPrivate;

class KXFW_API KToolTip : public KFollowWindow
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KToolTip)

public:
	enum TipTrigger
	{
		CursorInTarget = 0x1,
		CursorStopAtTarget = 0x2,
	};

	enum TipStyle
	{
		SimpleText,
		CustomWidget,
	};
	
public:
	explicit KToolTip(TipStyle style = SimpleText);
	~KToolTip();
	
	/*确保w对象已经关联到QGraphicsView对像里。*/
	bool addTarget(KWidget *w);
	void removeTarget(KWidget *w);
	void removeAllTarget();

	void setTrigger(TipTrigger trigger);

	static KToolTip *textTipInstance();
private slots:
	void on_tipTimeout();

private:
	virtual bool eventFilter(QObject *obj, QEvent *e);
	virtual void resetGeometry();
};