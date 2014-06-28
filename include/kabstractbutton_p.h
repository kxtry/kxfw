
#pragma once

#include "kwidget_p.h"

class KAbstractButtonPrivate : public KWidgetPrivate
{
	Q_DECLARE_PUBLIC(KAbstractButton)
public:
	KAbstractButtonPrivate(KAbstractButton::ButtonType type);
	void init();
	const KAbstractButton::ButtonType typeButton;
	KAbstractButton::ButtonStates statesButton;
};
