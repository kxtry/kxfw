/**
 *	PROJECT - HTML Reader Class Library
 *
 *	LiteHTMLEntityResolver.h - Defines CLiteHTMLEntityResolver
 *	
 *	Written By Gurmeet S. Kochar <gomzygotit@hotmail.com>
 *	Copyright (c) 2004. All rights reserved.
 *
 *	This code may be used in compiled form in any way you desire 
 *	(including commercial use). The code may be redistributed 
 *	unmodified by any means PROVIDING it is not sold for profit 
 *	without the authors written consent, and providing that this 
 *	notice and the authors name and all copyright notices remains 
 *	intact. However, this file and the accompanying source code may 
 *	not be hosted on a website or bulletin board without the authors 
 *	written permission.
 *	
 *	This file is provided "AS IS" with no expressed or implied warranty.
 *	The author accepts no liability for any damage/loss of business that
 *	this product may cause.
 *
 *	Although it is not necessary, but if you use this code in any of 
 *	your application (commercial or non-commercial), please INFORM me 
 *	so that I may know how useful this library is. This will encourage 
 *	me to keep updating it.
 *
 *	HISTORY:
 *
 *	Version 1.0				Gurmeet S. Kochar (GSK)
 *	Mar 21, 2004			First release version.
 */
#ifndef __LITEHTMLENTITYRESOLVER_H__
#define __LITEHTMLENTITYRESOLVER_H__


#ifndef __LITEHTMLCOMMON_H__
#	include "LiteHTMLCommon.h"
#endif	// !__LITEHTMLCOMMON_H__

#include <map>
#include <xstring>
#include <algorithm>

#pragma warning(push, 4)

/**
 * CLiteHTMLEntityResolver
 * This is a utility class that helps in resolving the character 
 * and numeric entity references.
 *
 * @version 1.0 (Mar 17, 2004)
 * @author Gurmeet S. Kochar
 */
class CLiteHTMLEntityResolver
{
private:
	class CCharEntityRefs
	{
	public:
		std::map<std::wstring, wchar_t> m_mapEnt;

		CCharEntityRefs()
		{

			// C0 Controls and Basic Latin 
			m_mapEnt[L"quot"]		= L'\x22';
			m_mapEnt[L"amp"]		= L'\x26';
			m_mapEnt[L"apos"]		= L'\x27';
			m_mapEnt[L"lt"]		= L'\x3C';
			m_mapEnt[L"gt"]		= L'\x3E';
			// ISO 8859-1 (Latin-1) characters 
			m_mapEnt[L"nbsp"]		= L'\xA0';
			m_mapEnt[L"iexcl"]	= L'\xA1';
			m_mapEnt[L"cent"]		= L'\xA2';
			m_mapEnt[L"pound"]	= L'\xA3';
			m_mapEnt[L"current"]	= L'\xA4';
			m_mapEnt[L"yen"]		= L'\xA5';
			m_mapEnt[L"brvbar"]	= L'\xA6';
			m_mapEnt[L"sect"]		= L'\xA7';
			m_mapEnt[L"uml"]		= L'\xA8';
			m_mapEnt[L"copy"]		= L'\xA9';
			m_mapEnt[L"ordf"]		= L'\xAA';
			m_mapEnt[L"laquo"]	= L'\xAB';
			m_mapEnt[L"not"]		= L'\xAC';
			m_mapEnt[L"shy"]		= L'\xAD';
			m_mapEnt[L"reg"]		= L'\xAE';
			m_mapEnt[L"macr"]		= L'\xAF';
			m_mapEnt[L"deg"]		= L'\xB0';
			m_mapEnt[L"plusmn"]	= L'\xB1';
			m_mapEnt[L"sup2"]		= L'\xB2';
			m_mapEnt[L"sup3"]		= L'\xB3';
			m_mapEnt[L"acute"]	= L'\xB4';
			m_mapEnt[L"micro"]	= L'\xB5';
			m_mapEnt[L"para"]		= L'\xB6';
			m_mapEnt[L"middot"]	= L'\xB7';
			m_mapEnt[L"cedil"]	= L'\xB8';
			m_mapEnt[L"sup1"]		= L'\xB9';
			m_mapEnt[L"ordm"]		= L'\xBA';
			m_mapEnt[L"raquo"]	= L'\xBB';
			m_mapEnt[L"frac14"]	= L'\xBC';
			m_mapEnt[L"frac12"]	= L'\xBD';
			m_mapEnt[L"frac34"]	= L'\xBE';
			m_mapEnt[L"iquest"]	= L'\xBF';
			m_mapEnt[L"Agrave"]	= L'\xC0';
			m_mapEnt[L"Aacute"]	= L'\xC1';
			m_mapEnt[L"Acirc"]	= L'\xC2';
			m_mapEnt[L"Atilde"]	= L'\xC3';
			m_mapEnt[L"Auml"]		= L'\xC4';
			m_mapEnt[L"Aring"]	= L'\xC5';
			m_mapEnt[L"AElig"]	= L'\xC6';
			m_mapEnt[L"Ccedil"]	= L'\xC7';
			m_mapEnt[L"Egrave"]	= L'\xC8';
			m_mapEnt[L"Eacute"]	= L'\xC9';
			m_mapEnt[L"Ecirc"]	= L'\xCA';
			m_mapEnt[L"Euml"]		= L'\xCB';
			m_mapEnt[L"Igrave"]	= L'\xCC';
			m_mapEnt[L"Iacute"]	= L'\xCD';
			m_mapEnt[L"Icirc"]	= L'\xCE';
			m_mapEnt[L"Iuml"]		= L'\xCF';
			m_mapEnt[L"ETH"]		= L'\xD0';
			m_mapEnt[L"Ntilde"]	= L'\xD1';
			m_mapEnt[L"Ograve"]	= L'\xD2';
			m_mapEnt[L"Oacute"]	= L'\xD3';
			m_mapEnt[L"Ocirc"]	= L'\xD4';
			m_mapEnt[L"Otilde"]	= L'\xD5';
			m_mapEnt[L"Ouml"]		= L'\xD6';
			m_mapEnt[L"times"]	= L'\xD7';
			m_mapEnt[L"Oslash"]	= L'\xD8';
			m_mapEnt[L"Ugrave"]	= L'\xD9';
			m_mapEnt[L"Uacute"]	= L'\xDA';
			m_mapEnt[L"Ucirc"]	= L'\xDB';
			m_mapEnt[L"Uuml"]		= L'\xDC';
			m_mapEnt[L"Yacute"]	= L'\xDD';
			m_mapEnt[L"THORN"]	= L'\xDE';
			m_mapEnt[L"szlig"]	= L'\xDF';
			m_mapEnt[L"agrave"]	= L'\xE0';
			m_mapEnt[L"aacute"]	= L'\xE1';
			m_mapEnt[L"acirc"]	= L'\xE2';
			m_mapEnt[L"atilde"]	= L'\xE3';
			m_mapEnt[L"auml"]		= L'\xE4';
			m_mapEnt[L"aring"]	= L'\xE5';
			m_mapEnt[L"aelig"]	= L'\xE6';
			m_mapEnt[L"ccedil"]	= L'\xE7';
			m_mapEnt[L"egrave"]	= L'\xE8';
			m_mapEnt[L"eacute"]	= L'\xE9';
			m_mapEnt[L"ecirc"]	= L'\xEA';
			m_mapEnt[L"euml"]		= L'\xEB';
			m_mapEnt[L"igrave"]	= L'\xEC';
			m_mapEnt[L"iacute"]	= L'\xED';
			m_mapEnt[L"icirc"]	= L'\xEE';
			m_mapEnt[L"iuml"]		= L'\xEF';
			m_mapEnt[L"eth"]		= L'\xF0';
			m_mapEnt[L"ntilde"]	= L'\xF1';
			m_mapEnt[L"ograve"]	= L'\xF2';
			m_mapEnt[L"oacute"]	= L'\xF3';
			m_mapEnt[L"ocirc"]	= L'\xF4';
			m_mapEnt[L"otilde"]	= L'\xF5';
			m_mapEnt[L"ouml"]		= L'\xF6';
			m_mapEnt[L"divide"]	= L'\xF7';
			m_mapEnt[L"oslash"]	= L'\xF8';
			m_mapEnt[L"ugrave"]	= L'\xF9';
			m_mapEnt[L"uacute"]	= L'\xFA';
			m_mapEnt[L"ucirc"]	= L'\xFB';
			m_mapEnt[L"uuml"]		= L'\xFC';
			m_mapEnt[L"yacute"]	= L'\xFD';
			m_mapEnt[L"thorn"]	= L'\xFE';
			m_mapEnt[L"yuml"]		= L'\xFF';

		}
	};

// Constructors
public:
	CLiteHTMLEntityResolver()	{ }

// Operations
public:
	/**
	 * Resolves a character or numeric entity reference.
	 *
	 * @param rStr - entity to resolve
	 * @param ch - variable that will receive the result
	 *
	 * @return number of TCHARs used to resolve entity reference
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	static UINT resolveEntity(const wchar_t* lpszEntity, wchar_t &chSubst)
	{
		_ASSERT(m_CharEntityRefs.m_mapEnt.size());
		_ASSERT(lpszEntity);

		const wchar_t*	lpszBegin = lpszEntity; 
		const wchar_t* lpszEnd = ::wcschr(lpszEntity, L';');
		
		// entity references always end with a semi-colon ';'
		if (lpszEnd == NULL)
			return (0);

		// skip leading white-space characters
		while (::iswspace(*lpszBegin))
			lpszBegin = ::_wcsinc(lpszBegin);
		
		// remaining string (including semi-colon) 
		// must be at least 4 characters in length
		if (lpszEnd - lpszBegin < 3)
			return (0U);

		// entity references always begin with an ampersand '&' symbol
		if (*lpszBegin != L'&')
			return (0U);
		lpszBegin = ::_wcsinc(lpszBegin);

		// numeric (decimal or hexadecimal) entity reference?
		if (*lpszBegin == L'#')
		{
			wchar_t	chTemp = 0;
			lpszBegin = ::_wcsinc(lpszBegin);
			chTemp = *lpszBegin;
			int	radix = (::iswdigit(chTemp) ? 10 : 
						(chTemp == L'x' || 
							chTemp == L'X' ? 16 : 0));
			if (radix)
			{
				if (radix == 16)
					lpszBegin = ::_wcsinc(lpszBegin);
				
				unsigned long	ulNum = ::wcstoul(lpszBegin, NULL, radix);
				chSubst = (wchar_t)ulNum;
				lpszEnd = ::_wcsinc(lpszEnd);
				return (lpszEnd - lpszEntity);
			}
		}

		// character entity reference?
		else
		{
			std::wstring strKey(lpszBegin, lpszEnd - lpszBegin);
			
			// because some character entity references are 
			// case-sensitive, we must fix them manually
			if ( !::_wcsicmp(strKey.c_str(), L"eth") || 
				!::_wcsicmp(strKey.c_str(), L"thorn") )
			{
				if (::iswupper(strKey[0]))
				{
					std::transform(strKey.begin(), strKey.end(), strKey.begin(), toupper); 
				}
				else
				{
					std::transform(strKey.begin(), strKey.end(), strKey.begin(), tolower); 
				}
			}
			else if (!::_wcsicmp(strKey.c_str(), L"Oslash"))
			{
				std::transform(strKey.begin(), strKey.end(), strKey.begin(), tolower); 
				strKey[0] = L'O';
			}
			else if (!::_wcsicmp(strKey.c_str(), L"AElig"))
			{
				std::transform(strKey.begin(), strKey.end(), strKey.begin(), tolower); 
				strKey[0] = L'A';
				strKey[1] = L'E';
			}
			else
			{
				std::wstring strT = strKey.substr(1);
				std::transform(strKey.begin(), strKey.end(), strKey.begin(), tolower); 
				if (::_wcsicmp(strT.c_str(), L"grave") == 0 || 
					::_wcsicmp(strT.c_str(), L"acute") == 0 || 
					::_wcsicmp(strT.c_str(), L"circ")  == 0 || 
					::_wcsicmp(strT.c_str(), L"uml")   == 0 || 
					::_wcsicmp(strT.c_str(), L"tilde") == 0 || 
					::_wcsicmp(strT.c_str(), L"cedil") == 0 || 
					::_wcsicmp(strT.c_str(), L"ring")  == 0)
				{
					strKey[0] = strT[0];
				}
			}
			
			// is this a known entity reference?
			if (m_CharEntityRefs.m_mapEnt.find(strKey) != m_CharEntityRefs.m_mapEnt.end())
			{
				chSubst = m_CharEntityRefs.m_mapEnt[strKey];
				lpszEnd = ::_wcsinc(lpszEnd);
				return (lpszEnd - lpszEntity);

			}

		}

		return (0U);
	}

// Data Members
private:
	static CCharEntityRefs	m_CharEntityRefs;
};
#pragma warning(pop)

#endif	// !__LITEHTMLENTITYRESOLVER_H__
