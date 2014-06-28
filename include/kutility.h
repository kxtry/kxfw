#pragma once;


class KXFW_API KUtility
{
public:
	static quint32 crc32(quint32 crc, const quint8 *buf, qint32 len);
	static QPoint screenCursorPos();
	static QPoint mapItemToScreen(QGraphicsItem *item);
	static QPoint mapItemToScreen( QGraphicsView* view, QGraphicsItem *item );
	static QRect mapWidgetToScreen( QGraphicsWidget *widget );
	static QRect mapWidgetToScene( QGraphicsWidget *widget );
	static QPointF mapScreenToWidget( const QPoint &pos, QGraphicsWidget *widget );
	static QGraphicsView *graphicsViewFromWidget( QGraphicsWidget *widget );
	static void openUrlWithPostData( const QString &url, const QString &postData );
	static bool openDirAndSelectFile(const QString& file);
	static QString saveImageToMd5File(const QImage &image, const QString& dstPath);
	static QPoint nextCascadesWindowPosition(); 
	static bool widgetInScreen(QWidget *widget);
	static QRect desktopWorkArea();
	static QSize virtualScreenSize();
	static QRect virtualScreenRect();
	static QString convertGIFToPNG( const QString& gif );
	static QObject *findObject(QObject *parent, const QString &name);
	static QObject *findObjectByPath(QObject *objRoot, const QString& path);
	static QString osAppDataPath();
	static QString osTempPath();
	static QString osSystemPath();
	static QString osDesktopPath();
	static QString osProgramPath();
	static QString osExeInstancePath();
	static bool osCreateDirectory(const QString& path, bool isFile);
	static void osEnumFiles(const QString& path, QFileInfoList& fils, QStringList& fls);
	static void osRemoveDirectory(const QString& path, bool itself=true);

	static PCHAR* CommandLineToArgvA(PCHAR CmdLine, int* _argc);
	static PWCHAR * CommandLineToArgvW(PWCHAR CmdLine, int *_argc);

	static QStringList analyseHtmlImageList(const QString& html);
	static QByteArray fragmentsToByteArray(const QList<QPair<QString,QTextCharFormat>>& frags);
	static QList<QPair<QString,QTextCharFormat>> fragmentsFromByteArray(const QByteArray& bytes);

	static void CopyVS2008ManifestFiles(const QString& dstPath);
	static void addPathToEnvironment(const QString& path);
	static void addVS2008ManifestFilesToEnvironment();
	static void addParameterToEnvironment(const QString& name, const QString& value);


	/*Register*/
	static bool createDesktopShortcut(const QString& file, const QString& name);
	static bool createQuickLaunchShortcut(const QString& file, const QString& name);
	static bool createProgramShortcut(const QString& file, const QString& name, const QString& subdir="");
	static bool createWindowStartRun(const QString& file, const QString& name);

	static bool createSpecifiedShortcut(const QString& shortcutFile, const QString& file);
	static QString loadSpecifiedShortcutTargetPath(const QString& shortcutFile);
	static QString loadDesktopShortcutPath();
	static QString loadQuickLaunchShortcutPath();
	static QString loadProgramShortcutPath();
	static QString loadCommonProgramShortcutPath();

	static bool removeDesktopShortcut(const QString& name);
	static bool removeQuickLaunchShortcut(const QString& name);
	static bool removeProgramShortcut(const QString& name, const QString& subdir="");
	static bool removeWindowStartRun(const QString& name);

	static bool hasDesktopShortcut(const QString& name);
	static bool hasQuickLaunchShortcut(const QString& name);
	static bool hasProgramShortcut(const QString& name, const QString& subdir="");
	static bool hasWindowStartRun(const QString& name);



	static BOOL WtoA(LPCWSTR lpwszSrc, int cch/*=-1*/, std::string &strDst);
	static BOOL AtoW(LPCSTR lpszSrc, int cch/*=-1*/, std::wstring &wstrDst);

	static void drawOneTopLeft(QPainter *painter, QRectF& itemRt, const QPixmap& pixmap);
	static void drawOneTopRight(QPainter *painter, QRectF& itemRt, const QPixmap& pixmap);
	static void drawOneBotLeft(QPainter *painter, QRectF& itemRt, const QPixmap& pixmap);
	static void drawOneBotRight(QPainter *painter, QRectF& itemRt, const QPixmap& pixmap);
	static void drawOneHCenter(QPainter *painter, QRectF& itemRt, const QPixmap& pixmap);
	static void drawOneVCenter(QPainter *painter, QRectF& itemRt, const QPixmap& pixmap);
	static void drawOneHVCenter(QPainter *painter, QRectF& itemRt, const QPixmap& pixmap);
	static void drawTileH(QPainter *painter, QRectF& itemRt, const QPixmap& pixmap);
	static void drawTileV(QPainter *painter, QRectF& itemRt, const QPixmap& pixmap);
	static void drawTileHV(QPainter *painter, QRectF& itemRt, const QPixmap& pixmap);
	static void drawStretchH(QPainter *painter, QRectF& itemRt, const QPixmap& pixmap, bool bsmooth);
	static void drawStretchV(QPainter *painter, QRectF& itemRt, const QPixmap& pixmap, bool bsmooth);
	static void drawStretchHV(QPainter *painter, QRectF& itemRt, const QPixmap& pixmap, bool bsmooth);
	static void drawStretchHTwo(QPainter *painter, QRectF& itemRt, const QPixmap& pixmap, bool bsmooth);
	static void drawStretchHMiddle(QPainter *painter, QRectF& itemRt, const QPixmap& pixmap, bool bsmooth);
	static void drawStretchVTwo(QPainter *painter, QRectF& itemRt, const QPixmap& pixmap, bool bsmooth);
	static void drawStretchVMiddle(QPainter *painter, QRectF& itemRt, const QPixmap& pixmap, bool bsmooth);
	static void drawGridFour(QPainter *painter, QRectF& itemRt, const QPixmap& pixmap);
	static void drawGridFrameFour(QPainter *painter, QRectF& itemRt, const QPixmap& pixmap);
	static void drawGridNine(QPainter *painter, QRectF& itemRt, const QPixmap& pixmap);

	

	#define WUNKNOWNSTR	_T("unknown Windows version")

	#define W95STR			_T("Windows 95")
	#define W95SP1STR		_T("Windows 95 SP1")
	#define W95OSR2STR		_T("Windows 95 OSR2")
	#define W98STR			_T("Windows 98")
	#define W98SP1STR		_T("Windows 98 SP1")
	#define W98SESTR		_T("Windows 98 SE")
	#define WMESTR			_T("Windows ME")

	#define WNT351STR		_T("Windows NT 3.51")
	#define WNT4STR			_T("Windows NT 4")
	#define W2KSTR			_T("Windows 2000")
	#define WXPSTR			_T("Windows XP")
	#define W2003SERVERSTR	_T("Windows 2003 Server R2")
	#define W2003SERVERST	_T("Windows 2003 Server")

	#define WCESTR			_T("Windows CE")

	#define	WINVISTA		_T("Windows Vista")
	#define WIN7			_T("Windows 7")			
	#define	W2008SERVERSTR	_T("Windows Server 2008 R2")
	#define	W2008SERVERST	_T("Windows Server 2008")



	#define WUNKNOWN	0

	#define W9XFIRST	1
	#define W95			1
	#define W95SP1		2
	#define W95OSR2		3
	#define W98			4
	#define W98SP1		5
	#define W98SE		6
	#define WME			7
	#define W9XLAST		99

	#define WNTFIRST	101
	#define WNT351		101
	#define WNT4		102
	#define W2K			103
	#define WXP			104
	#define W2003SERVER	105
	#define W2003SERVE	106
	#define WNTLAST		199

	#define WCEFIRST	201
	#define WCE			201
	#define WCELAST		299

	#define WVISTA		301
	#define W7			302

	#define W2008SERVER 303 
	#define W2008SERVE	304


	static BOOL GetWinVer(LPTSTR pszVersion, int nCountVer,LPTSTR pszMajorMinorBuild, int nCountMajor, int *nVersion);

	
};

#define COMPUTE_FUNCTION_TIME(OPERATOR) \
{\
	int start = GetTickCount(); \
	{OPERATOR} \
	int stop = GetTickCount();\
	string info; \
	char szTemp[32]; \
	memset(szTemp, 0, 32); \
	info = ""#OPERATOR" "; \
	info += "time cost: "; \
	info += _itoa(stop-start, szTemp, 10); \
	info += "\n"; \
	qWarning() << info.c_str();\
}