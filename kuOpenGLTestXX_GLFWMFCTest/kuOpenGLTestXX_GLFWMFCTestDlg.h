
// kuOpenGLTestXX_GLFWMFCTestDlg.h : ���Y��
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

// CkuOpenGLTestXX_GLFWMFCTestDlg ��ܤ��
class CkuOpenGLTestXX_GLFWMFCTestDlg : public CDialogEx
{
// �غc
public:
	CkuOpenGLTestXX_GLFWMFCTestDlg(CWnd* pParent = NULL);	// �зǫغc�禡

	GLFWwindow		*	GLWnd;
	HWND				GLWndHWND;

// ��ܤ�����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KUOPENGLTESTXX_GLFWMFCTEST_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �䴩

	bool			GLDisplay;

	int				GLWndWidth;
	int				GLWndHeight;

	GLFWwindow	*	kuGlInit(int xRes, int yRes, int hWnd);
	void			kuOpenGLDisplayProc();

	thread			GLDispThread;

// �{���X��@
protected:
	HICON m_hIcon;

	// ���ͪ��T�������禡
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();
};
