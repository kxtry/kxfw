#pragma once;

class KAction;
class KMenuPrivate;
class KWidget;

struct MenuStyleData 
{
	QPixmap frame;//菜单边框的图片，它将执行四宫格拉伸。
	QPixmap mask;//掩码图，控制背景色的填充，确保不会填充出边框区域，将会执行四宫格拉伸，确保背景填充无误。
	QMargins maskMargin;//菜单边框的空白区域。
	int menuHMargin; //菜单内容空白区域。
	int menuVMargin;
	int leftFrameThinkness; //Hover的绘制的左微调。
	int rightFrameThinkness;//Hover的绘制的右微调
	QColor background;//菜单背景色。
	QPixmap check;//菜单Check属性图。
	QPixmap checkDisable;//菜单选中为不可用的状态。
	int iconMinimumSize; //菜单图标的大小。
	int itemMinimumHeight; //菜单项的默认高度，当它为0时，表示使用系统默认的高度。
	int menuMinimumWidth;// 菜单的最小长度。
	int overlapSize; //父子菜单重叠大小。
	QPixmap hovered; //Hover状态下的图片
	QPixmap seperator; //分割线图片
	QPixmap arrow; //子菜单的箭头。
	QColor textColor; //正常情况下字体颜色
	QColor textColorDisable; //禁用情况下字体颜色。

	MenuStyleData()
	{
		iconMinimumSize = 0;
		overlapSize = 10;
		itemMinimumHeight = 0;
		menuHMargin = 3;
		menuVMargin = 5;
		leftFrameThinkness = 2;
		rightFrameThinkness = 2;
		menuMinimumWidth = 100;
	}
};

class KXFW_API KMenu : public QMenu
{
	Q_OBJECT
public:
	enum Direction
	{
		NotDefine,
		TopLeft,
		TopCenter,
		TopRight,
		BottomLeft,
		BottomCenter,
		BottomRight,
		LeftTop,
		LeftCenter,
		LeftBottom,
		RightTop,
		RightCenter,
		RightBottom
	};
public:
	explicit KMenu(QWidget *parent = NULL);
	~KMenu();

	static bool addMenuStyle(int key, MenuStyleData& style);
	static KMenu *createPopupMenu(int stylekey = 0);

	void setTextAlignCenter(bool on);
	void setBackgroundColor(const QColor &clr);
	void setTextColor(const QColor& clr);
	void setTextDisableColor(const QColor& clr);

	QAction* exec();
	QAction* exec(KWidget *target, Direction direction, const QPoint& offset=QPoint(0,0));


signals:
	void aboutToAdjust(QWidget *w, bool *pbHandled);

private slots:
	void on_aboutToShow();

protected:
	virtual void showEvent(QShowEvent *event);
	virtual void resetGeometry();

private:
	KMenuPrivate *d;

	friend class KMenuStyleHandler;
};