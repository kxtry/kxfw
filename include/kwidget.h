
#pragma once

#include <QGraphicsWidget>

class QGraphicsLayout;
struct XMLNode;

class KWidgetPrivate;

class KXFW_API KWidget : public QGraphicsWidget
{
public:
	enum DragPolicy
	{
		Nothing = 0,
		WindowMove,
		AskParent
	};

	enum LayoutType
	{
		None = 0,
		VBox = 1,
		HBox = 2,
		Flow = 3,	//Linear = VBox|HBox
		Anchor = 4
	};

	enum AnchorPoint
	{
		Left = 0,
		HCenter,
		Right,
		Top,
		VCenter,
		Bottom
	};

	//Start:0,3|Final:3,0|Color:0.5:255,233,222|Color:0.5:255,233,222|Spread:Pad
	//start:0,3|final:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Repeat
	//start:0,3|final:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Reflect
	//Center:0,3|Radius:3,0|Color:0.5:255,233,222|Color:0.5:255,233,222|Spread:Reflect	
	//center:0,3|radius:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Reflect	
	//center:0,3|Angle:3,0|color:0.5:255,233,222|color:0.5:255,233,222|spread:Reflect
	enum GradientKey
	{
		Start,
		Final,
		Center,
		Radius,
		Angle,
		Color,//Color:0.5:255,233,222
		Spread,
	};
	enum SpreadKey
	{
		Pad,
		Repeat,
		Reflect
	};

	enum WidgetFlag
	{
		Default = 0,
		HasNoContents = QGraphicsItem::ItemHasNoContents,
		ClipsChildrenToShape = QGraphicsItem::ItemClipsChildrenToShape,
		ClipsToShape = QGraphicsItem::ItemClipsToShape,
		IgnoresParentOpacity = QGraphicsItem::ItemIgnoresParentOpacity,
		Moveable = QGraphicsItem::ItemIsMovable,
		ScenePosition = QGraphicsItem::ItemSendsScenePositionChanges,

		All = HasNoContents|ClipsChildrenToShape|ClipsToShape|IgnoresParentOpacity|Moveable|ScenePosition,
	};

	enum ThemePolicy
	{
		NoTheme = 0,
		ThemeWithParent,
		HasTheme
	};

	Q_DECLARE_FLAGS(WidgetFlags, WidgetFlag)
	Q_FLAGS(WidgetFlags)


	Q_ENUMS(ThemePolicy)
	Q_ENUMS(LayoutType)
	Q_ENUMS(DragPolicy)
	Q_ENUMS(AnchorPoint)
	Q_ENUMS(GradientKey)
	Q_ENUMS(SpreadKey)

	Q_OBJECT
	Q_DECLARE_PRIVATE(KWidget)
	Q_DISABLE_COPY(KWidget)

	Q_PROPERTY(qreal width READ width)
	Q_PROPERTY(qreal height READ height)
	Q_PROPERTY(QRectF rect READ rect)
	Q_PROPERTY(qreal fixWidth READ fixWidth WRITE setFixWidth)
	Q_PROPERTY(qreal fixHeight READ fixHeight WRITE setFixHeight)
	Q_PROPERTY(QSizeF fixSize READ fixSize WRITE setFixSize)
	Q_PROPERTY(bool sizeToChild READ sizeToChild WRITE setSizeToChild)
	Q_PROPERTY(bool widthToChild READ widthToChild WRITE setWidthToChild)
	Q_PROPERTY(bool heightToChild READ heightToChild WRITE setHeightToChild)
	Q_PROPERTY(qreal minimumWidth READ minimumWidth WRITE setMinimumWidth)
	Q_PROPERTY(qreal minimumHeight READ minimumHeight WRITE setMinimumHeight)
	Q_PROPERTY(qreal preferredWidth READ preferredWidth WRITE setPreferredWidth)
	Q_PROPERTY(qreal preferredHeight READ preferredHeight WRITE setPreferredHeight)
	Q_PROPERTY(qreal maximumWidth READ maximumWidth WRITE setMaximumWidth)
	Q_PROPERTY(qreal maximumHeight READ maximumHeight WRITE setMaximumHeight)
	Q_PROPERTY(QSizeF minimumSize READ minimumSize WRITE setMinimumSize)
	Q_PROPERTY(QSizeF maximumSize READ maximumSize WRITE setMaximumSize)
	Q_PROPERTY(QCursor cursor READ cursor WRITE setCursor RESET unsetCursor)
	Q_PROPERTY(bool focus READ hasFocus)
	Q_PROPERTY(Qt::ContextMenuPolicy contextMenuPolicy READ contextMenuPolicy WRITE setContextMenuPolicy)
	Q_PROPERTY(bool acceptDrops READ acceptDrops WRITE setAcceptDrops)
	Q_PROPERTY(QString toolTip READ toolTip WRITE setToolTip)
	Q_PROPERTY(bool bylayout READ bylayout WRITE setByLayout)
	Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
	Q_PROPERTY(QtGradient backgroundGradient READ backgroundGradient WRITE setBackgroundGradient)
	Q_PROPERTY(QBrush backgroundBrush READ backgroundBrush WRITE setBackgroundBrush)
	Q_PROPERTY(Qt::BrushStyle backgroundStyle READ backgroundStyle WRITE setBackgroundStyle)
	Q_PROPERTY(bool content READ hasContent WRITE enableContent)
	Q_PROPERTY(Qt::Alignment alignment READ alignment WRITE setAlignment)
	Q_PROPERTY(DragPolicy dragPolicy READ dragPolicy WRITE setDragPolicy )
	Q_PROPERTY(WidgetFlags widgetFlags READ widgetFlags WRITE setWidgetFlags)
	Q_PROPERTY(bool itemPanel READ itemPanel WRITE setItemPanel)
	Q_PROPERTY(ThemePolicy themePolicy READ themePolicy WRITE setThemePolicy)
	Q_PROPERTY(QString xmlPath READ xmlPath WRITE setXmlPath)
	Q_PROPERTY(bool grayToDisable READ grayToDisable WRITE setGrayToDisable)
	Q_PROPERTY(qreal xRotate READ xRotateAngle WRITE setxRotateAngle)
	Q_PROPERTY(qreal yRotate READ yRotateAngle WRITE setyRotateAngle)
	Q_PROPERTY(qreal zRotate READ zRotateAngle WRITE setzRotateAngle)
	Q_PROPERTY(QPointF originPoint READ transformOriginPoint WRITE setTransformOriginPoint)

	/*布局器*/
	Q_PROPERTY(LayoutType layout READ layoutType WRITE setLayoutType)
	Q_PROPERTY(int spacing READ spacing WRITE setSpacing)
	Q_PROPERTY(int itemSpacing READ itemSpacing WRITE setItemSpacing)
	Q_PROPERTY(QMargins margins READ margins WRITE setMargins)
	Q_PROPERTY(QString anchor READ anchor WRITE setAnchor)
	//皮肤设置
	Q_PROPERTY(QString skinName READ skinName WRITE setSkinName)
public:
	explicit KWidget(QGraphicsItem *parent = NULL);
	virtual ~KWidget();
public:
	qreal width() const;
	qreal height() const;

	qreal fixWidth() const;
	void setFixWidth(qreal w);

	qreal fixHeight() const;
	void setFixHeight(qreal h);

	QSizeF fixSize() const;
	void setFixSize(const QSizeF& s);
	void setFixSize(qreal w, qreal h);

	bool sizeToChild();
	void setSizeToChild(bool on);

	bool widthToChild();
	void setWidthToChild(bool on);

	bool heightToChild();
	void setHeightToChild(bool on);

	Qt::ContextMenuPolicy contextMenuPolicy();
	void setContextMenuPolicy(Qt::ContextMenuPolicy c);

	QString toolTip();
	void setToolTip(const QString& tip);

	bool bylayout();
	void setByLayout(bool by);

	bool grayToDisable();
	void setGrayToDisable(bool on);

	QColor backgroundColor() const;
	void setBackgroundColor(const QColor& clr);
	void setBackgroundColor(int r, int g, int b, int a = 255);

	QBrush backgroundBrush() const;
	void setBackgroundBrush(const QBrush& brush);

	Qt::BrushStyle backgroundStyle() const;
	void setBackgroundStyle(Qt::BrushStyle bs);

	QtGradient backgroundGradient() const;
	void setBackgroundGradient(const QtGradient& gd);

	void unsetBackgroundBrush();

	bool hasContent();
	void enableContent(bool on);

	qreal xRotateAngle();
	void setxRotateAngle(qreal angle);

	qreal yRotateAngle();
	void setyRotateAngle(qreal angle);

	qreal zRotateAngle();
	void setzRotateAngle(qreal angle);

	void setDragPolicy( DragPolicy flag );
	DragPolicy dragPolicy() const;

	ThemePolicy themePolicy() const;
	void setThemePolicy(ThemePolicy tp);

	QString skinName() const;
	void setSkinName(const QString& name);

	int spacing();
	void setSpacing(int space);
	int itemSpacing();
	void setItemSpacing(int space);
	QMargins margins();
	void setMargins(const QMargins& m);
	QString anchor();
	void setAnchor(const QString& a);

	WidgetFlags widgetFlags() const;
	void setWidgetFlags(WidgetFlags flags);

	bool itemPanel() const;
	void setItemPanel(bool bPanel);

	Qt::Alignment alignment();
	void setAlignment(Qt::Alignment align);

	QString xmlPath() const;
	void setXmlPath(const QString& xml);

	QRectF rectToScreen() const;
	QRectF rectToView() const;
	QRectF rectToScene() const;
	
	QSizeF calcIdealSize();
	void invalidateLayout();

	QObject *findObject(const QString& name);

	QVariant data(int key);
	void setData(int key, const QVariant& v);

signals:
	void scenePosition(const QPointF& pt);

public:
	/*重写以下函数，目的是为了复盖基类的同名函数。*/
	void setAcceptedMouseButtons(Qt::MouseButtons buttons);
	bool setPropertyEx(const char* name, const char* value);
	bool isUnderMouseEx();

public:
	Q_INVOKABLE QGraphicsView *view() const;
	Q_INVOKABLE QWidget *viewport() const;

	Q_INVOKABLE virtual KWidget *layoutWidget();

	/*加或减到以下3树中*/
	Q_INVOKABLE virtual void addItem(KWidget *item);
	Q_INVOKABLE virtual void insertItem(KWidget *item, int i);
	Q_INVOKABLE virtual void removeItem(KWidget *item);

	/*加或减到控件树和对象树中*/
	Q_INVOKABLE void addChild(KWidget* child);
	Q_INVOKABLE void removeChild(KWidget* child);

	/*加或减到控件树中*/
	Q_INVOKABLE void addGraphicsItem(KWidget* ctrl);
	Q_INVOKABLE void removeGraphicsItem(KWidget* ctrl);

	/*加或减到对象树中*/
	Q_INVOKABLE void addObjectItem(KWidget* item);
	Q_INVOKABLE void removeObjectItem(KWidget *item);
	/*加或减到布局树中*/
	Q_INVOKABLE void addLayoutItem(KWidget* item);
	Q_INVOKABLE void removeLayoutItem(KWidget *item);
	Q_INVOKABLE void insertLayoutItem(KWidget* item, int i);

	Q_INVOKABLE virtual void setData(const QVariant& v);
	Q_INVOKABLE virtual void setData(const XMLNode *v);
	Q_INVOKABLE void resetSizeHint();
	Q_INVOKABLE void clearSizeHint();
	Q_INVOKABLE void activateLayout();
	Q_INVOKABLE QRect geometryToScreen();
	Q_INVOKABLE void removeBadEventWhenDelete();
	Q_INVOKABLE void changeTheme();

	virtual DragPolicy hitTest(const QPointF& point);
	virtual void construct();
	virtual void doThemeChange();

	virtual KWidget::LayoutType layoutType();
	virtual void setLayoutType(KWidget::LayoutType type);

protected:
	explicit KWidget( KWidgetPrivate &dd, QGraphicsItem *parent /* = NULL*/ );
	void setLayout(QGraphicsLayout *layout);
	QGraphicsLayout *parentLayout();
	LayoutType parentLayoutType();
	QVariant itemChange(GraphicsItemChange change, const QVariant & value);
	bool hasTheme();

protected:
	KWidgetPrivate * d_ptr;

private:
	void updateBackgroundBrush();
	void clearThemeCheckFlag();

private slots:
	void on_enabledChanged();

protected:
	/*代替原showEvent和hideEvent函数，因为原先两个同名函数不能准确反映当前item的隐藏或显示状态。
	也请不要相信ItemVisibleHasChanged的参数传值，因为该参数在以下特例中也是错的。
	特例一：把item的父对象进行隐藏或显示，则可证实。
	如需要获得准确的隐藏或显示状态，可通过visibleChanged信号连接。

	如果需要直接使用itemChange中的值，应该如下使用，否则会以上特例的初始化时出错。相关详细可搜索QGraphicScene的ItemVisibleHasChanged关键字。

	if(change == QGraphicsItem::ItemVisibleHasChanged)
	{
	bool bvis = isVisible();
	visibleEvent(bvis);
	}
	*/
	virtual void visibleEvent(bool bvis);
	/*
	与visibleEvent的true参数等价，目的希望提醒后来人：原Qt的showEvent(QShowEvent*)以上特例中一定会出错。
	*/
	virtual void showEvent();
	/*
	与visibleEvent的false参数等价，目的希望提醒后来人：原Qt的showEvent(QShowEvent*)以上特例中一定会出错。
	*/
	virtual void hideEvent();

protected:
};