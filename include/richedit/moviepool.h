#pragma once;

#include <QBasicTimer>

class MoviePoolPrivate;
class MoviePool : public QObject
{
	Q_OBJECT
	Q_DECLARE_PRIVATE(MoviePool)
public:
	explicit MoviePool(QTextDocument *doc);
	~MoviePool();
public:
	bool insertMovie(const QString &imageKey, const QString &filePath, bool bForceReplace=false);
	bool contain(const QString &imageKey);
	QImage currentImage(const QString &imageKey) const;
private:
	void timerEvent(QTimerEvent *e);
private:
	MoviePoolPrivate *d_ptr;
};