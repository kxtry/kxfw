// MonitorHelp.cpp: implementation of the MonitorHelp class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MonitorHelp.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void MonitorHelp::EnumMonitorRect(std::vector<RECT>* lpMonitorRectVector)
{
	EnumDisplayMonitors(NULL, NULL, GetMonitorRect, (LPARAM)lpMonitorRectVector	);
}

int MonitorHelp::GetMonitorCount()
{
	int nCount = 0;
	EnumDisplayMonitors(NULL, NULL, AddMonitorCount, (LPARAM)nCount	);
	return nCount;
}

BOOL MonitorHelp::GetMonitorRect(	HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData )
{
	std::vector<RECT>* lpMonitorRectVector =  (std::vector<RECT>* )dwData;
	lpMonitorRectVector->push_back(RECT(*lprcMonitor));
	return TRUE;
}
BOOL MonitorHelp::AddMonitorCount(	HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData )
{
	dwData++;
	return TRUE;
}

BOOL MonitorHelp::GetMonitorInfo(POINT ptScreen, MONITORINFO *lpmi, DWORD dwFlag )
{
	HMONITOR hMi = MonitorFromPoint(ptScreen, dwFlag);
	if (hMi != NULL){
		lpmi->cbSize = sizeof(MONITORINFO);
		return ::GetMonitorInfo(hMi, lpmi);}
	return FALSE;	
}
BOOL MonitorHelp::GetMonitorInfo(LPCRECT lprc, MONITORINFO *lpmi, DWORD dwFlag )
{
	HMONITOR hMi = MonitorFromRect(lprc, dwFlag);
	if (hMi != NULL){
		lpmi->cbSize = sizeof(MONITORINFO);
		return ::GetMonitorInfo(hMi, lpmi);}
	return FALSE;
}
BOOL MonitorHelp::GetMonitorInfo(HWND hwnd, MONITORINFO *lpmi, DWORD dwFlag )
{
	HMONITOR hMi = MonitorFromWindow(hwnd, dwFlag);
	if (hMi != NULL){
		lpmi->cbSize = sizeof(MONITORINFO);
		return ::GetMonitorInfo(hMi, lpmi);}
	return FALSE;
}

RECT MonitorHelp::GetVirtualScreen()
{
	RECT rtVirtualScreen;
	rtVirtualScreen.left = GetSystemMetrics(SM_XVIRTUALSCREEN);
	rtVirtualScreen.top = GetSystemMetrics(SM_YVIRTUALSCREEN);
	rtVirtualScreen.right = GetSystemMetrics(SM_CXVIRTUALSCREEN) - abs(rtVirtualScreen.left);
	rtVirtualScreen.bottom = GetSystemMetrics(SM_CYVIRTUALSCREEN) - abs(rtVirtualScreen.top);
	return rtVirtualScreen;
}


BOOL MonitorHelp::IsValidPoint(POINT ptPoint)
{
	MONITORINFO mi = {0};
	HMONITOR hMi = MonitorFromPoint(ptPoint, MONITOR_DEFAULTTONULL);
	return hMi!=NULL;
}
RECT MonitorHelp::GetNearMonitorRect(POINT ptPoint, BOOL bWork )
{
	MONITORINFO mi = {0};
	GetMonitorInfo(ptPoint, &mi, MONITOR_DEFAULTTONEAREST);
	if ( bWork )
		return RECT(mi.rcWork);
	return RECT(mi.rcMonitor);
}

BOOL MonitorHelp::IsValidRect(LPRECT lpRect)
{
	MONITORINFO mi = {0};
	HMONITOR hMi = MonitorFromRect(lpRect, MONITOR_DEFAULTTONULL);
	return hMi!=NULL;
}
RECT MonitorHelp::GetNearMonitorRect(LPRECT lpRect, BOOL bWork )
{
	MONITORINFO mi = {0};
	GetMonitorInfo(lpRect, &mi, MONITOR_DEFAULTTONEAREST);
	if ( bWork )
		return RECT(mi.rcWork);
	return RECT(mi.rcMonitor);
}
