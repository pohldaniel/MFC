#include <afxwin.h>
#include <sstream>
#include <iostream>
#include <vector>

#include "resource.h"

class CMainFrame;

class CMainView : public CView {

public:

	CMainView::CMainView(CFrameWnd *parent);
	POINT p0;
	POINT p1;
	CMainFrame* m_mainFrame;
	HWND m_hWnd;
	bool gMouseDown = false;

protected:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()

private:

	void OnDraw(class CDC *pDC);
	BOOL m_ChangeCursor = true;
};

class CMainFrame : public CFrameWnd {

public:

	enum SHAPE {LINE, RECTANGLE, ELLIPSE };

	CMainFrame::CMainFrame(std::string title);

protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMove(int x, int y);
	//afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDoSomething(UINT nID);

	
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void Exit();
	HMENU     m_hMenu;

	const COLORREF BLACK = RGB(0, 0, 0);
	const COLORREF WHITE = RGB(255, 255, 255);
	const COLORREF RED = RGB(255, 0, 0);
	const COLORREF GREEN = RGB(0, 255, 0);
	const COLORREF BLUE = RGB(0, 0, 255);

	int gCurrPrimSel = ID_PRIMITIVE_LINE;
	int gCurrPenColSel = ID_PENCOLOR_BLACK;
	int gCurrBrushColSel = ID_BRUSHCOLOR_BLACK;
	int gCurrPenStyleSel = ID_PENSTYLE_SOLID;
	int gCurrBrushStyleSel = ID_BRUSHSTYLE_SOLID;

	LOGPEN gLogPen;
	LOGBRUSH gLogBrush;

	SHAPE m_shape;
};

class CPaintProgram : public CWinApp {
public:

	BOOL InitInstance();

};