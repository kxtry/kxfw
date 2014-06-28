#pragma once;

#include "kwidget_p.h"

class KFrameItem;
class KFollowWindow;
class KSpacer;
class KIcon;
class KCommonListWidget;

class KXFW_API KComboBoxWidgetPrivate : public KWidgetPrivate
{
	Q_DECLARE_PUBLIC(KComboBoxWidget)
public:
	KComboBoxWidgetPrivate();

	QPointer<KSpacer> content;
	QPointer<KCommonListWidget> dropList;
	QPointer<KFollowWindow> dropListView;
	QPointer<KFrameItem> frame;
	QPointer<KFrameItem> dropButton;
	QPointer<KIcon> buttonIcon;
	int dropListMaxHeight;
	int dropListMinWidth;
	QMargins listItemMargins;
};
