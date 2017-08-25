
// kuOpenGLTestXX_GLFWMFCTestDlg.cpp : ��@��
//

#include "stdafx.h"
#include "kuOpenGLTestXX_GLFWMFCTest.h"
#include "kuOpenGLTestXX_GLFWMFCTestDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// �� App About �ϥ� CAboutDlg ��ܤ��

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// ��ܤ�����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �䴩

// �{���X��@
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CkuOpenGLTestXX_GLFWMFCTestDlg ��ܤ��



CkuOpenGLTestXX_GLFWMFCTestDlg::CkuOpenGLTestXX_GLFWMFCTestDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_KUOPENGLTESTXX_GLFWMFCTEST_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CkuOpenGLTestXX_GLFWMFCTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

GLFWwindow * CkuOpenGLTestXX_GLFWMFCTestDlg::kuGlInit(int xRes, int yRes, int hWnd)
{
	if (!glfwInit())
	{
		cout << "GLFW initialization failed." << endl;
		return NULL;
	}

	GLFWwindow * window = glfwCreateWindowEx(xRes, yRes, "Test", NULL, NULL, hWnd);

	if (!window)
	{
		glfwTerminate();
		cout << "GLFW window creation failed." << endl;

		return NULL;
	}

	glfwMakeContextCurrent(window);

	//glfwSetKeyCallback(window, key_callback);

	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetCursorPosCallback(window, mouse_callback);				// �U�W��q...�j���u����m��T�ӨS��button�ƥ��T�a

																	// need to create OpenGL window before glew initialization.
																	//glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	if (err != GLEW_OK)
	{
		//Problem: glewInit failed, something is seriously wrong.
		cout << "glewInit failed, aborting." << endl;
	}

	// Define the viewport dimensions
	glViewport(0, 0, xRes, yRes);

	// Setup OpenGL options (z-buffer)
	//glEnable(GL_DEPTH_TEST);

	// get version info
	const GLubyte* renderer = glGetString(GL_RENDERER);
	// get renderer string (graphic card)
	const GLubyte* version = glGetString(GL_VERSION);
	// version as a string (OpenGL supported version and graphic card driver version)
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	return window;
}

void CkuOpenGLTestXX_GLFWMFCTestDlg::kuOpenGLDisplayProc()
{
	while (GLDisplay)
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glfwSwapBuffers(GLWnd);
		glfwPollEvents();
	}
}

BEGIN_MESSAGE_MAP(CkuOpenGLTestXX_GLFWMFCTestDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CkuOpenGLTestXX_GLFWMFCTestDlg �T���B�z�`��

BOOL CkuOpenGLTestXX_GLFWMFCTestDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �N [����...] �\���[�J�t�Υ\���C

	// IDM_ABOUTBOX �����b�t�ΩR�O�d�򤧤��C
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �]�w����ܤ�����ϥܡC�����ε{�����D�������O��ܤ���ɡA
	// �ج[�|�۰ʱq�Ʀ��@�~
	SetIcon(m_hIcon, TRUE);			// �]�w�j�ϥ�
	SetIcon(m_hIcon, FALSE);		// �]�w�p�ϥ�

	// TODO: �b���[�J�B�~����l�]�w
	CRect rect;
	GetDlgItem(IDC_GLView)->GetWindowRect(&rect);

	GLWndWidth  = rect.Width();
	GLWndHeight = rect.Height();

	int glhwnd = (int)GetDlgItem(IDC_GLView)->m_hWnd;

	GLWnd = kuGlInit(GLWndWidth, GLWndHeight, (int)GetDlgItem(IDC_GLView)->m_hWnd);

	if (!GLWnd)
	{
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	GLDisplay = true;

	GLDispThread = thread(&CkuOpenGLTestXX_GLFWMFCTestDlg::kuOpenGLDisplayProc, this);

	return TRUE;  // �Ǧ^ TRUE�A���D�z�ﱱ��]�w�J�I
}

void CkuOpenGLTestXX_GLFWMFCTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �p�G�N�̤p�ƫ��s�[�J�z����ܤ���A�z�ݭn�U�C���{���X�A
// �H�Kø�s�ϥܡC���ϥΤ��/�˵��Ҧ��� MFC ���ε{���A
// �ج[�|�۰ʧ������@�~�C

void CkuOpenGLTestXX_GLFWMFCTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ø�s���˸m���e

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N�ϥܸm����Τ�ݯx��
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �yø�ϥ�
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ��ϥΪ̩즲�̤p�Ƶ����ɡA
// �t�ΩI�s�o�ӥ\����o�����ܡC
HCURSOR CkuOpenGLTestXX_GLFWMFCTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CkuOpenGLTestXX_GLFWMFCTestDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �b���[�J�z���T���B�z�`���{���X
	if (GLDisplay)
	{
		GLDisplay = false;
		GLDispThread.join();
	}
}
