#pragma once;

#include "kwidget.h"
#include "kwindow.h"
#include "kfollowwindow.h"

class KComboBoxWidgetPrivate;
class KComboBoxWidget;
class KListWidget;


//去除KLineEdit控件，让其抽象化做真正的基类，而将其内部功能转化为派生类KTextComboBox进行实现。

class KXFW_API KComboBoxWidget : public KWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KComboBoxWidget)
	Q_PROPERTY(QString frameImage READ frameImage WRITE setFrameImage)
	Q_PROPERTY(QString buttonImage READ buttonImage WRITE setButtonImage)
	Q_PROPERTY(QString buttonIcon READ buttonIconPath WRITE setButtonIconPath)
	Q_PROPERTY(int buttonWidth READ buttonWidth WRITE setButtonWidth)
	Q_PROPERTY(int dropListMaxHeight READ dropListMaxHeight WRITE setDropListMaxHeight)
	Q_PROPERTY(int dropListMinWidth READ dropListMinWidth WRITE setDropListMinWidth)
	Q_PROPERTY(bool dropListShowUp READ dropListShowUp WRITE setDropListShowUp)
	Q_PROPERTY(int index READ selectIndex WRITE setSelectIndex)
	Q_PROPERTY(QMargins contentMargins READ contentMargins WRITE setContentMargins)
	Q_PROPERTY(QMargins itemMargins READ itemMargins WRITE setItemMargins)
	Q_PROPERTY(QString itemsText READ itemsText WRITE setItemsText)
	Q_PROPERTY(QObject *dropButton READ dropButton)
	Q_PROPERTY(QObject *dropList READ dropList)
	Q_PROPERTY(QObject *dropListView READ dropListView)
	Q_PROPERTY(QObject *dropIcon READ dropIcon)
public:
	explicit KComboBoxWidget(QGraphicsItem *parent = NULL);
	~KComboBoxWidget();

	int dropListMaxHeight() const;
	void setDropListMaxHeight(const int h);

	int dropListMinWidth() const;
	void setDropListMinWidth(const int w);

	QMargins itemMargins() const;
	void setItemMargins(const QMargins& m);

	int selectIndex() const;
	void setSelectIndex(int index);
	void clearSelection();

	QMargins contentMargins() const;
	void setContentMargins(const QMargins& m);

	QString frameImage() const;
	void setFrameImage(const QString& file);

	QString buttonIconPath() const;
	void setButtonIconPath(const QString& file);

	QString buttonImage() const;
	void setButtonImage(const QString& file);

	int buttonWidth();
	void setButtonWidth(int w);

	bool dropListShowUp();
	void setDropListShowUp(bool on);

	void addItem(const QString& text);
	void addItem(const QString& text, const QVariant& other);
	void addItem(const QString& icon, const QString& text, const QVariant& other);
	void insertItem(int index, const QString& icon, const QString& text, const QVariant& other);
	void removeItem(const QString& text);
	void removeItem(int index);
	void removeAll();

	int indexOfText(const QString& text);

	int listCount() const;

	QString itemsText() const;
	void setItemsText(const QString& items);

	QString itemIcon(int i) const;
	QString itemText(int i) const;
	QVariant itemOther(int i) const;

	//KCommonListWidget
	QObject *dropList() const;
	//KFrameItem
	QObject *dropButton() const;
	//KIcon
	QObject *dropIcon() const;
	//KFollowWindow
	QObject *dropListView() const;
	
signals:
	void indexChanged(int idx);

protected:
	explicit KComboBoxWidget( KComboBoxWidgetPrivate &dd, QGraphicsItem *parent /* = NULL*/ );

	void setupContent();
	virtual void createContent(KWidget* contentRoot);
	virtual void updateContent(KWidget* contentRoot, int index);
	virtual void deleteContent(KWidget* contentRoot);
	virtual void clearContent(KWidget* contentRoot);
	void cleanupContent();

	virtual void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
	virtual void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);

protected slots:
	virtual void showDropList();
	virtual void hideDropList();
	virtual void stateChanged();
	virtual void on_dropbutton_clicked();
	virtual void on_aboutToHide(KFollowWindow::HideFlag flag, bool *pbAborted);

private:
	void init();
	void resizeEvent(QGraphicsSceneResizeEvent *event);
	bool eventFilter(QObject *watched, QEvent *event);
	void _showDropList();
	void _hideDropList();

private slots:
	void on_dropList_indexChanged(int i);
	void on_dropList_itemClicked(int i,Qt::MouseButton mb);
};