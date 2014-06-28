#include "stdafx.h"
#include "kutility.h"
#include "kwidget.h"
#include "olectl.h"

#include <shlwapi.h>
#pragma comment(lib, "shlwapi.lib")

struct UtilsData
{
	QString appRootPath;
	QString resourcePath;
};

Q_GLOBAL_STATIC(UtilsData, utilsData)

QPoint KUtility::mapItemToScreen( QGraphicsView* view, QGraphicsItem *item )
{
	Q_ASSERT(view);
	Q_ASSERT(item);

	QPointF scencePoint = item->mapToScene(QPointF(0, 0));
	QPoint viewPoint = view->mapFromScene(scencePoint);
	QPoint screenPoint = view->mapToGlobal(viewPoint);

	return screenPoint;
}

QPoint KUtility::mapItemToScreen( QGraphicsItem *item )
{
	QGraphicsScene *scene = item->scene();
	QGraphicsView *view = scene->views().at(0);
	return mapItemToScreen(view, item);
}

QRect KUtility::mapWidgetToScreen( QGraphicsWidget *widget )
{
	Q_ASSERT(widget);
	Q_ASSERT(widget->scene());

	QGraphicsView *view = graphicsViewFromWidget(widget);
	Q_ASSERT(view);

	QPointF screenPos = mapItemToScreen(view, widget);

	return QRectF(screenPos, widget->size()).toRect();
}


QRect KUtility::mapWidgetToScene( QGraphicsWidget *widget )
{
	Q_ASSERT(widget);
	Q_ASSERT(widget->scene());

	QGraphicsView *view = graphicsViewFromWidget(widget);
	Q_ASSERT(view);

	QPointF scencePoint = widget->mapToScene(QPointF(0, 0));

	return QRectF(scencePoint, widget->size()).toRect();
}

QPointF KUtility::mapScreenToWidget( const QPoint &pos, QGraphicsWidget *widget )
{
	QGraphicsView *view = graphicsViewFromWidget(widget);
	Q_ASSERT(view);

	QPoint scenePos = view->mapFromGlobal(pos);
	return widget->mapFromScene(scenePos);
}

QGraphicsView * KUtility::graphicsViewFromWidget( QGraphicsWidget *widget )
{
	return widget->scene()->views()[0];
}

void KUtility::openUrlWithPostData( const QString &url, const QString &postData )
{
#if 0
	if (SUCCEEDED(OleInitialize(NULL)))
	{
		IWebBrowser2*    pBrowser2;
		CoCreateInstance(CLSID_InternetExplorer, NULL, CLSCTX_LOCAL_SERVER, IID_IWebBrowser2, (void**)&pBrowser2);
		if (pBrowser2)
		{
			QByteArray utf8Data = postData.toUtf8();

			LPSAFEARRAY psa = SafeArrayCreateVector(VT_UI1, 0, utf8Data.size());
			LPSTR pPostData;
			SafeArrayAccessData(psa, (LPVOID*)&pPostData);
			memcpy(pPostData, utf8Data.data(), utf8Data.size());
			SafeArrayUnaccessData(psa);

			VARIANT vEmpty;
			VariantInit(&vEmpty);

			VARIANT postData;
			//VariantInit(&postData);
			postData.vt = VT_ARRAY | VT_UI1;
			postData.parray = psa;
			VARIANT header;
			//VariantInit(&header);
			header.vt = VT_BSTR;
			header.bstrVal = SysAllocString(L"Content-Type: application/x-www-form-urlencoded;");
			std::wstring urlWstr = url.toStdWString();
			BSTR bstrURL = SysAllocString(urlWstr.c_str());

			HRESULT hr = pBrowser2->Navigate(bstrURL, &vEmpty, &vEmpty, &postData, &header);
			if (SUCCEEDED(hr))
			{
				pBrowser2->put_Visible(VARIANT_TRUE);
			}
			else
			{
				pBrowser2->Quit();
			}

			SafeArrayDestroy(psa);
			SysFreeString(header.bstrVal);
			SysFreeString(bstrURL);
			pBrowser2->Release();
		}
		OleUninitialize();
	}
#endif
}

QPoint KUtility::screenCursorPos()
{
	return QCursor::pos();
	POINT pt;
	GetCursorPos(&pt);
	return QPoint(pt.x, pt.y);
}

bool KUtility::openDirAndSelectFile( const QString& file )
{
	LPSHELLFOLDER pDesktopFolder;
	if (SUCCEEDED(SHGetDesktopFolder( &pDesktopFolder)))
	{
		LPITEMIDLIST pFileIdList, pFolderIdList;
		HRESULT hr;
		QString szFolder, szFile;
		ULONG chEaten, dwAttributes = SFGAO_COMPRESSED;

		szFolder = file;
		szFolder.replace('/', '\\');
		int idx = szFolder.lastIndexOf('\\');
		szFolder.resize(idx);
		QStdWString wszFolder = szFolder.toStdWString();
		hr = pDesktopFolder->ParseDisplayName(NULL, NULL, &wszFolder[0], &chEaten, &pFolderIdList, &dwAttributes);
		if (FAILED(hr))
		{
			pDesktopFolder->Release();
			return false;
		}

		szFile = file;
		szFile.replace('/', '\\');
		QStdWString wszFile = szFile.toStdWString();
		hr = pDesktopFolder->ParseDisplayName(NULL, NULL, &wszFile[0], &chEaten, &pFileIdList, &dwAttributes);
		if (FAILED(hr))
		{
			pDesktopFolder->Release();
			return false;
		}
		hr = SHOpenFolderAndSelectItems( pFolderIdList, 1, (LPCITEMIDLIST*)&pFileIdList, 0);
		pDesktopFolder->Release();
		return true;
	}
	return false;
}

QString KUtility::saveImageToMd5File( const QImage &image, const QString& dstPath )
{
	if ( image.isNull() )
	{
		return QString();
	}

	QString tempPath = dstPath;

	if ( !QFile::exists(tempPath) )
	{
		QDir dir;
		dir.mkpath(tempPath);
	}
	QString fileName = QCryptographicHash::hash( QByteArray( reinterpret_cast<const char*>(image.bits() ), image.byteCount() ),
		QCryptographicHash::Md5 ).toHex();
	QString tempFilePath = tempPath + fileName;

	tempFilePath += ".jpg";

	if ( !QFile::exists(tempFilePath) )
	{
		if ( image.save(tempFilePath, 0, 90) )
		{
			return tempFilePath;
		}
	}
	else
	{
		return tempFilePath;
	}

	return QString();
}

QPoint KUtility::nextCascadesWindowPosition()
{
	static int span = 30;
	static QPoint pt = QPoint(20,20);
	static QPoint tick = QPoint(span, span);
	static QRect workRt;
	if(workRt.isEmpty())
	{
		RECT rt;
		SystemParametersInfo(SPI_GETWORKAREA, 0, &rt, 0);
		workRt.setCoords(rt.left + 20, rt.top + 20, (rt.right + rt.left) * 2 / 3, (rt.bottom + rt.top) * 2 / 3);
	}
	pt += tick;
	if(!workRt.contains(pt))
	{
		if(pt.x() > workRt.right())
		{
			tick.setX(-span);
		}
		else if(pt.x() < workRt.left())
		{
			tick.setX(span);
		}
		if(pt.y() > workRt.bottom())
		{
			tick.setY(-span);
		}
		else if(pt.y() < workRt.top())
		{
			tick.setY(span);
		}
	}
	return pt;
}

bool KUtility::widgetInScreen( QWidget *widget )
{
	if(widget == NULL)
		return false;
	QRect frame = widget->frameGeometry();
	if (!widget->isWindow())
		frame.moveTopLeft(widget->mapToGlobal(QPoint(0,0)));
	QDesktopWidget *desktop = QApplication::desktop();
	int count = desktop->screenCount();

	for(int i = 0; i < count; i++)
	{
		QRect screenRt = desktop->screenGeometry(i);
		if(screenRt.intersects(frame))
			return true;
	}
	return false;
}

QRect KUtility::desktopWorkArea()
{
	RECT r;
	SystemParametersInfo(SPI_GETWORKAREA, 0, &r, 0);
	QRect qr = QRect(QPoint(r.left, r.top), QPoint(r.right - 1, r.bottom - 1));
	return qr;
}

QSize KUtility::virtualScreenSize()
{
	static quint32 lastTime = 0;
	static QSize lastSize = QSize(0, 0); 
	quint32 elapse = ::GetTickCount();
	if((elapse - lastTime) > 1000*1)
	{
		lastTime = elapse;
		lastSize.setWidth(GetSystemMetrics(SM_CXVIRTUALSCREEN));
		lastSize.setHeight(GetSystemMetrics(SM_CYVIRTUALSCREEN));
	}
	return lastSize;
}

QRect KUtility::virtualScreenRect()
{
	static quint32 lastTime = 0;
	static QRect lastScreenRt; 
	quint32 elapse = ::GetTickCount();
	if((elapse - lastTime) > 1000*1)
	{
		int width = GetSystemMetrics(SM_CXVIRTUALSCREEN);
		int height = GetSystemMetrics(SM_CYVIRTUALSCREEN);
		int x = GetSystemMetrics(SM_XVIRTUALSCREEN);
		int y = GetSystemMetrics(SM_YVIRTUALSCREEN);

		lastScreenRt = QRect(x, y, width, height);
	}
	return lastScreenRt;
}


QObject *KUtility::findObject(QObject *parent, const QString &name)
{
	if (parent == NULL)
		return NULL;
	const QObjectList &children = parent->children();
	for (int i = 0; i < children.size(); ++i) 
	{
		QObject *obj = children.at(i);
		if (name.isNull() || obj->objectName() == name)
			return obj;
	}
	for (int i = 0; i < children.size(); ++i)
	{
		QObject *obj = findObject(children.at(i), name);
		if (obj)
		{
			return obj;
		}
	}
	return NULL;
}

QObject * KUtility::findObjectByPath( QObject *objRoot, const QString& path )
{
	QString targetPath = path;
	if(targetPath.isEmpty())
		return NULL;
	if(objRoot == NULL)
		return NULL;
	targetPath.remove('.\\');
	targetPath.remove('./');
	targetPath.remove(' ');
	targetPath.replace('\\', '/');
	QStringList targets = targetPath.split('/');
	if(targets.isEmpty())
		return NULL;

	QObject *tmpRoot = objRoot;
	if(targets.at(0) == ":")
	{
		//表示从路径的根开始查找,即VIEW开始。
		if(!objRoot->isWidgetType())
		{
			KWidget *widget = qobject_cast<KWidget*>(objRoot);
			if(widget == NULL)
			{
				while(tmpRoot->parent())
				{
					tmpRoot = tmpRoot->parent();
				}
			}
			else
			{
				tmpRoot = widget->view();
			}
		}
		targets.removeAt(0);
	}
	QObject *tmpTarget = NULL;
	for(int i = 0; i < targets.count(); i++)
	{
		tmpTarget = findObject(tmpRoot, targets.at(i));
		if(tmpTarget == NULL)
			return NULL;
		tmpRoot = tmpTarget;
	}
	return tmpTarget;
}

QString KUtility::osAppDataPath()
{
	static char buffer[MAX_PATH] = {0};
	if(buffer[0] == '\0')
	{
		SHGetSpecialFolderPathA(NULL, buffer, CSIDL_APPDATA, NULL);
	}
	return QString(buffer);
}

QString KUtility::osTempPath()
{
	char buffer[MAX_PATH] = {0};
	GetTempPathA(MAX_PATH, buffer);
	return QString(buffer);
}

QString KUtility::osSystemPath()
{
	static char buffer[MAX_PATH] = {0};
	if(buffer[0] == '\0')
	{
		GetSystemDirectoryA(buffer, MAX_PATH);
	}
	return QString(buffer);
}

QString KUtility::osDesktopPath()
{
	static char buffer[MAX_PATH] = {0};
	if(buffer[0] == '\0')
	{
		SHGetSpecialFolderPathA(NULL, buffer, CSIDL_DESKTOP, NULL);
	}
	return QString(buffer);
}

QString KUtility::osProgramPath()
{
	static char buffer[MAX_PATH] = {0};
	if(buffer[0] == '\0')
	{
		SHGetSpecialFolderPathA(NULL, buffer, CSIDL_PROGRAM_FILES, NULL);
	}
	return QString(buffer);
}

QString KUtility::osExeInstancePath()
{
	static char buffer[MAX_PATH] = {0};
	if(buffer[0] == '\0')
	{
		GetModuleFileNameA(NULL, buffer, MAX_PATH);
		char *pTmp = strrchr(buffer, '\\');
		*pTmp = '\0';
	}
	return QString(buffer);
}


BOOL KUtility::WtoA(LPCWSTR lpwszSrc, int cch/*=-1*/, std::string &strDst)
{
	if (cch == -1)
		cch = wcslen(lpwszSrc);
	UINT len = (UINT)cch * 2;
	strDst.resize(len, NULL);
	int n = WideCharToMultiByte(CP_ACP, 0, lpwszSrc, cch, &strDst[0], len, NULL, NULL);
	strDst.resize(n);
	return n > 0;
}

BOOL KUtility::AtoW(LPCSTR lpszSrc, int cch/*=-1*/, std::wstring &wstrDst)
{
	if (cch == -1)
		cch = (int)strlen(lpszSrc);

	wstrDst.resize(cch, NULL);
	int n = MultiByteToWideChar(CP_ACP, 0, lpszSrc, cch, &wstrDst[0], cch);
	wstrDst.resize(n);
	return n > 0;
} 



// from winbase.h
#ifndef VER_PLATFORM_WIN32s
#define VER_PLATFORM_WIN32s             0
#endif
#ifndef VER_PLATFORM_WIN32_WINDOWS
#define VER_PLATFORM_WIN32_WINDOWS      1
#endif
#ifndef VER_PLATFORM_WIN32_NT
#define VER_PLATFORM_WIN32_NT           2
#endif
#ifndef VER_PLATFORM_WIN32_CE
#define VER_PLATFORM_WIN32_CE           3
#endif

#ifndef _countof
#define _countof(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif

/*
    This table has been assembled from Usenet postings, personal
    observations, and reading other people's code.  Please feel
    free to add to it or correct it.


         dwPlatFormID  dwMajorVersion  dwMinorVersion  dwBuildNumber
95             1              4               0             950
95 SP1         1              4               0        >950 && <=1080
95 OSR2        1              4             <10           >1080
98             1              4              10            1998
98 SP1         1              4              10       >1998 && <2183
98 SE          1              4              10          >=2183
ME             1              4              90            3000

NT 3.51        2              3              51
NT 4           2              4               0            1381
2000           2              5               0            2195
XP             2              5               1            2600
2003 Server    2              5               2            3790

CE             3

*/

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);
TCHAR * Is64OS(TCHAR *szOSBit, int nBuffCount)
{
	BOOL bIsWow64 = FALSE;
	
	if(64 == sizeof(long))
	{
		_sntprintf(szOSBit,nBuffCount, _T("osbit unknow 64bit programe"));
		return szOSBit;
	}

	LPFN_ISWOW64PROCESS fnIsWow64Process;
	fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(GetModuleHandle(TEXT("kernel32")),("IsWow64Process"));
	if (NULL != fnIsWow64Process)
	{
		if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64))
		{
			// handle error
			_sntprintf(szOSBit,nBuffCount, _T("osbit unknow 1"));
		}
		else
		{
			_sntprintf(szOSBit,nBuffCount, bIsWow64 ? _T("osbit 64"):_T("osbit 32"));
		}
	}
	else
	{
		_sntprintf(szOSBit,nBuffCount, _T("osbit unknow 2"));
	}

	return szOSBit;
}



///////////////////////////////////////////////////////////////////////////////
// GetWinVer
BOOL KUtility::GetWinVer(LPTSTR pszVersion, int nCountVer,LPTSTR pszMajorMinorBuild, int nCountMajor, int *nVersion)
{
	if (!pszVersion || !nVersion || !pszMajorMinorBuild)
		return FALSE;
	lstrcpyn(pszVersion, WUNKNOWNSTR, nCountVer);
	*nVersion = WUNKNOWN;

	OSVERSIONINFO osinfo ={0};
	osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	if (!GetVersionEx(&osinfo))
		return FALSE;

	OSVERSIONINFOEX osviex;

	ZeroMemory(&osviex, sizeof(OSVERSIONINFOEX));
	osviex.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	if( !GetVersionEx ((OSVERSIONINFO *) &osviex)) 
      return FALSE;

	DWORD dwPlatformId   = osinfo.dwPlatformId;
	DWORD dwMinorVersion = osinfo.dwMinorVersion;
	DWORD dwMajorVersion = osinfo.dwMajorVersion;
	DWORD dwBuildNumber  = osinfo.dwBuildNumber & 0xFFFF;	// Win 95 needs this
	
	TCHAR szOSBit[MAX_PATH] = {0};
	_snwprintf(pszMajorMinorBuild,nCountMajor, _T("%u.%u.%u	%s"), dwMajorVersion, dwMinorVersion, dwBuildNumber
			  ,Is64OS(szOSBit, _countof(szOSBit)-1));

	if ((dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) && (dwMajorVersion == 4))
	{
		if ((dwMinorVersion < 10) && (dwBuildNumber == 950))
		{
			lstrcpyn(pszVersion, W95STR,nCountVer);
			*nVersion = W95;
		}
		else if ((dwMinorVersion < 10) && 
				((dwBuildNumber > 950) && (dwBuildNumber <= 1080)))
		{
			lstrcpyn(pszVersion, W95SP1STR, nCountVer);
			*nVersion = W95SP1;
		}
		else if ((dwMinorVersion < 10) && (dwBuildNumber > 1080))
		{
			lstrcpyn(pszVersion, W95OSR2STR, nCountVer);
			*nVersion = W95OSR2;
		}
		else if ((dwMinorVersion == 10) && (dwBuildNumber == 1998))
		{
			lstrcpyn(pszVersion, W98STR, nCountVer);
			*nVersion = W98;
		}
		else if ((dwMinorVersion == 10) && 
				((dwBuildNumber > 1998) && (dwBuildNumber < 2183)))
		{
			lstrcpyn(pszVersion, W98SP1STR, nCountVer);
			*nVersion = W98SP1;
		}
		else if ((dwMinorVersion == 10) && (dwBuildNumber >= 2183))
		{
			lstrcpyn(pszVersion, W98SESTR, nCountVer);
			*nVersion = W98SE;
		}
		else if (dwMinorVersion == 90)
		{
			lstrcpyn(pszVersion, WMESTR, nCountVer);
			*nVersion = WME;
		}
	}
	else if (dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		if ((dwMajorVersion == 3) && (dwMinorVersion == 51))
		{
			lstrcpyn(pszVersion, WNT351STR, nCountVer);
			*nVersion = WNT351;
		}
		else if ((dwMajorVersion == 4) && (dwMinorVersion == 0))
		{
			lstrcpyn(pszVersion, WNT4STR, nCountVer);
			*nVersion = WNT4;
		}
		else if ((dwMajorVersion == 5) && (dwMinorVersion == 0))
		{
			lstrcpyn(pszVersion, W2KSTR, nCountVer);
			*nVersion = W2K;
		}
		else if ((dwMajorVersion == 5) && (dwMinorVersion == 1))
		{
			lstrcpyn(pszVersion, WXPSTR, nCountVer);
			*nVersion = WXP;
		}
		else if ((dwMajorVersion == 5) && (dwMinorVersion == 2) && GetSystemMetrics(SM_SERVERR2) != 0)
		{
			lstrcpyn(pszVersion, W2003SERVERSTR, nCountVer);
			*nVersion = W2003SERVER;
		}
		else if ((dwMajorVersion == 5) && (dwMinorVersion == 2) && GetSystemMetrics(SM_SERVERR2) == 0)
		{
			lstrcpyn(pszVersion, W2003SERVERST, nCountVer);
			*nVersion = W2003SERVE;
		}
		else if( (dwMajorVersion == 6) && (dwMinorVersion == 0) && (osviex.wProductType == VER_NT_WORKSTATION ) )
		{	
			lstrcpyn(pszVersion, WINVISTA, nCountVer);
			*nVersion = WVISTA;
		}
		else if( (dwMajorVersion == 6) && (dwMinorVersion == 0) && (osviex.wProductType != VER_NT_WORKSTATION ) )
		{	
			lstrcpyn(pszVersion, W2008SERVERST, nCountVer);
			*nVersion = W2008SERVE;
		}
		else if( (dwMajorVersion == 6) && (dwMinorVersion == 1) && (osviex.wProductType == VER_NT_WORKSTATION ) )
		{	
			lstrcpyn(pszVersion, WIN7, nCountVer);
			*nVersion = W7;
		}
		else if( (dwMajorVersion == 6) && (dwMinorVersion == 1) && (osviex.wProductType != VER_NT_WORKSTATION ) )
		{	
			lstrcpyn(pszVersion, W2008SERVERSTR, nCountVer);
			*nVersion = W2008SERVER;
		}
	}


	else if (dwPlatformId == VER_PLATFORM_WIN32_CE)
	{
		lstrcpyn(pszVersion, WCESTR, nCountVer);
		*nVersion = WCE;
	}
	return TRUE;
}

quint32 KUtility::crc32( quint32 crc, const quint8 *buf, qint32 len )
{
	static const unsigned long crc32_table[256] = 
	{ 
		0x00000000,   0x77073096,   0xee0e612c,   0x990951ba,   0x076dc419, 
		0x706af48f,   0xe963a535,   0x9e6495a3,   0x0edb8832,   0x79dcb8a4, 
		0xe0d5e91e,   0x97d2d988,   0x09b64c2b,   0x7eb17cbd,   0xe7b82d07, 
		0x90bf1d91,   0x1db71064,   0x6ab020f2,   0xf3b97148,   0x84be41de, 
		0x1adad47d,   0x6ddde4eb,   0xf4d4b551,   0x83d385c7,   0x136c9856, 
		0x646ba8c0,   0xfd62f97a,   0x8a65c9ec,   0x14015c4f,   0x63066cd9, 
		0xfa0f3d63,   0x8d080df5,   0x3b6e20c8,   0x4c69105e,   0xd56041e4, 
		0xa2677172,   0x3c03e4d1,   0x4b04d447,   0xd20d85fd,   0xa50ab56b, 
		0x35b5a8fa,   0x42b2986c,   0xdbbbc9d6,   0xacbcf940,   0x32d86ce3, 
		0x45df5c75,   0xdcd60dcf,   0xabd13d59,   0x26d930ac,   0x51de003a, 
		0xc8d75180,   0xbfd06116,   0x21b4f4b5,   0x56b3c423,   0xcfba9599, 
		0xb8bda50f,   0x2802b89e,   0x5f058808,   0xc60cd9b2,   0xb10be924, 
		0x2f6f7c87,   0x58684c11,   0xc1611dab,   0xb6662d3d,   0x76dc4190, 
		0x01db7106,   0x98d220bc,   0xefd5102a,   0x71b18589,   0x06b6b51f, 
		0x9fbfe4a5,   0xe8b8d433,   0x7807c9a2,   0x0f00f934,   0x9609a88e, 
		0xe10e9818,   0x7f6a0dbb,   0x086d3d2d,   0x91646c97,   0xe6635c01, 
		0x6b6b51f4,   0x1c6c6162,   0x856530d8,   0xf262004e,   0x6c0695ed, 
		0x1b01a57b,   0x8208f4c1,   0xf50fc457,   0x65b0d9c6,   0x12b7e950, 
		0x8bbeb8ea,   0xfcb9887c,   0x62dd1ddf,   0x15da2d49,   0x8cd37cf3, 
		0xfbd44c65,   0x4db26158,   0x3ab551ce,   0xa3bc0074,   0xd4bb30e2, 
		0x4adfa541,   0x3dd895d7,   0xa4d1c46d,   0xd3d6f4fb,   0x4369e96a, 
		0x346ed9fc,   0xad678846,   0xda60b8d0,   0x44042d73,   0x33031de5, 
		0xaa0a4c5f,   0xdd0d7cc9,   0x5005713c,   0x270241aa,   0xbe0b1010, 
		0xc90c2086,   0x5768b525,   0x206f85b3,   0xb966d409,   0xce61e49f, 
		0x5edef90e,   0x29d9c998,   0xb0d09822,   0xc7d7a8b4,   0x59b33d17, 
		0x2eb40d81,   0xb7bd5c3b,   0xc0ba6cad,   0xedb88320,   0x9abfb3b6, 
		0x03b6e20c,   0x74b1d29a,   0xead54739,   0x9dd277af,   0x04db2615, 
		0x73dc1683,   0xe3630b12,   0x94643b84,   0x0d6d6a3e,   0x7a6a5aa8, 
		0xe40ecf0b,   0x9309ff9d,   0x0a00ae27,   0x7d079eb1,   0xf00f9344, 
		0x8708a3d2,   0x1e01f268,   0x6906c2fe,   0xf762575d,   0x806567cb, 
		0x196c3671,   0x6e6b06e7,   0xfed41b76,   0x89d32be0,   0x10da7a5a, 
		0x67dd4acc,   0xf9b9df6f,   0x8ebeeff9,   0x17b7be43,   0x60b08ed5, 
		0xd6d6a3e8,   0xa1d1937e,   0x38d8c2c4,   0x4fdff252,   0xd1bb67f1, 
		0xa6bc5767,   0x3fb506dd,   0x48b2364b,   0xd80d2bda,   0xaf0a1b4c, 
		0x36034af6,   0x41047a60,   0xdf60efc3,   0xa867df55,   0x316e8eef, 
		0x4669be79,   0xcb61b38c,   0xbc66831a,   0x256fd2a0,   0x5268e236, 
		0xcc0c7795,   0xbb0b4703,   0x220216b9,   0x5505262f,   0xc5ba3bbe, 
		0xb2bd0b28,   0x2bb45a92,   0x5cb36a04,   0xc2d7ffa7,   0xb5d0cf31, 
		0x2cd99e8b,   0x5bdeae1d,   0x9b64c2b0,   0xec63f226,   0x756aa39c, 
		0x026d930a,   0x9c0906a9,   0xeb0e363f,   0x72076785,   0x05005713, 
		0x95bf4a82,   0xe2b87a14,   0x7bb12bae,   0x0cb61b38,   0x92d28e9b, 
		0xe5d5be0d,   0x7cdcefb7,   0x0bdbdf21,   0x86d3d2d4,   0xf1d4e242, 
		0x68ddb3f8,   0x1fda836e,   0x81be16cd,   0xf6b9265b,   0x6fb077e1, 
		0x18b74777,   0x88085ae6,   0xff0f6a70,   0x66063bca,   0x11010b5c, 
		0x8f659eff,   0xf862ae69,   0x616bffd3,   0x166ccf45,   0xa00ae278, 
		0xd70dd2ee,   0x4e048354,   0x3903b3c2,   0xa7672661,   0xd06016f7, 
		0x4969474d,   0x3e6e77db,   0xaed16a4a,   0xd9d65adc,   0x40df0b66, 
		0x37d83bf0,   0xa9bcae53,   0xdebb9ec5,   0x47b2cf7f,   0x30b5ffe9, 
		0xbdbdf21c,   0xcabac28a,   0x53b39330,   0x24b4a3a6,   0xbad03605, 
		0xcdd70693,   0x54de5729,   0x23d967bf,   0xb3667a2e,   0xc4614ab8, 
		0x5d681b02,   0x2a6f2b94,   0xb40bbe37,   0xc30c8ea1,   0x5a05df1b, 
		0x2d02ef8d 
	}; 

	const unsigned char *end; 
	crc = ~crc & 0xffffffff; 
	for (end = buf + len; buf < end; ++buf) 
		crc = crc32_table[(crc ^ *buf) & 0xff] ^ (crc >> 8); 
	return ~crc & 0xffffffff; 
}

QByteArray KUtility::fragmentsToByteArray( const QList<QPair<QString,QTextCharFormat>>& frags )
{
	QList<QByteArray> lstFrags;
	for(int i = 0; i < frags.count(); i++)
	{
		const QPair<QString,QTextCharFormat> &frag = frags.at(i);
		QString txt = frag.first;
		QTextCharFormat txtfmt = frag.second;
		QByteArray buf;
		QDataStream ds(&buf, QIODevice::WriteOnly);
		ds << txt << txtfmt;
		lstFrags.push_back(buf);
	}
	QByteArray fragsBuffer;
	QDataStream dataStream(&fragsBuffer, QIODevice::WriteOnly);
	dataStream << lstFrags;
	return fragsBuffer;
}

QList<QPair<QString,QTextCharFormat>> KUtility::fragmentsFromByteArray( const QByteArray& bytes )
{
	QList<QPair<QString,QTextCharFormat>> frags;
	QDataStream ds(bytes);
	QList<QByteArray> lstFrags;
	ds >> lstFrags;
	for(int i = 0; i < lstFrags.count(); i++)
	{
		const QByteArray frag = lstFrags.at(i);
		QDataStream ds(frag);
		QString text;
		QTextCharFormat format;
		ds >> text >> format;
		frags.push_back(QPair<QString,QTextCharFormat>(text, format));
	}
	return frags;
}


#ifdef _DEBUG
#define VS2008_MANIFEST_DIR_NAME "Microsoft.VC90.DebugCRT"
#else
#define VS2008_MANIFEST_DIR_NAME "Microsoft.VC90.CRT"
#endif

void KUtility::CopyVS2008ManifestFiles(const QString& dstPath)
{
	QDir dstDir(dstPath + "/" + VS2008_MANIFEST_DIR_NAME);

	if (!dstDir.exists())
	{		
		if(!dstDir.mkpath("."))
		{
			qDebug("crt dstDir mk fail...\n");
			return;
		}
	}

	QDir srcDir(QApplication::applicationDirPath() + "/" + VS2008_MANIFEST_DIR_NAME);
	if (!srcDir.exists())
	{
		qDebug("crt src is null...\n");
		return;
	}
	QStringList list = srcDir.entryList(QDir::AllEntries|QDir::NoDot|QDir::NoDotDot);
	//qDebug(QString("crt src file count is %1 \n").arg(list.count()));
	for (int i = 0; i < list.count(); ++i)
	{
		if(!dstDir.exists(dstDir.absoluteFilePath(list[i])))
		{
			QFile::copy(srcDir.absoluteFilePath(list[i]), dstDir.absoluteFilePath(list[i]));
		}
	}
}

void KUtility::addPathToEnvironment( const QString& path )
{
	const int bufLength = 32767+1;
	char* oldPathBuffer = new char[bufLength];
	::GetEnvironmentVariableA("path",oldPathBuffer,bufLength);
	QString szEnvPath(oldPathBuffer);
	QStringList lstEvn = szEnvPath.split(';');
	if(!lstEvn.contains(path, Qt::CaseInsensitive))
	{
		QString dir = szEnvPath + ";" + path;
		std::wstring pathEnvWstr = dir.toStdWString();
		::SetEnvironmentVariable(L"path", pathEnvWstr.c_str());
		qputenv("path", dir.toStdString().c_str());
	}
}

void KUtility::addVS2008ManifestFilesToEnvironment()
{
	QString srcDir = QApplication::applicationDirPath() + "/" + VS2008_MANIFEST_DIR_NAME;
	addPathToEnvironment(srcDir);
}

void KUtility::addParameterToEnvironment( const QString& name, const QString& val )
{
	::SetEnvironmentVariable(name.toStdWString().c_str(), val.toStdWString().c_str());
	qputenv(name.toStdString().c_str(), val.toStdString().c_str());
}

void KUtility::drawOneTopLeft( QPainter *painter, QRectF& itemRt, const QPixmap& pixmap )
{
	QRectF srcRt(0, 0, pixmap.width(), pixmap.height());
	painter->drawPixmap(itemRt.topLeft(), pixmap, srcRt);
}

void KUtility::drawOneTopRight( QPainter *painter, QRectF& itemRt, const QPixmap& pixmap )
{
	QRectF srcRt(0, 0, pixmap.width(), pixmap.height());
	painter->drawPixmap(itemRt.topLeft() + QPointF(itemRt.width() - srcRt.width(), 0), pixmap, srcRt);
}

void KUtility::drawOneBotLeft( QPainter *painter, QRectF& itemRt, const QPixmap& pixmap )
{
	QRectF srcRt(0, 0, pixmap.width(), pixmap.height());

	painter->drawPixmap(itemRt.topLeft() + QPointF(0, itemRt.height() - srcRt.height()), pixmap, srcRt);
}

void KUtility::drawOneBotRight( QPainter *painter, QRectF& itemRt, const QPixmap& pixmap )
{
	QRectF srcRt(0, 0, pixmap.width(), pixmap.height());

	painter->drawPixmap(itemRt.topLeft() + QPointF(itemRt.width() - srcRt.width(), itemRt.height() - srcRt.height()), pixmap, srcRt);
}

void KUtility::drawOneHCenter( QPainter *painter, QRectF& itemRt, const QPixmap& pixmap )
{
	QRectF srcRt(0, 0, pixmap.width(), pixmap.height());

	painter->drawPixmap(itemRt.topLeft() + QPointF((itemRt.width() - srcRt.width()) / 2, 0), pixmap, srcRt);
}

void KUtility::drawOneVCenter( QPainter *painter, QRectF& itemRt, const QPixmap& pixmap )
{
	QRectF srcRt(0, 0, pixmap.width(), pixmap.height());

	painter->drawPixmap(itemRt.topLeft() + QPointF(0, (itemRt.height() - srcRt.height()) / 2), pixmap, srcRt);	
}

void KUtility::drawOneHVCenter( QPainter *painter, QRectF& itemRt, const QPixmap& pixmap )
{
	QRectF srcRt(0, 0, pixmap.width(), pixmap.height());
	
	painter->drawPixmap(itemRt.topLeft() + QPointF((itemRt.width() - srcRt.width()) / 2, (itemRt.height() - srcRt.height()) / 2), pixmap, srcRt);
}

void KUtility::drawTileH( QPainter *painter, QRectF& itemRt, const QPixmap& pixmap )
{
	QRectF srcRt(0, 0, pixmap.width(), pixmap.height());

	int col = itemRt.width() / srcRt.width() + 1;
	QRectF drawRt(0, 0, srcRt.width(), srcRt.height());
	drawRt.translate(itemRt.topLeft());
	for(int c = 0; c < col; c++)
	{
		painter->drawPixmap(drawRt, pixmap, QRectF(srcRt.left(), srcRt.top(), drawRt.width(), drawRt.height()));
		drawRt.translate(srcRt.width(), 0);
	}
}

void KUtility::drawTileV( QPainter *painter, QRectF& itemRt, const QPixmap& pixmap )
{
	QRectF srcRt(0, 0, pixmap.width(), pixmap.height());

	int row = itemRt.height() / srcRt.height() + 1;
	QRectF drawRt(0, 0, srcRt.width(), srcRt.height());
	drawRt.translate(itemRt.topLeft());
	for(int r = 0; r < row; r++)
	{
		painter->drawPixmap(drawRt, pixmap, QRectF(srcRt.left(), srcRt.top(), drawRt.width(), drawRt.height()));
		drawRt.translate(0, srcRt.height());
	}
}

void KUtility::drawTileHV( QPainter *painter, QRectF& itemRt, const QPixmap& pixmap )
{
	QRectF srcRt(0, 0, pixmap.width(), pixmap.height());

	int row = itemRt.height() / srcRt.height() + 1;
	int col = itemRt.width() / srcRt.width() + 1;
	for(int r = 0; r < row; r++)
	{
		QRectF drawRt(0, r * srcRt.height(), srcRt.width(), srcRt.height());
		drawRt.translate(itemRt.topLeft());
		for(int c = 0; c < col; c++)
		{
			painter->drawPixmap(drawRt, pixmap, QRectF(srcRt.left(), srcRt.top(), drawRt.width(), drawRt.height()));
			drawRt.translate(srcRt.width(), 0);
		}
	}
}

void KUtility::drawStretchH( QPainter *painter, QRectF& itemRt, const QPixmap& pixmap, bool bsmooth )
{
	QRectF srcRt(0, 0, pixmap.width(), pixmap.height());
	QRectF drawRt(0, 0, itemRt.width(), srcRt.height());
	drawRt.translate(itemRt.topLeft());
	painter->setRenderHint(QPainter::SmoothPixmapTransform, bsmooth);
	painter->drawPixmap(drawRt, pixmap, srcRt);
	painter->setRenderHint(QPainter::SmoothPixmapTransform, !bsmooth);
}

void KUtility::drawStretchV( QPainter *painter, QRectF& itemRt, const QPixmap& pixmap, bool bsmooth )
{
	QRectF srcRt(0, 0, pixmap.width(), pixmap.height());
	QRectF drawRt(0, 0, pixmap.width(), itemRt.height());
	drawRt.translate(itemRt.topLeft());
	painter->setRenderHint(QPainter::SmoothPixmapTransform, bsmooth);
	painter->drawPixmap(drawRt, pixmap, srcRt);
	painter->setRenderHint(QPainter::SmoothPixmapTransform, !bsmooth);
}

void KUtility::drawStretchHV( QPainter *painter, QRectF& itemRt, const QPixmap& pixmap, bool bsmooth )
{
	QRectF srcRt(0, 0, pixmap.width(), pixmap.height());
	QRectF drawRt(0, 0, itemRt.width(), itemRt.height());
	drawRt.translate(itemRt.topLeft());
	painter->setRenderHint(QPainter::SmoothPixmapTransform, bsmooth);
	painter->drawPixmap(drawRt, pixmap, srcRt);
	painter->setRenderHint(QPainter::SmoothPixmapTransform, !bsmooth);	
}

void KUtility::drawStretchHTwo( QPainter *painter, QRectF& itemRt, const QPixmap& pixmap, bool bsmooth )
{
	QRectF srcRt(0, 0, pixmap.width(), pixmap.height());
	QRectF drawRt;
	drawRt.setRect((itemRt.width() - pixmap.width()) / 2, 0, pixmap.width(), pixmap.height());
	srcRt.setRect(0, 0, pixmap.width(), pixmap.height());
	drawRt.translate(itemRt.topLeft());
	painter->drawPixmap(drawRt, pixmap, srcRt);
	
	drawRt.setRect(0, 0, (itemRt.width() - pixmap.width()) / 2, pixmap.height());
	if(!drawRt.isEmpty())
	{
		srcRt.setRect(0, 0, 1, pixmap.height());
		drawRt.translate(itemRt.topLeft());
		painter->drawPixmap(drawRt, pixmap, srcRt);
	}
	drawRt.setRect((itemRt.width() + pixmap.width()) / 2 , 0, itemRt.width(), pixmap.height());
	if(!drawRt.isEmpty())
	{
		srcRt.setRect(pixmap.width() - 1, 0, 1, pixmap.height());
		drawRt.translate(itemRt.topLeft());
		painter->drawPixmap(drawRt, pixmap, srcRt);
	}
}

void KUtility::drawStretchHMiddle( QPainter *painter, QRectF& itemRt, const QPixmap& pixmap, bool bsmooth)
{
	QRectF srcRt(0, 0, pixmap.width()/2, pixmap.height());
	QRectF drawRt;
	drawRt = srcRt;
	drawRt.translate(itemRt.topLeft());
	painter->drawPixmap(drawRt, pixmap, srcRt);
	srcRt.setRect(pixmap.width()/2, 0, pixmap.width() - pixmap.width()/2, pixmap.height());
	drawRt.setRect(itemRt.width()-srcRt.width(), 0, srcRt.width(), srcRt.height());
	drawRt.translate(itemRt.topLeft());
	painter->drawPixmap(drawRt, pixmap, srcRt);
	srcRt.setRect(pixmap.width()/2,0,1,pixmap.height());
	drawRt.setRect(pixmap.width()/2, 0, itemRt.width()-pixmap.width(), pixmap.height());
	if(!drawRt.isEmpty())
	{
		drawRt.translate(itemRt.topLeft());
		painter->drawPixmap(drawRt, pixmap, srcRt);
	}
}

void KUtility::drawStretchVTwo( QPainter *painter, QRectF& itemRt, const QPixmap& pixmap, bool bsmooth )
{
	QRectF srcRt(0, 0, pixmap.width(), pixmap.height());
	QRectF drawRt;
	drawRt.setRect(0, (itemRt.height() - pixmap.height()) / 2, pixmap.width(), pixmap.height());
	srcRt.setRect(0, 0, pixmap.width(), pixmap.height());
	drawRt.translate(itemRt.topLeft());
	painter->drawPixmap(drawRt, pixmap, srcRt);

	drawRt.setRect(0, 0, pixmap.width(), (itemRt.height() - pixmap.height()) / 2);
	if(!drawRt.isEmpty())
	{
		srcRt.setRect(0, 0, pixmap.width(), 1);
		drawRt.translate(itemRt.topLeft());
		painter->drawPixmap(drawRt, pixmap, srcRt);
	}
	drawRt.setRect(0, (itemRt.height() + pixmap.height()) / 2 , pixmap.width(), itemRt.height()-pixmap.height());
	if(!drawRt.isEmpty())
	{
		srcRt.setRect(0, pixmap.height()-1, pixmap.width(), 1);
		drawRt.translate(itemRt.topLeft());
		painter->drawPixmap(drawRt, pixmap, srcRt);
	}
}

void KUtility::drawStretchVMiddle( QPainter *painter, QRectF& itemRt, const QPixmap& pixmap, bool bsmooth )
{
	QRectF srcRt(0, 0, pixmap.width(), pixmap.height()/2);
	QRectF drawRt;
	drawRt = srcRt;
	drawRt.translate(itemRt.topLeft());
	painter->drawPixmap(drawRt, pixmap, srcRt);
	srcRt.setRect(0, pixmap.height()/2, pixmap.width(), pixmap.height()-pixmap.height()/2);
	drawRt.setRect(0, itemRt.height()-srcRt.height(), srcRt.width(), srcRt.height());
	drawRt.translate(itemRt.topLeft());
	painter->drawPixmap(drawRt, pixmap, srcRt);
	drawRt.setRect(0, pixmap.height()/2, pixmap.width(), itemRt.height()-pixmap.height());
	if(!drawRt.isEmpty())
	{
		srcRt.setRect(0, pixmap.height()/2, pixmap.width(), 1);
		drawRt.translate(itemRt.topLeft());
		painter->drawPixmap(drawRt, pixmap, srcRt);
	}
}

void KUtility::drawGridFour( QPainter *painter, QRectF& itemRt, const QPixmap& pixmap )
{
	QRectF srcRt(0, 0, pixmap.width(), pixmap.height());
	QRect dstRt = itemRt.toRect();
	QRect fromRt = srcRt.toRect();
	QRect blockRt;
	QRect drawRt;
	/*左上角*/
	blockRt.setRect(fromRt.left(), fromRt.top(), fromRt.width() / 2, fromRt.height() / 2);
	drawRt.setRect(dstRt.left(), dstRt.top(), blockRt.width(), blockRt.height());
	painter->drawPixmap(drawRt, pixmap, blockRt);
	/*上部中间*/
	blockRt.setRect(fromRt.left() + fromRt.width() / 2, fromRt.top(), 1, fromRt.height() / 2);
	drawRt.setRect(dstRt.left() + fromRt.width() / 2, dstRt.top(), dstRt.width() - fromRt.width(), blockRt.height());
	painter->drawPixmap(drawRt, pixmap, blockRt);
	/*右上角*/
	blockRt.setRect(fromRt.left() + fromRt.width() / 2, fromRt.top(), fromRt.width() - fromRt.width() / 2, fromRt.height() / 2);
	drawRt.setRect(dstRt.right() + 1 - blockRt.width(), dstRt.top(), blockRt.width(), blockRt.height());
	painter->drawPixmap(drawRt, pixmap, blockRt);
	/*左边中部*/
	blockRt.setRect(fromRt.left(), fromRt.top() + fromRt.height() / 2, fromRt.width() / 2, 1);
	drawRt.setRect(dstRt.left(), dstRt.top() + fromRt.height() / 2, blockRt.width(), dstRt.height() - fromRt.height());
	painter->drawPixmap(drawRt, pixmap, blockRt);
	/*中部*/
	blockRt.setRect(fromRt.left() + fromRt.width() / 2, fromRt.top() + fromRt.height() / 2, 1, 1);
	drawRt.setRect(dstRt.left() + fromRt.width() / 2, dstRt.top() + fromRt.height() / 2, dstRt.width() - fromRt.width(), dstRt.height() - fromRt.height());
	painter->drawPixmap(drawRt, pixmap, blockRt);
	/*右边中部*/
	blockRt.setRect(fromRt.left() + fromRt.width() / 2, fromRt.top() + fromRt.height() / 2, fromRt.width() - fromRt.width() / 2, 1);
	drawRt.setRect(dstRt.right() + 1 - blockRt.width(), dstRt.top() + fromRt.height() / 2, blockRt.width(), dstRt.height() - fromRt.height());
	painter->drawPixmap(drawRt, pixmap, blockRt);
	/*左下角*/
	blockRt.setRect(fromRt.left(), fromRt.top() + fromRt.height() / 2, fromRt.width() / 2, fromRt.height() - fromRt.height() / 2);
	drawRt.setRect(dstRt.left(), dstRt.bottom() + 1 - blockRt.height(), blockRt.width(), blockRt.height());
	painter->drawPixmap(drawRt, pixmap, blockRt);
	/*右下中部*/
	blockRt.setRect(fromRt.left() + fromRt.width() / 2, fromRt.top() + fromRt.height() / 2, 1, fromRt.height() - fromRt.height() / 2);
	drawRt.setRect(dstRt.left() + fromRt.width() / 2, dstRt.bottom() + 1 - blockRt.height(), dstRt.width() - fromRt.width(), blockRt.height());
	painter->drawPixmap(drawRt, pixmap, blockRt);
	/*右下角*/
	blockRt.setRect(fromRt.left() + fromRt.width() / 2, fromRt.top() + fromRt.height() / 2, fromRt.width() - fromRt.width() / 2, fromRt.height() - fromRt.height() / 2);
	drawRt.setRect(dstRt.right() + 1 - blockRt.width(), dstRt.bottom() + 1 - blockRt.height(), blockRt.width(), blockRt.height());
	painter->drawPixmap(drawRt, pixmap, blockRt);
}

void KUtility::drawGridFrameFour( QPainter *painter, QRectF& itemRt, const QPixmap& pixmap )
{
	QRectF srcRt(0, 0, pixmap.width(), pixmap.height());
	QRect dstRt = itemRt.toRect();
	QRect fromRt = srcRt.toRect();
	QRect blockRt;
	QRect drawRt;
	/*左上角*/
	blockRt.setRect(fromRt.left(), fromRt.top(), fromRt.width() / 2, fromRt.height() / 2);
	drawRt.setRect(dstRt.left(), dstRt.top(), blockRt.width(), blockRt.height());
	painter->drawPixmap(drawRt, pixmap, blockRt);
	/*上部中间*/
	blockRt.setRect(fromRt.left() + fromRt.width() / 2, fromRt.top(), 1, fromRt.height() / 2);
	drawRt.setRect(dstRt.left() + fromRt.width() / 2, dstRt.top(), dstRt.width() - fromRt.width(), blockRt.height());
	painter->drawPixmap(drawRt, pixmap, blockRt);
	/*右上角*/
	blockRt.setRect(fromRt.left() + fromRt.width() / 2, fromRt.top(), fromRt.width() - fromRt.width() / 2, fromRt.height() / 2);
	drawRt.setRect(dstRt.right() + 1 - blockRt.width(), dstRt.top(), blockRt.width(), blockRt.height());
	painter->drawPixmap(drawRt, pixmap, blockRt);
	/*左边中部*/
	blockRt.setRect(fromRt.left(), fromRt.top() + fromRt.height() / 2, fromRt.width() / 2, 1);
	drawRt.setRect(dstRt.left(), dstRt.top() + fromRt.height() / 2, blockRt.width(), dstRt.height() - fromRt.height());
	painter->drawPixmap(drawRt, pixmap, blockRt);
/*
	/ *中部* /
	blockRt.setRect(fromRt.left() + fromRt.width() / 2, fromRt.top() + fromRt.height() / 2, 1, 1);
	drawRt.setRect(dstRt.left() + fromRt.width() / 2, dstRt.top() + fromRt.height() / 2, dstRt.width() - fromRt.width(), dstRt.height() - fromRt.height());
	painter->drawPixmap(drawRt, pixmap, blockRt);
*/
	/*右边中部*/
	blockRt.setRect(fromRt.left() + fromRt.width() / 2, fromRt.top() + fromRt.height() / 2, fromRt.width() - fromRt.width() / 2, 1);
	drawRt.setRect(dstRt.right() + 1 - blockRt.width(), dstRt.top() + fromRt.height() / 2, blockRt.width(), dstRt.height() - fromRt.height());
	painter->drawPixmap(drawRt, pixmap, blockRt);
	/*左下角*/
	blockRt.setRect(fromRt.left(), fromRt.top() + fromRt.height() / 2, fromRt.width() / 2, fromRt.height() - fromRt.height() / 2);
	drawRt.setRect(dstRt.left(), dstRt.bottom() + 1 - blockRt.height(), blockRt.width(), blockRt.height());
	painter->drawPixmap(drawRt, pixmap, blockRt);
	/*右下中部*/
	blockRt.setRect(fromRt.left() + fromRt.width() / 2, fromRt.top() + fromRt.height() / 2, 1, fromRt.height() - fromRt.height() / 2);
	drawRt.setRect(dstRt.left() + fromRt.width() / 2, dstRt.bottom() + 1 - blockRt.height(), dstRt.width() - fromRt.width(), blockRt.height());
	painter->drawPixmap(drawRt, pixmap, blockRt);
	/*右下角*/
	blockRt.setRect(fromRt.left() + fromRt.width() / 2, fromRt.top() + fromRt.height() / 2, fromRt.width() - fromRt.width() / 2, fromRt.height() - fromRt.height() / 2);
	drawRt.setRect(dstRt.right() + 1 - blockRt.width(), dstRt.bottom() + 1 - blockRt.height(), blockRt.width(), blockRt.height());
	painter->drawPixmap(drawRt, pixmap, blockRt);
}

void KUtility::drawGridNine( QPainter *painter, QRectF& itemRt, const QPixmap& pixmap )
{
	QRectF srcRt(0, 0, pixmap.width(), pixmap.height());

	QRect dstRt = itemRt.toRect();
	QRect fromRt = srcRt.toRect();
	QMargins dstMargin(dstRt.width() / 3, dstRt.height() / 3, dstRt.width() / 3, dstRt.height() / 3);
	QMargins srcMargin(fromRt.width() / 3, fromRt.height() / 3, fromRt.width() / 3, fromRt.height() / 3);
	qDrawBorderPixmap(painter, dstRt, dstMargin, pixmap, fromRt, srcMargin, QTileRules(Qt::StretchTile));
}

void KUtility::osEnumFiles( const QString& path, QFileInfoList& fils, QStringList& fls )
{
	QDir dir(path);
	QFileInfoList fil = dir.entryInfoList(QDir::AllEntries);
	for(int i = 0; i < fil.count(); i++)
	{
		const QFileInfo &fi = fil.at(i);
		QString path = fi.absoluteFilePath();
		if(fi.isDir())
		{
			osEnumFiles(path, fils, fls);
		}
		else
		{
			fils.push_back(fi);
			fls.push_back(path);
		}
	}
}

bool KUtility::osCreateDirectory( const QString& path, bool isFile )
{
	QString mydir = path;
	mydir = QDir::cleanPath(mydir);
	mydir = QDir::toNativeSeparators(mydir);
	if(isFile)
	{
		int idx = mydir.lastIndexOf('\\');
		mydir = mydir.left(idx);
	}
	int iret = SHCreateDirectoryExA(NULL, mydir.toStdString().c_str(), NULL);
	switch(iret)
	{
	case ERROR_SUCCESS:
	case ERROR_FILE_EXISTS:
	case ERROR_ALREADY_EXISTS:
		return true;
	}
	return false;
}

void KUtility::osRemoveDirectory( const QString& path, bool itself/*=true*/ )
{
	QString mydir = path;
	mydir = QDir::cleanPath(mydir);
	mydir = QDir::toNativeSeparators(mydir);

	QString strTemp = mydir + "\\*.*";

	WIN32_FIND_DATAA fdFind;
	HANDLE hFind = ::FindFirstFileA(strTemp.toStdString().c_str(), &fdFind);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			QString strFile = fdFind.cFileName;
			if(strFile == "." || strFile == "..")
			{
				continue;
			}
			else if (fdFind.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				// 如果是文件夹的话，采用递归删除
				QString strDir = path + "\\" + strFile;
				osRemoveDirectory(strDir, false);
				::RemoveDirectoryA(strDir.toStdString().c_str());
				continue;
			}

			QString strFilePath = path + "\\" + strFile;
			::DeleteFileA(strFilePath.toStdString().c_str());
		}
		while (::FindNextFileA(hFind, &fdFind));

		::FindClose(hFind);
	}
	if(itself)
	{
		::RemoveDirectoryA(path.toStdString().c_str());
	}
}

#define FOLDER_QUICKLAUNCH ("%APPDATA%\\Microsoft\\Internet Explorer\\Quick Launch")

bool KUtility::createDesktopShortcut( const QString& file, const QString& name )
{
	LPITEMIDLIST lpidl;
	if(FAILED(SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOPDIRECTORY, &lpidl)))
	{
		return false;
	}

	char strDir[MAX_PATH] = {0};
	if(!SHGetPathFromIDListA(lpidl, strDir))
	{
		return false;
	}

	QString strPath = QString(strDir) + "\\" + name + ".lnk";
	return createSpecifiedShortcut(strPath, file);
}

bool KUtility::createQuickLaunchShortcut( const QString& file, const QString& name )
{
	char szPath[MAX_PATH + 1] = { 0 };
	if(!ExpandEnvironmentStringsA(FOLDER_QUICKLAUNCH, szPath, MAX_PATH))
	{
		return false;
	}

	QString strDstPath = QString(szPath) + "\\" + name + ".lnk";

	return createSpecifiedShortcut(strDstPath, file);
}

bool KUtility::createProgramShortcut( const QString& file, const QString& name, const QString& subdir )
{
	LPITEMIDLIST lpidl;
	if(FAILED(SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAMS, &lpidl)))
	{
		return false;
	}

	char strDir[MAX_PATH] = {0};
	if(!SHGetPathFromIDListA(lpidl, strDir))
	{
		return false;
	}

	QString strPath = QString(strDir) + "\\" + name + ".lnk";
	if(!subdir.isEmpty())
	{
		strPath = QString(strDir) + "\\" + subdir + "\\" + name + ".lnk";
		osCreateDirectory(strPath, true);
	}

	return createSpecifiedShortcut(strPath, file);
}

bool KUtility::createWindowStartRun( const QString& file, const QString& name )
{
	QString path = QDir::cleanPath(file);
	path = QDir::toNativeSeparators(path);

	QSettings settings("Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
	settings.setValue(name, path);
	settings.sync();

	return true;
}

bool KUtility::removeDesktopShortcut( const QString& name )
{
	LPITEMIDLIST lpidl;
	if(FAILED(SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOPDIRECTORY, &lpidl)))
	{
		return false;
	}

	char strDir[MAX_PATH] = {0};
	if(!SHGetPathFromIDListA(lpidl, strDir))
	{
		return false;
	}

	QString strPath = QString(strDir) + "\\" + name + ".lnk";
	if(!DeleteFileA(strPath.toStdString().c_str()))
	{
		switch(GetLastError())
		{
		case ERROR_FILE_NOT_FOUND:
			return true;
		case ERROR_ACCESS_DENIED:
			return false;
		}
		return false;
	}
	return true;
}

bool KUtility::removeQuickLaunchShortcut( const QString& name )
{
	char szPath[MAX_PATH + 1] = { 0 };
	if(!ExpandEnvironmentStringsA(FOLDER_QUICKLAUNCH, szPath, MAX_PATH))
	{
		return false;
	}

	QString strPath = QString(szPath) + "\\" + name + ".lnk";
	if(!DeleteFileA(strPath.toStdString().c_str()))
	{
		switch(GetLastError())
		{
		case ERROR_FILE_NOT_FOUND:
			return true;
		case ERROR_ACCESS_DENIED:
			return false;
		}
		return false;
	}
	return true;
}

bool KUtility::removeProgramShortcut( const QString& name, const QString& subdir/*=""*/ )
{
	LPITEMIDLIST lpidl;
	if(FAILED(SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAMS, &lpidl)))
	{
		return false;
	}

	char strDir[MAX_PATH] = {0};
	if(!SHGetPathFromIDListA(lpidl, strDir))
	{
		return false;
	}

	QString strPath = QString(strDir) + "\\" + name + ".lnk";
	if(!subdir.isEmpty())
	{
		strPath = QString(strDir) + "\\" + subdir + "\\" + name + ".lnk";
	}

	if(!DeleteFileA(strPath.toStdString().c_str()))
	{
		switch(GetLastError())
		{
		case ERROR_FILE_NOT_FOUND:
			return true;
		case ERROR_ACCESS_DENIED:
			return false;
		}
		return false;
	}
	return true;
}

bool KUtility::removeWindowStartRun( const QString& name )
{
	QSettings settings("Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
	settings.remove(name);
	settings.sync();

	return true;
}

bool KUtility::hasDesktopShortcut( const QString& name )
{
	LPITEMIDLIST lpidl;
	if(FAILED(SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOPDIRECTORY, &lpidl)))
	{
		return false;
	}

	char strDir[MAX_PATH] = {0};
	if(!SHGetPathFromIDListA(lpidl, strDir))
	{
		return false;
	}

	QString strPath = QString(strDir) + "\\" + name + ".lnk";
	return PathFileExistsA(strPath.toStdString().c_str());
}

bool KUtility::hasQuickLaunchShortcut( const QString& name )
{
	char szPath[MAX_PATH + 1] = { 0 };
	if(!ExpandEnvironmentStringsA(FOLDER_QUICKLAUNCH, szPath, MAX_PATH))
	{
		return false;
	}

	QString strPath = QString(szPath) + "\\" + name + ".lnk";
	return PathFileExistsA(strPath.toStdString().c_str());
}

bool KUtility::hasProgramShortcut( const QString& name, const QString& subdir/*=""*/ )
{
	LPITEMIDLIST lpidl;
	if(FAILED(SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAMS, &lpidl)))
	{
		return false;
	}

	char strDir[MAX_PATH] = {0};
	if(!SHGetPathFromIDListA(lpidl, strDir))
	{
		return false;
	}

	QString strPath = QString(strDir) + "\\" + name + ".lnk";
	if(!subdir.isEmpty())
	{
		strPath = QString(strDir) + "\\" + subdir + "\\" + name + ".lnk";
	}
	return PathFileExistsA(strPath.toStdString().c_str());
}

bool KUtility::hasWindowStartRun( const QString& name )
{
	QSettings settings("Software\\Microsoft\\Windows\\CurrentVersion\\Run", QSettings::NativeFormat);
	QVariant vexePath = settings.value(name);
	return !vexePath.isNull();
}

PCHAR* KUtility::CommandLineToArgvA( PCHAR CmdLine, int* _argc )
{
	PCHAR* argv;
	PCHAR  _argv;
	ULONG   len;
	ULONG   argc;
	CHAR   a;
	ULONG   i, j;

	BOOLEAN  in_QM;
	BOOLEAN  in_TEXT;
	BOOLEAN  in_SPACE;

	len = strlen(CmdLine);
	i = ((len+2)/2)*sizeof(PVOID) + sizeof(PVOID);

	argv = (PCHAR*)GlobalAlloc(GMEM_FIXED,
		i + (len+2)*sizeof(CHAR));

	_argv = (PCHAR)(((PUCHAR)argv)+i);

	argc = 0;
	argv[argc] = _argv;
	in_QM = FALSE;
	in_TEXT = FALSE;
	in_SPACE = TRUE;
	i = 0;
	j = 0;

	while( a = CmdLine[i] ) 
	{
		if(in_QM) 
		{
			if(a == '\"') 
			{
				in_QM = FALSE;
			} 
			else 
			{
				_argv[j] = a;
				j++;
			}
		} 
		else 
		{
			switch(a) 
			{
			case '\"':
				in_QM = TRUE;
				in_TEXT = TRUE;
				if(in_SPACE) 
				{
					argv[argc] = _argv+j;
					argc++;
				}
				in_SPACE = FALSE;
				break;
			case ' ':
			case '\t':
			case '\n':
			case '\r':
				if(in_TEXT) 
				{
					_argv[j] = '\0';
					j++;
				}
				in_TEXT = FALSE;
				in_SPACE = TRUE;
				break;
			default:
				in_TEXT = TRUE;
				if(in_SPACE) 
				{
					argv[argc] = _argv+j;
					argc++;
				}
				_argv[j] = a;
				j++;
				in_SPACE = FALSE;
				break;
			}
		}
		i++;
	}
	_argv[j] = '\0';
	argv[argc] = NULL;

	(*_argc) = argc;
	return argv;
}

PWCHAR * KUtility::CommandLineToArgvW( PWCHAR CmdLine, int *_argc )
{
	return ::CommandLineToArgvW(CmdLine, _argc);
}

bool KUtility::createSpecifiedShortcut( const QString& _shortcutFile, const QString& _file )
{
	QString file = QDir::cleanPath(_file);
	file = QDir::toNativeSeparators(file);
	QString shortcutFile = QDir::cleanPath(_shortcutFile);
	shortcutFile = QDir::toNativeSeparators(shortcutFile);

	HRESULT hres = ::CoInitialize(NULL);
	if (FAILED(hres))
	{
		return false;
	}

	IShellLink *psl;
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (void**)&psl);
	if(FAILED(hres))
	{
		psl->Release();
		::CoUninitialize();
		return false;
	}

	psl->SetPath(file.toStdWString().c_str());

	// set the working directory.
	QString fileDir = file.left(file.lastIndexOf('\\'));
	psl->SetWorkingDirectory(fileDir.toStdWString().c_str());

	IPersistFile* ppf;
	hres = psl->QueryInterface(IID_IPersistFile,(void**)&ppf);
	if(FAILED(hres))
	{
		ppf->Release();
		psl->Release();
		::CoUninitialize();
		return false;
	}

	hres = ppf->Save(shortcutFile.toStdWString().c_str(), STGM_READWRITE);

	ppf->Release();
	psl->Release();
	::CoUninitialize();
	return true;
}


QString KUtility::loadSpecifiedShortcutTargetPath( const QString& shortcutFile )
{
	HRESULT hres = ::CoInitialize(NULL);
	if (FAILED(hres))
	{
		::CoUninitialize();
		return false;
	}

	//Create the ShellLink object
	IShellLink * psl;
	hres = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLink, (LPVOID*) &psl);
	if (FAILED(hres))
	{
		::CoUninitialize();
		return false;
	}

	IPersistFile* ppf;
	hres = psl->QueryInterface(IID_IPersistFile,(void**)&ppf);
	if (FAILED(hres))
	{
		psl->Release();
		::CoUninitialize();
		return false;
	}

	ppf->Load(shortcutFile.toStdWString().c_str(), 0);

	// get the original target path.
	wchar_t path[MAX_PATH + 1] = {0};
	psl->GetPath(path, MAX_PATH, NULL, SLGP_UNCPRIORITY);
	psl->Release();
	::CoUninitialize();
	return QString::fromWCharArray(path);
}

QString KUtility::loadDesktopShortcutPath()
{
	LPITEMIDLIST lpidl;
	if(FAILED(SHGetSpecialFolderLocation(NULL, CSIDL_DESKTOPDIRECTORY, &lpidl)))
	{
		return false;
	}

	char strDir[MAX_PATH] = {0};
	if(!SHGetPathFromIDListA(lpidl, strDir))
	{
		return false;
	}
	return QString(strDir);
}

QString KUtility::loadQuickLaunchShortcutPath()
{
	char szPath[MAX_PATH + 1] = { 0 };
	if(!ExpandEnvironmentStringsA(FOLDER_QUICKLAUNCH, szPath, MAX_PATH))
	{
		return false;
	}
	return QString(szPath);
}

QString KUtility::loadProgramShortcutPath()
{
	LPITEMIDLIST lpidl;
	if(FAILED(SHGetSpecialFolderLocation(NULL, CSIDL_PROGRAMS, &lpidl)))
	{
		return false;
	}

	char strDir[MAX_PATH] = {0};
	if(!SHGetPathFromIDListA(lpidl, strDir))
	{
		return false;
	}
	return QString(strDir);
}

QString KUtility::loadCommonProgramShortcutPath()
{
	LPITEMIDLIST lpidl;
	if(FAILED(SHGetSpecialFolderLocation(NULL, CSIDL_COMMON_PROGRAMS, &lpidl)))
	{
		return false;
	}

	char strDir[MAX_PATH] = {0};
	if(!SHGetPathFromIDListA(lpidl, strDir))
	{
		return false;
	}
	return QString(strDir);
}

QString KUtility::convertGIFToPNG( const QString& gif )
{
	QMovie movie(gif);
	if(!movie.isValid())
		return QString();
	int frameNum = movie.frameCount();
	QString png = QString("%1.%2.png").arg(gif).arg(frameNum);
	if(QFile::exists(png))
		return png;
	if(movie.jumpToFrame(0))
	{
		QImage img = movie.currentImage();
		int width = img.width();
		int height = img.height();
		QImage imgNew(width * frameNum, height, QImage::Format_ARGB32);
		imgNew.fill(0); //先直接初始化为0，才不会有底色。
		QPainter p(&imgNew);
		p.setRenderHint(QPainter::Antialiasing);
		for(int i = 0; i < frameNum; i++)
		{
			if(movie.jumpToFrame(i))
			{
				QImage img = movie.currentImage();
				p.drawImage(i * width, 0, img);
			}
		}
		bool bok = imgNew.save(png);
		return bok ? png : QString();
	}
	return QString();
}
