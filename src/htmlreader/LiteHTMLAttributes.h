/**
 *	PROJECT - HTML Reader Class Library
 *
 *	LiteHTMLAttributes.h - Defines CLiteHTMLAttributes and 
 *	                       CLiteHTMLElemAttr
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
#ifndef __LITEHTMLATTRIBUTES_H__
#define __LITEHTMLATTRIBUTES_H__

/*
 * Conditional Includes
 */

#ifndef __LITEHTMLCOMMON_H__
#	include "LiteHTMLCommon.h"
#endif	// !__LITEHTMLCOMMON_H__

#ifndef __LITEHTMLENTITYRESOLVER_H__
#	include "LiteHTMLEntityResolver.h"
#endif	// !__LITEHTMLENTITYRESOLVER_H__

#pragma warning(push, 4)
#pragma warning (disable : 4290)	// C++ Exception Specification ignored
#pragma warning (disable : 4127)	// C++ Exception Specification ignored



class CLiteHTMLAttributes;	// forward declaration

/**
 * CLiteHTMLElemAttr
 *
 * @version 1.0 (Mar 18, 2004)
 * @author Gurmeet S. Kochar
 */
class CLiteHTMLElemAttr
{
	// Friends
	friend class CLiteHTMLAttributes;

// Constructors
public:
	CLiteHTMLElemAttr(const wchar_t* lpszAttribName = NULL, 
					  const wchar_t* lpszAttribValue = NULL)
	{
		Init();
		
		if (lpszAttribName)
		{
			m_strAttrName = lpszAttribName;
		}

		if (lpszAttribValue)
		{
			m_strAttrValue = lpszAttribValue;
		}
	}

	CLiteHTMLElemAttr(const CLiteHTMLElemAttr &rSource)
	{
		Init();
		m_strAttrName = rSource.m_strAttrName;
		m_strAttrValue = rSource.m_strAttrValue;
	}

// Initialization Helpers
private:
	static void Init(void)
	{
		if (_namedColors.size())
			return;

		// 28 system colors 
		_namedColors[L"activeborder"]		= (COLORREF)0x8000000A;
		_namedColors[L"activecaption"]		= (COLORREF)0x80000002;
		_namedColors[L"appworkspace"]		= (COLORREF)0x8000000C;
		_namedColors[L"background"]			= (COLORREF)0x80000001;
		_namedColors[L"buttonface"]			= (COLORREF)0x8000000F;
		_namedColors[L"buttonhighlight"]		= (COLORREF)0x80000014;
		_namedColors[L"buttonshadow"]		= (COLORREF)0x80000010;
		_namedColors[L"buttontext"]			= (COLORREF)0x80000012;
		_namedColors[L"captiontext"]			= (COLORREF)0x80000009;
		_namedColors[L"graytext"]			= (COLORREF)0x80000011;
		_namedColors[L"highlight"]			= (COLORREF)0x8000000D;
		_namedColors[L"highlighttext"]		= (COLORREF)0x8000000E;
		_namedColors[L"inactiveborder"]		= (COLORREF)0x8000000B;
		_namedColors[L"inactivecaption"]		= (COLORREF)0x80000003;
		_namedColors[L"inactivecaptiontext"]	= (COLORREF)0x80000013;
		_namedColors[L"infobackground"]		= (COLORREF)0x80000018;
		_namedColors[L"infotext"]			= (COLORREF)0x80000017;
		_namedColors[L"menu"]				= (COLORREF)0x80000004;
		_namedColors[L"menutext"]			= (COLORREF)0x80000007;
		_namedColors[L"scrollbar"]			= (COLORREF)0x80000000;
		_namedColors[L"threeddarkshadow"]	= (COLORREF)0x80000015;
		_namedColors[L"threedface"]			= (COLORREF)0x8000000F;
		_namedColors[L"threedhighlight"]		= (COLORREF)0x80000014;
		_namedColors[L"threedlightshadow"]	= (COLORREF)0x80000016;
		_namedColors[L"threedshadow"]		= (COLORREF)0x80000010;
		_namedColors[L"window"]				= (COLORREF)0x80000005;
		_namedColors[L"windowframe"]			= (COLORREF)0x80000006;
		_namedColors[L"windowtext"]			= (COLORREF)0x80000008;

		// 16 basic colors 
		_namedColors[L"black"]				= RGB(0x00, 0x00, 0x00);
		_namedColors[L"gray"]				= RGB(0x80, 0x80, 0x80);
		_namedColors[L"silver"]				= RGB(0xC0, 0xC0, 0xC0);
		_namedColors[L"white"]				= RGB(0xFF, 0xFF, 0xFF);
		_namedColors[L"yellow"]				= RGB(0xFF, 0xFF, 0x00);
		_namedColors[L"olive"]				= RGB(0x80, 0x80, 0x00);
		_namedColors[L"red"]					= RGB(0xFF, 0x00, 0x00);
		_namedColors[L"maroon"]				= RGB(0x80, 0x00, 0x00);
		_namedColors[L"fuchsia"]				= RGB(0xFF, 0x00, 0xFF);
		_namedColors[L"purple"]				= RGB(0x80, 0x00, 0x80);
		_namedColors[L"blue"]				= RGB(0x00, 0x00, 0xFF);
		_namedColors[L"navy"]				= RGB(0x00, 0x00, 0x80);
		_namedColors[L"aqua"]				= RGB(0x00, 0xFF, 0xFF);
		_namedColors[L"teal"]				= RGB(0x00, 0x80, 0x80);
		_namedColors[L"lime"]				= RGB(0x00, 0xFF, 0x00);
		_namedColors[L"green"]				= RGB(0x00, 0x80, 0xFF);

		// additional named colors 
		_namedColors[L"darkolivegreen"]		= RGB(0x55, 0x6B, 0x2F);
		_namedColors[L"olivedrab"]			= RGB(0x6B, 0x8E, 0x23);
		_namedColors[L"yellowgreen"]			= RGB(0x9A, 0xCD, 0x32);
		_namedColors[L"lawngreen"]			= RGB(0x7C, 0xFC, 0x00);
		_namedColors[L"chartreuse"]			= RGB(0x7F, 0xFF, 0x00);
		_namedColors[L"greenyellow"]			= RGB(0xAD, 0xFF, 0x2F);
		_namedColors[L"palegreen"]			= RGB(0x98, 0xFB, 0x98);
		_namedColors[L"lightgreen"]			= RGB(0x90, 0xEE, 0x90);
		_namedColors[L"darkgreen"]			= RGB(0x00, 0x64, 0x00);
		_namedColors[L"forestgreen"]			= RGB(0x22, 0x8B, 0x22);
		_namedColors[L"seagreen"]			= RGB(0x2E, 0x8B, 0x57);
		_namedColors[L"mediumseagreen"]		= RGB(0x3C, 0xB3, 0x71);
		_namedColors[L"limegreen"]			= RGB(0x32, 0xCD, 0x32);
		_namedColors[L"darkseagreen"]		= RGB(0x8F, 0xBC, 0x8B);
		_namedColors[L"springgreen"]			= RGB(0x00, 0xFF, 0x7F);
		_namedColors[L"mediumspringgreen"]	= RGB(0x00, 0xFA, 0x99);
		_namedColors[L"darkslategray"]		= RGB(0x2F, 0x4F, 0x4F);
		_namedColors[L"darkcyan"]			= RGB(0x00, 0x8B, 0x8B);
		_namedColors[L"cadetblue"]			= RGB(0x5F, 0x9E, 0xA0);
		_namedColors[L"lightseagreen"]		= RGB(0x20, 0xB2, 0xAA);
		_namedColors[L"mediumaquamarine"]	= RGB(0x66, 0xCD, 0xAA);
		_namedColors[L"turquoise"]			= RGB(0x40, 0xE0, 0xD0);
		_namedColors[L"aquamarine"]			= RGB(0x7F, 0xFF, 0xD4);
		_namedColors[L"paleturquoise"]		= RGB(0xAF, 0xEE, 0xEE);
		_namedColors[L"slategray"]			= RGB(0x70, 0x80, 0x90);
		_namedColors[L"lightslategray"]		= RGB(0x77, 0x88, 0x99);
		_namedColors[L"steelblue"]			= RGB(0x46, 0x82, 0xB4);
		_namedColors[L"deepskyblue"]			= RGB(0x00, 0xBF, 0xFF);
		_namedColors[L"darkturquoise"]		= RGB(0x00, 0xCE, 0xD1);
		_namedColors[L"mediumturquoise"]		= RGB(0x48, 0xD1, 0xCC);
		_namedColors[L"powderblue"]			= RGB(0xB0, 0xE0, 0xE6);
		_namedColors[L"lightcyan"]			= RGB(0xE0, 0xFF, 0xFF);
		_namedColors[L"darkblue"]			= RGB(0x00, 0x00, 0x8B);
		_namedColors[L"mediumblue"]			= RGB(0x00, 0x00, 0xCD);
		_namedColors[L"royalblue"]			= RGB(0x41, 0x69, 0xe1);
		_namedColors[L"dodgerblue"]			= RGB(0x1E, 0x90, 0xFF);
		_namedColors[L"cornflowerblue"]		= RGB(0x64, 0x95, 0xED);
		_namedColors[L"skyblue"]				= RGB(0x87, 0xCE, 0xEB);
		_namedColors[L"lightskyblue"]		= RGB(0x87, 0xCE, 0xFA);
		_namedColors[L"lightblue"]			= RGB(0xAD, 0xD8, 0xE6);
		_namedColors[L"midnightblue"]		= RGB(0x19, 0x19, 0x70);
		_namedColors[L"darkslateblue"]		= RGB(0x48, 0x3D, 0x8B);
		_namedColors[L"blueviolet"]			= RGB(0x8A, 0x2B, 0xE2);
		_namedColors[L"slateblue"]			= RGB(0x6A, 0x5A, 0xCD);
		_namedColors[L"mediumslateblue"]		= RGB(0x7B, 0x68, 0xEE);
		_namedColors[L"mediumpurple"]		= RGB(0x93, 0x70, 0xDB);
		_namedColors[L"lightsteelblue"]		= RGB(0xB0, 0xC4, 0xDE);
		_namedColors[L"lavender"]			= RGB(0xE6, 0xE6, 0xFA);
		_namedColors[L"indigo"]				= RGB(0x4B, 0x00, 0x82);
		_namedColors[L"darkviolet"]			= RGB(0x94, 0x00, 0xD3);
		_namedColors[L"darkorchid"]			= RGB(0x99, 0x32, 0xCC);
		_namedColors[L"mediumorchid"]		= RGB(0xBA, 0x55, 0xD3);
		_namedColors[L"orchid"]				= RGB(0xDA, 0x70, 0xD6);
		_namedColors[L"violet"]				= RGB(0xEE, 0x82, 0xEE);
		_namedColors[L"plum"]				= RGB(0xDD, 0xA0, 0xDD);
		_namedColors[L"thistle"]				= RGB(0xD8, 0xDF, 0xD8);
		_namedColors[L"darkmagenta"]			= RGB(0x8B, 0x00, 0x8B);
		_namedColors[L"mediumvioletred"]		= RGB(0xC7, 0x15, 0x85);
		_namedColors[L"deeppink"]			= RGB(0xFF, 0x14, 0x93);
		_namedColors[L"palmvioletred"]		= RGB(0xDB, 0x70, 0x93);
		_namedColors[L"hotpink"]				= RGB(0xFF, 0x69, 0xB4);
		_namedColors[L"lightpink"]			= RGB(0xFF, 0xB6, 0xC1);
		_namedColors[L"pink"]				= RGB(0xFF, 0xC0, 0xCB);
		_namedColors[L"mistyrose"]			= RGB(0xFF, 0xE4, 0xE1);
		_namedColors[L"brown"]				= RGB(0xA5, 0x2A, 0x2A);
		_namedColors[L"indianred"]			= RGB(0xCD, 0x5C, 0x5C);
		_namedColors[L"rosybrown"]			= RGB(0xBC, 0x8F, 0x8F);
		_namedColors[L"salmon"]				= RGB(0xFA, 0x80, 0x72);
		_namedColors[L"lightcoral"]			= RGB(0xF0, 0x80, 0x80);
		_namedColors[L"darksalmon"]			= RGB(0xE9, 0x96, 0x7A);
		_namedColors[L"lightsalmon"]			= RGB(0xFF, 0xA0, 0x7A);
		_namedColors[L"peachpuff"]			= RGB(0xFF, 0xDA, 0xB9);
		_namedColors[L"darkred"]				= RGB(0x8B, 0x00, 0x00);
		_namedColors[L"firebrick"]			= RGB(0xB2, 0x22, 0x22);
		_namedColors[L"crimson"]				= RGB(0xDC, 0x14, 0x3C);
		_namedColors[L"orangered"]			= RGB(0xFF, 0x45, 0x00);
		_namedColors[L"tomato"]				= RGB(0xFF, 0x63, 0x47);
		_namedColors[L"coral"]				= RGB(0xFF, 0x7F, 0x50);
		_namedColors[L"wheat"]				= RGB(0xF5, 0xDE, 0xB3);
		_namedColors[L"papayawhip"]			= RGB(0xFF, 0xEF, 0xD5);
		_namedColors[L"sienna"]				= RGB(0xA0, 0x52, 0x2D);
		_namedColors[L"chocolate"]			= RGB(0xD2, 0x69, 0x1E);
		_namedColors[L"darkorange"]			= RGB(0xFF, 0x8C, 0x00);
		_namedColors[L"sandybrown"]			= RGB(0xF4, 0xA4, 0x60);
		_namedColors[L"orange"]				= RGB(0xFF, 0xA5, 0x00);
		_namedColors[L"navajowhite"]			= RGB(0xFF, 0xDE, 0xAD);
		_namedColors[L"moccasin"]			= RGB(0xFF, 0xE4, 0xB5);
		_namedColors[L"saddlebrown"]			= RGB(0x8B, 0x45, 0x13);
		_namedColors[L"peru"]				= RGB(0xCD, 0x85, 0x3F);
		_namedColors[L"burlywood"]			= RGB(0xDE, 0xB8, 0x87);
		_namedColors[L"tan"]					= RGB(0xD2, 0xB4, 0x8C);
		_namedColors[L"bisque"]				= RGB(0xFF, 0xE4, 0xC4);
		_namedColors[L"blanchedalmond"]		= RGB(0xFF, 0xEB, 0xCD);
		_namedColors[L"antiquewhite"]		= RGB(0xFA, 0xEB, 0xD7);
		_namedColors[L"darkgoldenrod"]		= RGB(0xB8, 0x86, 0x0B);
		_namedColors[L"goldenrod"]			= RGB(0xDA, 0xA5, 0x20);
		_namedColors[L"darkkhaki"]			= RGB(0xBD, 0xB7, 0x6B);
		_namedColors[L"gold"]				= RGB(0xFF, 0xD7, 0x00);
		_namedColors[L"khaki"]				= RGB(0xF0, 0xE6, 0x8C);
		_namedColors[L"palegoldenrod"]		= RGB(0xEE, 0xE8, 0xAA);
		_namedColors[L"lemonchiffon"]		= RGB(0xFF, 0xFA, 0xCD);
		_namedColors[L"beige"]				= RGB(0xF5, 0xF5, 0xDC);
		_namedColors[L"lightgoldenrodyellow"]= RGB(0xFA, 0xFA, 0xD2);
		_namedColors[L"lightyellow"]			= RGB(0xFF, 0xFF, 0xE0);
		_namedColors[L"ivory"]				= RGB(0xFF, 0xFF, 0x00);
		_namedColors[L"cornsilk"]			= RGB(0xFF, 0xF8, 0xDC);
		_namedColors[L"oldlace"]				= RGB(0xFD, 0xF5, 0xE6);
		_namedColors[L"florawhite"]			= RGB(0xFF, 0xFA, 0xF0);
		_namedColors[L"honeydew"]			= RGB(0xF0, 0xFF, 0xF0);
		_namedColors[L"mintcream"]			= RGB(0xF5, 0xFF, 0xFA);
		_namedColors[L"azure"]				= RGB(0xF0, 0xFF, 0xFF);
		_namedColors[L"ghostwhite"]			= RGB(0xF8, 0xF8, 0xFF);
		_namedColors[L"linen"]				= RGB(0xFA, 0xF0, 0xE6);
		_namedColors[L"seashell"]			= RGB(0xFF, 0xF5, 0xEE);
		_namedColors[L"snow"]				= RGB(0xFF, 0xFA, 0xFA);
		_namedColors[L"dimgray"]				= RGB(0x69, 0x69, 0x69);
		_namedColors[L"darkgray"]			= RGB(0xA9, 0xA9, 0xA9);
		_namedColors[L"lightgray"]			= RGB(0xD3, 0xD3, 0xD3);
		_namedColors[L"gainsboro"]			= RGB(0xDC, 0xDC, 0xDC);
		_namedColors[L"whitesmoke"]			= RGB(0xF5, 0xF5, 0xF5);
		_namedColors[L"ghostwhite"]			= RGB(0xF8, 0xF8, 0xFF);
		_namedColors[L"aliceblue"]			= RGB(0xF0, 0xF8, 0xFF);

	}

// Attributes
public:
	/**
	 * @return name of this CLiteHTMLElemAttr
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	std::wstring getName(void) const
		{ return (m_strAttrName); }

	/**
	 * @return value of this CLiteHTMLElemAttr
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	std::wstring getValue(void) const
		{ return (m_strAttrValue); }

	/**
	 * Determines if the attribute value is a named color value
	 * @return true if attribute value is a named color, otherwise, false
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	bool isNamedColorValue(void) const
	{
		if ( (m_strAttrValue.size()) && (::iswalpha(m_strAttrValue[0])) )
		{
//			COLORREF crTemp = _clrInvalid;
			std::wstring		strKey(m_strAttrValue);

			std::transform(strKey.begin(), strKey.end(), strKey.begin(), tolower); 
			if (_namedColors.find(strKey) != _namedColors.end())
				return (true);
		}
		return (false);
	}

	/**
	 * Determines if the attribute value is a named system color value
	 * @return true if value is a named system color, false otherwise
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	bool isSysColorValue(void) const
	{
		if ( (m_strAttrValue.size()) && (::iswalpha(m_strAttrValue[0])) )
		{
			COLORREF	crTemp = _clrInvalid;
			std::wstring	strKey(m_strAttrValue);

			std::transform(strKey.begin(), strKey.end(), strKey.begin(), tolower); 
			if (_namedColors.find(strKey) != _namedColors.end())
			{
				crTemp = _namedColors[strKey];
				return (crTemp >= 0x80000000 && crTemp <= 0x80000018);
			}
		}
		return (false);
	}

	/**
	 * Determines if the attribute value is a color value in 
	 * hexadecimal format
	 * @return true if attribute value is a color value, otherwise, false
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	bool isHexColorValue(void) const
	{
		// zero-length attribute value?
		if (m_strAttrValue.empty())
			return (false);

		if (m_strAttrValue[0] == L'#')
		{
			if (m_strAttrValue.size() > 1)
			{
				for (int i = 1; i < (int)m_strAttrValue.size(); i++)
				{
					if (!::iswxdigit(m_strAttrValue[i]))
						return (false);
				}
				return (true);
			}
		}

		return (false);
	}

	/**
	 * Determines if the attribute value contains a color reference
	 * @return true, if attribute value is color value, false otherwise.
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	bool isColorValue(void) const
		{ return (isNamedColorValue() || isHexColorValue()); }

	/**
	 * Returns the color value of the attribute
	 * @return a COLORREF representing the color
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	COLORREF getColorValue(void) const
	{
		COLORREF crTemp = _clrInvalid;
		if (isNamedColorValue())
		{
			std::wstring strKey(m_strAttrValue);
			std::transform(strKey.begin(), strKey.end(), strKey.begin(), tolower); 
			if (_namedColors.find(strKey) != _namedColors.end())
			{
				crTemp = _namedColors[strKey];
				// is this a system named color value?
				if (crTemp >= 0x80000000 && crTemp <= 0x80000018)
					crTemp = ::GetSysColor(crTemp & 0x7FFFFFFF);
			}
		}
		else if (isHexColorValue())
			crTemp = ::wcstoul(m_strAttrValue.substr(1).c_str(), NULL, 16);
		return (crTemp);
	}
	
	/**
	 * Returns the RGB value of the attribute in hexadecimal format
	 * @return hexadecimal string representing the color value
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	std::wstring getColorHexValue(void) const
	{
		std::wstring	strColorHex;
		if (isHexColorValue())
			strColorHex = m_strAttrValue.substr(1);
		else
		{
			COLORREF crTemp = getColorValue();
			if (crTemp != _clrInvalid)
			{
				wchar_t tmp[64]; 
				swprintf_s(tmp, 64, L"#%06x", crTemp);
				strColorHex = tmp;
			}
		}
		return (strColorHex);
	}

	/**
	 * Checks to see if the attribute contains a percent value
	 * @return true if value is a percent value, otherwise, false
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	bool isPercentValue(void) const
	{
		if (!m_strAttrValue.size())
		{
			return false;
		}
		
		return (m_strAttrValue[m_strAttrValue.size()-1] == L'%' ? true : false); 
	}

	/**
	 * Returns a percent value of the attribute
	 * @return percentage value
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	unsigned short getPercentValue(unsigned short max = _percentMax) const
	{
		_ASSERT(max > 0);
		if (!isPercentValue())	
			return (0);
		unsigned short	percentVal = (unsigned short)((short)*this);
		return ((percentVal > max ? max : percentVal));
	}
	
	/**
	 * Parses a length value from the attribute/value 
	 * and identifies its length unit also
	 *
	 * @param rUnit - this will receive the type of the length unit
	 *
	 * @return an integer value of the attribute 
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	enum LengthUnitsEnum { em, ex, px, per, in, cm, mm, pt, pc };
	short getLengthValue(LengthUnitsEnum &rUnit) const
	{
		static const wchar_t	_szUnits[][4] = 
		{
			/** relative length units */
			L"em", L"ex", L"px", L"%", 
			/** absolute length units */
			L"in", L"cm", L"mm", L"pt", L"pc" 
		};

		if (m_strAttrValue.empty())
			return (0);

		int i = 0;
		for (i = 0; i < sizeof(_szUnits)/sizeof(_szUnits[0]); i++)
		{
			if (0 == _wcsicmp(m_strAttrValue.substr(m_strAttrValue.size()-::lstrlenW(_szUnits[i])).c_str(),_szUnits[i]))
			{
				rUnit = (LengthUnitsEnum)i;
				break;
			}
		}
		if (i == sizeof(_szUnits)/sizeof(_szUnits[0]))
			return (0);
		return (*this);
	}

// Operators
public:
	/**
	 * Converts attribute value to bool
	 * @return true or false
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	operator bool() const
	{
		if (!_wcsicmp(m_strAttrValue.c_str(),(_T("true"))))
			return (true);
		if (!_wcsicmp(m_strAttrValue.c_str(),(_T("false"))))
			return (false);
		return (((short)*this ? true : false));
	}

	/**
	 * Converts attribute value to BYTE (unsigned char)
	 * @return the left-most character of attribute value
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	operator BYTE() const
		{ return ((BYTE)(m_strAttrValue.size() ? m_strAttrValue[0] : 0)); }
	
	/**
	 * Converts attribute value to double
	 * @return 0.00 on failure, otherwise, a numeric value
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	operator double() const
		{ return (::wcstod(m_strAttrValue.c_str(), NULL)); }
	
	/**
	 * Converts attribute value to signed short int
	 * @return 0 on failure, otherwise, an integer value
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	operator short() const
		{ return ((short)::_wtoi(m_strAttrValue.c_str())); }
	
	/**
	 * @return attribute value
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	operator const wchar_t* () const
		{ return (m_strAttrValue.c_str()); }

// Private Operations
private:
	/**
	 * Sets the value of an attribute. Takes care of the following:
	 *  1. Ignores leading and trailing white-space characters
	 *  2. Replaces character entities with appropriate characters.
	 *  3. Ignores line feeds (LF).
	 *  4. Replaces each carriage-return (CR) or tab with a single space.
	 *
	 * @param lpszValue - new attribute value
	 * 
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	void putValue(const wchar_t* lpszValue)
	{
		_ASSERT(lpszValue);

		m_strAttrValue = lpszValue;
		
		// ignore leading white-spaces
		ltrim(m_strAttrValue);
		
		// ignore trailing white-spaces
		rtrim(m_strAttrValue);
		
		// ignore line feeds
		remove(m_strAttrValue, L'\n');
	
		// replace tab and carriage-return with a single space
		replace(m_strAttrValue, L"\r", L" ");
		replace(m_strAttrValue, L"\t", L" ");

		/** resolve entity reference(s) */
		int		iCurPos = -1, iParseLen = 0;
		wchar_t	chSubst = 0;
		do
		{
			if ((iCurPos = (int)m_strAttrValue.find(L'&', ++iCurPos)) == (int)std::wstring::npos)
				break;
			
			iParseLen = CLiteHTMLEntityResolver::resolveEntity(m_strAttrValue.substr(iCurPos).c_str(), chSubst);
			if (iParseLen)
			{
				std::wstring tmp(1,chSubst);
				
				replace(m_strAttrValue, m_strAttrValue.substr(iCurPos, iParseLen),tmp);
			}
		}
		while (true);
	}

// Parsing Helpers
public:
	// parses an attribute/value pair from the given string
	UINT parseFromStr(const wchar_t* lpszString);

// Data Members
public:
	static const COLORREF		_clrInvalid;	// an invalid color
	static const unsigned short	_percentMax;	// maximum allowable percentage value
private:
	typedef std::map<std::wstring, COLORREF>	CNamedColors;

	static CNamedColors		_namedColors;	// collection of named colors
	std::wstring			m_strAttrName,  // attribute name
							m_strAttrValue; // attribute value
};

/**
 * CLiteHTMLElemAttr::parseFromStr
 *
 * @param lpszString - string to parse
 *
 * @return number of TCHARs successfully parsed
 * @since 1.0
 * @author Gurmeet S. Kochar
 */
inline UINT CLiteHTMLElemAttr::parseFromStr(const wchar_t* lpszString)
{
	_ASSERT(lpszString);

	const wchar_t*	lpszBegin = lpszString;
	const wchar_t*	lpszEnd;
	wchar_t	ch = 0;

	// skip leading white-space characters
	while (::iswspace(*lpszBegin))
		lpszBegin = ::_wcsinc(lpszBegin);

	if ( (!::iswalnum(*lpszBegin)) && 
		(*lpszBegin != L'-') && (*lpszBegin != L':') && 
		(*lpszBegin != L'_') && (*lpszBegin != L'.') )
	{
		return (0U);
	}

	lpszEnd = lpszBegin;
	do
	{
		// attribute name may contain letters (a-z, A-Z), digits (0-9), 
		// underscores '_', hyphen '-', colons ':', and periods '.'
		if ( (!::iswalnum(*lpszEnd)) && 
			 (*lpszEnd != L'-') && (*lpszEnd != L':') && 
			 (*lpszEnd != L'_') && (*lpszEnd != L'.') )
		{
			_ASSERT(lpszEnd != lpszBegin);

			// only white-space characters, a null-character, an 
			// equal-sign, a greater-than symbol, or a forward-slash 
			// can act as the separator between an attribute and its 
			// value
			if (*lpszEnd == NULL || ::iswspace(*lpszEnd) || 
				*lpszEnd == L'=' || 
				*lpszEnd == L'>' || *lpszEnd == L'/')
			{
				break;
			}

			return (0U);	// any other character will fail parsing process
		}

		lpszEnd = ::_wcsinc(lpszEnd);
	}
	while (true);

	// extract attribute name
	std::wstring	strAttrName(lpszBegin, lpszEnd - lpszBegin);

	// skip leading white-space characters
	while (::iswspace(*lpszEnd))
		lpszEnd = ::_wcsinc(lpszEnd);
	
	if (*lpszEnd != L'=')
	{
		m_strAttrName = strAttrName;
		m_strAttrValue.clear();
		return (lpszEnd - lpszString);
	}
	else
	{
		// skip white-space characters after equal-sign 
		// and the equal-sign itself
		do {
			lpszEnd = ::_wcsinc(lpszEnd);
		} while (::iswspace(*lpszEnd));

		lpszBegin = lpszEnd;
		ch = *lpszEnd;
		
		// is attribute value wrapped in quotes?
		if (ch == L'\'' || ch == L'\"')
		{
			lpszBegin = ::_wcsinc(lpszBegin);	// skip quote symbol
			do
			{
				lpszEnd = ::_wcsinc(lpszEnd);
			}
			// Loop until we find the same quote character that 
			// was used at the starting of the attribute value.
			// Anything within these quotes is considered valid!
			// NOTE that the entity references are resolved later.
			while (*lpszEnd != NULL && *lpszEnd != ch);
		}

		// open attribute value i.e. not wrapped in quotes?
		else
		{
			do 
			{ 
				lpszEnd = ::_wcsinc(lpszEnd);
			}
			// loop until we find a tag ending delimeter or any 
			// white-space character, or until we reach at the 
			// end of the string buffer
			while ( (*lpszEnd != NULL && !::iswspace(*lpszEnd) && *lpszEnd != L'/' && *lpszEnd != L'>') || 
					(*lpszEnd == L'/' && *(lpszEnd+1) != L'>') //FIX something like: <SCRIPT type=text/javascript>show_ads_zone(421);</SCRIPT> by Zhangzhou, 2011/10/27
				  );
		}

		m_strAttrName = strAttrName;
		if (lpszEnd == lpszBegin)	// empty attribute value?
			m_strAttrValue.clear();
		else
			// use putValue() instead of direct assignment; 
			// this will automatically normalize data before 
			// assigning according to the specs and will 
			// also resolve entity references!!!
			putValue(std::wstring(lpszBegin, lpszEnd - lpszBegin).c_str());

		// calculate and return the count of characters successfully parsed
		return ((lpszEnd - lpszString) + 
				(ch == L'\'' || ch == L'\"' ? /*sizeof(wchar_t)*/1 : 0) );
	}

	return (0U);
}

/**
 * CLiteHTMLAttributes
 *
 * @version 1.0 (Mar 18, 2004)
 * @author Gurmeet S. Kochar
 */
class CLiteHTMLAttributes
{
// Construction/Destruction
public:
	CLiteHTMLAttributes() 
		: m_parrAttrib(NULL)
	{ }

	/**
	 * @param bCopy - if true, this CLiteHTMLAttributes makes a copy 
	 * of the encapsulated pointer. if false, this constructor takes 
	 * ownership of the encapsulated pointer.
	 *
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	CLiteHTMLAttributes(CLiteHTMLAttributes &rSource, bool bCopy = false)
		: m_parrAttrib(NULL)
	{
		if (!bCopy)
		{
			m_parrAttrib = rSource.m_parrAttrib;
			rSource.m_parrAttrib = NULL;
		}
		else
		{
			const int	nElemCount = rSource.getCount();
			if (nElemCount)
			{
				if ((m_parrAttrib = new CElemAttrArray) == NULL)
					return;
				
				CLiteHTMLElemAttr	*pItem = NULL;
				//m_parrAttrib->SetSize(nElemCount);
				
				/** DEEP COPY BEGIN */
				for (int iElem = 0; iElem < nElemCount; iElem++)
				{
					if ((pItem = new CLiteHTMLElemAttr(rSource[iElem])) == NULL)
					{
						removeAll();
						return;
					}
					
					(*m_parrAttrib).push_back(pItem);
					pItem = NULL;
				}
				/** DEEP COPY END */
			}
		}
	}

	virtual ~CLiteHTMLAttributes()
		{ removeAll(); }

// Initialization
public:
	// parses attribute/value pairs from the given string
	UINT parseFromStr(const wchar_t* lpszString);

// Attributes
public:
	/**
	 * Returns the count of CLiteHTMLElemAttr items in this collection
	 * @return number of items
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	int getCount(void) const
	{
		if (m_parrAttrib != NULL)
			return ((int)m_parrAttrib->size());
		return (0);
	}

	/**
	 * Look up the index of an attribute given its name.
	 * If more than one attribute with the same name exist, 
	 * this will return the index of the first match.
	 *
	 * @param lpszAttributeName - name of the attribute
	 *
	 * @return zero-based index of an attribute, or -1 if not found
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	int getIndexFromName(const wchar_t* lpszAttributeName) const
	{
		_ASSERT(lpszAttributeName);
		CLiteHTMLElemAttr	*pItem = NULL;
		for (int iElem = 0; iElem < getCount(); iElem++)
		{
			if ((pItem = (*m_parrAttrib)[iElem]) == NULL)	// just in case
				continue;
			
			// perform a CASE-INSENSITIVE search
			if (_wcsicmp(pItem->m_strAttrName.c_str(), lpszAttributeName) == 0)
				return (iElem);
		}
		return (-1);
	}

	/**
	 * Returns a CLiteHTMLElemAttr object given an attribute's index
	 *
	 * @return CLiteHTMLElemAttr containing the name and value of an attribute
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	CLiteHTMLElemAttr operator[](int nIndex) const
	{
		if (!(nIndex >= 0 && nIndex < getCount()))
		{
			//_ASSERT(FALSE);
			return (CLiteHTMLElemAttr());
		}
		return ( *((*m_parrAttrib)[nIndex]) );
	}

	/**
	 * Returns a CLiteHTMLElemAttr object given an attribute name
	 *
	 * @return CLiteHTMLElemAttr containing the name and value of an attribute
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	CLiteHTMLElemAttr operator[](const wchar_t* lpszIndex) const
	{
		_ASSERT(lpszIndex);
		return ((*this)[getIndexFromName(lpszIndex)]);
	}

	/**
	 * Returns a CLiteHTMLElemAttr object given an attribute's index
	 *
	 * @return CLiteHTMLElemAttr containing the name and value of an attribute
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	CLiteHTMLElemAttr getAttribute(int nIndex) const
		{ return ((*this)[nIndex]); }

	/**
	 * Returns a CLiteHTMLElemAttr object given an attribute name
	 *
	 * @return CLiteHTMLElemAttr containing the name and value of an attribute
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	CLiteHTMLElemAttr getAttribute(const wchar_t* lpszIndex) const
	{
		_ASSERT(lpszIndex);
		return ((*this)[getIndexFromName(lpszIndex)]);
	}

	/**
	 * Returns the name of an attribute given its index
	 *
	 * @return name of an attribute
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	std::wstring getName(int nIndex) const
		{ return ((*this)[nIndex].m_strAttrName); }

	/**
	 * Returns the value of an attribute given its index
	 *
	 * @return value of an attribute
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	std::wstring getValue(int nIndex) const
		{ return ((*this)[nIndex].m_strAttrValue); }

	/**
	 * Returns the value of an attribute given its name
	 *
	 * @return value of an attribute
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	std::wstring getValueFromName(const wchar_t* lpszAttributeName) const
		{ return ((*this)[lpszAttributeName].m_strAttrValue); }

// Operations
public:
	/**
	 * Adds a new CLiteHTMLElemAttr item to the collection
	 *
	 * @param lpszName - attribute name (serves as the key to the item)
	 * @param lpszValue - attribute value
	 * @param bReplaceOld - If an item with the same name as specified 
	 *        by lpszName already exists in the collection, this 
	 *        parameter is used to determine whether to replace the 
	 *        existing item or add a new one
	 *
	 * @return pointer to a CLiteHTMLElemAttr
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	CLiteHTMLElemAttr* addAttribute(const wchar_t* lpszName, const wchar_t* lpszValue)
	{
		_ASSERT(lpszName);
		_ASSERT(lpszValue);

		CLiteHTMLElemAttr	*pItem = new CLiteHTMLElemAttr(lpszName, lpszValue);
		if (pItem != NULL)
		{
			if (m_parrAttrib == NULL)
			{
				if ((m_parrAttrib = new CElemAttrArray) == NULL)
				{
					SAFE_DELETE_POINTER(pItem);
					return (NULL);
				}
			}
			
			m_parrAttrib->push_back(pItem);
		}
		return (pItem);
	}
	
	/**
	 * Removes an CLiteHTMLElemAttr item from the collection
	 *
	 * @param lpszName - attribute to remove
	 *
	 * @return true if successful, false otherwise
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	bool removeAttribute(int nIndex)
	{
		if (!(nIndex >= 0 && nIndex < getCount()))
			return (false);
		CLiteHTMLElemAttr	*pItem = (*m_parrAttrib)[nIndex];
		SAFE_DELETE_POINTER(pItem);
		return (true);
	}
	
	/**
	 * Removes all CLiteHTMLElemAttr items from the collection
	 * @return true if successful, false otherwise
	 * @since 1.0
	 * @author Gurmeet S. Kochar
	 */
	bool removeAll(void)
	{
		CLiteHTMLElemAttr	*pItem = NULL;
		for (int iElem = 0; iElem < getCount(); iElem++)
		{
			_ASSERT((pItem = (*m_parrAttrib)[iElem]) != NULL);
			SAFE_DELETE_POINTER(pItem);
		}
		SAFE_DELETE_POINTER(m_parrAttrib);
		return (true);
	}

// Data Members
private:
	//typedef CTypedPtrArray<CPtrArray, CLiteHTMLElemAttr*>	CElemAttrArray;
	typedef std::vector<CLiteHTMLElemAttr*> CElemAttrArray;
	CElemAttrArray	*m_parrAttrib;	// array of attributes/value pairs
};

/**
 * CLiteHTMLAttributes::parseFromStr
 *
 * @param lpszString - string to parse. It can contain pairs such as: 
 *
 *          1. NAME
 *          2. NAME=VALUE
 *          3. NAME='VALUE'
 *          4. NAME="VALUE"
 *
 *        NAME consist of letters, digits, underscores, 
 *        colons, hyphens, and periods
 *
 *        NOTE that white-spaces between NAME and equal-sign AND 
 *        equal-sign and VALUE is allowed.
 *
 * @return number of TCHARs successfully parsed
 * @since 1.0
 * @author Gurmeet S. Kochar
 */
inline UINT CLiteHTMLAttributes::parseFromStr(const wchar_t* lpszString)
{
	_ASSERT(lpszString);

	CElemAttrArray		*pcoll = NULL;
	CLiteHTMLElemAttr	oElemAttr;
	const UINT			nStrLen = ::wcslen(lpszString);
	UINT				nRetVal = 0U, 
						nTemp = 0U;

	do
	{
		// try to parse an attribute/value 
		// pair from the rest of the string
		if (!(nTemp = oElemAttr.parseFromStr(&lpszString[nRetVal])))
		{
			if (!nRetVal)
				goto LError;
			break;
		}

		// collection has not been instantiated until now?
		if (pcoll == NULL)
		{
			// instantiate now
			if ((pcoll = new CElemAttrArray) == NULL)
				// out of memory?
			{
				goto LError;
			}
		}

		// add attribute/value pair to collection
		pcoll->push_back(new CLiteHTMLElemAttr(oElemAttr));
			

		// advance seek pointer
		nRetVal += nTemp;
	}
	
	// do we still have something in the buffer to parse?
	while (nRetVal < nStrLen);

	// collection was never instantiated?
	if (pcoll == NULL)
		goto LError;
	// collection is empty?
	if (pcoll->empty())
		goto LError;
	// current collection could not be emptied?
	if (!removeAll())
		goto LError;

	m_parrAttrib = pcoll;
	pcoll = NULL;
	goto LCleanExit;	// success!

LError:
	SAFE_DELETE_POINTER(pcoll);
	nRetVal = 0U;

LCleanExit:
	return (nRetVal);
}

#pragma warning(default : 4290)
#pragma warning(pop)

#endif	// !__LITEHTMLATTRIBUTES_H__
