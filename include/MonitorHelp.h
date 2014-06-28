// MonitorHelp.h: interface for the MonitorHelp class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MONITORHELP_H__A716E089_B6B3_4135_8DA9_5423D0B885E4__INCLUDED_)
#define AFX_MONITORHELP_H__A716E089_B6B3_4135_8DA9_5423D0B885E4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class KXFW_API MonitorHelp  
{
public:
	static int  GetMonitorCount();
	static void EnumMonitorRect(std::vector<RECT>* lpMonitorRectVector);
	static BOOL GetMonitorInfo(POINT ptScreen, MONITORINFO *lpmi, DWORD dwFlag = MONITOR_DEFAULTTONULL);
	static BOOL GetMonitorInfo(LPCRECT lprc, MONITORINFO *lpmi, DWORD dwFlag = MONITOR_DEFAULTTONULL);
	static BOOL GetMonitorInfo(HWND hwnd, MONITORINFO *lpmi, DWORD dwFlag = MONITOR_DEFAULTTONULL);
	static RECT GetVirtualScreen();
	static BOOL  IsValidRect(LPRECT lpRect);
	static RECT GetNearMonitorRect(LPRECT lpRect, BOOL bWork = FALSE );
	static BOOL  IsValidPoint(POINT ptPoint);
	static RECT GetNearMonitorRect(POINT ptPoint, BOOL bWork = FALSE );
private:
	static BOOL CALLBACK GetMonitorRect(	HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData );
	static BOOL CALLBACK AddMonitorCount(	HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData );
};

#endif // !defined(AFX_MONITORHELP_H__A716E089_B6B3_4135_8DA9_5423D0B885E4__INCLUDED_)
