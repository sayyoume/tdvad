
#include "StdAfx.h"
#include "ImageEx.h"
#include <Gdiplus.h>


namespace DuiLib
{
	LPCTSTR CImageExUI::GetClass() const
	{
		return _T("ImageExUI");
	}
	CImageExUI::CImageExUI()
	{
		m_img = NULL;
	
	}

	CImageExUI::~CImageExUI()
	{
		if (NULL != m_img)
		{
			delete m_img;
		}
	}

	void CImageExUI::SetFkImage(LPCTSTR pstrValue)
	{
		m_strPath = pstrValue;
		Invalidate();
	}

	void CImageExUI::SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue)
	{
		if ((_tcsicmp(pstrName, _T("fkimage")) == 0))
		{
			DuiLib::CDuiString sFile = DuiLib::CPaintManagerUI::GetResourcePath();
			if (DuiLib::CPaintManagerUI::GetResourceZip().IsEmpty())
			{
				sFile += pstrValue;
			}			
			m_strPath = sFile;
		}
		else CControlUI::SetAttribute(pstrName, pstrValue);
	}
	
	Image* getimagefromfile(std::wstring& str)
	{
		return Image::FromFile(str.c_str());
	}

	void CImageExUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
	{
		DuiLib::CControlUI::DoPaint(hDC, rcPaint,pStopControl);

		Graphics graphics(hDC);
		
		m_img = getimagefromfile(m_strPath);
		
		
		graphics.SetInterpolationMode(InterpolationMode::InterpolationModeHighQualityBicubic);

		Rect rect(m_rcItem.left, m_rcItem.top, m_rcItem.right- m_rcItem.left, m_rcItem.bottom - m_rcItem.top);

		graphics.DrawImage(m_img, rect);
		
	}

	void CImageExUI::DoEvent(DuiLib::TEventUI& event)
	{
		DuiLib::CControlUI::DoEvent(event);
	}

}

