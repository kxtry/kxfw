#include "stdafx.h"
#include "kcolorcombobox.h"
#include "kwidget_p.h"
#include "kcomboboxwidget_p.h"
#include "kxmlui.h"
#include "kcommonlistwidget.h"
#include "klabel.h"
#include "kshapeitem.h"


KX_WIDGET_CREATOR_GLOBAL_STATIC(KColorComboBox)

class KColorComboBoxItem : public KListItem
{
public:
	KShapeItem *m_color;
public:
	explicit KColorComboBoxItem(KListWidget *view)
		: KListItem(view)
	{
		setLayoutType(KWidget::HBox);
		m_color = new KShapeItem(this);
		m_color->setPenWidth(1);
		m_color->setPenStyle(Qt::SolidLine);
		m_color->setBrushStyle(Qt::SolidPattern);
		addItem(m_color);
	}

	~KColorComboBoxItem()
	{

	}

	void itemReset()
	{
		QString text = itemText();
		QColor clr(text);
		m_color->setBrushColor(clr);
		m_color->setPenColor(clr.darker());
	}
	/*״̬�仯*/
	void setSelected(const QVariant& bv)
	{
		__super::setSelected(bv);
	}

	void setUnselected(const QVariant& bv)
	{
		__super::setUnselected(bv);
	}

	void setHovered(const QVariant& bv)
	{
		__super::setHovered(bv);
	}
};

class KColorComboBoxPrivate : public KComboBoxWidgetPrivate
{
	Q_DECLARE_PUBLIC(KColorComboBox)
public:
	KColorComboBoxPrivate()
	{
	}
	QPointer<KShapeItem> m_color;
};


KColorComboBox::KColorComboBox( QGraphicsItem *parent /*= NULL*/ )
: KComboBoxWidget(*new KColorComboBoxPrivate(), parent)
{
	Q_D(KColorComboBox);

	QStringList lstCheck;
	for(int i = 0; i <= 19; i++)
	{
		Qt::GlobalColor gclr = (Qt::GlobalColor)i;
		QColor clr(gclr);
		QString name = clr.name();
		if(lstCheck.contains(name))
			continue;
		lstCheck.push_back(name);
		if(name.length())
		{
			addItem(name);
		}
	}
	setItemMargins(QMargins(2,1,2,1));
	d->dropList->setItemCreator(new KListItemCreatorT<KColorComboBoxItem>());
	setupContent();
}

KColorComboBox::~KColorComboBox()
{
	cleanupContent();
}

void KColorComboBox::createContent( KWidget* contentRoot )
{
	Q_D(KColorComboBox);

	contentRoot->setLayoutType(KWidget::HBox);
	d->m_color = new KShapeItem(contentRoot);
	d->m_color->setShapeMargins(QMargins(2,1,2,1));
	d->m_color->setPenStyle(Qt::SolidLine);
	d->m_color->setBrushStyle(Qt::SolidPattern);
	contentRoot->setMargins(QMargins(2,2,2,2));
	contentRoot->addItem(d->m_color);
}

void KColorComboBox::updateContent( KWidget* contentRoot, int index )
{
	Q_D(KColorComboBox);

	QString txt = itemText(index);
	QColor clr(txt);
	d->m_color->setBrushColor(clr);
	d->m_color->setPenColor(clr.darker());
}

void KColorComboBox::deleteContent( KWidget* contentRoot )
{
	Q_D(KColorComboBox);
	delete d->m_color;	
}

void KColorComboBox::clearContent( KWidget* contentRoot )
{
	Q_D(KColorComboBox);
	d->m_color->setBrushColor(QColor());
}

bool KColorComboBox::hasBorder()
{
	Q_D(KColorComboBox);
	if(d->m_color)
		return d->m_color->penWidth() > 0;
	return false;
}

void KColorComboBox::setHasBorder( bool on )
{
	Q_D(KColorComboBox);
	if(d->m_color == NULL)
	{
		d->m_color->setPenWidth(1);
	}
}

QColor KColorComboBox::itemColor( int idx )
{
	QString txt = itemText(idx);
	return QColor(txt);
}

void KColorComboBox::setIndexByColor( const QColor& clr )
{
	QString txt = clr.name();
	int idx = indexOfText(txt);
	if(idx != -1)
	{
		setSelectIndex(idx);
	}
}
