
// LiangYufan_mfcApplicationView.cpp: CLiangYufanmfcApplicationView 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "LiangYufan_mfcApplication.h"
#endif

#include "LiangYufan_mfcApplicationDoc.h"
#include "LiangYufan_mfcApplicationView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLiangYufanmfcApplicationView

IMPLEMENT_DYNCREATE(CLiangYufanmfcApplicationView, CView)

BEGIN_MESSAGE_MAP(CLiangYufanmfcApplicationView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_COMMAND(ID_FILE_OPEN, &CLiangYufanmfcApplicationView::OnFileOpen)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CLiangYufanmfcApplicationView 构造/析构

CLiangYufanmfcApplicationView::CLiangYufanmfcApplicationView() noexcept
{
	// TODO: add construction code here      
	this->m_GLPixelIndex = 0;
	this->m_hGLContext = NULL;

	m_xrotation = 0.0;
	m_yrotation = 0.0;

	m_leftButtonDown = FALSE;
	m_read = FALSE;

}

CLiangYufanmfcApplicationView::~CLiangYufanmfcApplicationView()
{
}

BOOL CLiangYufanmfcApplicationView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式
	cs.style |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
	return CView::PreCreateWindow(cs);
}

// CLiangYufanmfcApplicationView 绘图

void CLiangYufanmfcApplicationView::OnDraw(CDC* pDC)
{
	CLiangYufanmfcApplicationDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	glRotatef(m_xrotation, 1.0, 0.0, 0.0);
	glRotatef(m_yrotation, 0.0, 1.0, 0.0);

	// TODO: 在此处为本机数据添加绘制代码
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_read)
		m_obj.DrawOBJ();

	SwapBuffers(pDC->m_hDC);

}


// CLiangYufanmfcApplicationView 打印

BOOL CLiangYufanmfcApplicationView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CLiangYufanmfcApplicationView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CLiangYufanmfcApplicationView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// CLiangYufanmfcApplicationView 诊断

#ifdef _DEBUG
void CLiangYufanmfcApplicationView::AssertValid() const
{
	CView::AssertValid();
}

void CLiangYufanmfcApplicationView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLiangYufanmfcApplicationDoc* CLiangYufanmfcApplicationView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLiangYufanmfcApplicationDoc)));
	return (CLiangYufanmfcApplicationDoc*)m_pDocument;
}
#endif //_DEBUG


BOOL CLiangYufanmfcApplicationView::SetWindowPixelFormat(HDC hDC) {
	PIXELFORMATDESCRIPTOR  pixelDesc = { sizeof(PIXELFORMATDESCRIPTOR), 1,
																			   PFD_DRAW_TO_WINDOW |
																			   PFD_SUPPORT_OPENGL |
																			   PFD_DOUBLEBUFFER,
																			   PFD_TYPE_RGBA,   24,  0,0,0,0,0,0,  0,  0,  0,
																			   0,0,0,0,  32,   0,  0, 0,  0,  0,0,0 };
	m_GLPixelIndex = ChoosePixelFormat(hDC, &pixelDesc);

	if (m_GLPixelIndex == 0)
	{
		return FALSE;
	}

	if (SetPixelFormat(hDC, m_GLPixelIndex, &pixelDesc) == FALSE)
	{
		return FALSE;
	}

	return TRUE;
}
// CLiangYufanmfcApplicationView 消息处理程序

BOOL CLiangYufanmfcApplicationView::CreateViewGLContext(HDC hDC) {
	m_hGLContext = wglCreateContext(hDC);
	if (m_hGLContext == NULL) //创建失败
	{
		return FALSE;
	}

	if (wglMakeCurrent(hDC, m_hGLContext) == FALSE)
	{//选为当前RC失败
		return FALSE;
	}
	return TRUE;

}


int CLiangYufanmfcApplicationView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	HWND hWnd = this->GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);

	if (this->SetWindowPixelFormat(hDC) == FALSE)
	{
		return 0;
	}

	if (this->CreateViewGLContext(hDC) == FALSE)
	{
		return 0;
	}
//initialize/////////////////
	glewInit();
	glDrawBuffer(GL_BACK);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClearDepth(1.0);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
////////////////////////////

		///////////////////
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	float light_ambient[] = { 0.1,0.1,0.1,1 };
	float light_diffuse[] = { 1,1,1,1 };
	float light_specular[] = { 1,1,1,1 };
	float light_position[] = { 1,1,1,0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	///////////////////

	return 0;
}

void CLiangYufanmfcApplicationView::OnDestroy()
{
	CView::OnDestroy();
	// TODO: Add your message handler code here
	if (wglGetCurrentContext() != NULL)
	{
		wglMakeCurrent(NULL, NULL);
	}

	if (this->m_hGLContext != NULL)
	{
		wglDeleteContext(this->m_hGLContext);
		this->m_hGLContext = NULL;
	}

}

void CLiangYufanmfcApplicationView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	glViewport(0, 0, cx, cy);
	GLdouble aspect;
	if (cy == 0)
	{
		aspect = (GLdouble)cx;
	}
	else
	{
		aspect = (GLdouble)cx / (GLdouble)cy;
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-3.0 * aspect, 3.0 * aspect, -3.0, 3.0, -13.0, 13.0);
	Invalidate();

}

BOOL CLiangYufanmfcApplicationView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	/*return CView::OnEraseBkgnd(pDC);*/return true;
}

void CLiangYufanmfcApplicationView::OnFileOpen()
{
	// TODO: 在此添加命令处理程序代码
	CString filename;  TCHAR strFilter[] = _T("OBJ file(*.obj)|*.obj||");
	CFileDialog fDlg(TRUE, _T("OBJ"), _T("*.obj"), OFN_HIDEREADONLY |
		OFN_OVERWRITEPROMPT, strFilter);
	TCHAR strTitle[] = _T("Open OBJ file...");
	fDlg.m_ofn.lpstrTitle = strTitle;

	if (fDlg.DoModal() == IDOK)
	{
		filename = fDlg.GetPathName();
		//获取缓冲区的大小，并申请空间，缓冲区大小是按字节计算的  
		int len = WideCharToMultiByte(CP_ACP, 0, filename.GetBuffer(filename.GetLength()),
			filename.GetLength(), NULL, 0, NULL, NULL);
		char* buffer = new char[len + 1];
		WideCharToMultiByte(CP_ACP, 0, filename.GetBuffer(filename.GetLength()),
			filename.GetLength(), buffer, len, NULL, NULL);
		buffer[len] = '\0';
		m_obj.readOBJ(buffer);
		delete[]buffer;  m_read = true;
	}

	Invalidate(TRUE);
}

void CLiangYufanmfcApplicationView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_leftButtonDown)
	{
		m_yrotation += (point.x - m_leftDownPos.x) / 3.0f;
		m_xrotation += (point.y - m_leftDownPos.y) / 3.0f;
		m_leftDownPos = point;
		Invalidate();
	}

	CView::OnMouseMove(nFlags, point);
}

void CLiangYufanmfcApplicationView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_leftButtonDown = TRUE;
	m_leftDownPos = point;

	CView::OnLButtonDown(nFlags, point);
}

void CLiangYufanmfcApplicationView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_leftButtonDown = FALSE;
	CView::OnLButtonUp(nFlags, point);
}
