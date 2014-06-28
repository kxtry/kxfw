#pragma once;

class KMenuPrivate;

class KXFW_API KAction : public QAction
{
	Q_OBJECT
public:
	explicit KAction(QObject *parent);
	~KAction();
protected:
	//只需返回当前内容区域的大小。
	virtual QSize contentSize() = 0;
	//tabWidth:为快捷键区域长度。
	//tabSpace:为快捷键和文字之间的区域长度。
	//textWidth:为文字长度区域
	//iconWidth:为图标显示区域长度。
	virtual void paint(QPainter *p, const QRectF& drawRt, int iconWidth, int textWidth, int tabSpace, int tabWidth) = 0;
private:
	friend class KMenuPrivate;
};