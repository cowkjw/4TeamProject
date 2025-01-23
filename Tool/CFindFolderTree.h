#pragma once
#include "afxdialogex.h"


// CFindFolderTree 대화 상자

class CFindFolderTree : public CDialog
{
	DECLARE_DYNAMIC(CFindFolderTree)

public:
	CFindFolderTree(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CFindFolderTree();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CFindFolderTree };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_treeCtrl;
	CMFCShellTreeCtrl m_MFCTreeCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTvnSelchangedMfcshelltree1(NMHDR* pNMHDR, LRESULT* pResult);
};
