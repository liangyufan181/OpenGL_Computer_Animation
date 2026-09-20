
// LiangYufan_mfcApplicationView.h: CLiangYufanmfcApplicationView 类的接口
//

#pragma once
#include "OBJ.h"

class CLiangYufanmfcApplicationView : public CView
{
protected: // 仅从序列化创建
	CLiangYufanmfcApplicationView() noexcept;
	DECLARE_DYNCREATE(CLiangYufanmfcApplicationView)




// 特性
public:
	CLiangYufanmfcApplicationDoc* GetDocument() const;

	int m_GLPixelIndex;
	HGLRC m_hGLContext;
	OBJ  m_obj;
	BOOL  m_read;

	float m_xrotation;
	float m_yrotation;

	BOOL m_leftButtonDown;
	CPoint m_leftDownPos;



// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	BOOL SetWindowPixelFormat(HDC hDC);
	BOOL CreateViewGLContext(HDC hDC);


// 实现
public:
	virtual ~CLiangYufanmfcApplicationView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnFileOpen();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // LiangYufan_mfcApplicationView.cpp 中的调试版本
inline CLiangYufanmfcApplicationDoc* CLiangYufanmfcApplicationView::GetDocument() const
   { return reinterpret_cast<CLiangYufanmfcApplicationDoc*>(m_pDocument); }
#endif

