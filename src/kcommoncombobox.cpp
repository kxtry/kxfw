#include "stdafx.h"
#include "kcommoncombobox.h"
#include "kwidget_p.h"
#include "kcomboboxwidget_p.h"
#include "kxmlui.h"
#include "kcommonlistwidget.h"
#include "klabel.h"


KX_WIDGET_CREATOR_GLOBAL_STATIC(KCommonComboBox)

#define ASSIST_DATA_NAME		(1000)

class KCommonComboBoxItem : public KListItem
{
public:
	QPointer<KLabel> m_text;
public:
	explicit KCommonComboBoxItem(KListWidget *view)
		: KListItem(view)
	{
		setLayoutType(KWidget::HBox);
		m_text = new KLabel(this);
		addItem(m_text);
	}

	~KCommonComboBoxItem()
	{

	}

	void itemReset()
	{
		__super::itemReset();
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

class KCommonComboBoxPrivate : public KComboBoxWidgetPrivate
{
	Q_DECLARE_PUBLIC(KCommonComboBox)
public:
	KCommonComboBoxPrivate()
	{
		label = NULL;
	}
	KLabel *label;
};


KCommonComboBox::KCommonComboBox( QGraphicsItem *parent /*= NULL*/ )
: KComboBoxWidget(*new KCommonComboBoxPrivate(), parent)
{
	Q_D(KCommonComboBox);

	KListWidget *pList = qobject_cast<KListWidget*>(dropList());
	
	pList->setItemCreator(new KListItemCreatorT<KCommonComboBoxItem>());
	setupContent();

	setTextAlignment(Qt::AlignCenter);
}

KCommonComboBox::~KCommonComboBox()
{
	cleanupContent();
}

void KCommonComboBox::createContent( KWidget* contentRoot )
{
	Q_D(KCommonComboBox);
	contentRoot->setLayoutType(HBox);
	d->label = new KLabel(contentRoot);
	contentRoot->addItem(d->label);
}

void KCommonComboBox::updateContent( KWidget* contentRoot, int index )
{
	Q_D(KCommonComboBox);

	QString txt = itemText(index);
	d->label->setText(txt);
}

void KCommonComboBox::deleteContent( KWidget* contentRoot )
{
	Q_D(KCommonComboBox);
	delete d->label;
}

Qt::Alignment KCommonComboBox::textAlignment() const
{
	Q_D(const KCommonComboBox);
	KCommonListWidget *listView = d->dropList;
	QVariant var = listView->data(ASSIST_DATA_NAME);
	quint32 val = var.value<quint32>();
	return Qt::Alignment(val);
}

void KCommonComboBox::setTextAlignment( Qt::Alignment alignment )
{
	Q_D(KCommonComboBox);
	KCommonListWidget *listView = d->dropList;
	static_cast<QGraphicsItem*>(listView)->setData(ASSIST_DATA_NAME, quint32(alignment));
}

void KCommonComboBox::clearContent( KWidget* contentRoot )
{
	Q_D(KCommonComboBox);
	d->label->setText("");
}