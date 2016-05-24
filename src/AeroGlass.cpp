#include "stdafx.h"
#include "AeroGlass.h"

HRESULT (WINAPI *KG7_DwmDefWindowProc)(HWND hwnd,UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *plResult);
HRESULT (WINAPI *KG7_DwmEnableBlurBehindWindow)(HWND hWnd, const DWM_BLURBEHIND *pBlurBehind);
HRESULT (WINAPI *KG7_DwmEnableComposition)(UINT uCompositionAction);
HRESULT (WINAPI *KG7_DwmEnableMMCSS)(BOOL fEnableMMCSS);
HRESULT (WINAPI *KG7_DwmExtendFrameIntoClientArea)(HWND hWnd, const MARGINS *pMarInset);
HRESULT (WINAPI *KG7_DwmGetColorizationColor)(DWORD *pcrColorization, BOOL *pfOpaqueBlend);
HRESULT (WINAPI *KG7_DwmGetCompositionTimingInfo)(HWND hwnd, DWM_TIMING_INFO *pTimingInfo);
HRESULT (WINAPI *KG7_DwmGetWindowAttribute)(HWND hwnd, DWORD dwAttribute, PVOID pvAttribute, DWORD cbAttribute);
HRESULT (WINAPI *KG7_DwmIsCompositionEnabled)(BOOL *pfEnabled);
HRESULT (WINAPI *KG7_DwmModifyPreviousDxFrameDuration)(HWND hwnd, INT cRefreshes, BOOL fRelative);
HRESULT (WINAPI *KG7_DwmQueryThumbnailSourceSize)(HTHUMBNAIL hThumbnail, PSIZE pSize);
HRESULT (WINAPI *KG7_DwmRegisterThumbnail)(HWND hwndDestination, HWND *hwndSource, PHTHUMBNAIL phThumbnailId);
HRESULT (WINAPI *KG7_DwmSetDxFrameDuration)(HWND hwnd, INT cRefreshes);
HRESULT (WINAPI *KG7_DwmSetPresentParameters)(HWND hwnd, DWM_PRESENT_PARAMETERS *pPresentParams);
HRESULT (WINAPI *KG7_DwmSetWindowAttribute)(HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute);
HRESULT (WINAPI *KG7_DwmUnregisterThumbnail)(HTHUMBNAIL hThumbnailId);
HRESULT (WINAPI *KG7_DwmUpdateThumbnailProperties)(HTHUMBNAIL hThumbnailId, const DWM_THUMBNAIL_PROPERTIES *ptnProperties);


HMODULE CAeroGlass::m_hModuleAero = NULL;

HRESULT CAeroGlass::DwmDefWindowProc( HWND hwnd,UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *plResult )
{
	if(KG7_DwmDefWindowProc == NULL){
		return E_FAIL;}
	return KG7_DwmDefWindowProc(hwnd, msg, wParam, lParam, plResult);
}

HRESULT CAeroGlass::DwmEnableBlurBehindWindow( HWND hWnd, const DWM_BLURBEHIND *pBlurBehind )
{
	if(KG7_DwmEnableBlurBehindWindow == NULL){
		return E_FAIL;}
	return KG7_DwmEnableBlurBehindWindow(hWnd, pBlurBehind);
}

HRESULT CAeroGlass::DwmEnableComposition( UINT uCompositionAction )
{
	if(KG7_DwmEnableComposition == NULL){
		return E_FAIL;}
	return KG7_DwmEnableComposition(uCompositionAction);
}

HRESULT CAeroGlass::DwmEnableMMCSS( BOOL fEnableMMCSS )
{
	if(KG7_DwmEnableMMCSS == NULL){
		return E_FAIL;}
	return KG7_DwmEnableMMCSS(fEnableMMCSS);
}

HRESULT CAeroGlass::DwmExtendFrameIntoClientArea( HWND hWnd, const MARGINS *pMarInset )
{
	if(KG7_DwmExtendFrameIntoClientArea == NULL){
		return E_FAIL;}
	return KG7_DwmExtendFrameIntoClientArea(hWnd, pMarInset);
}

HRESULT CAeroGlass::DwmGetColorizationColor( DWORD *pcrColorization, BOOL *pfOpaqueBlend )
{
	if(KG7_DwmGetColorizationColor == NULL){
		return E_FAIL;}
	return KG7_DwmGetColorizationColor(pcrColorization, pfOpaqueBlend);
}

HRESULT CAeroGlass::DwmGetCompositionTimingInfo( HWND hwnd, DWM_TIMING_INFO *pTimingInfo )
{
	if(KG7_DwmGetCompositionTimingInfo == NULL){
		return E_FAIL;}
	return KG7_DwmGetCompositionTimingInfo(hwnd, pTimingInfo);
}

HRESULT CAeroGlass::DwmGetWindowAttribute( HWND hwnd, DWORD dwAttribute, PVOID pvAttribute, DWORD cbAttribute )
{
	if(KG7_DwmGetWindowAttribute == NULL){
		return E_FAIL;}
	return KG7_DwmGetWindowAttribute(hwnd, dwAttribute, pvAttribute, cbAttribute);
}

HRESULT CAeroGlass::DwmIsCompositionEnabled( BOOL *pfEnabled )
{
	if(KG7_DwmIsCompositionEnabled == NULL){
		return E_FAIL;}
	return KG7_DwmIsCompositionEnabled(pfEnabled);
}

HRESULT CAeroGlass::DwmModifyPreviousDxFrameDuration( HWND hwnd, INT cRefreshes, BOOL fRelative )
{
	if(KG7_DwmModifyPreviousDxFrameDuration == NULL){
		return E_FAIL;}
	return KG7_DwmModifyPreviousDxFrameDuration(hwnd, cRefreshes, fRelative);
}

HRESULT CAeroGlass::DwmQueryThumbnailSourceSize( HTHUMBNAIL hThumbnail, PSIZE pSize )
{
	if(KG7_DwmQueryThumbnailSourceSize == NULL){
		return E_FAIL;}
	return KG7_DwmQueryThumbnailSourceSize(hThumbnail, pSize);
}

HRESULT CAeroGlass::DwmRegisterThumbnail( HWND hwndDestination, HWND *hwndSource, PHTHUMBNAIL phThumbnailId )
{
	if(KG7_DwmRegisterThumbnail == NULL){
		return E_FAIL;}
	return KG7_DwmRegisterThumbnail(hwndDestination, hwndSource, phThumbnailId);
}

HRESULT CAeroGlass::DwmSetDxFrameDuration( HWND hwnd, INT cRefreshes )
{
	if(KG7_DwmSetDxFrameDuration == NULL){
		return E_FAIL;}
	return KG7_DwmSetDxFrameDuration(hwnd, cRefreshes);
}

HRESULT CAeroGlass::DwmSetPresentParameters( HWND hwnd, DWM_PRESENT_PARAMETERS *pPresentParams )
{
	if(KG7_DwmSetPresentParameters == NULL){
		return E_FAIL;}
	return KG7_DwmSetPresentParameters(hwnd, pPresentParams);
}

HRESULT CAeroGlass::DwmSetWindowAttribute( HWND hwnd, DWORD dwAttribute, LPCVOID pvAttribute, DWORD cbAttribute )
{
	if(KG7_DwmSetWindowAttribute == NULL){
		return E_FAIL;}
	return KG7_DwmSetWindowAttribute(hwnd, dwAttribute, pvAttribute, cbAttribute);
}

HRESULT CAeroGlass::DwmUnregisterThumbnail( HTHUMBNAIL hThumbnailId )
{
	if(KG7_DwmUnregisterThumbnail == NULL){
		return E_FAIL;}
	return KG7_DwmUnregisterThumbnail(hThumbnailId);
}

HRESULT CAeroGlass::DwmUpdateThumbnailProperties( HTHUMBNAIL hThumbnailId, const DWM_THUMBNAIL_PROPERTIES *ptnProperties )
{
	if(KG7_DwmUpdateThumbnailProperties == NULL){
		return E_FAIL;}
	return KG7_DwmUpdateThumbnailProperties(hThumbnailId, ptnProperties);
}

BOOL CAeroGlass::Create()
{
	if(m_hModuleAero)
		return (BOOL)m_hModuleAero;
	m_hModuleAero = LoadLibrary(_T("dwmapi.dll"));
	if(m_hModuleAero == NULL)
		return FALSE;
	FARPROC *ppFarProc = (FARPROC*)&KG7_DwmDefWindowProc;
	*ppFarProc = GetProcAddress(m_hModuleAero, "DwmDefWindowProc");
	ppFarProc = (FARPROC*)&KG7_DwmEnableBlurBehindWindow;
	*ppFarProc = GetProcAddress(m_hModuleAero, "DwmEnableBlurBehindWindow");
	ppFarProc = (FARPROC*)&KG7_DwmEnableComposition;
	*ppFarProc = GetProcAddress(m_hModuleAero, "DwmEnableComposition");
	ppFarProc = (FARPROC*)&KG7_DwmEnableMMCSS;
	*ppFarProc = GetProcAddress(m_hModuleAero, "DwmEnableMMCSS");
	ppFarProc = (FARPROC*)&KG7_DwmExtendFrameIntoClientArea;
	*ppFarProc = GetProcAddress(m_hModuleAero, "DwmExtendFrameIntoClientArea");
	ppFarProc = (FARPROC*)&KG7_DwmEnableBlurBehindWindow;
	*ppFarProc = GetProcAddress(m_hModuleAero, "DwmEnableBlurBehindWindow");
	ppFarProc = (FARPROC*)&KG7_DwmGetColorizationColor;
	*ppFarProc = GetProcAddress(m_hModuleAero, "DwmGetColorizationColor");
	ppFarProc = (FARPROC*)&KG7_DwmGetCompositionTimingInfo;
	*ppFarProc = GetProcAddress(m_hModuleAero, "DwmGetCompositionTimingInfo");
	ppFarProc = (FARPROC*)&KG7_DwmGetWindowAttribute;
	*ppFarProc = GetProcAddress(m_hModuleAero, "DwmGetWindowAttribute");
	ppFarProc = (FARPROC*)&KG7_DwmIsCompositionEnabled;
	*ppFarProc = GetProcAddress(m_hModuleAero, "DwmIsCompositionEnabled");
	ppFarProc = (FARPROC*)&KG7_DwmModifyPreviousDxFrameDuration;
	*ppFarProc = GetProcAddress(m_hModuleAero, "DwmModifyPreviousDxFrameDuration");
	ppFarProc = (FARPROC*)&KG7_DwmQueryThumbnailSourceSize;
	*ppFarProc = GetProcAddress(m_hModuleAero, "DwmQueryThumbnailSourceSize");
	ppFarProc = (FARPROC*)&KG7_DwmRegisterThumbnail;
	*ppFarProc = GetProcAddress(m_hModuleAero, "DwmRegisterThumbnail");
	ppFarProc = (FARPROC*)&KG7_DwmSetDxFrameDuration;
	*ppFarProc = GetProcAddress(m_hModuleAero, "DwmSetDxFrameDuration");
	ppFarProc = (FARPROC*)&KG7_DwmSetPresentParameters;
	*ppFarProc = GetProcAddress(m_hModuleAero, "DwmSetPresentParameters");
	ppFarProc = (FARPROC*)&KG7_DwmSetWindowAttribute;
	*ppFarProc = GetProcAddress(m_hModuleAero, "DwmSetWindowAttribute");
	ppFarProc = (FARPROC*)&KG7_DwmUnregisterThumbnail;
	*ppFarProc = GetProcAddress(m_hModuleAero, "DwmUnregisterThumbnail");
	ppFarProc = (FARPROC*)&KG7_DwmUpdateThumbnailProperties;
	*ppFarProc = GetProcAddress(m_hModuleAero, "DwmUpdateThumbnailProperties");
	return (BOOL)m_hModuleAero;
}

void CAeroGlass::Destroy()
{
	if(m_hModuleAero == NULL)
		return;
	KG7_DwmDefWindowProc = NULL;
	KG7_DwmEnableBlurBehindWindow = NULL;
	KG7_DwmEnableComposition = NULL;
	KG7_DwmEnableMMCSS = NULL;
	KG7_DwmExtendFrameIntoClientArea = NULL;
	KG7_DwmGetColorizationColor = NULL;
	KG7_DwmGetCompositionTimingInfo = NULL;
	KG7_DwmGetWindowAttribute = NULL;
	KG7_DwmIsCompositionEnabled = NULL;
	KG7_DwmModifyPreviousDxFrameDuration = NULL;
	KG7_DwmSetWindowAttribute = NULL;
	KG7_DwmSetDxFrameDuration = NULL;
	KG7_DwmUnregisterThumbnail = NULL;
	KG7_DwmUpdateThumbnailProperties = NULL;
	FreeLibrary(m_hModuleAero);
	m_hModuleAero = NULL;
}