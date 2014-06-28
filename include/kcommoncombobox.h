#pragma once;

#include "kcomboboxwidget.h"

class KCommonComboBoxPrivate;

class KXFW_API KCommonComboBox : public KComboBoxWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KCommonComboBox)
	Q_PROPERTY(Qt::Alignment textAlignment READ textAlignment WRITE setTextAlignment)

public:
	explicit KCommonComboBox(QGraphicsItem *parent = NULL);
	~KCommonComboBox();

	Qt::Alignment textAlignment() const;
	void setTextAlignment(Qt::Alignment alignment);

protected:
	virtual void createContent(KWidget* contentRoot);
	virtual void updateContent(KWidget* contentRoot, int index);
	virtual void deleteContent(KWidget* contentRoot);
	virtual void clearContent(KWidget* contentRoot);
};