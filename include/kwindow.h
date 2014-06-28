#pragma once;

class KWidget;
class KSpacer;
class QGraphicsScene;
class KThemeChangeEvent;

#include <QGraphicsView>

class KWindowPrivate;

class KXFW_API KWindow : public QGraphicsView
{
	Q_OBJECT
	Q_DISABLE_COPY(KWindow)
	Q_DECLARE_PRIVATE(KWindow)
	Q_PROPERTY(QMargins sceneMargins READ sceneMargins WRITE setSceneMargins)
	Q_PROPERTY(int fontSize READ fontSize WRITE setFontSize)
	Q_PROPERTY(QString fontName READ fontName WRITE setFontName)
	Q_PROPERTY(bool bold READ bold WRITE setBold)
	Q_PROPERTY(QString skinName READ skinName WRITE setSkinName)
public:
	explicit KWindow(QWidget *parent = NULL);
	virtual ~KWindow();

	QMargins sceneMargins() const;
	void setSceneMargins(const QMargins& m);

	void setRealWindow(bool on);
	void abortCloseWindow();

	void setFontSize( int fs );
	int fontSize() const;

	void setBold( bool enable );
	bool bold() const;

	QString fontName() const;
	void setFontName(const QString& name);

	QString skinName() const;
	void setSkinName(const QString& name);

	void setFont(const QFont& f);

	QSizeF calcWindowPreferSize();

	void skipMyCloseExecution();

	void setAutoDeleteWhenAppQuit(bool on);

public slots:
	/*
	Widget设置了WA_DeleteOnClose属性后。
	重写close事件，目的是由于在一些特殊条下，例如IE或Flash的script回调里直接调用Qt，由于是运行标准的WIN32消息，
	没有直接经过Qt内部导致thread的loopLevel为1，此时而导致调用deleteLater无效。
	只适合new的对象操作。
	如果是栈对象，则需要调用skipMyCloseExecution();
	*/
	virtual void close();
	virtual void showNormal();
	virtual void showMaximized();
	virtual void showMinimized();
	virtual void showMaximizeOrRestore();

private slots:
	void on_aboutToQuit();

signals:
	void visibleChanged();
	void aboutToClose();
	void resized();
	void posChanged();

public:
	virtual KWidget *rootWidget();
	virtual void construct();

protected:
	explicit KWindow( KWindowPrivate &dd, QWidget *parent /* = NULL*/ );
	virtual void resizeEvent(QResizeEvent *event);
	virtual void closeEvent(QCloseEvent *event);
	virtual void showEvent(QShowEvent *event);
	virtual void hideEvent(QHideEvent *event);
	virtual void themeChange(KThemeChangeEvent *event);
	virtual bool event(QEvent * e);
	virtual bool winEvent(MSG *message, long *result);

	void doThemeChange();

private slots:
	void on_viewport_update_bugfix();

private:
	void setContentsRect();
protected:
	KWindowPrivate * d_ptr;

};