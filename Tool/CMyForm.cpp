// CMyForm.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "CMyForm.h"
#include "MainFrm.h"
#include "ToolView.h"
#include "CTerrain.h"


// CMyForm

IMPLEMENT_DYNCREATE(CMyForm, CFormView)

CMyForm::CMyForm()
	: CFormView(IDD_CMyForm)
{
	
}

CMyForm::~CMyForm()
{
}

void CMyForm::Change_ListBox()
{
	//if (nullptr == m_FindFolderTree.GetSafeHwnd())
	//	m_FindFolderTree.Create(IDD_CFindFolderTree);	// 해당 id에 맞는 다이얼로그 생성

	//m_FindFolderTree.ShowWindow(SW_SHOW);
	m_TextureListBox.Load_TextureListOfObjcet();
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_TextureListBox);
	DDX_Control(pDX, IDC_STATIC_IMAGE, m_TextrueImage);
}

BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMyForm::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyForm::Change_ListBox)
END_MESSAGE_MAP()


// CMyForm 진단

#ifdef _DEBUG
void CMyForm::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyForm::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CMyForm 메시지 처리기


void CMyForm::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	SetScrollSizes(MM_TEXT, CSize(0, 0)); // 스크롤 없애기
	m_TextureListBox.Load_TextureList();
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}

void CMyForm::OnLbnSelchangeList1()
{
	int nIndex = m_TextureListBox.GetCurSel();
	if (nIndex != LB_ERR)
	{
		CString strFileName;
		m_TextureListBox.GetText(nIndex, strFileName);
		strFileName = m_TextureListBox.Get_CurFilePath() +L"\\" + strFileName;
		CImage image;
		if (image.Load(strFileName) == S_OK)
		{
			CImage resizedImage;
			resizedImage.Create(200, 200, image.GetBPP()); // 원하는 크기 지정
			image.StretchBlt(resizedImage.GetDC(), 0, 0, 200, 200);
			resizedImage.ReleaseDC();
			m_TextrueImage.SetBitmap((HBITMAP)resizedImage.Detach());

			CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
			CToolView* pView = dynamic_cast<CToolView*>(pMainFrm->m_ThirdSplitter.GetPane(0, 0));

			CTerrain* pTerrain = pView->m_pTerrain;

			pTerrain->Change_DrawID(nIndex);

		}
	}
}
