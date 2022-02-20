#include "code_page.h"
#include <assert.h>


using namespace std;


void yzs::Unicode_to_UTF8(const wchar_t* in, unsigned int len, string& out)
{   
	if (in == NULL) out = "";
	size_t out_len = len * 3 + 1;
	char* pBuf = new char[out_len];
	if ( NULL == pBuf )
	{
		return;
	}
	char* pResult = pBuf;
	memset(pBuf, 0, out_len);

	out_len = ::WideCharToMultiByte(CP_UTF8, 0, in, len, pBuf, len * 3, NULL, NULL);
	out.assign( pResult, out_len );

	delete [] pResult;
	pResult = NULL;
	return;
}

string yzs::Unicode_to_UTF8(const wstring& in)
{
	if (in == L"") return "";
	string out;
	Unicode_to_UTF8(in.c_str(), in.size(), out);
	return out;
}

void yzs::UTF8_to_Unicode(const char* in, unsigned int len, wstring& out)
{
	assert(in != NULL);
	if (in == NULL) out = L"";
	wchar_t* pBuf = new wchar_t[len + 1];
	if ( NULL == pBuf )
	{
		return;
	}
	size_t out_len = (len + 1) * sizeof(wchar_t);
	memset(pBuf, 0, (len + 1) * sizeof(wchar_t));
	wchar_t* pResult = pBuf;

	out_len = ::MultiByteToWideChar(CP_UTF8, 0, in, len, pBuf, len * sizeof(wchar_t));
	out.assign( pResult, out_len );


	delete [] pResult;
	pResult = NULL;
}

wstring yzs::UTF8_to_Unicode(const string& in)
{
	if (in == "") return L"";
	wstring out;
	UTF8_to_Unicode(in.c_str(), in.size(), out);
	return out;
}

void yzs::Unicode_to_ANSI(const wchar_t* in, unsigned int len, string& out)
{
	assert(in != NULL);
	if (in == NULL) out = "";
	int bufferlen = (int)::WideCharToMultiByte(CP_ACP,0,in,(int)len,NULL,0,NULL,NULL);
	char* pBuffer = new char[bufferlen + 4];
	if ( NULL == pBuffer )
	{
		return;
	}
	int out_len = ::WideCharToMultiByte(CP_ACP,0,in,(int)len,pBuffer,bufferlen+2,NULL,NULL);   
	pBuffer[bufferlen] = '\0';
	out.assign( pBuffer, out_len );
	delete[] pBuffer;
}

string yzs::Unicode_to_ANSI(const wstring& in)
{
	if (in == L"") return "";
	string out;
	Unicode_to_ANSI(in.c_str(), in.size(), out);
	return out;
}

void yzs::ANSI_to_Unicode(const char* in, unsigned int len, wstring& out)
{
	assert(in != NULL);
	if (in == NULL) out = L"";
	int wbufferlen = (int)::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,in,(int)len,NULL,0);
	wchar_t* pwbuffer = new wchar_t[wbufferlen+4];
	if ( NULL == pwbuffer )
	{
		return;
	}
	wbufferlen = (int)::MultiByteToWideChar(CP_ACP,MB_PRECOMPOSED,in,(int)len,pwbuffer,wbufferlen+2);
	pwbuffer[wbufferlen] = '\0';
	out.assign( pwbuffer, wbufferlen );
	delete[] pwbuffer;
	return;
}

wstring yzs::ANSI_to_Unicode(const string& in)
{
	if (in == "") return L"";
	wstring out;
	ANSI_to_Unicode(in.c_str(), in.size(), out);
	return out;
}

void yzs::ANSI_to_UTF8(const char* in, unsigned int len, std::string& out)
{
	assert(in != NULL);
	if (in == NULL) out = "";
	wstring wout;
	ANSI_to_Unicode(in, len, wout);
	Unicode_to_UTF8(wout.c_str(), wout.size(), out);
}

string yzs::ANSI_to_UTF8(const string& in)
{
	return Unicode_to_UTF8(ANSI_to_Unicode(in));
}

void yzs::UTF8_to_ANSI(const char* in, unsigned int len, std::string& out)
{
	wstring str;
	UTF8_to_Unicode(in, len, str);
	Unicode_to_ANSI(str.c_str(), str.length(), out);
}

string yzs::UTF8_to_ANSI(const string& in)
{
	return Unicode_to_ANSI(UTF8_to_Unicode(in));
}


string yzs::NormalizeString( std::string &str_source)
{
	string result = "";

	int index = 0;
	int start_pos = 0;
	int end_pos = str_source.length() - 1;
	int is_find_start = 0;

	for (std::string::iterator itr = str_source.begin(); itr != str_source.end(); ++itr)
	{
		if (*itr != ' ')
		{
			if (!is_find_start)
			{
				start_pos = index;
				is_find_start = 1;
			}
			end_pos = index;
		}
		index++;
	}
	int len = end_pos - start_pos + 1;
	result = str_source.substr(start_pos, len);
	return result;
}

wstring yzs::NormalizeWstring( std::wstring &wstr_source)
{
	wstring result = L"";


	int index = 0;
	int start_pos = 0;
	int end_pos = wstr_source.length() - 1;
	int is_find_start = 0;

	for (std::wstring::iterator itr = wstr_source.begin(); itr != wstr_source.end(); ++itr)
	{
		if (*itr != ' ')
		{
			if (!is_find_start)
			{
				start_pos = index;
				is_find_start = 1;
			}
			end_pos = index;
		}
		index++;
	}
	int len = end_pos - start_pos + 1;
	result = wstr_source.substr(start_pos, len);
	

	return result;
}
