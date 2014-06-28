#pragma once;

#include "kcheckbutton.h"

class KXFW_API KRadioButton : public KCheckButton
{
	Q_OBJECT
public:
	explicit KRadioButton(QGraphicsItem *parent = NULL);
	~KRadioButton();
};