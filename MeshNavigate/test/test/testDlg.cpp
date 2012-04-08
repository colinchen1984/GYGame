
// testDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "test.h"
#include "testDlg.h"
#include <math.h>
#include "../../ObjectMesh.h"
#include "../../Polygon.h"
#include "../../PolygonClipping.h"
#include "../../VectorMath.h"
#include "../../ConcavePolygonDecompose.h"
#include "../../MeshNavigate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestDlg 对话框




CtestDlg::CtestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CtestDlg::IDD, pParent)
	, m_maxVertexCount(3)
	, m_stringVertexCount(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_test = CreateMeshNavigateGenarator();
	float test[65* 65];
	InitMeshNavigateGenarator(m_test, 64, 64, 1, test, 65 * 65);
}


void CtestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Slider(pDX, IDC_SLIDER2, m_maxVertexCount);
	DDV_MinMaxInt(pDX, m_maxVertexCount, 3, 1000);
	DDX_Text(pDX, IDC_STATIC1111, m_stringVertexCount);
	
}

BEGIN_MESSAGE_MAP(CtestDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	
	ON_COMMAND(IDPOLYGONREDOWITHCURRENTDATA, &CtestDlg::ReDrawPolygonWithCurrentData)
	ON_COMMAND(IDPOLYGON, &CtestDlg::DrawPolygon)
	//}}AFX_MSG_MAP

	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER2, &CtestDlg::OnChangeMaxVertexCount)
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()


// CtestDlg 消息处理程序

BOOL CtestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码



	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CtestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文
		
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CRect rect;
		GetClientRect(&rect);
		CDialog::OnPaint();
	}

	vector<Point>::iterator it = m_pointList.begin();
	vector<Point>::iterator beginIt = m_pointList.begin();
	vector<Point>::iterator endIt = m_pointList.end();
	CClientDC dc(this);
	CBrush brush;
	CBrush* oldBrush;
	brush.CreateSolidBrush(RGB(0, 0, 0));
	oldBrush = dc.SelectObject(&brush);
	for (; it != endIt; ++it)
	{
		const int radiu = 1;
		dc.Ellipse(it->x - radiu, it->z - radiu , it->x + radiu, it->z + radiu);
		CString s;
		//s.Format(L"%d %d %d", it - beginIt, (int)it->x, (int)it->z);
		//dc.TextOut(it->x + 5, it->z + 5, s);
	}
	dc.SelectObject(oldBrush);
	if (!m_result.empty())
	{
		vector<pair<Point, Point>>::iterator iti = m_result.begin();
		vector<pair<Point, Point>>::iterator endIti = m_result.end();
		for (; iti != endIti; ++iti)
		{
			dc.MoveTo(iti->first.x, iti->first.z);
			dc.LineTo(iti->second.x, iti->second.z);
		}
		//dc.LineTo(m_polygonPointList[0].x, m_polygonPointList[0].z);
	}

}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CtestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CtestDlg::DrawPolygon()
{
	RECT rect;
	this->GetClientRect(&rect);
	Queue* result = Endtest(m_test, rect.right, rect.bottom);
	int resultCount = GetDataCountFromQueue(result);
	m_result.clear();
	for (int i = 0; i < resultCount; ++i)
	{
		MeshPolygon* p = (MeshPolygon*)GetDataFromQueueByIndex(result, i);
		const Point* pointList = GetPolygonPointList(p);
		for (int t = 0; t < 3; ++t)
		{
			int n = t + 1 == 3 ? 0 : t + 1;
			Point a = {pointList[t].x, pointList[t].z};
			Point b = {pointList[n].x, pointList[n].z};
			m_result.push_back(std::make_pair(a, b));
		}
		ReleasePolygon(p);
	}
	ReleaseQueue(result);
	RedrawWindow();
	return;
}

void CtestDlg::ReDrawPolygonWithCurrentData()
{
	m_pointList.clear();
	m_result.clear();
	BeginTest(m_test);
	FILE* f = fopen("test", "rb");
	int count = 0;
	fread(&count, sizeof(count), 1, f);
	count = count >> 2;
	int breakCount = count;
	for (int i  =0; i < count; ++i)
	{
		if (breakCount <= i)
		{
			break;
		}
		float x;
		float z;
		fread(&x, sizeof(x), 1, f);
		fread(&z, sizeof(z), 1, f);
		Point p = {(float)x, (float)z};
		m_pointList.push_back(p);
		AddPointForTest(m_test, p.x, p.z);
	}
	RedrawWindow();
	return;
}


void CtestDlg::OnChangeMaxVertexCount(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 此功能要求 Windows Vista 或更高版本。
	// _WIN32_WINNT 符号必须 >= 0x0600。
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	UpdateData(TRUE);
	
}


void CtestDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Point p = {point.x, point.y};
	m_pointList.push_back(p);
	AddPointForTest(m_test, p.x, p.z);
	DrawPolygon();
	CDialog::OnLButtonDown(nFlags, point);
}

void CtestDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Point p = {point.x, point.y};
	m_pointList.clear();
	m_result.clear();
	BeginTest(m_test);
	RedrawWindow();
	CDialog::OnRButtonDown(nFlags, point);
}

CtestDlg::~CtestDlg()
{
	ReleaseMeshNavigateGenarator(m_test);
	CDialog::~CDialog();
}
