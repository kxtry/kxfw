#include "stdafx.h"
#include "kradiobutton.h"
#include "kxmlui.h"

KX_WIDGET_CREATOR_GLOBAL_STATIC(KRadioButton);

KRadioButton::KRadioButton( QGraphicsItem *parent ) : KCheckButton(parent)
{
	setImagePath(":/image/theme/radiobox_default_normal.png|"
		":/image/theme/radiobox_default_hover.png|"
		":/image/theme/radiobox_default_pressdown.png|"
		":/image/theme/radiobox_default_disable.png|"
		":/image/theme/radiobox_selected_normal.png|"
		":/image/theme/radiobox_selected_hover.png|"
		":/image/theme/radiobox_selected_pressdown.png|"
		":/image/theme/radiobox_selected_disable.png");
}

KRadioButton::~KRadioButton()
{

}

