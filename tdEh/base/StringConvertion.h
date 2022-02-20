/************************************************************************/
/* 
    Date :   12/2/2015
	File :   StringConvertion.h
    Author : Eadwin
    Funtion :  
	Version : 1.0
	Copyright (c) 2014-2015 Oasgames
*/
/************************************************************************/  

#ifndef BASE_STRING_CONVERSIONS_H_
#define BASE_STRING_CONVERSIONS_H_

#include <windows.h>
#include <string>

namespace base{

	// Converts between wide and UTF-8 representations of a string. On error, the
	// result is system-dependent.
	std::string SysWideToUTF8(const std::wstring& wide);
	std::wstring SysUTF8ToWide(const std::string& utf8);

	// Converts between wide and the system multi-byte representations of a string.
	// DANGER: This will lose information and can change (on Windows, this can
	// change between reboots).
	std::string SysWideToNativeMB(const std::wstring& wide);
	std::wstring SysNativeMBToWide(const std::string& native_mb);

	// Windows-specific ------------------------------------------------------------

	// Converts between 8-bit and wide strings, using the given code page. The
	// code page identifier is one accepted by the Windows function
	// MultiByteToWideChar().
	std::wstring SysMultiByteToWide(const std::string& mb,
		UINT code_page);
	std::string SysWideToMultiByte(const std::wstring& wide,
		UINT code_page);
}

#endif //BASE_STRING_CONVERSIONS_H_