#pragma once;

#include "kwidget.h"

class KRadioButton;
class KButtonGroupPrivate;

class KXFW_API KButtonGroup : public KWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(KButtonGroup)
	Q_DECLARE_PRIVATE(KButtonGroup)
public:
	explicit KButtonGroup(QGraphicsItem *parent = NULL);
	~KButtonGroup();

signals:
	void radioSelected(QObject* button);

private slots:
	void on_button_state_changed();

public:
	virtual void construct();
	virtual void addItem(KWidget *item);
	virtual void removeItem(KWidget *item);
	virtual void childEvent(QChildEvent * event);
private:
	void addRadioButton(KWidget* widget);	
};