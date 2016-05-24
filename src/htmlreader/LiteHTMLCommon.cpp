
#include "stdafx.h"
#include "LiteHTMLCommon.h"

#ifdef _DEBUG
#	define new DEBUG_NEW
#	undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif	// _DEBUG

#pragma warning(push, 4)

// trim from both ends 
std::wstring &trim(std::wstring &s) { 
	return ltrim(rtrim(s)); 
} 
/*
// trim from start 
std::wstring &ltrim(std::wstring &s) { 
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(isspace)))); 
	return s; 
} 

// trim from end 
std::wstring &rtrim(std::wstring &s) { 
	
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(isspace))).base(), s.end()); 
	return s; 
} 
*/

std::wstring &ltrim(std::wstring& str) 
{ 
	size_t pos = str.find_first_not_of(L" \n\r\t");
	if (pos == std::wstring::npos)
	{
		str = L"";
		return str;
	}

	return str = str.substr(pos); 
} 

std::wstring &rtrim(std::wstring& str) 
{ 
	size_t pos = str.find_last_not_of(L" \n\r\t");
	if (pos == std::wstring::npos)
	{
		str = L"";
		return str;
	}

	return str = str.substr(0,pos+1); 
} 


std::wstring &remove(std::wstring &s, wchar_t c)
{
	std::wstring::size_type k = 0;
	while((k=s.find(c,k))!=s.npos) 
	{
		s.erase(k, 1);
	}

	return s;
}

std::wstring &replace(std::wstring &source, const std::wstring &find, const std::wstring& szReplace)
{
	size_t j;
	for (;(j = source.find( find )) != std::wstring::npos;)
	{
		source.replace( j, find.length(), szReplace );
	}

	return source;
}



#pragma warning(pop)