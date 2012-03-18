
// testDlg.h : 头文件
//

#pragma once
#include "../../BaseStruct.h"
#include <vector>
using std::vector;

// CtestDlg 对话框
class CtestDlg : public CDialog
{
// 构造
public:
	CtestDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	vector<Point> m_pointList;
	vector<Point> m_polygonPointList;
	
	vector<vector<Point>> m_clippingPoint;
	vector<Point> m_clippedPoint;
	vector<Point> m_clipingWindow;
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
