#pragma once;

#include "kcomboboxwidget.h"

class KFontComboBoxPrivate;

class KXFW_API KFontComboBox : public KComboBoxWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KFontComboBox)
public:
	explicit KFontComboBox(QGraphicsItem *parent = NULL);
	~KFontComboBox();

protected:
	virtual void createContent(KWidget* contentRoot);
	virtual void updateContent(KWidget* contentRoot, int index);
	virtual void deleteContent(KWidget* contentRoot);
	virtual void clearContent(KWidget* contentRoot);
};