
#pragma once

const int KImageKey	= QTextFormat::UserProperty + 1;
const int KLinkKey	= QTextFormat::UserProperty + 2;

const int KAnimationImage = QTextFormat::UserProperty + 3;//为快速查找出动画类型，增加该属性。

class KTextCursor;

class KTextCursorPrivate;
class MoviePool;

class KXFW_API KTextCursor : public QTextCursor
{
	Q_DECLARE_PRIVATE(KTextCursor);
public:
	KTextCursor();
	KTextCursor(const QTextCursor &tc);
	KTextCursor(const KTextCursor &tc);
	~KTextCursor();
	KTextCursor &operator=(const QTextCursor &other);
	KTextCursor &operator=(const KTextCursor &other);
public:
	void insertImage(const QString &name);
	void insertImage(const QString &name, const QString &key);
	void insertImage(const QString &name, const QTextImageFormat &format);
	void insertImage(const QString &name, const QImage &image, const QTextImageFormat &format = QTextImageFormat());
	void insertImage(const QString &name, const QString &key, const QString &linkValue, const QString &linkKey);

	void insertText(const QString &text);
	void insertText(const QString &text, const QTextCharFormat &format);
	void insertText(const QString &text, const QColor& clr, bool bUnderLine=false);
	void insertText(const QString &text, const QColor& clr, const QFont& font);
	void insertText(const QString &text, const QTextCharFormat &format, const QString &linkValue, const QString &linkKey);
	void insertText(const QString &text, const QColor &color, const QString &linkValue, const QString &linkKey, bool bUnderLine = false);

	void insertHtml(const QString &text);
	//QMap<QString, QString>，<name,path>，name对应的是HTML中的<img src="name">, path对应的是image文件位置。
	void insertHtml(const QString &text, const QMap<QString, QString>& imgs);
	void insertHtml(const QString &text, const QMap<QString, QImage>& imgs);

	void insertFragment(const QByteArray& frag);
	void insertFragment(QList<QPair<QString,QTextCharFormat>>& frags);

	MoviePool *moviePool();
private:
	KTextCursorPrivate *d_ptr;
};

