/**
 *	PROJECT - HTML Reader Class Library
 *
 *	LiteHTMLTag.h - Defines CLiteHTMLTag
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
 *	Mar 18, 2004			First release version.
 */
#ifndef __LITEHTMLTAG_H__
#define __LITEHTMLTAG_H__

/*
 * Conditional Includes
 */
#ifndef __LITEHTMLCOMMON_H__
#	include "LiteHTMLCommon.h"
#endif	// !__LITEHTMLCOMMON_H__

#ifndef __LITEHTMLATTRIBUTES_H__
#	include "LiteHTMLAttributes.h"
#endif	// !__LITEHTMLATTRIBUTES_H__

#pragma warning(push, 4)
#pragma warning (disable : 4290)	// C++ Exception Specification ignored
#pragma warning (disable : 4127)	// C++ Exception Specification ignored

/**
 * CLiteHTMLTag
 *
 * @version 1.0 (March 18, 2004)
 * @author Gurmeet S. Kochar
 */
class CLiteHTMLTag
{
// Construction/Destruction
public:
	CLiteHTMLTag()
		: m_pcollAttr(NULL)
	{ }

	/**
	 * @param bCopy - if true, this CLiteHTMLTag makes a copy of the 
	 * CLiteHTMLAttributes pointer. if false, this constructor takes 
	 * ownership of the encapsulated CLiteHTMLAttributes pointer.
	 *
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	CLiteHTMLTag(CLiteHTMLTag &rSource, bool bCopy = false) 
		:m_pcollAttr(NULL)
	{
		m_strTagName = rSource.m_strTagName;
		if (!bCopy)
		{
			m_pcollAttr = rSource.m_pcollAttr;
			rSource.m_pcollAttr = NULL;
		}
		else if (rSource.m_pcollAttr != NULL)
		{
			/** DEEP COPY BEGIN */
			m_pcollAttr = new CLiteHTMLAttributes(*(rSource.m_pcollAttr), true);
			/** DEEP COPY END */
		}
	}

	virtual ~CLiteHTMLTag()
		{ SAFE_DELETE_POINTER(m_pcollAttr); }

// Attributes
public:
	/**
	 * @return name of the tag
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	std::wstring getTagName(void) const
		{ return (m_strTagName); }

	/**
	 * @return pointer to the attribute collection
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	const CLiteHTMLAttributes* getAttributes(void) const
		{ return (m_pcollAttr);  }

// Parsing Helpers
public:
	// parses tag information from the given string
	UINT parseFromStr(const wchar_t* lpszString, 
					  bool &bIsOpeningTag, 
					  bool &bIsClosingTag, 
					  bool bParseAttrib = true);

// Data Members
private:
	/**
	 * collection of CLiteHTMLElemAttr objects
	 * @since 1.0
	 */
	CLiteHTMLAttributes	*m_pcollAttr;

	/**
	 * tag name of the element
	 * @since 1.0
	 */
	std::wstring 				m_strTagName;
};

/**
 * CLiteHTMLTag::parseFromStr
 * 
 * @param rStr - string to parse
 * @param bIsOpeningTag - receives true if the tag parsed 
 *        is a opening tag, false otherwise.
 * @param bIsClosingTag - receives true if the tag parsed 
 *        is a closing tag, false otherwise.
 * @param bParseAttrib - determines whether attribute/value 
 *        pairs should be parsed. Default is true.
 *
 * @return number of characters successfully parsed
 * @since 1.0
 * @author Gurmeet S. Kochar
 */
inline UINT CLiteHTMLTag::parseFromStr(const wchar_t* lpszString, 
									   bool &bIsOpeningTag, 
									   bool &bIsClosingTag, 
									   bool bParseAttrib /* = true */)
{
	_ASSERT(lpszString);

	bool				bClosingTag = false;
	bool				bOpeningTag = false;
	CLiteHTMLAttributes	*pcollAttr = NULL;
	std::wstring		strTagName;
	UINT				nRetVal = 0U;
	const wchar_t*				lpszBegin = lpszString;
	const wchar_t*				lpszEnd = NULL;

	// skip leading white-space characters
	while (iswspace(*lpszBegin))
		lpszBegin = ::_wcsinc(lpszBegin);

	// HTML tags always begin with a less-than symbol
	if (*lpszBegin != L'<')
		return (0U);
	
	// skip tag's opening delimeter '<'
	lpszBegin = ::_wcsinc(lpszBegin);

	// optimization for empty opening tags
	if (*lpszBegin == L'>')
	{
		_ASSERT(strTagName.empty());
		_ASSERT(pcollAttr == NULL);
		_ASSERT(!bClosingTag);
		nRetVal = lpszBegin - lpszString;
		goto LUpdateAndExit;
	}

	// tag names always begin with an alphabet
	if (!::iswalpha(*lpszBegin))
	{
		bClosingTag = (*lpszBegin == L'/');
		if (bClosingTag)
			lpszBegin = ::_wcsinc(lpszBegin);
		else
			return (0U);
	}

	bOpeningTag = !bClosingTag;
	lpszEnd = lpszBegin;
	do
	{
		// tag name may contain letters (a-z, A-Z), digits (0-9), 
		// underscores '_', hyphen '-', colons ':', and periods '.'
		if ( (!::iswalnum(*lpszEnd)) && 
			 (*lpszEnd != L'-') && (*lpszEnd != L':') && 
			 (*lpszEnd != L'_') && (*lpszEnd != L'.') )
		{
			_ASSERT(lpszEnd != lpszBegin);
			
			// only white-space characters, a null-character, a 
			// greater-than symbol, or a forward-slash can break 
			// a tag name
			if (*lpszEnd == NULL || ::iswspace(*lpszEnd) || 
				*lpszEnd == L'>' || 
				(*lpszEnd == L'/' && (!bClosingTag)) )
			{
				break;
			}

			return (0U);	// any other character will fail parsing process
		}

		lpszEnd = ::_wcsinc(lpszEnd);
	}
	while(true);

	// store tag name for later use
	strTagName = std::wstring(lpszBegin, lpszEnd - lpszBegin);

	// is this a closing tag?
	if (bClosingTag)
	{
		// in a closing tag, there can be only one symbol after 
		// tag name, i.e., the tag end delimeter '>'. Anything 
		// else will result in parsing failure
		if (*lpszEnd != L'>')
			return (0U);

		// skip tag's ending delimeter
		lpszEnd = ::_wcsinc(lpszEnd);

		_ASSERT(strTagName.size());
		_ASSERT(pcollAttr == NULL);
		nRetVal = lpszEnd - lpszString;
		goto LUpdateAndExit;
	}

	// tag contains attribute/value pairs?
	if (*lpszEnd != L'>')
	{
		lpszBegin = lpszEnd;
		lpszEnd = NULL;

		// skip white-space characters after tag name
		while (::iswspace(*lpszBegin))
			lpszBegin = ::_wcsinc(lpszBegin);

		UINT nTemp = 0U;
		if (bParseAttrib)	// parse attribute/value pairs?
		{
			_ASSERT(pcollAttr == NULL);
			// instantiate collection ...
			if ((pcollAttr = new CLiteHTMLAttributes) == NULL)
			{
				return (0U);
			}

			// ... and delegate parsing process
			nTemp = pcollAttr->parseFromStr(lpszBegin);
		}

		if (nTemp == 0)	// attribute/value pair parsing is disabled? 
						//	- OR - 
						// attribute/value pairs could not be parsed?
		{
			SAFE_DELETE_POINTER(pcollAttr);
			if ((lpszEnd = ::wcsstr(lpszBegin, L"/>")) == NULL)
			{
				if ((lpszEnd = ::wcschr(lpszBegin, L'>')) == NULL)
					return (0U);
			}
		}
		else
		{
			lpszEnd = lpszBegin + nTemp;

			// skip white-space after attribute/value pairs
			while (::iswspace(*lpszEnd))
				lpszEnd = ::_wcsinc(lpszEnd);
			
			// tag's ending delimeter could not be found?
			if (*lpszEnd == NULL)
			{
				SAFE_DELETE_POINTER(pcollAttr);
				return (0U);
			}
		}

		// a tag like this one: <BR/>
		if (*lpszEnd == L'/')
		{
			_ASSERT(bOpeningTag);
			bClosingTag = true;
			lpszEnd = ::_wcsinc(lpszEnd);
		}
	}

	// HTML tags always end with a greater-than '>' symbol
	if (*lpszEnd != L'>')
	{
		SAFE_DELETE_POINTER(pcollAttr);
		return (0U);
	}
	else
		lpszEnd = ::_wcsinc(lpszEnd);

	nRetVal = lpszEnd - lpszString;
	goto LUpdateAndExit;	// just to show the flow-of-control

LUpdateAndExit:
	bIsClosingTag = bClosingTag;
	bIsOpeningTag = bOpeningTag;
	m_strTagName = strTagName;
	trim(m_strTagName);
	SAFE_DELETE_POINTER(m_pcollAttr);
	m_pcollAttr = pcollAttr;
	pcollAttr = NULL;

	return (nRetVal);
}

#pragma warning (default : 4290)	// C++ Exception Specification ignored
#pragma warning(pop)

#endif	// !__LITEHTMLTAG_H__
