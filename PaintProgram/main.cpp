#include "Header.h"

CMainView::CMainView(CFrameWnd *parent) {

	m_mainFrame = (CMainFrame*)parent;
	Create(0, 0, WS_CHILD | WS_VISIBLE, CRect(90, 120, 800, 600), parent, AFX_IDW_PANE_FIRST);
	m_hWnd = this->GetSafeHwnd();
}

void CMainView::OnDraw(class CDC *pDC) {

	newPen.CreatePenIndirect(&m_mainFrame->gLogPen);
	mhOldPen = pDC->SelectObject(&newPen);

	newBrush.CreateBrushIndirect(&m_mainFrame->gLogBrush);
	mhOldBrush = pDC->SelectObject(&newBrush);
	
	switch (m_mainFrame->m_shape) {

		case CMainFrame::SHAPE::LINE:
			pDC->MoveTo(p0.x, p0.y);
			pDC->LineTo(p1.x, p1.y);
			return;
		case CMainFrame::SHAPE::RECTANGLE:
			pDC->Rectangle(p0.x, p0.y, p1.x, p1.y);
			return;
		case CMainFrame::SHAPE::ELLIPSE:
			pDC->Ellipse(p0.x, p0.y, p1.x, p1.y);
			return;
	}

	pDC->SelectObject(mhOldPen);
	pDC->SelectObject(mhOldBrush);
}

BEGIN_MESSAGE_MAP(CMainView, CView)
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

BOOL CMainView::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) {

	if (m_ChangeCursor) {
		::SetCursor(AfxGetApp()->LoadCursorW(IDC_BLUE_CURSOR));
		return TRUE;
	}

	return CView::OnSetCursor(pWnd, nHitTest, message);
}


void CMainView::OnMouseMove(UINT nFlags, CPoint point){
	
	char *MsgCoord = new char[40];
	sprintf(MsgCoord, "Mainview: P(%d, %d)", point.x, point.y);
	CWnd *pWnd = AfxGetMainWnd();
	HWND hwnd = NULL;
	if (pWnd){
		hwnd = pWnd->GetSafeHwnd();

		SetWindowTextA(hwnd, MsgCoord);
	}

	if (gMouseDown){
		// Current mouse position is stored in the lParam.
		p1.x = point.x;
		p1.y = point.y;
		Invalidate();
	}	
}


void CMainView::OnLButtonDown(UINT nFlags, CPoint point){
	::SetCursor(AfxGetApp()->LoadCursorW(IDC_RED_CURSOR));

	p0.x = point.x;
	p0.y = point.y;

	p1.x = 0;
	p1.y = 0;

	SetCapture();
	gMouseDown = true;

}

void CMainView::OnLButtonUp(UINT nFlags, CPoint point){

	// Release the captured mouse when the left mouse button is lifted.
	ReleaseCapture();
	gMouseDown = false;

	// Current mouse position is stored in the lParam.
	p1.x = point.x;
	p1.y = point.y;

	Invalidate();
}

void CMainView::OnRButtonDown(UINT nFlags, CPoint point){
	::SetCursor(AfxGetApp()->LoadCursorW(IDC_CURSOR));
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CMainFrame::CMainFrame(std::string title) {

	//LoadFrame(FRAME_LOAD, WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);

	/*** use the LoadAccelTable from CFrameWnd **/
	this->LoadAccelTable(MAKEINTRESOURCE(IDR_ACCELERATOR_RES));

	CString strWndClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW | CS_OWNDC, AfxGetApp()->LoadCursorW(IDC_CURSOR), (HBRUSH)GetStockObject(WHITE_BRUSH), AfxGetApp()->LoadIconW(IDR_MAINFRAME));

	wchar_t *wtitle = new wchar_t[strlen(title.c_str()) + 1];
	mbstowcs(wtitle, title.c_str(), strlen(title.c_str()) + 1);

	Create(strWndClass,
		   wtitle,
		   WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
		   CRect(90, 120, 800, 600), NULL, MAKEINTRESOURCE(IDR_MENU_RES));

	delete[] wtitle;

	new CMainView(this);

	//get the HMENU out of the CMenu
	m_hMenu = this->GetMenu()->GetSafeHmenu();

	CheckMenuItem(m_hMenu, ID_PRIMITIVE_LINE, MF_CHECKED);
	CheckMenuItem(m_hMenu, ID_PENCOLOR_BLACK, MF_CHECKED);	
	CheckMenuItem(m_hMenu, ID_BRUSHCOLOR_BLACK, MF_CHECKED);
	CheckMenuItem(m_hMenu, ID_PENSTYLE_SOLID, MF_CHECKED);
	CheckMenuItem(m_hMenu, ID_BRUSHSTYLE_SOLID, MF_CHECKED);

	m_shape = SHAPE::LINE;

	gLogPen.lopnColor = BLACK;
	gLogPen.lopnStyle = PS_SOLID;
	gLogPen.lopnWidth = CPoint(1, 105);

	gLogBrush.lbColor = BLACK;
	gLogBrush.lbStyle = BS_SOLID;
	gLogBrush.lbHatch = HS_DIAGCROSS;
}

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SHOWWINDOW()
	//ON_WM_ACTIVATE()
	ON_WM_PAINT()
	ON_WM_MOVE()	
	//ON_WM_SIZE()
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_FILE_EXIT, &CMainFrame::Exit)
	ON_COMMAND_RANGE(ID_PRIMITIVE_LINE, ID_BRUSHSTYLE_CROSS, &CMainFrame::OnDoSomething)
END_MESSAGE_MAP()


int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) {

	if (CFrameWnd::OnCreate(lpCreateStruct) == 0) {
		
		return 0;
	}
	return -1;
}

void CMainFrame::OnShowWindow(BOOL bShow, UINT nStatus){

	CFrameWnd::OnShowWindow(bShow, nStatus);
	//ShowWindow(SW_MAXIMIZE);
}

void CMainFrame::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized){

	CFrameWnd::OnActivate(nState, pWndOther, bMinimized);
	switch (nState){
		case WA_ACTIVE:
			MessageBox(L"This window has been activated, without the mouse!");
			break;
	}
}

void CMainFrame::OnPaint(){
	CFrameWnd::OnPaint();

}

void CMainFrame::OnSize(UINT nType, int cx, int cy){
	//CMainFrame::OnSize(nType, cx, cy);
}

void CMainFrame::OnMove(int x, int y){

	CFrameWnd::OnMove(x, y);
	char *MsgCoord = new char[50];
	sprintf(MsgCoord, "Frame-Position: Left = %d | Top = %d", x, y);

	CWnd *pWnd = AfxGetMainWnd();
	HWND hwnd = NULL;
	if (pWnd){
		hwnd = pWnd->GetSafeHwnd();

		SetWindowTextA(hwnd, MsgCoord);
	}
}

void CMainFrame::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags){

	switch (nChar){
		case VK_RETURN:
			MessageBox(L"You pressed Enter");
			break;
		case VK_F1:
			MessageBox(L"Help is not available at the moment");
			break;
		case VK_DELETE:
			MessageBox(L"Can't Delete This");
			break;
	}
}

void CMainFrame::OnLButtonDown(UINT nFlags, CPoint point){
	char *MsgCoord = new char[40];
	sprintf(MsgCoord, "Left Button at P(%d, %d)", point.x, point.y);
	CWnd *pWnd = AfxGetMainWnd();
	HWND hwnd = NULL;
	if (pWnd){
		hwnd = pWnd->GetSafeHwnd();
		SetWindowTextA(hwnd, MsgCoord);
	}
}


void CMainFrame::OnMouseMove(UINT nFlags, CPoint point){
	char *MsgCoord = new char[40];
	sprintf(MsgCoord, "Mainframe: P(%d, %d)", point.x, point.y);
	CWnd *pWnd = AfxGetMainWnd();
	HWND hwnd = NULL;
	if (pWnd){
		hwnd = pWnd->GetSafeHwnd();

		SetWindowTextA(hwnd, MsgCoord);
	}
}

void CMainFrame::Exit(){
	MessageBox(L"Exit ");
}



void CMainFrame::OnDoSomething(UINT nID){
	switch (nID){
		
		//=======================================
		// Primitive Types (Shape Types)             
		//=======================================
	case ID_PRIMITIVE_LINE:
		CheckMenuItem(m_hMenu, ID_PRIMITIVE_LINE, MF_CHECKED);
		CheckMenuItem(m_hMenu, gCurrPrimSel, MF_UNCHECKED);
		gCurrPrimSel = ID_PRIMITIVE_LINE;
		m_shape = SHAPE::LINE;
		return;
	case ID_PRIMITIVE_RECTANGLE:
		CheckMenuItem(m_hMenu, ID_PRIMITIVE_RECTANGLE, MF_CHECKED);
		CheckMenuItem(m_hMenu, gCurrPrimSel, MF_UNCHECKED);
		gCurrPrimSel = ID_PRIMITIVE_RECTANGLE;
		m_shape = SHAPE::RECTANGLE;
		return;
	case ID_PRIMITIVE_ELLIPSE:
		CheckMenuItem(m_hMenu, ID_PRIMITIVE_ELLIPSE, MF_CHECKED);
		CheckMenuItem(m_hMenu, gCurrPrimSel, MF_UNCHECKED);
		gCurrPrimSel = ID_PRIMITIVE_ELLIPSE;
		m_shape = SHAPE::ELLIPSE;
		return;
		//=======================================
		// Pen Colors              
		//=======================================
	case ID_PENCOLOR_BLACK:
		CheckMenuItem(m_hMenu, ID_PENCOLOR_BLACK, MF_CHECKED);
		CheckMenuItem(m_hMenu, gCurrPenColSel, MF_UNCHECKED);
		gCurrPenColSel = ID_PENCOLOR_BLACK;
		gLogPen.lopnColor = BLACK;
		return;
	case ID_PENCOLOR_WHITE:
		CheckMenuItem(m_hMenu, ID_PENCOLOR_WHITE, MF_CHECKED);
		CheckMenuItem(m_hMenu, gCurrPenColSel, MF_UNCHECKED);
		gCurrPenColSel = ID_PENCOLOR_WHITE;
		gLogPen.lopnColor = WHITE;
		return;
	case ID_PENCOLOR_RED:
		CheckMenuItem(m_hMenu, ID_PENCOLOR_RED, MF_CHECKED);
		CheckMenuItem(m_hMenu, gCurrPenColSel, MF_UNCHECKED);
		gCurrPenColSel = ID_PENCOLOR_RED;
		gLogPen.lopnColor = RED;
		return;
	case ID_PENCOLOR_GREEN:
		CheckMenuItem(m_hMenu, ID_PENCOLOR_GREEN, MF_CHECKED);
		CheckMenuItem(m_hMenu, gCurrPenColSel, MF_UNCHECKED);
		gCurrPenColSel = ID_PENCOLOR_GREEN;
		gLogPen.lopnColor = GREEN;
		return;
	case ID_PENCOLOR_BLUE:
		CheckMenuItem(m_hMenu, ID_PENCOLOR_BLUE, MF_CHECKED);
		CheckMenuItem(m_hMenu, gCurrPenColSel, MF_UNCHECKED);
		gCurrPenColSel = ID_PENCOLOR_BLUE;
		gLogPen.lopnColor = BLUE;
		return;
		//=======================================
		// Brush Colors              
		//=======================================
	case ID_BRUSHCOLOR_BLACK:
		CheckMenuItem(m_hMenu, ID_BRUSHCOLOR_BLACK, MF_CHECKED);
		CheckMenuItem(m_hMenu, gCurrBrushColSel, MF_UNCHECKED);
		gCurrBrushColSel = ID_BRUSHCOLOR_BLACK;
		gLogBrush.lbColor = BLACK;
		return ;
	case ID_BRUSHCOLOR_WHITE:
		CheckMenuItem(m_hMenu, ID_BRUSHCOLOR_WHITE, MF_CHECKED);
		CheckMenuItem(m_hMenu, gCurrBrushColSel, MF_UNCHECKED);
		gCurrBrushColSel = ID_BRUSHCOLOR_WHITE;
		gLogBrush.lbColor = WHITE;
		return;
	case ID_BRUSHCOLOR_RED:
		CheckMenuItem(m_hMenu, ID_BRUSHCOLOR_RED, MF_CHECKED);
		CheckMenuItem(m_hMenu, gCurrBrushColSel, MF_UNCHECKED);
		gCurrBrushColSel = ID_BRUSHCOLOR_RED;
		gLogBrush.lbColor = RED;
		return;
	case ID_BRUSHCOLOR_GREEN:
		CheckMenuItem(m_hMenu, ID_BRUSHCOLOR_GREEN, MF_CHECKED);
		CheckMenuItem(m_hMenu, gCurrBrushColSel, MF_UNCHECKED);
		gCurrBrushColSel = ID_BRUSHCOLOR_GREEN;
		gLogBrush.lbColor = GREEN;
		return;
	case ID_BRUSHCOLOR_BLUE:
		CheckMenuItem(m_hMenu, ID_BRUSHCOLOR_BLUE, MF_CHECKED);
		CheckMenuItem(m_hMenu, gCurrBrushColSel, MF_UNCHECKED);
		gCurrBrushColSel = ID_BRUSHCOLOR_BLUE;
		gLogBrush.lbColor = BLUE;
		return;
		//=======================================
		// Pen Styles              
		//=======================================
	case ID_PENSTYLE_SOLID:
		CheckMenuItem(m_hMenu, ID_PENSTYLE_SOLID, MF_CHECKED);
		CheckMenuItem(m_hMenu, gCurrPenStyleSel, MF_UNCHECKED);
		gCurrPenStyleSel = ID_PENSTYLE_SOLID;
		gLogPen.lopnStyle = PS_SOLID;
		return;
	case ID_PENSTYLE_DOTTED:
		CheckMenuItem(m_hMenu, ID_PENSTYLE_DOTTED, MF_CHECKED);
		CheckMenuItem(m_hMenu, gCurrPenStyleSel, MF_UNCHECKED);
		gCurrPenStyleSel = ID_PENSTYLE_DOTTED;
		gLogPen.lopnStyle = PS_DOT;
		return;
	case ID_PENSTYLE_DASHED:
		CheckMenuItem(m_hMenu, ID_PENSTYLE_DASHED, MF_CHECKED);
		CheckMenuItem(m_hMenu, gCurrPenStyleSel, MF_UNCHECKED);
		gCurrPenStyleSel = ID_PENSTYLE_DASHED;
		gLogPen.lopnStyle = PS_DASH;
		return;
		//=======================================
		// Brush Styles              
		//=======================================
	case ID_BRUSHSTYLE_SOLID:
		CheckMenuItem(m_hMenu, ID_BRUSHSTYLE_SOLID, MF_CHECKED);
		CheckMenuItem(m_hMenu, gCurrBrushStyleSel, MF_UNCHECKED);
		gCurrBrushStyleSel = ID_BRUSHSTYLE_SOLID;
		gLogBrush.lbStyle = BS_SOLID;
		return;
	case ID_BRUSHSTYLE_NULL:
		CheckMenuItem(m_hMenu, ID_BRUSHSTYLE_NULL, MF_CHECKED);
		CheckMenuItem(m_hMenu, gCurrBrushStyleSel, MF_UNCHECKED);
		gCurrBrushStyleSel = ID_BRUSHSTYLE_NULL;
		gLogBrush.lbStyle = BS_NULL;
		return;
	case ID_BRUSHSTYLE_DIAGONAL:
		CheckMenuItem(m_hMenu, ID_BRUSHSTYLE_DIAGONAL, MF_CHECKED);
		CheckMenuItem(m_hMenu, gCurrBrushStyleSel, MF_UNCHECKED);
		gCurrBrushStyleSel = ID_BRUSHSTYLE_DIAGONAL;
		gLogBrush.lbStyle = BS_HATCHED;
		gLogBrush.lbHatch = HS_BDIAGONAL;
		return;
	case ID_BRUSHSTYLE_CROSS:
		CheckMenuItem(m_hMenu, ID_BRUSHSTYLE_CROSS, MF_CHECKED);
		CheckMenuItem(m_hMenu, gCurrBrushStyleSel, MF_UNCHECKED);
		gCurrBrushStyleSel = ID_BRUSHSTYLE_CROSS;
		gLogBrush.lbStyle = BS_HATCHED;
		gLogBrush.lbHatch = HS_CROSS;
		return;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CPaintProgram::InitInstance() {

	AllocConsole();
	AttachConsole(GetCurrentProcessId());
	freopen("CON", "w", stdout);
	SetConsoleTitle(L"Debug console");
	MoveWindow(GetConsoleWindow(), 740, 0, 550, 200, true);

	CMainFrame *Frame = new CMainFrame("Paint Program");
	m_pMainWnd = Frame;

	Frame->ShowWindow(SW_SHOW);
	Frame->UpdateWindow();

	return TRUE;
}

CPaintProgram paintProgram;