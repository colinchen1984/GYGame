
// testDlg.h : ͷ�ļ�
//

#pragma once
#include "../../BaseStruct.h"
#include <vector>
using std::vector;

// CtestDlg �Ի���
class CtestDlg : public CDialog
{
// ����
public:
	CtestDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	vector<Point> m_pointList;
	vector<Point> m_polygonPointList;
	
	vector<vector<Point>> m_clippingPoint;
	vector<Point> m_clippedPoint;
	vector<Point> m_clipingWindow;
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void DrawPolygon();
	afx_msg void ReDrawPolygonWithCurrentData();
	DECLARE_MESSAGE_MAP()
private:
	int m_maxVertexCount;
public:

	afx_msg void OnChangeMaxVertexCount(NMHDR *pNMHDR, LRESULT *pResult);
	CString m_stringVertexCount;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};
