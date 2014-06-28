#pragma once;

#include "kwindow.h"

class KFollowWindowPrivate;

/*
该类需要派生后才能使用。
*/
class KXFW_API KFollowWindow : public KWindow
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KFollowWindow)
public:
	enum Direction
	{
		TopLeft,
		TopCenter,
		TopRight,
		BottomLeft,
		BottomCenter,
		BottomRight,
		LeftTop,
		LeftCenter,
		LeftBottom,
		RightTop,
		RightCenter,
		RightBottom,

		/*TipUse*/
		UnderCursor,
		Custom,
	};

	enum ListenFlag
	{
		abortAll = 0,
		targetEvent = 0x1,
		targetScenePosition = 0x2,
		targetVisible = 0x4,
		targetWindowEvent = 0x8,

		dockFlag = targetWindowEvent | targetScenePosition | targetVisible,
		tipFlag = targetEvent | targetVisible | targetWindowEvent,
	};

	enum HideFlag
	{
		NotDefine = 0,
		ClickAtTarget = 0x1,
		ClickAtMySelf = 0x2,
		ClickAtOther = 0x4,
		TargetWindowInactive = 0x8,
		TargetWindowResize = 0x10,
		TargetWindowMoving = 0x20,
		HideAsTargetHidden = 0x40,
		MySelfWindowActiveToInactive = 0x80,

		AlwaysHide = 0xFFFFFFFF
	};

	Q_DECLARE_FLAGS(HideFlags, HideFlag)
	Q_DECLARE_FLAGS(ListenFlags, ListenFlag)

	Q_ENUMS(Direction)
	Q_FLAGS(HideFlags)

	Q_PROPERTY(Direction direction READ direction WRITE setDirection)
	Q_PROPERTY(HideFlags hideFlags READ hideFlags WRITE setHideFlags)
	Q_PROPERTY(QPointF offset READ posOffset WRITE setPosOffset)
	Q_PROPERTY(bool translucent READ translucent WRITE setTranslucent)

public:
	explicit KFollowWindow(QWidget *parent=NULL);
	~KFollowWindow();

	HideFlags hideFlags();
	void setHideFlags(HideFlags flags);

	KFollowWindow::Direction direction();
	void setDirection(KFollowWindow::Direction d);

	bool translucent() const;
	void setTranslucent(bool on);

	QPointF posOffset() const;
	void setPosOffset(const QPointF& pt);
	void setPosOffset(int x, int y);

	void setWindowSize(const QSize& s);
	void setWindowSize(int w, int h);

	void adjustGeometry();

	KWidget *target() const;

signals:
	void aboutToShow();
	void aboutToHide(KFollowWindow::HideFlag flag, bool *pbAborted);
	void aboutToAdjust(QWidget *w, bool *pbHandled);

public slots:
	void on_scenePosition(const QPointF& pt);
	void on_posChanged();
	void on_visibleChanged();


protected:
	explicit KFollowWindow( KWindowPrivate &dd, QWidget *parent /* = NULL*/ );

	virtual void resetGeometry();
	virtual void resizeEvent(QResizeEvent *event);
	virtual bool eventFilter(QObject *obj, QEvent *e);
	virtual void setTarget(KWidget *w);

protected:
	void listenEvent(KWidget *w, ListenFlags flags);
};