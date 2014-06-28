#pragma once;

#include "kframeitem.h"

class KTabWidgetPrivate;

class KXFW_API KTabItem: public KFrameItem
{
	Q_OBJECT
	Q_PROPERTY(QString imagePath READ imagePath WRITE setImagePath)
public:
	explicit KTabItem(QGraphicsItem *parent=NULL);
	~KTabItem();

	void setImagePath(const QString& path);
};

class KXFW_API KTabWidget : public KWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KTabWidget)
	Q_PROPERTY(int index READ selectIndex WRITE setSelectIndex)
public:
	explicit KTabWidget(QGraphicsItem *parent=NULL);
	~KTabWidget();
	
	int selectIndex();
	void setSelectIndex(int index);
	int indexOfItem(KTabItem *tab);
	KTabItem *itemOfIndex(int idx);
	int itemCount();

signals:
	void tabSelected(KTabItem* tab);
	void indexSelected(int index);

public:
	virtual void construct();
	virtual void addItem(KWidget *item);
	virtual void removeItem(KWidget *item);
	virtual void childEvent(QChildEvent * event);

private slots:
	void on_tab_state_changed();

private:
	void addTabItem(KTabItem* widget);	
	void doTabSelected(KTabItem* tab);
};