
#pragma once

#include "kwindow.h"

class KWindowPrivate
{
	Q_DECLARE_PUBLIC(KWindow)
public:
	KWindowPrivate();
	virtual ~KWindowPrivate();

	void init();
	void setFont(const QFont& f);

protected:
	KWindow *q_ptr;
public:
	QGraphicsScene *scene;
	KSpacer *root;
	QMargins sceneMargin;
	bool m_bCancelClose;
	QTimer m_timerUpdate;//这个用于修改QWidget的更新BUG。
	bool m_isClosing;
	bool m_bStopMyCloseExecution;
};
