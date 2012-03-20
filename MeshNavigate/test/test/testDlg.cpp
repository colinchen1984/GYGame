
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
#define CLIPPING
#ifdef CONVEXHULL
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
		const int radiu = 2;
		dc.Ellipse(it->x - radiu, it->z - radiu , it->x + radiu, it->z + radiu);
		//CString s;
		//s.Format(L"%d %d %d", it - beginIt, (int)it->x, (int)it->z);
		//dc.TextOut(it->x + 5, it->z + 5, s);
	}
	dc.SelectObject(oldBrush);
	if (!m_polygonPointList.empty())
	{
		it = m_polygonPointList.begin();
		endIt = m_polygonPointList.end();
		dc.MoveTo(it->x, it->z);
		++it;
		for (; it != endIt; ++it)
		{
			dc.LineTo(it->x, it->z);
		}
		dc.LineTo(m_polygonPointList[0].x, m_polygonPointList[0].z);
	}
#endif 
#ifdef CLIPPING
	CClientDC dc(this);
	CBrush brush;
	CBrush* oldBrush;
	brush.CreateSolidBrush(RGB(0, 0, 0));
	oldBrush = dc.SelectObject(&brush);
	vector<Point>::iterator it = m_clippedPoint.begin();
	vector<Point>::iterator beginIt = m_clippedPoint.begin();
	if (m_clippingPoint.empty())
	{
		for (; it != m_clippedPoint.end(); ++it)
		{
			const int radiu = 2;
			dc.Ellipse(it->x - radiu, it->z - radiu , it->x + radiu, it->z + radiu);
			CString s;
			s.Format(L"%d %d %d", it - beginIt, (int)it->x, (int)it->z);
			dc.TextOut(it->x + 5, it->z + 5, s);
		}

		if (m_clippedPoint.size() >= 3)
		{
			it = m_clippedPoint.begin();
			dc.MoveTo(it->x, it->z);
			++it;
			for (; it != m_clippedPoint.end(); ++it)
			{
				dc.LineTo(it->x, it->z);
			}
			dc.LineTo(m_clippedPoint[0].x, m_clippedPoint[0].z);
		}
		it = m_clipingWindow.begin();
		beginIt = m_clipingWindow.begin();
		for (; it != m_clipingWindow.end(); ++it)
		{
			const int radiu = 2;
			dc.Ellipse(it->x - radiu, it->z - radiu , it->x + radiu, it->z + radiu);
			CString s;
			s.Format(L"%d %d %d", it - beginIt, (int)it->x, (int)it->z);
			dc.TextOut(it->x + 5, it->z + 5, s);
		}

		if (m_clipingWindow.size() >= 3)
		{
			it = m_clipingWindow.begin();
			dc.MoveTo(it->x, it->z);
			++it;
			for (; it != m_clipingWindow.end(); ++it)
			{
				dc.LineTo(it->x, it->z);
			}
			dc.LineTo(m_clipingWindow[0].x, m_clipingWindow[0].z);
		}
	}
	dc.SelectObject(oldBrush);

	for (int i  = 0; i < m_clippingPoint.size(); ++i)
	{
		vector<Point>::iterator it = m_clippingPoint[i].begin();
		vector<Point>::iterator endIt = m_clippingPoint[i].end();
		dc.MoveTo(it->x, it->z);
		CString s;
		s.Format(L"%d %d", (int)it->x, (int)it->z);
		//dc.TextOut(it->x + 5, it->z + 5, s);
		++it;
		for (; it != endIt; ++it)
		{
			dc.LineTo(it->x, it->z);
			s.Format(L"%d %d", (int)it->x, (int)it->z);
			//dc.TextOut(it->x + 5, it->z + 5, s);
		}
		dc.LineTo(m_clippingPoint[i][0].x, m_clippingPoint[i][0].z);
	}
#endif
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CtestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CtestDlg::DrawPolygon()
{
/*	CRect rect;
	GetClientRect(&rect);
	int xMax = 400, yMax = 320;
	//srand(time(NULL));
	int vertexCount = m_maxVertexCount * 10; (rand() + 3) % 100;
	m_pointList.clear();
	m_polygonPointList.clear();
	ItemNavigateMesh* item = CreateItemNavigateMesh(vertexCount);
	for (int i = 0; i < vertexCount; ++i)
	{
		float x = (rand() + 3) % xMax;
		float y = (rand() + 3) % yMax;
		Point temp = {x, y};
		if(AddVertexToItemNavigateMesh(item, x, y))
		{
			m_pointList.push_back(temp);
		}
	}
	(MakeConvexHullFromItem(item));
	const Point* polygonPoint = GetConvexHullPointList(item);
	const int polygonPointCount = GeConvexHullPointCount(item);
	m_polygonPointList.assign(polygonPoint, polygonPoint + polygonPointCount);
	ReleaseItemNavigateMesh(item);*/

	/*MeshPolygon* clipped = CreatePolygon(m_clippedPoint.size());
	for (int i = 0; i < m_clippedPoint.size(); ++i)
	{
		AddPointToPolygon(clipped, m_clippedPoint[i].x, m_clippedPoint[i].z);
	}
	MeshPolygon* testT = CreatePolygon(5);
	AddPointToPolygon(testT, 0.0f + 1.0f, 0.0f + 1.0f);
	AddPointToPolygon(testT, 1.0f + 1.0f, -1.0f + 1.0f);
	AddPointToPolygon(testT, 0.50f + 1.0f, 0.00f + 1.0f);
	AddPointToPolygon(testT, 1.0f + 1.0f, 1.0f + 1.0f);
	Queue* resultQueue = CreateQueue(16);
	ConcavePolygonDecompose(testT, resultQueue);
	Point p = {0.5f, 0.5f};
	Matrix3x3 testM;
	IdentityMatrix(&testM);
	MakeRotateMatrix(&testM, 3.1415926 / 2, true);
	MritrixPlusPoint( &testM, &p);
	IdentityMatrix(&testM);
	MakeRotateMatrix(&testM, 3.1415926 / 2, false);
	MritrixPlusPoint( &testM, &p);
	IsConvexPolygon(testT);
	MeshPolygon* clippingWindow = CreatePolygon(m_clipingWindow.size());
	for (int i = 0; i < m_clipingWindow.size();++i)
	{
		AddPointToPolygon(clippingWindow, m_clipingWindow[i].x, m_clipingWindow[i].z);
	}
	CleanQueue(resultQueue);
	ConvexPolygonClipping(clipped, clippingWindow, resultQueue);
	ReleasePolygon(clipped);
	ReleasePolygon(clippingWindow);
	m_clippingPoint.clear();
	while(MeshPolygon* polygon = (MeshPolygon*)PopDataFromQueue(resultQueue))
	{
		const Point* polygonPoint = GetPolygonPointList(polygon);
		const int polygonPointCount = GetPolygonPointCount(polygon);
		vector<Point> temp;
		temp.assign(polygonPoint, polygonPoint+ polygonPointCount);
		m_clippingPoint.push_back(temp);
		ReleasePolygon(polygon);
		
	}
	m_clippedPoint.clear();
	m_clipingWindow.clear();*/


	MeshPolygon* clippingWindow = CreatePolygon(m_clipingWindow.size());
	for (int i = 0; i < m_clipingWindow.size();++i)
	{
		AddPointToPolygon(clippingWindow, m_clipingWindow[i].x, m_clipingWindow[i].z);
	}
	Queue* resultQueue = CreateQueue(16);
	int result = ConcavePolygonDecompose(clippingWindow, resultQueue);
	m_clipingWindow.clear();
	while(MeshPolygon* polygon = (MeshPolygon*)PopDataFromQueue(resultQueue))
	{
		const Point* polygonPoint = GetPolygonPointList(polygon);
		const int polygonPointCount = GetPolygonPointCount(polygon);
		vector<Point> temp;
		temp.assign(polygonPoint, polygonPoint+ polygonPointCount);
		m_clippingPoint.push_back(temp);
		ReleasePolygon(polygon);
	}
	ReleaseQueue(resultQueue);
	ReleasePolygon(clippingWindow);

	RedrawWindow();
	return;
}

void CtestDlg::ReDrawPolygonWithCurrentData()
{
	m_polygonPointList.clear();
	ItemNavigateMesh* item = CreateItemNavigateMesh(m_pointList.size());
	for (int i = 0; i < m_pointList.size(); ++i)
	{
		assert(AddVertexToItemNavigateMesh(item, m_pointList[i].x, m_pointList[i].z));		
	}
	(MakeConvexHullFromItem(item));
	const Point* polygonPoint = GetConvexHullPointList(item);
	const int polygonPointCount = GetConvexHullPointCount(item);
	m_polygonPointList.assign(polygonPoint, polygonPoint + polygonPointCount);
	ReleaseItemNavigateMesh(item);
	RedrawWindow();
	return;
}


void CtestDlg::OnChangeMaxVertexCount(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 此功能要求 Windows Vista 或更高版本。
	// _WIN32_WINNT 符号必须 >= 0x0600。
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	UpdateData(TRUE);
	m_maxVertexCount = m_maxVertexCount < 3 ? 3 : m_maxVertexCount;
	m_stringVertexCount.Format(L"%d", m_maxVertexCount);
	UpdateData(FALSE);
	*pResult = 0;




	int xMax = 400, yMax = 320;
	//srand(time(NULL));
	int vertexCount = m_maxVertexCount * 10; (rand() + 3) % 100;
	ItemNavigateMesh* itemA = CreateItemNavigateMesh(vertexCount);
	ItemNavigateMesh* itemB = CreateItemNavigateMesh(vertexCount);
	ItemNavigateMesh* itemC = CreateItemNavigateMesh(vertexCount);
	/*for (int i = 0; i < vertexCount; ++i)
	{
		{
			float x = (rand() + 3) % xMax;
			float y = (rand() + 3) % yMax;
			Point temp = {x, y};
			(AddVertexToItemNavigateMesh(itemA, x, y));
		}

		{
			float x = (rand() + 3) % xMax;
			float y = (rand() + 3) % yMax;
			Point temp = {x, y};
			(AddVertexToItemNavigateMesh(itemB, x, y));
		}

	}*/
	float rans = 200.0f;
	(AddVertexToItemNavigateMesh(itemA, -100.0f + rans, -100.0f+ rans));
	(AddVertexToItemNavigateMesh(itemA, -100.0f+ rans, 100.0f+ rans));
	(AddVertexToItemNavigateMesh(itemA, 100.0f+ rans, 100.0f+ rans));
	(AddVertexToItemNavigateMesh(itemA, 100.0f+ rans, -100.0f+ rans));


	(AddVertexToItemNavigateMesh(itemB, 50.0f+ rans, 50.0f+ rans));
	(AddVertexToItemNavigateMesh(itemB, 50.0f+ rans, 150.0f+ rans));
	(AddVertexToItemNavigateMesh(itemB, 150.0f+ rans, 150.0f+ rans));
	(AddVertexToItemNavigateMesh(itemB, 150.0f+ rans, 50.0f+ rans));

	(AddVertexToItemNavigateMesh(itemC, 70.0f + rans, 70.0f + rans));
	(AddVertexToItemNavigateMesh(itemC, 70.0f+ rans, -70.0f+ rans));
	(AddVertexToItemNavigateMesh(itemC, 200.0f+ rans, -70.0f+ rans));
	(AddVertexToItemNavigateMesh(itemC, 200.0f + rans, 70.0f+ rans));


	bool result = (MakeConvexHullFromItem(itemA));
	result = (MakeConvexHullFromItem(itemB));
	result = (MakeConvexHullFromItem(itemC));


	MeshNavigateSystem* sys = CreateGridSystem();
	float* height = (float*)(malloc(sizeof(float) * ((513 * 513))));
	InitGridSystem(sys, 512, 512, 1, height, 513 * 513);
	AddItemToGridSystem(sys, itemA);
	AddItemToGridSystem(sys, itemB);
	AddItemToGridSystem(sys, itemC);
	MakeMeshNavigateData(sys);
	Queue* resultQueue = GetItemQueue(sys);
	m_clippingPoint.clear();
	while(MeshPolygon* polygon = (MeshPolygon*)PopDataFromQueue(resultQueue))
	{
		const Point* polygonPoint = GetPolygonPointList(polygon);
		const int polygonPointCount = GetPolygonPointCount(polygon);
		vector<Point> temp;
		temp.assign(polygonPoint, polygonPoint+ polygonPointCount);
		m_clippingPoint.push_back(temp);
		ReleasePolygon(polygon);
	}
}


void CtestDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Point p = {point.x, point.y};
	m_clippedPoint.push_back(p);
	m_clippingPoint.clear();

	RedrawWindow();
	CDialog::OnLButtonDown(nFlags, point);
}

void CtestDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Point p = {point.x, point.y};
	m_clipingWindow.push_back(p);
	m_clippingPoint.clear();
	RedrawWindow();
	CDialog::OnRButtonDown(nFlags, point);
}
