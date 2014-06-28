#include "stdafx.h"
#include "kfontcombobox.h"
#include "kwidget_p.h"
#include "kcomboboxwidget_p.h"
#include "kxmlui.h"
#include "kcommonlistwidget.h"
#include "klabel.h"
#include "kshapeitem.h"


KX_WIDGET_CREATOR_GLOBAL_STATIC(KFontComboBox)

#define TEXT_EXAMPLE		("  ABC-·¶Àý")
#define TEXT_FONTSIZE		(12)

class KFontComboBoxItem : public KListItem
{
public:
	KLabel *m_fontLabel;
	KLabel *m_textLabel;
public:
	explicit KFontComboBoxItem(KListWidget *view)
		: KListItem(view)
	{
		setLayoutType(KWidget::HBox);
		m_fontLabel = new KLabel(this);
		m_fontLabel->setAutoAdjust(true);
		m_textLabel = new KLabel(this);
		m_textLabel->setTextMargins(QMargins(0, 0, 0, 0));
		m_textLabel->setText(TEXT_EXAMPLE);
		addItem(m_fontLabel);
		addItem(m_textLabel);
	}

	~KFontComboBoxItem()
	{

	}

	void itemReset()
	{
		QString text = itemText();
		m_fontLabel->setText(text);
		QFont font(text,TEXT_FONTSIZE);
		m_textLabel->setFont(font);
	}
	/*×´Ì¬±ä»¯*/
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

class KFontComboBoxPrivate : public KComboBoxWidgetPrivate
{
	Q_DECLARE_PUBLIC(KFontComboBox)
public:
	KFontComboBoxPrivate()
	{
	}
	QPointer<KLabel> fontLabel;
};


KFontComboBox::KFontComboBox( QGraphicsItem *parent /*= NULL*/ )
: KComboBoxWidget(*new KFontComboBoxPrivate(), parent)
{
	Q_D(KFontComboBox);

	QFontDatabase fdb;
	QStringList familys = fdb.families();
	int max_width = 0;
	for(int i = 0; i < familys.count(); i++)
	{
		QString name = familys.at(i);
		if(name.length())
		{
			QFont font(name,TEXT_FONTSIZE);
			QFontMetrics fm(font);
			QString txt = name;
			txt += TEXT_EXAMPLE;
			int w = fm.width(txt);
			max_width = qMax(w, max_width);
			addItem(name);
		}
	}
	setDropListMinWidth(max_width);
	setItemMargins(QMargins(2,1,2,1));
	d->dropList->setItemCreator(new KListItemCreatorT<KFontComboBoxItem>());
	setupContent();
}

KFontComboBox::~KFontComboBox()
{
	cleanupContent();
}

void KFontComboBox::createContent( KWidget* contentRoot )
{
	Q_D(KFontComboBox);

	contentRoot->setLayoutType(KWidget::HBox);
	d->fontLabel = new KLabel(contentRoot);
	contentRoot->addItem(d->fontLabel);
}

void KFontComboBox::updateContent( KWidget* contentRoot, int index )
{
	Q_D(KFontComboBox);

	QString txt = itemText(index);
	d->fontLabel->setText(txt);
}

void KFontComboBox::deleteContent( KWidget* contentRoot )
{
	Q_D(KFontComboBox);
	delete d->fontLabel;	
}

void KFontComboBox::clearContent( KWidget* contentRoot )
{
	Q_D(KFontComboBox);
	d->fontLabel->setText("");
}