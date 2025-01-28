
// ToolView.cpp: CToolView 클래스의 구현
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "Tool.h"
#endif

#include "ToolDoc.h"
#include "ToolView.h"
#include "CDevice.h"
#include "CTextureMgr.h"
#include "CTerrain.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "CMiniView.h"
#include "CKeyManager.h"
#include "CUndoManager.h"
#include "CMyForm.h"

HWND	g_hWnd;


// CToolView

IMPLEMENT_DYNCREATE(CToolView, CView)

BEGIN_MESSAGE_MAP(CToolView, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

// CToolView 생성/소멸

CToolView::CToolView() noexcept
	: m_pDevice(CDevice::Get_Instance()), m_pTerrain(nullptr), m_nTimer(0), m_fSrollSpeed(0.f), m_fAlpha(0.f), m_dwDisplayTime(0)
	//, m_pSingle(nullptr)

{
	// TODO: 여기에 생성 코드를 추가합니다.

}

CToolView::~CToolView()
{
}

void CToolView::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	m_nTimer = SetTimer(1, 16, NULL);
	// AfxGetMainWnd : 현재 메인 윈도우의 값을 반환하는 전역함수

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();

	RECT rcWnd{};

	// GetWindowRect : 현재 윈도우(창)의 rect 정보를 얻어오는 함수
	pMainFrm->GetWindowRect(&rcWnd);

	// SetRect : 지정한 매개 변수 대로 Rect 정보를 기록하는 함수

	SetRect(&rcWnd, 0, 0, rcWnd.right - rcWnd.left, rcWnd.bottom - rcWnd.top);

	// rcWnd.right : 창의 가로 사이즈
	// rcWnd.bottom : 창의 세로 사이즈

	RECT rcMainView{};

	// GetClientRect : 현재 View창의 정보를 Rect 형식으로 얻어오는 함수
	GetClientRect(&rcMainView);

	float	fRowFrm = float(rcWnd.right - rcMainView.right);
	float	fColFrm = float(rcWnd.bottom - rcMainView.bottom);

	// SetWindowPos : 매개 변수대로 윈도우 위치와 크기를 조정하는 함수
	// SetWindowPos(배치할 윈도우의 Z순서에 대한 포인터, X좌표, Y좌표, 가로 크기, 세로 크기, 배치할 윈도우의 크기 및 위치지정 옵션)

	pMainFrm->SetWindowPos(nullptr,		// 순서 변경하지 않음
		0,
		0,
		int(WINCX + fRowFrm),
		int(WINCY + fColFrm),
		SWP_NOZORDER); // 현재 순서를 유지함


	g_hWnd = m_hWnd;

	if (FAILED(CDevice::Get_Instance()->Init_Device()))
	{
		AfxMessageBox(L"Init_Device Create Failed");
		return;
	}



	if (!m_pTerrain)
	{
		m_pTerrain = new CTerrain;
		CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(GetParentFrame());
		CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));
		m_pTerrain->Set_MainView(this);
		m_pTerrain->Set_MiniView(pMiniView);
		m_pTerrain->Initialize();
	}

	m_fSrollSpeed = 5.f;
	m_fAlpha = 255.f;

}

void CToolView::OnLButtonDown(UINT nFlags, CPoint point)
{
	CView::OnLButtonDown(nFlags, point);

	CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
	CMyForm* pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));
	if (nullptr != pMyForm)
	{
		if (0 == pMyForm->m_TextureListBox.GetCount())
			return;
	}

	m_pTerrain->Picking_Tile(D3DXVECTOR3((float)point.x, (float)point.y, 0.f));

	//	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(GetParentFrame());
	CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));

	pMiniView->Invalidate(FALSE);
}


void CToolView::OnDraw(CDC* /*pDC*/)
{
	CToolDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	m_pDevice->Render_Begin();


	//m_pDevice->Get_Sprite()->SetTransform(&matWorld);

	// float	fCenterX = m_pSingle->Get_Texture()->tImgInfo.Width / 2.f;
	// float	fCenterY = m_pSingle->Get_Texture()->tImgInfo.Height / 2.f;

	//const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", 7);

	//float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
	//float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

	//D3DXVECTOR3	vTemp{ fCenterX, fCenterY, 0.f };

	//m_pDevice->Get_Sprite()->Draw(pTexInfo->pTexture, //출력할 텍스처 컴객체
	//								nullptr,		// 출력할 이미지 영역에 대한 Rect 주소, null인 경우 이미지의 0, 0기준으로 출력
	//								&vTemp,		// 출력할 이미지의 중심 좌표 vec3 주소, null인 경우 0, 0 이미지 중심
	//								nullptr,		// 위치 좌표에 대한 vec3 주소, null인 경우 스크린 상 0, 0 좌표 출력	
	//								D3DCOLOR_ARGB(255, 255, 255, 255)); // 출력할 이미지와 섞을 색상 값, 0xffffffff를 넘겨주면 섞지 않고 원본 색상 유지
	m_pTerrain->Update();
	m_pTerrain->Render();
	// 폰트의 출력 위치를 조정 가능
	// D3DXMatrixTranslation(&matTrans, 200.f, 200.f, 0.f);
	// m_pDevice->Get_Sprite()->SetTransform(&matTrans);

	//TCHAR	szBuf[MIN_STR] = L"";

	//int	iNumber = 1000;

	//swprintf_s(szBuf, L"%d", iNumber);

	//m_pDevice->Get_Font()->DrawTextW(m_pDevice->Get_Sprite(),
	//	szBuf,		// 출력할 문자열
	//	lstrlen(szBuf),  // 문자열 버퍼의 크기
	//	nullptr,	// 출력할 렉트 위치
	//	0,			// 정렬 기준(옵션)
	//	D3DCOLOR_ARGB(255, 255, 255, 255));

	CDevice::Get_Instance()->Get_Sprite()->Begin(D3DXSPRITE_ALPHABLEND);
	TCHAR szSpeed[32];
	float fDeltaTime = (GetTickCount() - m_dwDisplayTime) / 1000.0f;
	m_fAlpha = max(0.0f, 255.0f - (fDeltaTime * 85.0f)); // 3초에 걸쳐 사라짐

	// alpha값 적용

	RECT rc;
	GetClientRect(&rc);
	RECT textRect = { rc.right / 2 - 100, rc.bottom / 2 - 15, rc.right / 2 + 100, rc.bottom / 2 + 15 };
	D3DXMATRIX matTrans;
	D3DXMATRIX matScale;
	D3DXMatrixScaling(&matScale, 10.f, 10.f, 5.f); // 폰트 크기 2배
	D3DXMatrixTranslation(&matTrans, rc.right / 2.0f, rc.bottom / 2.0f, 0.f);
	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matTrans);
	swprintf_s(szSpeed, L"Scroll Speed: %.1f", m_fSrollSpeed);
	CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
		szSpeed,
		lstrlen(szSpeed),
		nullptr,
		DT_CENTER,
		D3DCOLOR_ARGB((BYTE)m_fAlpha, 255, 255, 255));
	m_pDevice->Render_End();


}

void CToolView::OnDestroy()
{
	KillTimer(m_nTimer);
	CView::OnDestroy();

	//Safe_Delete(m_pSingle);
	CKeyManager::Destroy_Instance();
	Safe_Delete(m_pTerrain);
	CTextureMgr::Destroy_Instance();
	CUndoManager::Destroy_Instance();
	m_pDevice->Destroy_Instance();

}


#pragma region 생략
BOOL CToolView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CToolView 인쇄

BOOL CToolView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CToolView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CToolView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CToolView 진단

#ifdef _DEBUG
void CToolView::AssertValid() const
{
	CView::AssertValid();
}

void CToolView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CToolDoc* CToolView::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CToolDoc)));
	return (CToolDoc*)m_pDocument;
}
#endif //_DEBUG


// CToolView 메시지 처리기

#pragma endregion


BOOL CToolView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (zDelta > 0)
	{
		if (CKeyManager::Get_Instance()->Key_Pressing(VK_RBUTTON))
		{
			m_fSrollSpeed = min(15.f, m_fSrollSpeed + 0.5f);
			m_dwDisplayTime = GetTickCount();
		}
		else
		{
			m_pTerrain->Set_CameraZoom(true);

		}
	}
	else
	{
		if (CKeyManager::Get_Instance()->Key_Pressing(VK_RBUTTON))
		{
			m_fSrollSpeed = max(2.f, m_fSrollSpeed - 0.5f);
			m_dwDisplayTime = GetTickCount();
		}
		else
		{

			m_pTerrain->Set_CameraZoom(false);
		}
	}

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CToolView::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == m_nTimer)
	{
		// 게임 로직 업데이트
		if (m_pDevice)
		{

			m_pTerrain->Update();
			if (CKeyManager::Get_Instance()->Key_Pressing('A'))
			{
				m_pTerrain->Set_CameraOffsetX(-m_fSrollSpeed);
			}
			if (CKeyManager::Get_Instance()->Key_Pressing('D'))
			{
				m_pTerrain->Set_CameraOffsetX(m_fSrollSpeed);
			}

			if (CKeyManager::Get_Instance()->Key_Pressing('W'))
			{
				m_pTerrain->Set_CameraOffsetY(-m_fSrollSpeed);
			}
			if (CKeyManager::Get_Instance()->Key_Pressing('S'))
			{
				m_pTerrain->Set_CameraOffsetY(m_fSrollSpeed);
			}

			if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (GetAsyncKeyState('Z') & 0x0001))
			{
				CUndoManager::Get_Instance()->Undo();

			}


			// 명시적 렌더링
			Invalidate(FALSE);
			CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
			CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));

			pMiniView->Invalidate(FALSE);
			//CKeyManager::Get_Instance()->Update();
		}
	}

	CView::OnTimer(nIDEvent);
}


void CToolView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (CKeyManager::Get_Instance()->Key_Pressing(VK_LBUTTON))
	{
		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		CMyForm* pMyForm = dynamic_cast<CMyForm*>(pMainFrm->m_SecondSplitter.GetPane(1, 0));
		if (nullptr != pMyForm)
		{
			if (0 == pMyForm->m_TextureListBox.GetCount())
				return;
		}

		m_pTerrain->Picking_Tile(D3DXVECTOR3((float)point.x, (float)point.y, 0.f));

		//	CMainFrame* pMainFrm = dynamic_cast<CMainFrame*>(GetParentFrame());
		CMiniView* pMiniView = dynamic_cast<CMiniView*>(pMainFrm->m_SecondSplitter.GetPane(0, 0));

		pMiniView->Invalidate(FALSE);
	}
	CView::OnMouseMove(nFlags, point);
}


void CToolView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	m_pTerrain->OnLButtonUp();

	CView::OnLButtonUp(nFlags, point);
}
