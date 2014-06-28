#pragma once;

#include "kpopupwindow.h"

class KFrameWindowPrivate;

class KXFW_API KFrameWindow : public KPopupWindow
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KFrameWindow)
public:
	Q_PROPERTY(QObject *titlebar READ titlebar)
public:
	explicit KFrameWindow(QWidget *parent=NULL);
	~KFrameWindow();

	QObject *titlebar() const;
	KWidget * rootWidget();
protected:
	explicit KFrameWindow( KPopupWindowPrivate &dd, QWidget *parent /* = NULL*/ );

private:
	void init();
};