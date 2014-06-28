
#pragma once

#include "kwidget.h"

class KWidget;

class KXFW_API KWidgetPrivate
{
	Q_DECLARE_PUBLIC(KWidget)

public:
	KWidgetPrivate();
	virtual ~KWidgetPrivate();

	void init();

protected:
	KWidget *q_ptr;
public:
	QString toolTipText;
	Qt::ContextMenuPolicy contexMenuPolicy;
	KWidget::LayoutType layoutType;
	KWidget::DragPolicy dragPolicy;
	bool bAddToAnchor;
	bool bRemovePostEvent;
	KWidget::ThemePolicy themePolicy;
	bool bThemeCheck;
	bool bThemeResult;
	QBrush backgroundBrush;
	QGraphicsColorizeEffect * grayEffect;

	QString signalName;
	QString slotName;
};
