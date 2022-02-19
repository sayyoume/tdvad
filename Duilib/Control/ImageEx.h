#pragma once



namespace DuiLib
{

	class UILIB_API CImageExUI : public DuiLib::CControlUI
	{
	public:
		LPCTSTR GetClass() const;
		CImageExUI();
		~CImageExUI();

		void SetFkImage(LPCTSTR pstrValue);
	private:
		void SetAttribute(LPCTSTR pstrName, LPCTSTR pstrValue);
		void DoPaint(HDC hDC, const RECT& rcPaint, CControlUI* pStopControl);
		void DoEvent(DuiLib::TEventUI& event);

		std::wstring m_strPath;
		
		Image* m_img;
	};
}