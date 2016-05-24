#pragma once

class KXFW_API KThemeChangeEvent : public QEvent
{
public:
	KThemeChangeEvent();
	static int EventType;
};

class KXFW_API KTheme
{
public:
	enum Level
	{
		TEXT_COLOR = 0,
		ITEM_DEFAULT_COLOR,
		ITEM_HOVER_COLOR,
		ITEM_SELECT_COLOR,
		ITEM_DISABLE_COLOR,

		LEVEL_COUNT
	};
	static QPixmap getPixmap(const QString& fileName, bool bTheme=false, bool bCache=true);
	static QPixmap getThemePixmap(const QString& fileName);
	static QPixmap getRawPixmap(const QString& fileName);
	static QImage getImage(const QString& fileName);
	static QColor themeColor();//当前正在使用的主题色
	static void setThemeColor(const QColor& refclr);
	static void setThemeIndex(int index);
	static QColor textColor();//当前正在使用的主题对应的文本颜色
	static QColor levelColor(int level);
	static int themeIndex();//当前的主题索引值，是由主题色计算出来的。
	static QColor convertToThemeColor(const QColor& clrref);
	static QPixmap convertToThemePixmap(const QPixmap& pixmapref);
	static QImage convertToThemeImage(const QImage& imgref);
	static QBrush convertToThemeBrush(const QBrush& brushref);

	static void setSkinName(QObject* obj, const QString& name);

	static void EnableAeroSnap(bool on);

	static QSize virtualScreenSize();
	static QRect virtualScreenRect();
	static void resetTheme();
private:
	static int calcNearestThemeIndex(const QColor& refclr);
};