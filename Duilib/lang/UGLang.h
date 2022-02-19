#ifndef __TDLANG_H
#define __TDLANG_H

#include "UGLangMgr.h"
#include <wtypes.h>
#include "../UIlib.h"

namespace UG{
	namespace Lang{

		UILIB_API std::wstring LoadLang(LPCTSTR lpszLangFile, LPCTSTR lplangue);
		UILIB_API void LoadLang2(LPCTSTR lpszLangFile, LPCTSTR lplangue);
		UILIB_API std::wstring getLang(int id);
	}
}

#endif