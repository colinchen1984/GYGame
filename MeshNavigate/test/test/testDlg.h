
// testDlg.h : ͷ�ļ�
//

#pragma once
#include "../../BaseStruct.h"
#include <vector>
#include <utility>
using std::vector;
using std::pair;
struct MeshNavigateGenerator;
// CtestDlg �Ի���
class CtestDlg : public CDialog
{
// ����
public:
	CtestDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CtestDlg();
// �Ի�������
	enum { IDD = IDD_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	vector<Point> m_pointList;
	MeshNavigateGenerator* m_test;
	vector<pair<Point, Point>> m_result;
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
