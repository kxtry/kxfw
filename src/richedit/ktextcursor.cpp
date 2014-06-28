
#include "stdafx.h"
#include "ktextcursor.h"
#include "qtextbrowserex.h"
#include "moviepool.h"
#include "kutility.h"

Q_DECLARE_METATYPE(QTextCharFormat)

class KTextCursorPrivate
{
public:
	QPointer<MoviePool> m_movies;

public:
	explicit KTextCursorPrivate()
	{

	}
};

KTextCursor::KTextCursor()
:d_ptr(new KTextCursorPrivate())
{

}

KTextCursor::KTextCursor( const QTextCursor &tc )
: QTextCursor( tc )
, d_ptr( new KTextCursorPrivate() )
{
	
}

KTextCursor::KTextCursor( const KTextCursor &tc )
 : QTextCursor( tc )
 , d_ptr( new KTextCursorPrivate(*(tc.d_ptr)) )
{

}

KTextCursor::~KTextCursor(void)
{
	KX_SAFE_DELETE( d_ptr );
}

void KTextCursor::insertImage( const QString & name )
{
	QTextImageFormat format;
	format.setName(name);
	format.setProperty(KImageKey, name);

	insertImage(name, format);
}

void KTextCursor::insertImage( const QString &name, const QString &key )
{
	QTextImageFormat imageFormat;
	imageFormat.setName(name);
	imageFormat.setProperty(KImageKey, key);

	insertImage(name, imageFormat);
}

void KTextCursor::insertImage( const QString &name, const QImage &image, const QTextImageFormat &format )
{
	QTextImageFormat inserFormat = format;
	inserFormat.setName(name);
	if(!inserFormat.hasProperty(KImageKey))
	{
		inserFormat.setProperty(KImageKey, name);
	}

	document()->addResource(QTextDocument::ImageResource, QUrl(name), image);
	__super::insertImage(inserFormat);
}


void KTextCursor::insertImage( const QString &name, const QString &key, const QString &linkValue, const QString &linkKey )
{
	QTextImageFormat imageFormat;

	imageFormat.setAnchor(true);
	imageFormat.setAnchorHref(linkValue);
	imageFormat.setName(name);
	imageFormat.setProperty(KImageKey, key);

	if(!linkKey.isEmpty())
	{
		imageFormat.setProperty( KLinkKey, linkKey );
	}

	insertImage(name, imageFormat);
}

void KTextCursor::insertImage( const QString &name, const QTextImageFormat &format )
{
	QTextImageFormat imageForamt = format;

	imageForamt.setName(name);
	if(!imageForamt.hasProperty(KImageKey))
	{
		imageForamt.setProperty(KImageKey, name);
	}

	if(moviePool()->insertMovie(name, name))
	{
		Q_D(KTextCursor);

		imageForamt.setProperty(KAnimationImage,true);
		document()->addResource(QTextDocument::ImageResource, QUrl(name), moviePool()->currentImage(name));
	}
	else
	{
		QImage image(name);
		if(!image.isNull())
		{
			document()->addResource(QTextDocument::ImageResource,QUrl(name), image);
		}
	}

	__super::insertImage(imageForamt);
}

void KTextCursor::insertText( const QString &displayName, const QColor &color, const QString &linkValue, const QString &linkKey, bool bUnderLine )
{
	Q_D(KTextCursor);

	QTextCharFormat linkFormat;

	linkFormat.setFontUnderline(bUnderLine);
	linkFormat.setForeground(color);

	insertText(displayName, linkFormat, linkValue, linkKey);
}

void KTextCursor::insertText( const QString &displayName, const QTextCharFormat &format, const QString &linkValue, const QString &linkKey )
{
	QTextCharFormat linkFormat = format;

	linkFormat.setAnchor(true);
	linkFormat.setAnchorName(displayName);
	linkFormat.setAnchorHref(linkValue);
	if(!linkKey.isEmpty())
	{
		linkFormat.setProperty( KLinkKey, linkKey );
	}

	insertText( displayName, linkFormat );
}

void KTextCursor::insertText( const QString& text )
{
	__super::insertText(text);
}

void KTextCursor::insertText( const QString &text, const QTextCharFormat &format )
{
	QTextCharFormat fmt = format;

	__super::insertText(text, fmt);
}

void KTextCursor::insertText( const QString &text, const QColor& clr, bool bUnderLine )
{
	QTextCharFormat format;
	format.setForeground(clr);
	format.setFontUnderline(bUnderLine);

	__super::insertText(text, format);
}

void KTextCursor::insertText( const QString &text, const QColor& clr, const QFont& font )
{
	QTextCharFormat format;
	format.setFont(font);
	format.setForeground(clr);
	__super::insertText(text, format);
}

MoviePool * KTextCursor::moviePool()
{
	Q_D(KTextCursor);
	if(d->m_movies)
		return d->m_movies;
	QTextDocument *doc = document();
	QVariant vmovie = doc->property("movies");
	if(vmovie.isValid())
	{
		MoviePool *pool = qobject_cast<MoviePool*>(vmovie.value<QObject*>());
		d->m_movies = pool;
		return pool;
	}

	MoviePool *pool = new MoviePool(doc);
	doc->setProperty("movies", QVariant::fromValue<QObject*>(pool));
	d->m_movies = pool;
	return pool;
}

KTextCursor & KTextCursor::operator=( const QTextCursor &other )
{
	__super::operator=(other);
	if(document() != other.document())
	{
		d_ptr->m_movies = NULL;
	}
	return *this;
}

KTextCursor & KTextCursor::operator=( const KTextCursor &other )
{
	__super::operator=(other);
	*d_ptr = *other.d_ptr;
	return *this;
}

void KTextCursor::insertHtml( const QString &text, const QMap<QString, QString>& imgs )
{
	if(!imgs.isEmpty())
	{
		for(QMap<QString,QString>::const_iterator iter = imgs.begin(); iter != imgs.end(); iter++)
		{
			QString key = iter.key();
			QString file = iter.value();
			if(moviePool()->insertMovie(file, file))
			{
				//修改属性为动画。
				document()->addResource(QTextDocument::ImageResource, QUrl(file), moviePool()->currentImage(file));
			}
			else
			{
				QImage image(file);
				if(!image.isNull())
				{
					document()->addResource(QTextDocument::ImageResource, QUrl(file), image);
				}
			}
		}
	}

	int istart = position();
	insertHtml(text);
	int inow = position();

	if(!imgs.isEmpty() && inow > istart)
	{
		setPosition(istart);
		movePosition(NextCharacter, KeepAnchor, inow-istart);
		QString txt = selectedText();
		int index = txt.indexOf(QChar::ObjectReplacementCharacter, 0);
		while(index >= 0)
		{
			/*修改字体类型。*/
			setPosition(istart+index);
			movePosition(NextCharacter, KeepAnchor, 1);
			QTextCharFormat fmt = charFormat();			
			QTextImageFormat imgFmt = fmt.toImageFormat();
			QString key = imgFmt.name();
			if(imgs.contains(key))
			{
				imgFmt.setProperty(KImageKey, key);
				imgFmt.setName(imgs.value(key));
				imgFmt.setProperty(KAnimationImage, true);
				setCharFormat(imgFmt);
			}
			int idx = index+1;
			index = txt.indexOf(QChar::ObjectReplacementCharacter, idx);
		}
	}

	setPosition(inow);
}

void KTextCursor::insertHtml( const QString &text, const QMap<QString, QImage>& imgs )
{
	if(!imgs.isEmpty())
	{
		for(QMap<QString,QImage>::const_iterator iter = imgs.begin(); iter != imgs.end(); iter++)
		{
			QString key = iter.key();
			QImage image = iter.value();
			if(!image.isNull())
			{
				document()->addResource(QTextDocument::ImageResource, QUrl(key), image);
			}
		}
	}
	
	__super::insertHtml(text);
}

void KTextCursor::insertHtml( const QString& text )
{
	__super::insertHtml(text);
}

void KTextCursor::insertFragment( const QByteArray& frag )
{
	QList<QPair<QString,QTextCharFormat>> frags = KUtility::fragmentsFromByteArray(frag);
	insertFragment(frags);
}

void KTextCursor::insertFragment( QList<QPair<QString,QTextCharFormat>>& frags )
{
	for(QList<QPair<QString,QTextCharFormat>>::iterator iter = frags.begin(); iter != frags.end(); iter++)
	{
		QPair<QString,QTextCharFormat>& item = *iter;
		QString text = item.first;
		QTextCharFormat fmt = item.second;
		if(text.at(0) == QChar::ObjectReplacementCharacter)
		{
			//图片
			QTextImageFormat imgFmt = fmt.toImageFormat();
			QString file = imgFmt.name();
			if(!file.isEmpty())
			{
				if(moviePool()->insertMovie(file, file))
				{
					fmt.setProperty(KAnimationImage, true);
					document()->addResource(QTextDocument::ImageResource, QUrl(file), moviePool()->currentImage(file));
				}
				else
				{
					QImage image(file);
					if(!image.isNull())
					{
						document()->addResource(QTextDocument::ImageResource, QUrl(file), image);
					}
				}
			}
		}
		insertText(text, fmt);
	}
}
