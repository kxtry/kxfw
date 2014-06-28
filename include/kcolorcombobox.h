#pragma once;

#include "kcomboboxwidget.h"

class KColorComboBoxPrivate;

class KXFW_API KColorComboBox : public KComboBoxWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KColorComboBox)
	Q_PROPERTY(bool hasBorder READ hasBorder WRITE setHasBorder)
public:
	explicit KColorComboBox(QGraphicsItem *parent = NULL);
	~KColorComboBox();

	bool hasBorder();
	void setHasBorder(bool on);

	QColor itemColor(int idx);
	void setIndexByColor(const QColor& clr);
protected:
	virtual void createContent(KWidget* contentRoot);
	virtual void updateContent(KWidget* contentRoot, int index);
	virtual void deleteContent(KWidget* contentRoot);
	virtual void clearContent(KWidget* contentRoot);
};