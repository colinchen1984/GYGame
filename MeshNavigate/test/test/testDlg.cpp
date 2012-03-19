
// testDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "test.h"
#include "testDlg.h"
#include "../../ObjectMesh.h"
#include "../../Polygon.h"
#include "../../PolygonClipping.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestDlg �Ի���




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


// CtestDlg ��Ϣ��������

BOOL CtestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ����Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի���������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CtestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������
		
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CRect rect;
		GetClientRect(&rect);
		CDialog::OnPaint();
	}
#define CONVEXHULL
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
		dc.TextOut(it->x + 5, it->z + 5, s);
		++it;
		for (; it != endIt; ++it)
		{
			dc.LineTo(it->x, it->z);
			s.Format(L"%d %d", (int)it->x, (int)it->z);
			dc.TextOut(it->x + 5, it->z + 5, s);
		}
		dc.LineTo(m_clippingPoint[i][0].x, m_clippingPoint[i][0].z);
	}
#endif
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CtestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CtestDlg::DrawPolygon()
{
	CRect rect;
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
	ReleaseItemNavigateMesh(item);

	MeshPolygon* clipped = CreatePolygon(m_clippedPoint.size());
	for (int i = 0; i < m_clippedPoint.size(); ++i)
	{
		AddPointToPolygon(clipped, m_clippedPoint[i].x, m_clippedPoint[i].z);
	}

	MeshPolygon* clippingWindow = CreatePolygon(m_clipingWindow.size());
	for (int i = 0; i < m_clipingWindow.size();++i)
	{
		AddPointToPolygon(clippingWindow, m_clipingWindow[i].x, m_clipingWindow[i].z);
	}
	MeshPolygon* test[100];
	int result;
	ConvexPolygonClipping(clipped, clippingWindow, test, 100, &result);
	ReleasePolygon(clipped);
	ReleasePolygon(clippingWindow);
	m_clippingPoint.clear();
	for (int i = 0; i < result; ++i)
	{
		const Point* polygonPoint = GetPolygonPointList(test[i]);
		const int polygonPointCount = GetPolygonPointCount(test[i]);
		vector<Point> temp;
		temp.assign(polygonPoint, polygonPoint+ polygonPointCount);
		m_clippingPoint.push_back(temp);
		ReleasePolygon(test[i]);
		
	}
	m_clippedPoint.clear();
	m_clipingWindow.clear();
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
	const int polygonPointCount = GeConvexHullPointCount(item);
	m_polygonPointList.assign(polygonPoint, polygonPoint + polygonPointCount);
	ReleaseItemNavigateMesh(item);
	RedrawWindow();
	return;
}


void CtestDlg::OnChangeMaxVertexCount(NMHDR *pNMHDR, LRESULT *pResult)
{
	// �˹���Ҫ�� Windows Vista ����߰汾��
	// _WIN32_WINNT ���ű��� >= 0x0600��
	NMTRBTHUMBPOSCHANGING *pNMTPC = reinterpret_cast<NMTRBTHUMBPOSCHANGING *>(pNMHDR);
	UpdateData(TRUE);
	m_maxVertexCount = m_maxVertexCount < 3 ? 3 : m_maxVertexCount;
	m_stringVertexCount.Format(L"%d", m_maxVertexCount);
	UpdateData(FALSE);
	*pResult = 0;
}


void CtestDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	Point p = {point.x, point.y};
	m_clippedPoint.push_back(p);
	m_clippingPoint.clear();
	RedrawWindow();
	CDialog::OnLButtonDown(nFlags, point);
}

void CtestDlg::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ�������Ϣ������������/�����Ĭ��ֵ
	Point p = {point.x, point.y};
	m_clipingWindow.push_back(p);
	m_clippingPoint.clear();
	RedrawWindow();
	CDialog::OnRButtonDown(nFlags, point);
}