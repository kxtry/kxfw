#pragma once;

#include "kwidget.h"

class KFrameItemPrivate;

class KXFW_API KFrameItem : public KWidget
{
	Q_OBJECT
	Q_DISABLE_COPY(KFrameItem)
	Q_DECLARE_PRIVATE(KFrameItem)

	Q_ENUMS(FrameType)
	Q_ENUMS(FrameState)
	Q_ENUMS(DefaultFrame)

	Q_PROPERTY(DefaultFrame defaultFrame READ defaultFrame WRITE setDefaultFrame)
	Q_PROPERTY(QString imagePath READ imagePath WRITE setImagePath)
	Q_PROPERTY(FrameType frameType READ frameType WRITE setFrameType)
	Q_PROPERTY(QString frameColor READ frameColor WRITE setFrameColor)
	Q_PROPERTY(int framePixel READ framePixel WRITE setFramePixel)
	Q_PROPERTY(FrameState frameState READ frameState WRITE setFrameState)
	Q_PROPERTY(bool acceptClick READ acceptClick WRITE setAcceptClick)
	Q_PROPERTY(bool frameSwitch READ frameSwitch WRITE setFrameSwitch)
	Q_PROPERTY(bool acceptHover READ acceptHover WRITE setAcceptHover)
	Q_PROPERTY(int index READ frameIndex WRITE setFrameIndex)
	Q_PROPERTY(bool smooth READ smooth WRITE setSmooth)
	Q_PROPERTY(int count READ frameCount)
public:
	enum FrameType
	{
		OneTopLeft = 0,			/*左上角*/
		OneTopRight,		/*右上角*/
		OneBotLeft,			/*左下角*/
		OneBotRight,		/*右下角*/
		OneHCenter,			/*水平中间*/
		OneVCenter,			/*竖向中间*/
		OneHVCenter,		/*正中心*/
		TileH,			/*水平平铺一行*/
		TileV,		/*垂直平铺一行*/
		TileHV,		/*水平平铺多行，直致满屏*/
		StretchH,	/*水平拉伸，如果高度超出图片高度，则空白。*/
		StretchV,	/*垂直拉伸，如果宽度超出图片宽度，则空白。*/
		StretchVTwoSize,
		StretchVMiddle,
		StretchHV,	/*水平垂直均拉伸，完全充满控件。*/
		StretchHTwoSize,	/*水平拉伸图片两边的边线且仅拉一像素图像，中间不拉伸。*/
		StretchHMiddle, /*水平拉伸图片的中间一条像素，两端不拉伸。*/
		GridFour,	/*4宫格*/	
		GridFrameFour, /*空心4宫格*/
		GridNine,
		BitmapEnd,
		Rectangle,		/*直角颜色*/
		RoundRect,		/*圆角颜色*/
		RoundRect4Pixel,  /*圆角颜色*/
		RoundRect5Pixel,  /*圆角颜色*/
		RoundRect6Pixel,  /*圆角颜色*/
		RoundRect7Pixel,  /*圆角颜色*/
		ColorFrameEnd
	};

	enum DefaultFrame
	{
		BadDefalut,
		RoundWhite,
		RoundTranparent,
		RoundFlatTranslucent,
		RoundRaiseTranslucent,
		RoundSunkenTranslucent,
		RectWhite
	};

	enum FrameState
	{
		Normal = 0,
		Hover = 1,
		Down = 2,
		Selected = Down,
		Disable = 3,
		CheckNormal = 4,
		CheckHover = 5,
		CheckDown = 6,
		CheckSelected = CheckDown,
		CheckDisable = 7,
		StateMax
	};

public:
	explicit KFrameItem(QGraphicsItem *parent = NULL);
	explicit KFrameItem(const QString& path, KFrameItem::FrameType type, QGraphicsItem *parent = NULL);
	~KFrameItem();
	
	KFrameItem::FrameType frameType() const;
	void setFrameType(KFrameItem::FrameType type);

	QString frameColor() const;
	void setFrameColor(const QString& clr);

	DefaultFrame defaultFrame() const;
	void setDefaultFrame(DefaultFrame type);

	int framePixel();
	void setFramePixel(int iPixel);

	bool frameSwitch();
	void setFrameSwitch(bool bSwitch);

	QString imagePath() const;
	void setImagePath(const QString& path);

	QPixmap currentPixmap();
	void appendPixmap(const QPixmap& img);

	void setFixSizeByPixmapSize();
	void setFixWidthByPixmapWidth();
	void setFixHeightByPixmapHeight();
	
	int frameCount();
	QSizeF pixmapSize();

	void setFrameIndex(int idx);
	int frameIndex() const;

	bool smooth() const;
	void setSmooth(bool on);

	bool acceptClick();
	void setAcceptClick(bool bClick);

	bool acceptHover();
	void setAcceptHover(bool bHover);

	/*常见的4态*/
	void setNormal();
	void setHover();
	void setDown();
	void setSelected();
	void setDisable();
	bool hasChecked();
	void setChecked(bool bCheck);

	void setEnabled(bool enabled);

	FrameState frameState() const;
	void setFrameState(FrameState state);

signals:
	void enter();
	void leave();
	void pressed();
	void released();
	void clicked();
protected:
	void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	void mousePressEvent(QGraphicsSceneMouseEvent * event);
	void mouseReleaseEvent(QGraphicsSceneMouseEvent * event);
	void hoverEnterEvent(QGraphicsSceneHoverEvent * event);
	void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
	void hoverLeaveEvent(QGraphicsSceneHoverEvent * event);
	void doThemeChange();
private:
	void drawOneTopLeft(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	void drawOneTopRight(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	void drawOneBotLeft(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	void drawOneBotRight(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	void drawOneHCenter(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	void drawOneVCenter(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	void drawOneHVCenter(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	void drawTileH(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	void drawTileV(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	void drawTileHV(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	void drawStretchH(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	void drawStretchV(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	void drawStretchHV(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	void drawStretchHTwo(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	void drawStretchHMiddle(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	void drawStretchVTwo(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	void drawStretchVMiddle(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	void drawGridFour(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	void drawGridFrameFour(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	void drawGridNine(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	void drawRectangle(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
	void drawRoundRect(QPainter *painter, const int iRadio, const QStyleOptionGraphicsItem *option, QWidget *widget = 0);
private:
	void init();
	int normalIndex(int idx);
	void updateTheme();
};