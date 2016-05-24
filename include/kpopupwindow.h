#pragma once;
#include "kwindow.h"

class KSpacer;
class KWidget;
class KImageItem;
class KPushButton;

class KPopupWindowPrivate;

class KXFW_API KPopupWindow : public KWindow
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KPopupWindow)
public:
	enum WindowStyle
	{
		Null = 0x0,
		Resizable = 0x01,
		CloseButton = 0x02,
		MinButton = 0x04,
		MaxButton = 0x08,
		AppWindow = 0x10,
		ToolWindow = 0x20,
		SystemMenu = 0x40,
		NoFrame = 0x80,

		Window =  Resizable | CloseButton | MinButton | MaxButton | SystemMenu,
		Dialog = CloseButton| AppWindow,
		MainWindow = Window | AppWindow
	};
	Q_DECLARE_FLAGS(WindowStyles, WindowStyle)
	Q_FLAGS(WindowStyles);

	Q_PROPERTY(WindowStyles windowStyle READ windowStyle WRITE setWindowStyle);
	Q_PROPERTY(bool dragWindow READ dragWindow WRITE setDragWindow)
	Q_PROPERTY(bool quitAppOnClose READ quitAppOnClose WRITE setQuitAppOnClose)
	Q_PROPERTY(bool closeOnEscape READ closeOnEscape WRITE setCloseOnEscape);
	Q_PROPERTY(bool closeOnF4 READ closeOnF4 WRITE setCloseOnF4);
	Q_PROPERTY(bool toolWindow READ toolWindow WRITE setToolWindow)
	Q_PROPERTY(bool maximumButton READ maximumButton WRITE setMaximumButton)
	Q_PROPERTY(bool minimumButton READ minimumButton WRITE setMinimumButton)
	Q_PROPERTY(bool systemMenu READ systemMenu WRITE setSystemMenu)
	Q_PROPERTY(bool translucent READ translucent WRITE setTranslucent)
	Q_PROPERTY(bool resizable READ resizable WRITE setResizable)
	Q_PROPERTY(bool hasFrame READ hasFrame WRITE setHasFrame)
	Q_PROPERTY(QSize frameSize READ frameSize WRITE setFrameSize)
	Q_PROPERTY(QSize minimumSize READ minimumSize WRITE setMinimumSize)
	Q_PROPERTY(QSize maximumSize READ maximumSize WRITE setMaximumSize)
	Q_PROPERTY(QSize fixSize READ fixSize WRITE setFixSize)
	Q_PROPERTY(QString maskImage READ maskImage WRITE setMaskImage)
	Q_PROPERTY(int borderPixel READ borderPixel WRITE setBorderPixel)
	Q_PROPERTY(QMargins maskMargins READ maskMargins WRITE setMaskMargins)
	Q_PROPERTY(QString frameImage READ frameImage WRITE setFrameImage)
	Q_PROPERTY(QString backgroundImage READ backgroundImage WRITE setBackgroundImage)
	Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
	Q_PROPERTY(bool enableAeroSnap READ enableAeroSnap WRITE setAeroSnapEnable)

public:
	explicit KPopupWindow(QWidget *parent=NULL);
	~KPopupWindow();

	bool dragWindow() const;
	void setDragWindow(bool on);

	bool quitAppOnClose() const;
	void setQuitAppOnClose(bool on);

	bool closeOnEscape() const;
	void setCloseOnEscape(bool on);

	bool closeOnF4() const;
	void setCloseOnF4(bool on);

	bool toolWindow() const;
	void setToolWindow(bool on);

	bool maximumButton() const;
	void setMaximumButton(bool bMax);

	bool minimumButton() const;
	void setMinimumButton(bool bMin);

	bool systemMenu() const;
	void setSystemMenu(bool on);

	bool translucent() const;
	void setTranslucent(bool on);

	bool hasFrame() const;
	void setHasFrame(bool on);

	bool resizable() const;
	void setResizable(bool on);

	int borderPixel() const;
	void setBorderPixel(int pixel);

	QSize frameSize() const;
	void setFrameSize(const QSize& s);
	
	QSize minimumSize() const;
	void setMinimumSize(const QSize& s);

	QSize maximumSize() const;
	void setMaximumSize(const QSize& s);

	QSize fixSize() const;
	void setFixSize(const QSize& s);

	QString maskImage() const;
	void setMaskImage(const QString& mask);

	QMargins maskMargins() const;
	void setMaskMargins(const QMargins& margins);

	QString frameImage() const;
	void setFrameImage(const QString& frame);

	QString backgroundImage() const;
	void setBackgroundImage(const QString& img);

	QColor backgroundColor() const;
	void setBackgroundColor(const QColor& clr);
	
	WindowStyles windowStyle() const;
	void setWindowStyle(WindowStyles style);

	bool enableAeroSnap() const;
	void setAeroSnapEnable(bool on);

	void setWindowIcon(HINSTANCE hInst, int resid);
	void setWindowIcon(HINSTANCE hInst, const char* ico);
	void setWindowIcon(const QIcon& ico);

signals:
	void enterPressed(Qt::KeyboardModifiers moidifier);
public:
	Q_INVOKABLE void updateRequest();
	Q_INVOKABLE void moveCenter( KWidget *center, bool bAsyn=false );
	Q_INVOKABLE void moveCenter( QPoint pt );
	Q_INVOKABLE void resetFrame();

protected:
	explicit KPopupWindow( KPopupWindowPrivate &dd, QWidget *parent /* = NULL*/ );
	virtual void resizeEvent(QResizeEvent *event);
	virtual void drawBackground( QPainter * painter, const QRectF & rect );
	virtual void drawForeground( QPainter * painter, const QRectF & rect );
	virtual bool winEvent ( MSG * message, long * result );
	virtual void showEvent ( QShowEvent * event );
	virtual void keyPressEvent( QKeyEvent *event );
	virtual void closeEvent ( QCloseEvent * event );
	virtual void themeChange(KThemeChangeEvent *event);

	virtual void onEnterPressed(Qt::KeyboardModifiers moidifier);
	virtual void onEscPressed(Qt::KeyboardModifiers moidifier);

	virtual void construct();

protected slots:
	virtual void onCloseClick();

private:
	void setWindowShape();
	void drawBackgroundImage(QPainter *painter);
	void drawBackgroundColor(QPainter *painter);
	void drawFrameImage(QPainter *painter);
	void createFourAngleRegion();
	void createWindowRegion(const QRect& winRt);
    void updateWindowStyle();

	bool keyDownEvent(MSG *pMsg, long *result);
	bool keyUpEvent(MSG *pMsg, long *result);
	bool ncCalcSizeEvent( MSG *pMsg, long *result );
	bool getMinMaxInfoEvent(MSG *pMsg, long *result);
	bool ncLefButtonDBClickEvent(MSG *pMsg, long *result);
	bool ncLeftButtonDownEvent(MSG *pMsg, long *result);
	bool leftButtonDownEvent(MSG *pMsg, long *result);
	bool leftButtonUpEvent(MSG *pMsg, long *result);
	bool leftButtonDBClickEvent(MSG *pMsg, long *result);
	bool mouseMoveEvent(MSG *pMsg, long *result);
	bool systemCommandEvent(MSG *pMsg, long *result);
	bool initSysMenuPopup( MSG *pMsg, long *result );
	bool setCursorEvent(MSG *pMsg, long *result);
	bool hitTestEvent(MSG *pMsg, long *result);
	long hitTestFrame(const QPoint& pt, const QRect& frameRt, const QMargins& border, int captionHeight);
	long hitTestClient(const QPoint& pt);
	long hitTestCaption(const QPoint& pt);
	

	void init();

};