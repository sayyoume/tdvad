#ifndef __UI_HEAD_SELECT_H__
#define __UI_HEAD_SELECT_H__

namespace DuiLib
{
	class UILIB_API CHeadSelectUI : public CControlUI
	{
	public:
		enum
		{
			SIZE_NULL = 0,
			SIZE_LEFTTOP,
			SIZE_RIGHTTOP,
			SIZE_LEFTBOTTOM,
			SIZE_RIGHTBOTTOM,
			SIZE_ALL
		};
		CHeadSelectUI();
		~CHeadSelectUI();

		LPCTSTR GetClass() const;
		UINT GetControlFlags() const;
		LPVOID GetInterface(LPCTSTR pstrName);
		void	DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);

		void DoEvent(TEventUI& event);
		//SIZE EstimateSize(SIZE szAvailable);

		int CheckCursor(POINT point);


		/**
		* 得到四角的拖拉小方块的宽度的一半
		* @return int 虚线区域宽度的一半
		*/
		int GetHalfBoxWidth();

		/**
		* 得到实际的选区（虚线区域）
		* @param[out] select_rect 实际选区
		* @return void	无返回值
		*/
		void GetRealSelectRect(RECT &select_rect);
	private:
		void DrawBox(HDC hdc, const RECT& rect);
		void DrawCircle(Gdiplus::Graphics& gp, const RECT& rect);

	protected:
		enum { MAX_LINK = 8 };
		
		RECT m_rcLinks[MAX_LINK];
		CDuiString m_sLinks[MAX_LINK];
	private:
		int					box_width_;     // 四边方块的宽度(同高度)
		RECT    box_rect_leftTop_;
		RECT    box_rect_rightTop_;
		RECT    box_rect_leftBottom_;
		RECT    box_rect_rightBottom_;
		RECT    box_rect_ALL_;
	};

} // namespace DuiLib

#endif //__UI_HEAD_SELECT_H__