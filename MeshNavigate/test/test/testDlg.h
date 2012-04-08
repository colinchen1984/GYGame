
// testDlg.h : 头文件
//

#pragma once
#include "../../BaseStruct.h"
#include <vector>
#include <utility>
using std::vector;
using std::pair;
struct MeshNavigateGenerator;
// CtestDlg 对话框
class CtestDlg : public CDialog
{
// 构造
public:
	CtestDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CtestDlg();
// 对话框数据
	enum { IDD = IDD_TEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	vector<Point> m_pointList;
	MeshNavigateGenerator* m_test;
	vector<pair<Point, Point>> m_result;
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
