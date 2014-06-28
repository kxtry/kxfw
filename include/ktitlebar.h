#include "kwidget.h"


class KTitlebarPrivate;

class KXFW_API KTitlebar : public KWidget
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(KTitlebar)

	Q_PROPERTY(bool iconClickExit READ iconClickExit WRITE setIconClickExit)
	Q_PROPERTY(QString iconPath READ iconPath WRITE setIconPath)
	Q_PROPERTY(QString maximumPath READ maximumPath WRITE setMaximumPath)
	Q_PROPERTY(QString restorePath READ restorePath WRITE setRestorePath)
	Q_PROPERTY(QString minimumPath READ minimumPath WRITE setMinimumPath)
	Q_PROPERTY(QString closePath READ closePath WRITE setClosePath)
	Q_PROPERTY(int titlebarHeight READ titlebarHeight WRITE setTitlebarHeight)
	Q_PROPERTY(QString title READ title WRITE setTitle)
	Q_PROPERTY(QColor titleColor READ titleColor WRITE setTitleColor)
	Q_PROPERTY(bool titleBold READ titleBold WRITE setTitleBold)
	Q_PROPERTY(bool doubleClick READ acceptDoubleClicked WRITE setAcceptMouseDoubleClick)
public:
	explicit KTitlebar(QGraphicsItem *parent = NULL);
	~KTitlebar();
	
	int titlebarHeight();
	void setTitlebarHeight(int height);

	bool iconClickExit() const;
	void setIconClickExit(bool bExit);

	QString iconPath() const;
	void setIconPath(const QString& f);
	
	QString maximumPath() const;
	void setMaximumPath(const QString& f);

	QString restorePath() const;
	void setRestorePath(const QString& f);

	QString minimumPath() const;
	void setMinimumPath(const QString& f);

	QString closePath() const;
	void setClosePath(const QString& f);

	QString title() const;
	void setTitle(const QString& t);

	QColor titleColor() const;
	void setTitleColor(const QColor& clr);

	bool titleBold();
	void setTitleBold(bool on);
	
	bool acceptDoubleClicked();
	void setAcceptMouseDoubleClick(bool on);
public:
	Q_INVOKABLE void appendButton(KWidget *btn);
	Q_INVOKABLE void updateTitelbar();
protected:
	void mouseDoubleClickEvent(QGraphicsSceneMouseEvent * event);
	void resizeEvent(QGraphicsSceneResizeEvent * event);
private slots:
	void buttonClicked();
};