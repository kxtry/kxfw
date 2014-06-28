#pragma once;

#include "klistwidget.h"

class KCommonListWidgetPrivate;

class KXFW_API KCommonListWidget : public KListWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KCommonListWidget)
	Q_PROPERTY(QString itemsText READ itemsText WRITE setItemsText)
public:
	explicit KCommonListWidget(QGraphicsItem *parent=NULL);
	~KCommonListWidget();

	void addItem(const QString& text);
	void addItem(const QString& text, const QVariant& other);
	void addItem(const QString& icon, const QString& text, const QVariant& other);
	void insertItem(int index, const QString& icon, const QString& text, const QVariant& other);
	void removeItem(const QString& text);
	void removeItem(int index);
	void removeAll();

	QString itemsText() const;
	void setItemsText(const QString& items);
};