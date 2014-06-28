#include "stdafx.h"
#include "kresource.h"
#include "kutility.h"
#include "xmlParser.h"

#include <shlwapi.h>

extern QString PixmapCache_pixmapCacheKey(const QString& fileName, bool bRaw);
extern bool PixmapCache_pixmapLoad(const QString& fileName, QPixmap& pixmap, bool bRaw, bool bCache);

QString reletivePath2AbsolutePath(const QString& path)
{
	QString appRootPath = QCoreApplication::applicationDirPath();

	QDir dir(appRootPath);

	return appRootPath + "/" + path;
}

QImage KResource::loadImage(const QString& path)
{
	QImage image;
	QString absPath;
	if(path.indexOf(":") != -1)
	{
		absPath = path;
	}
	else if(QDir::isRelativePath(path))
	{
		absPath = reletivePath2AbsolutePath(path);
	}

	if(!absPath.isEmpty())
	{
		image.load(absPath);
	}
	return image;
}

QPixmap KResource::loadPixmap(const QString& path, bool bTheme, bool bCache)
{
	QPixmap pixmap;
	QString absPath;
	if(path.indexOf(":") != -1)
	{
		absPath = path;
	}
	else if(QDir::isRelativePath(path))
	{
		absPath = reletivePath2AbsolutePath(path);
	}

	if(!absPath.isEmpty())
	{
		PixmapCache_pixmapLoad(absPath, pixmap, bTheme, bCache);
	}

	return pixmap;
}

QByteArray KResource::loadRawData(const QString& path)
{
	QByteArray bytes;
	QString absPath;
	if(path.indexOf(":") != -1)
	{
		absPath = path;
	}
	else if(QDir::isRelativePath(path))
	{
		absPath = reletivePath2AbsolutePath(path);
	}

	if(!absPath.isEmpty())
	{
		QFile file(absPath);
		if (file.open(QIODevice::ReadOnly))
		{
			bytes = file.readAll();
		}
	}
	return bytes;
}

bool KResource::loadXml(const QString& path, XMLNode& xml)
{
	/*
	QDir::setSearchPaths("icons", QStringList(QDir::homePath() + "/images"));
	QDir::setSearchPaths("docs", QStringList(":/embeddedDocuments"));
	QPixmap pixmap("icons:undo.png"); // will look for undo.png in QDir::homePath() + "/images"
	QFile file("docs:design.odf"); // will look in the :/embeddedDocuments resource path
	*/
	QString absPath;
	if(path.indexOf(":") != -1)
	{
		absPath = path;
	}
	else if(QDir::isRelativePath(path))
	{
		absPath = reletivePath2AbsolutePath(path);
	}

	if(!absPath.isEmpty())
	{
		QFile file(absPath);

		if (file.open(QFile::ReadOnly))
		{
			QByteArray xmlByte = file.readAll();
			QString xmlTxt = QString::fromUtf8(xmlByte.data(), xmlByte.size());
			XMLResults err;
			xml = XMLNode::parseString(xmlTxt.toStdString().data(), NULL, &err);
			return err.error == eXMLErrorNone;
		}
		return false;
	}

	return false;
}

QString KResource::pixmapCacheKey( const QString& path, bool bRaw )
{
	QString absPath;
	if(path.indexOf(":") != -1)
	{
		absPath = path;
	}
	else if(QDir::isRelativePath(path))
	{
		absPath = reletivePath2AbsolutePath(path);
	}

	if(!absPath.isEmpty())
	{
		return PixmapCache_pixmapCacheKey(absPath, bRaw);		
	}
	return false;
}

QString KResource::absoluteFileName( const QString& path )
{
	if(path.indexOf(":") != -1)
	{
		if(path.at(0) == ':')
			return path;

		int idx = path.indexOf(":");
		QString prefix = path.mid(0, idx);
		QString subpath = path.mid(idx+1);
		QStringList hitPaths = QDir::searchPaths(prefix);
		if(hitPaths.isEmpty())
			return path;
		if(hitPaths.count() == 1)
		{
			QString absPath = hitPaths.at(0);
			absPath += "/";
			absPath += subpath;
			return absPath;
		}
		QFileInfo fi(path);
		if(fi.exists())
		{
			return fi.absoluteFilePath();
		}

		QString absPath;
		absPath = hitPaths.at(0);
		absPath += "/";
		absPath += subpath;
		return absPath;
	}
	else if(QDir::isRelativePath(path))
	{
		return reletivePath2AbsolutePath(path);
	}
	return path;
}

void KResource::addSearchPath( const QString &prefix, const QString &path )
{
	if(PathFileExistsW(path.toStdWString().c_str()))
	{
		QDir::addSearchPath(prefix, path);
	}
}

QStringList KResource::searchPaths( const QString & prefix )
{
	return QDir::searchPaths(prefix);
}

void KResource::addDefaultSearchPath()
{
	QDir::addSearchPath("appdata", KUtility::osAppDataPath());
	QDir::addSearchPath("appdir", KUtility::osExeInstancePath());
}
