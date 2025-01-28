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
	OnDestroy();
}

void CMyForm::OnButtonObjectList()
{
	if (nullptr == m_FindFolderTree.GetSafeHwnd())
		m_FindFolderTree.Create(IDD_CFindFolderTree);	// 해당 id에 맞는 다이얼로그 생성

	m_FindFolderTree.RefreshTree(_T("../Texture/Human"));
	m_FindFolderTree.ShowWindow(SW_SHOW);
//	m_TextureListBox.Load_TextureListOfObjcet();
}

void CMyForm::OnButtonTileList()
{
	if (nullptr == m_FindFolderTree.GetSafeHwnd())
		m_FindFolderTree.Create(IDD_CFindFolderTree);	// 해당 id에 맞는 다이얼로그 생성

	m_FindFolderTree.RefreshTree(_T("../Texture/Stage/Terrain"));
	m_FindFolderTree.ShowWindow(SW_SHOW);
}

void CMyForm::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_TextureListBox);
	DDX_Control(pDX, IDC_STATIC_IMAGE, m_TextrueImage);
}


BEGIN_MESSAGE_MAP(CMyForm, CFormView)
	ON_LBN_SELCHANGE(IDC_LIST1, &CMyForm::OnLbnSelchangeList1)
	ON_BN_CLICKED(IDC_BUTTON5, &CMyForm::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON4, &CMyForm::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON2, &CMyForm::OnButtonObjectList)
	ON_BN_CLICKED(IDC_BUTTON3, &CMyForm::OnButtonTileList)
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
	//m_TextureListBox.Load_TextureList();
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}

void CMyForm::OnLbnSelchangeList1()
{
	int nIndex = m_TextureListBox.GetCurSel();
	if (nIndex != LB_ERR)
	{
		wstring strFileName;
		wstring CurCategory = m_TextureListBox.m_stCategory;
		strFileName = m_TextureListBox.Get_FilePath(nIndex);// FilePath() +
		CImage image;
		if (image.Load(strFileName.c_str()) == S_OK)
		{
			CClientDC dc(&m_TextrueImage);
			CRect rect;
			m_TextrueImage.GetClientRect(&rect);

			dc.FillSolidRect(rect, RGB(255, 255, 255));
			image.Draw(dc, rect);

			if (CurCategory == L"Tile")
			{

			CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
			CToolView* pView = dynamic_cast<CToolView*>(pMainFrm->m_ThirdSplitter.GetPane(0, 0));

			CTerrain* pTerrain = pView->m_pTerrain;
			pTerrain->m_stTileFolderName = m_TextureListBox.m_stFolderName;
			pTerrain->Change_DrawID(nIndex, m_TextureListBox.m_stFolderName);
			}

		}
	}
}

//Save 버튼
void CMyForm::OnBnClickedButton3()
{
	SaveTileData(, _T("tileDate.dat"));
}

//Load 버튼
void CMyForm::OnBnClickedButton4()
{
	vector<TILE*> vecLoadTile;
	LoadTileData(vecLoadTile, _T("tileDate.dat"));


}

void CMyForm::SaveTileData(const vector<TILE*>& vecTile, const CString& FileName)
{
	CFile File(FileName, CFile::modeCreate | CFile::modeWrite);
	CArchive Ar(&File, CArchive::store);

	int iTileCount = vecTile.size();
	Ar << iTileCount; // 타일의 개수를 저장

	for (int i = 0; i < iTileCount; ++i)
	{
		vecTile[i]->Serialize(Ar);
	}

	Ar.Close();
	File.Close();
}

void CMyForm::LoadTileData(vector<TILE*>& vecTile, const CString& FileName)
{
	CFile File(FileName, CFile::modeRead);
	CArchive Ar(&File, CArchive::load);

	int iTileCount;
	Ar >> iTileCount; // 타일의 개수를 불러오기

	vecTile.resize(iTileCount);

	for (int i = 0; i < iTileCount; ++i)
	{
		vecTile[i]->Serialize(Ar);
	}

	Ar.Close();
	File.Close();
}

