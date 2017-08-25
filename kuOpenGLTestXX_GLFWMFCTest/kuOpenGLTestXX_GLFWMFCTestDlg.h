
// kuOpenGLTestXX_GLFWMFCTestDlg.h : 標頭檔
//

#pragma once

#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL

#include <iostream>
#include <thread>

#include <GLEW/glew.h>
#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

using namespace std;

// CkuOpenGLTestXX_GLFWMFCTestDlg 對話方塊
class CkuOpenGLTestXX_GLFWMFCTestDlg : public CDialogEx
{
// 建構
public:
	CkuOpenGLTestXX_GLFWMFCTestDlg(CWnd* pParent = NULL);	// 標準建構函式

	GLFWwindow		*	GLWnd;
	HWND				GLWndHWND;

// 對話方塊資料
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KUOPENGLTESTXX_GLFWMFCTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支援

	bool			GLDisplay;

	int				GLWndWidth;
	int				GLWndHeight;

	GLFWwindow	*	kuGlInit(int xRes, int yRes, int hWnd);
	void			kuOpenGLDisplayProc();

	thread			GLDispThread;

// 程式碼實作
protected:
	HICON m_hIcon;

	// 產生的訊息對應函式
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
};
