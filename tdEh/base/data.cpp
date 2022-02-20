#include "data.h"
#include <shlwapi.h>
#include <shellapi.h>
#include <CommCtrl.h>
#include <commoncontrols.h>
#include <shlobj.h>
#include <string>
#include <locale>
#include <codecvt>
#include "json/value.h"
#include "json/reader.h"
#include <strsafe.h>
#include "base64.h"




void TDData::setDebugViewInfo(std::wstring sInfo, std::wstring key)
{
	if (TDData::Instance().m_IsOpenDebug) {
		wchar_t buff[84080]{};
		swprintf_s(buff, L"%s ---------- %s\n", key.c_str(), sInfo.c_str());
		OutputDebugString(buff);
	}
}

std::string TDData::getConfigStringA(std::string wsSection, std::string key)
{
	std::string sPath = TDData::Instance().ws2s(TDData::Instance().m_sIniPath.c_str());
	char wsIp[256] = { 0 };
	GetPrivateProfileStringA(wsSection.c_str(), key.c_str(), "", wsIp, sizeof(wsIp), sPath.c_str());
	std::string sRet = wsIp;
	return sRet;
}

std::wstring TDData::getConfigString(std::wstring wsSection, std::wstring key)
{
	TCHAR wsIp[256] = { 0 };
	GetPrivateProfileString(wsSection.c_str(), key.c_str(), L"", wsIp, sizeof(wsIp), TDData::Instance().m_sIniPath.c_str());
	std::wstring sRet = wsIp;
	return sRet;
}

int TDData::getConfigInt(std::wstring wsSection, std::wstring key)
{
	int iPort = GetPrivateProfileInt(wsSection.c_str(), key.c_str(), 0, TDData::Instance().m_sIniPath.c_str());
	return iPort;
}

void TDData::writeConfigString(std::wstring wsSection, std::wstring key, std::wstring value)
{
	WritePrivateProfileString(wsSection.c_str(), key.c_str(), value.c_str(), TDData::Instance().m_sIniPath.c_str());
}

void TDData::writeConfigInt(std::wstring wsSection, std::wstring key, int value)
{
	wchar_t buff[128]{};
	swprintf_s(buff, L"%d", value);
	WritePrivateProfileString(wsSection.c_str(), key.c_str(), buff, TDData::Instance().m_sIniPath.c_str());
}

bool TDData::resultIsSuccess(std::wstring sData)
{
	if (sData.empty()) return false;
	std::string data = tdws2s(sData);
	data = base64_decode(data);
	Json::Reader reader;
	Json::Value root;
	if (!reader.parse(data, root))
		return false;

	std::string sCode = root["result"].asString();
	if (sCode == "true")
		return true;
	if (sCode == "false")
		return false;
	return false;
}

std::string TDData::ws2s(const std::wstring& wstr)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;
	return converterX.to_bytes(wstr);
}


std::wstring TDData::s2ws(const std::string& str)
{
	using convert_typeX = std::codecvt_utf8<wchar_t>;
	std::wstring_convert<convert_typeX, wchar_t> converterX;
	return converterX.from_bytes(str);
}
std::string TDData::tdws2s(const std::wstring& wide)
{
	int wide_length = static_cast<int>(wide.length());
	if (wide_length == 0)
		return std::string();

	// Compute the length of the buffer we'll need.
	int charcount = WideCharToMultiByte(CP_UTF8, 0, wide.data(), wide_length,
		NULL, 0, NULL, NULL);
	if (charcount == 0)
		return std::string();

	std::string mb;
	mb.resize(charcount);
	WideCharToMultiByte(CP_UTF8, 0, wide.data(), wide_length,
		&mb[0], charcount, NULL, NULL);

	return mb;
}


std::wstring TDData::tds2ws(const std::string& mb)
{
	if (mb.empty())
		return std::wstring();

	int mb_length = static_cast<int>(mb.length());
	// Compute the length of the buffer.
	int charcount = MultiByteToWideChar(CP_UTF8, 0,
		mb.data(), mb_length, NULL, 0);
	if (charcount == 0)
		return std::wstring();

	std::wstring wide;
	wide.resize(charcount);
	MultiByteToWideChar(CP_UTF8, 0, mb.data(), mb_length, &wide[0], charcount);

	return wide;
}

//************************************
// Method:    string_replace
// FullName:  string_replace
// Access:    public 
// Returns:   void
// Qualifier: 把字符串的strsrc替换成strdst
// Parameter: std::string & strBig
// Parameter: const std::string & strsrc
// Parameter: const std::string & strdst
//************************************
void string_replace(std::wstring &strBig, const std::wstring &strsrc, const std::wstring &strdst)
{
	std::wstring::size_type pos = 0;
	std::wstring::size_type srclen = strsrc.size();
	std::wstring::size_type dstlen = strdst.size();

	while ((pos = strBig.find(strsrc, pos)) != std::string::npos)
	{
		strBig.replace(pos, srclen, strdst);
		pos += dstlen;
	}
}

//************************************
// Method:    GetFileOrURLShortName
// FullName:  GetFileOrURLShortName
// Access:    public 
// Returns:   std::string
// Qualifier: 获取路径或URL的文件名（包括后缀，如 C:\Test\abc.xyz --> abc.xyz）
// Parameter: std::string strFullName
//************************************
std::wstring TDData::GetPathOrURLShortName(std::wstring strFullName)
{
	if (strFullName.empty())
	{
		return L"";
	}

	string_replace(strFullName, L"/", L"\\");

	std::string::size_type iPos = strFullName.find_last_of('\\') + 1;

	return strFullName.substr(iPos, strFullName.length() - iPos);
}


bool TDData::IsDir(std::wstring& sPath)
{
	//if (!PathIsDirectory(wstrRet.c_str()))
	unsigned long ulAttriFile = GetFileAttributes(sPath.c_str());
	if ((ulAttriFile&FILE_ATTRIBUTE_DIRECTORY) == FILE_ATTRIBUTE_DIRECTORY)
	{
		return true;
	}
	return false;
}

void TDData::GetSelectFile(std::wstring& s, HWND hwnd)
{
	wchar_t sFileName[1024]{};
	wchar_t sPath[1024]{};
	wchar_t szOpenFileNames[1024]{};

	OPENFILENAME ofn;
	TCHAR* p;
	int nLen = 0;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.Flags = OFN_EXPLORER | OFN_ENABLEHOOK | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
	//ofn.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT;  //打开多个文本
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szOpenFileNames;
	ofn.nMaxFile = sizeof(szOpenFileNames);
	ofn.lpstrFile[0] = '\0';
	ofn.hwndOwner = hwnd;
	ofn.lpstrFilter = TEXT("All Files(*.*)\0*.*\0");
	//ofn.lpstrFilter = TEXT("(*.doc)\0*.doc\0(*.docx)\0*.docx\0\0");
	if (GetOpenFileName(&ofn))
	{
		lstrcpyn(sPath, szOpenFileNames, ofn.nFileOffset);

		p = szOpenFileNames + ofn.nFileOffset; //把指针移到第一个文件

		while (*p)
		{
			swprintf_s(sFileName, L"%s\\", sPath);
			// lstrcat(sFileName, p);    //加上文件名  
			StringCchCat(sFileName, 1024, p);    //加上文件名  
			//vec.push_back(sFileName);
			s = sFileName;
			break;
			p += lstrlen(p) + 1;     //移至下一个文件
		}
	}
}

std::string TDData::createUUID()
{
	std::string strUUID;
	GUID guid;
	if (!CoCreateGuid(&guid)) {
		char buffer[64] = { 0 };
		_snprintf_s(buffer, sizeof(buffer),
			"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
			guid.Data1, guid.Data2, guid.Data3,
			guid.Data4[0], guid.Data4[1], guid.Data4[2],
			guid.Data4[3], guid.Data4[4], guid.Data4[5],
			guid.Data4[6], guid.Data4[7]);
		strUUID = buffer;
	}
	return strUUID;
}

void TDData::GetSelectFileDirect(std::wstring&s, HWND hwnd)
{
	BROWSEINFO bInfo = { 0 };
	bInfo.hwndOwner = hwnd;
	bInfo.ulFlags = BIF_RETURNONLYFSDIRS;
	LPITEMIDLIST lpDlist;
	lpDlist = SHBrowseForFolder(&bInfo);
	if (lpDlist != NULL)
	{
		wchar_t buff[MAX_PATH] = { 0 };
		SHGetPathFromIDList(lpDlist, buff);
		s = buff;
	}
}

// 获取文件图标
HICON TDData::fileIcon(std::string extention)
{
	HICON icon = nullptr;
	if (extention.length() > 0)
	{
		LPCSTR name = extention.c_str();

		SHFILEINFOA info;
		ZeroMemory(&info, sizeof(SHFILEINFO));
		if (SHGetFileInfoA(name,
			FILE_ATTRIBUTE_NORMAL,
			&info,
			sizeof(info),
			SHGFI_SYSICONINDEX | SHGFI_ICON | SHGFI_USEFILEATTRIBUTES))
		{
			icon = info.hIcon;
		}
	}

	return icon;
}

// 获取文件类型
std::string TDData::fileType(std::string extention)
{
	std::string type = "";
	if (extention.length() > 0)
	{
		LPCSTR name = extention.c_str();

		SHFILEINFOA info;
		if (SHGetFileInfoA(name,
			FILE_ATTRIBUTE_NORMAL,
			&info,
			sizeof(info),
			SHGFI_TYPENAME | SHGFI_USEFILEATTRIBUTES))
		{
			type = info.szTypeName;
		}
	}

	return type;
}

// 获取文件夹图标
HICON TDData::folderIcon()
{
	std::string str = "folder";
	LPCSTR name = str.c_str();

	HICON icon = NULL;

	SHFILEINFOA info;
	if (SHGetFileInfoA(name,
		FILE_ATTRIBUTE_DIRECTORY,
		&info,
		sizeof(info),
		SHGFI_SYSICONINDEX | SHGFI_ICON | SHGFI_USEFILEATTRIBUTES))
	{
		icon = info.hIcon;
	}

	return icon;
}

// 获取文件夹类型
std::string TDData::folderType()
{
	std::string str = "folder";
	LPCSTR name = str.c_str();

	std::string type;

	SHFILEINFOA info;
	if (SHGetFileInfoA(name,
		FILE_ATTRIBUTE_DIRECTORY,
		&info,
		sizeof(info),
		SHGFI_TYPENAME | SHGFI_USEFILEATTRIBUTES))
	{
		type = info.szTypeName;
	}

	return type;
}
std::wstring TDData::GetCurrentPath()
{
	TCHAR szData[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szData, MAX_PATH);
	PathRemoveFileSpec(szData);
	std::wstring sPath = szData;
	return sPath;
}

std::wstring TDData::GetDbPath()
{
	wchar_t buff[1024] = { 0 };
	LPITEMIDLIST pidl = NULL;
	SHGetSpecialFolderLocation(NULL, CSIDL_LOCAL_APPDATA, &pidl);
	if (SHGetPathFromIDList(pidl, buff))
	{
		CoTaskMemFree(pidl);
	}
	std::wstring sDbPath = buff;
	return sDbPath.append(L"\\tdcsrc");
}

std::wstring TDData::GetAppData()
{
	wchar_t buff[1024] = { 0 };
	LPITEMIDLIST pidl = NULL;
	SHGetSpecialFolderLocation(NULL, CSIDL_LOCAL_APPDATA, &pidl);
	if (SHGetPathFromIDList(pidl, buff))
	{
		CoTaskMemFree(pidl);
	}
	std::wstring sDbPath = buff;
	return sDbPath;
}

HICON TDData::getFileIcon(std::wstring sFilePath)
{
	HICON hIcon = nullptr;
	SHFILEINFO sfi;
	ZeroMemory(&sfi, sizeof(SHFILEINFO));
	::SHGetFileInfo(sFilePath.c_str(), FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(SHFILEINFO), SHGFI_SYSICONINDEX);
	static  HIMAGELIST* imageList = NULL;
	HICON icon_handle = NULL;
	//SHIL_EXTRALARGE获取48 * 48的图标， SHIL_JUMBO 获取256 * 256的图标。
	if (imageList == NULL)
		HRESULT hResult = ::SHGetImageList(SHIL_EXTRALARGE, IID_IImageList, (void**)&imageList);

	if (imageList != NULL)
		((IImageList*)imageList)->GetIcon(sfi.iIcon, ILD_TRANSPARENT, &icon_handle);


	if (sfi.iIcon != 0)
		hIcon = icon_handle;

	return hIcon;
	
}

std::wstring TDData::getCurrentTime()
{
	auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	struct tm newtime;
	localtime_s(&newtime, &tt);
	wchar_t date[60] = { 0 };
	swprintf_s(date, L"%d-%02d-%02d %02d:%02d:%02d",
		(int)newtime.tm_year + 1900, (int)newtime.tm_mon + 1, (int)newtime.tm_mday,
		(int)newtime.tm_hour, (int)newtime.tm_min, (int)newtime.tm_sec);
	return date;
}


//UINT_PTR CALLBACK OFNHookProc(HWND hdlg, UINT uiMsg, WPARAM wParam, LPARAM lParam)
//{
//	if (uiMsg == WM_INITDIALOG)
//	{
//		RECT rcParent;
//		HWND hWndParent = GetParent(hdlg);
//		GetClientRect(hWndParent, &rcParent);
//		RECT rcHaotiMainFrame;
//		GetClientRect(hdlg, &rcHaotiMainFrame);
//		POINT ptParentInScreen;
//		ptParentInScreen.x = rcParent.left;
//		ptParentInScreen.y = rcParent.top;
//		::ClientToScreen(hWndParent, (LPPOINT)&ptParentInScreen);
//		SetWindowPos(hWndParent, NULL,
//			ptParentInScreen.x + ((rcHaotiMainFrame.right - rcHaotiMainFrame.left ) / 2),
//			ptParentInScreen.y + ((rcHaotiMainFrame.bottom - rcHaotiMainFrame.top  ) / 2),
//			0, 0, SWP_NOZORDER | SWP_NOSIZE);
//	}
//	UNREFERENCED_PARAMETER(wParam);
//	return 1;
//}
void TDData::SaveFile(std::wstring& sSaveFileName, HWND hwnd)
{
	wchar_t sFileName[1024]{};
	wchar_t sPath[1024]{};
	wchar_t szOpenFileNames[1024]{};
	std::wstring sTitle = L"导出";
	OPENFILENAME ofn;
	TCHAR* p;
	int nLen = 0;
	ZeroMemory(&ofn, sizeof(ofn));
	ofn.Flags = OFN_EXPLORER | OFN_ENABLEHOOK | OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST/* | OFN_OVERWRITEPROMPT*/;
	//ofn.Flags = OFN_EXPLORER | OFN_ALLOWMULTISELECT;  //打开多个文本
	ofn.lStructSize = sizeof(ofn);
	ofn.lpstrFile = szOpenFileNames;
	ofn.lpstrTitle = sTitle.c_str();
	ofn.nMaxFileTitle = sTitle.length();
	ofn.nMaxFile = sizeof(szOpenFileNames);
	ofn.lpstrFile[0] = '\0';
	ofn.hwndOwner = hwnd;
	//ofn.lpfnHook = OFNHookProc;
	//ofn.lpstrFilter = TEXT("All Files(*.*)\0*.*\0");
	ofn.lpstrFilter = TEXT("(*.xlsx)\0*.xlsx\0\0");
	if (GetSaveFileName(&ofn))
	{
		lstrcpyn(sPath, szOpenFileNames, ofn.nFileOffset);

		p = szOpenFileNames + ofn.nFileOffset; //把指针移到第一个文件

		while (*p)
		{
			swprintf_s(sFileName, L"%s\\", sPath);
			StringCchCat(sFileName, 1024, p);    //加上文件名  
			//vec.push_back(sFileName);
			sSaveFileName = sFileName;
			int nLength = sSaveFileName.length();
			std::wstring sTemp = sSaveFileName.substr(nLength - 4, nLength);
			if (sTemp != L".xlsx")
			{
				sSaveFileName += L".xlsx";
			}

			break;
			p += lstrlen(p) + 1;     //移至下一个文件
		}
	}
}

std::wstring TDData::getCurrentSystemTime()
{
	auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	struct tm newtime;
	localtime_s(&newtime,&tt);
	wchar_t date[60] = { 0 };
	/*sprintf(date, "%d-%02d-%02d      %02d:%02d:%02d",
		(int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday,
		(int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec);*/

	swprintf_s(date, L"%d-%02d-%02d",(int)newtime.tm_year + 1900,newtime.tm_mon + 1, (int)newtime.tm_mday);
	return std::wstring(date);
}

std::wstring TDData::getCurrentSystemTimeYear()
{
	auto tt = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());

	struct tm newtime;
	localtime_s(&newtime, &tt);
	wchar_t date[60] = { 0 };
	/*sprintf(date, "%d-%02d-%02d      %02d:%02d:%02d",
		(int)ptm->tm_year + 1900, (int)ptm->tm_mon + 1, (int)ptm->tm_mday,
		(int)ptm->tm_hour, (int)ptm->tm_min, (int)ptm->tm_sec);*/

	swprintf_s(date, L"%d", (int)newtime.tm_year + 1900);
	return std::wstring(date);
}
