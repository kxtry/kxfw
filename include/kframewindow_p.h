#pragma once;

#include "kpopupwindow_p.h"

class KTitlebar;
class KWidget;

class KXFW_API KFrameWindowPrivate : public KPopupWindowPrivate
{
public:
	KTitlebar *m_titlebar;
	KWidget *m_content;
};