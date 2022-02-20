/**
	@file       code_page.h
	@date       2016/6/7   9:41
	@author		GongJie
	@version    v1.0.0
	@brief		编码转换
	@details	各种字符编码间的相互转换，使用C++标准类型
*/
#pragma once
#include<windows.h>
#include <string>

namespace yzs
{

	void			Unicode_to_UTF8(const wchar_t* in, unsigned int len, std::string& out);
	std::string	Unicode_to_UTF8(const std::wstring& in);

	void			UTF8_to_Unicode(const char* in, unsigned int len, std::wstring& out);
	std::wstring UTF8_to_Unicode(const std::string& in);

	void			Unicode_to_ANSI(const wchar_t* in, unsigned int len, std::string& out);
	std::string	Unicode_to_ANSI(const std::wstring& in);

	void			ANSI_to_Unicode(const char* in, unsigned int len, std::wstring& out);
	std::wstring	ANSI_to_Unicode(const std::string& in);

	void			ANSI_to_UTF8(const char* in, unsigned int len, std::string& out);
	std::string	ANSI_to_UTF8(const std::string& in);

	void			UTF8_to_ANSI(const char* in, unsigned int len, std::string& out);
	std::string	UTF8_to_ANSI(const std::string& in);
	std::string  NormalizeString(std::string &str_source);
	std::wstring NormalizeWstring(std::wstring &wstr_source);

}