#include "StdAfx.h"
#include "UIHeadSelect.h"

namespace DuiLib
{
	CHeadSelectUI::CHeadSelectUI(): box_width_(8)
	{
	}

	CHeadSelectUI::~CHeadSelectUI()
	{
	}

	LPCTSTR CHeadSelectUI::GetClass() const
	{
		return _T("HeadSelectUI");
	}

	LPVOID CHeadSelectUI::GetInterface(LPCTSTR pstrName)
	{
		if( _tcscmp(pstrName, DUI_CTR_HEADSELECT) == 0 ) return static_cast<CHeadSelectUI*>(this);
		return CControlUI::GetInterface(pstrName);
	}

	void CHeadSelectUI::DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl)
	{
		Graphics gp(hDC);
		RECT paint_rect = m_rcItem;


		//画虚线
		Gdiplus::Pen   dash_pen_(Gdiplus::Color::MakeARGB(255, 0x2c, 0xcf, 0x9e), 1.0f);
		Gdiplus::REAL dash_info[2] = { 10.0f, 4.0f }; // line width， space width
		dash_pen_.SetDashPattern(dash_info, 2);
		::InflateRect(&paint_rect, -3, -3);
		gp.DrawRectangle(&dash_pen_, paint_rect.left, paint_rect.top, paint_rect.right-paint_rect.left, paint_rect.bottom - paint_rect.top);

		//画蒙版
		paint_rect = m_rcItem;
		::InflateRect(&paint_rect, -4, -4);
		Rect fill_rect(paint_rect.left, paint_rect.top, paint_rect.right - paint_rect.left + 1, paint_rect.bottom - paint_rect.top + 1);
		Gdiplus::SolidBrush mask_brush_(Gdiplus::Color::MakeARGB(60, 0x2c, 0xcf, 0x9e));
		gp.FillRectangle(&mask_brush_, fill_rect);

		paint_rect = m_rcItem;
		// 绘制四个Box
		RECT rect = paint_rect;
		rect.right = rect.left + box_width_;
		rect.bottom = rect.top + box_width_;
		box_rect_leftTop_ = rect;
		DrawBox(hDC, rect);

		rect = paint_rect;
		rect.left = rect.right - box_width_;
		rect.bottom = rect.top + box_width_;
		box_rect_rightTop_ = rect;
		DrawBox(hDC, rect);
		
		rect = paint_rect;
		rect.right = rect.left + box_width_;
		rect.top = rect.bottom - box_width_;
		box_rect_leftBottom_ = rect;
		DrawBox(hDC, rect);
		
		rect = paint_rect;
		rect.left = rect.right - box_width_;
		rect.top = rect.bottom - box_width_;
		box_rect_rightBottom_ = rect;
		DrawBox(hDC, rect);

		// 绘制圆形
		paint_rect = m_rcItem;
		::InflateRect(&paint_rect,-4,-4);
		box_rect_ALL_ = paint_rect;
		//DrawCircle(gp, paint_rect);
	}

	void CHeadSelectUI::DrawBox(HDC hdc, const RECT& rect)
	{

		HPEN box_pen = CreatePen(PS_SOLID, 1, RGB(0x61, 0x61, 0x61));
		HBRUSH white_brush = CreateSolidBrush(RGB(255, 255, 255));// 白色画刷

		HPEN old_pen = (HPEN)SelectObject(hdc, box_pen);
		HBRUSH old_brush = (HBRUSH)SelectObject(hdc, white_brush);
		Rectangle(hdc, rect.left, rect.top, rect.right, rect.bottom);
		SelectObject(hdc, old_pen);
		SelectObject(hdc, old_brush);

		DeleteObject(box_pen);
		DeleteObject(white_brush);
	}


	void CHeadSelectUI::DrawCircle(Gdiplus::Graphics& gp, const RECT& rect)
	{
		Gdiplus::Pen circle_pen(Gdiplus::Color::MakeARGB(255, 0x2c, 0xcf, 0x9e), 2.0f); // 用于绘制圆形选区
		gp.SetSmoothingMode(SmoothingModeAntiAlias);
#if (GDIPVER >= 0x0110)
		gp.SetSmoothingMode(SmoothingModeAntiAlias8x8);
#endif
		gp.SetCompositingMode(CompositingModeSourceOver);
		gp.SetInterpolationMode(InterpolationModeHighQuality);
		gp.SetPixelOffsetMode(PixelOffsetModeHighQuality);

		Rect ellipse_rect(rect.left, rect.top, rect.right- rect.left, rect.bottom - rect.top);
		gp.DrawEllipse(&circle_pen, ellipse_rect);
	}

	int CHeadSelectUI::GetHalfBoxWidth()
	{
		return box_width_ / 2/* - 1*/;
	}

	void CHeadSelectUI::GetRealSelectRect(RECT &select_rect)
	{
		CDuiRect rect = select_rect;
		rect.Deflate(box_width_ / 2 /*- 1*/, box_width_ / 2/* - 1*/); // 获取虚线的区域
		select_rect = rect;
	}


	UINT CHeadSelectUI::GetControlFlags() const
	{
		return 0;
	}



	void CHeadSelectUI::DoEvent(TEventUI& event)
	{
		CControlUI::DoEvent(event);
	}

	/*SIZE CHeadSelectUI::EstimateSize(SIZE szAvailable)
	{
		return m_cxyFixed;
	}
*/
	
	int CHeadSelectUI::CheckCursor(POINT point)
	{
		if (::PtInRect(&box_rect_rightBottom_, point))
		{
			return SIZE_RIGHTBOTTOM;
		}
		else if (::PtInRect(&box_rect_rightTop_, point))
		{
			return SIZE_RIGHTTOP;
		}
		else if (::PtInRect(&box_rect_leftTop_, point))
		{
			return SIZE_LEFTTOP;
		}
		else if (::PtInRect(&box_rect_leftBottom_, point))
		{
			return SIZE_LEFTBOTTOM;
		}
		else if (::PtInRect(&box_rect_ALL_, point))
		{
			return SIZE_ALL;
		}
		else
		{
			return SIZE_NULL;
		}
	}

}
