// CFindFolderTree.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "afxdialogex.h"
#include "CFindFolderTree.h"
#include "ToolView.h"
#include "MainFrm.h"
#include "CMyForm.h"



// CFindFolderTree 대화 상자

IMPLEMENT_DYNAMIC(CFindFolderTree, CDialog)

CFindFolderTree::CFindFolderTree(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_CFindFolderTree, pParent)
{

}

CFindFolderTree::~CFindFolderTree()
{
}

void CFindFolderTree::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_TREE1, m_treeCtrl);
	DDX_Control(pDX, IDC_MFCSHELLTREE1, m_MFCTreeCtrl);
}


BEGIN_MESSAGE_MAP(CFindFolderTree, CDialog)

	//ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CFindFolderTree::OnTvnSelchangedTree1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_MFCSHELLTREE1, &CFindFolderTree::OnTvnSelchangedMfcshelltree1)
END_MESSAGE_MAP()


BOOL CFindFolderTree::OnInitDialog()
{
	CDialog::OnInitDialog();

	//CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	//CMyForm* pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));
	//CString path = _T("../Texture/Stage/");
	//if (PathFileExists(path))
	//{
	//	HTREEITEM hRoot = m_treeCtrl.InsertItem(path);
	//}
	//else
	//{
	//	AfxMessageBox(_T("경로가 존재하지 않습니다."));
	//}
    m_MFCTreeCtrl.EnableShellContextMenu(TRUE);
    m_MFCTreeCtrl.SetFlags(m_MFCTreeCtrl.GetFlags() | SHCONTF_FOLDERS);
    
	TCHAR fullPath[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, fullPath);
	PathAppend(fullPath, _T("../Texture/Stage"));
	if (PathFileExists(fullPath))
		m_MFCTreeCtrl.SelectPath(fullPath);
	else
		AfxMessageBox(_T("경로가 존재하지 않음"));
	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CFindFolderTree::OnTvnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	HTREEITEM hItem = pNMTreeView->itemNew.hItem;

	// 아이템 텍스트(경로) 가져오기
	CString strPath = m_treeCtrl.GetItemText(hItem);
	*pResult = 0;
}


void CFindFolderTree::OnTvnSelchangedMfcshelltree1(NMHDR* pNMHDR, LRESULT* pResult)
{
	//LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	//// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//*pResult = 0;
}
