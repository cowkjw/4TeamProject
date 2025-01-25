#include "pch.h"
#include "CTerrain.h"
#include "CTextureMgr.h"
#include "CDevice.h"
#include <iostream>
#include "CPerformanceTimer.h"
#include "CJsonManager.h"

CTerrain::CTerrain() :
	m_bCanRender(false), m_pMainView(nullptr),m_pMiniView(nullptr), m_iChangeDrawId(0), m_dwContinuousTime(0ULL),
	m_dwDrawTileRenderTime(0ULL), vCameraOffset(D3DXVECTOR2(0.f,0.f))
{
}

CTerrain::~CTerrain()
{
	Release();
}

void CTerrain::Initialize()
{
	D3DXCreateLine(CDevice::Get_Instance()->Get_Device(), &m_pLine);

	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(
		L"../Texture/Stage/Terrain/Tile/Act2/Tile_%d.png",
		TEX_MULTI, L"Tile", L"Act2", 108)))
	{
		AfxMessageBox(L"Terrain Tile_Act2 Insert Failed");
		return;
	}
	m_stTileFolderName = L"Act2";
	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			TILE* pTile = new TILE;

			float	fY = (TILECY / 2.f) * i;
			float	fX = (TILECX * j) + (i % 2) * (TILECX / 2.f);

			pTile->vPos = { fX, fY, 0.f };
			pTile->vSize = { (float)TILECX, (float)TILECY };
			pTile->byOption = 0;
			pTile->byDrawID = 5;
			pTile->wstrStateKey = L"Act2";

			m_vecTile.push_back(pTile);
		}
	}

	for (int i = 0; i < TILEY; ++i)
	{
		for (int j = 0; j < TILEX; ++j)
		{
			float	fY = (TILECY / 2.f) * i;
			float	fX = (TILECX * j) + (i % 2) * (TILECX / 2.f);
			float	fCX = (TILECX / 2.f);
			float	fCY = (TILECY / 2.f);

			D3DXVECTOR2 vPos[5] =
			{
				{ fX, fY + fCY},
				{ fX + fCX, fY},
				{ fX, fY - fCY},
				{ fX - fCX, fY},
				{ fX, fY + fCY}
			};

			m_vecLine.push_back({
				vPos[0],
				vPos[1],
				vPos[2],
				vPos[3],
				vPos[4]
			});
		}
	}
}

void CTerrain::Update()
{
	

}

void CTerrain::Render()
{
	D3DXMATRIX	matWorld, matScale, matTrans;

	TCHAR	szBuf[MIN_STR] = L"";
	int		iIndex(0);

	for (auto pTile : m_vecTile)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, fCameraZoom, fCameraZoom, 1.f);
		D3DXMatrixTranslation(&matTrans,
			(pTile->vPos.x - vCameraOffset.x) * fCameraZoom,
			(pTile->vPos.y - vCameraOffset.y) * fCameraZoom,
			pTile->vPos.z);

		matWorld = matScale * matTrans;

		RECT	rc{};

		GetClientRect(m_pMainView->m_hWnd, &rc);

		float	fX = WINCX / float(rc.right - rc.left);
		float	fY = WINCY / float(rc.bottom - rc.top);

		Set_Ratio(matWorld, fX, fY);

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Tile", pTile->wstrStateKey, pTile->byDrawID);

		float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
		float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

		D3DXVECTOR3	vTemp{ fCenterX, fCenterY, 0.f };

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, //출력할 텍스처 컴객체
			nullptr,		// 출력할 이미지 영역에 대한 Rect 주소, null인 경우 이미지의 0, 0기준으로 출력
			&vTemp,		// 출력할 이미지의 중심 좌표 vec3 주소, null인 경우 0, 0 이미지 중심
			nullptr,		// 위치 좌표에 대한 vec3 주소, null인 경우 스크린 상 0, 0 좌표 출력	
			D3DCOLOR_ARGB(255, 255, 255, 255)); // 출력할 이미지와 섞을 색상 값, 0xffffffff를 넘겨주면 섞지 않고 원본 색상 유지

		swprintf_s(szBuf, L"%d", iIndex);

		CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
			szBuf,		// 출력할 문자열
			lstrlen(szBuf),  // 문자열 버퍼의 크기
			nullptr,	// 출력할 렉트 위치
			0,			// 정렬 기준(옵션)
			D3DCOLOR_ARGB(255, 255, 255, 255));

		iIndex++;
	}


	DrawDiamondGrid();
}

void CTerrain::Release()
{
	if (m_pLine)
	{
		m_pLine->Release();
		m_pLine = nullptr;
	}
	//vector<TILE> vecTmpTiles;
	//for_each(m_vecTile.begin(), m_vecTile.end(), [&](const TILE* tile)
	//	{
	//		vecTmpTiles.push_back(*tile);
	//	});
	//CJsonManager<vector<TILE>>::Save_File(vecTmpTiles, "TILE_Info");
	for_each(m_vecTile.begin(), m_vecTile.end(), Safe_Delete<TILE*>);
}

void CTerrain::Mini_Render()
{ 
	D3DXMATRIX	matWorld, matScale, matTrans;

	for (auto pTile : m_vecTile)
	{
		D3DXMatrixIdentity(&matWorld);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans,
			pTile->vPos.x,
			pTile->vPos.y,
			pTile->vPos.z);

		matWorld = matScale * matTrans;

		Set_Ratio(matWorld, 0.3f, 0.3f);

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);

		const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Tile", pTile->wstrStateKey, pTile->byDrawID);

		float	fCenterX = pTexInfo->tImgInfo.Width / 2.f;
		float	fCenterY = pTexInfo->tImgInfo.Height / 2.f;

		D3DXVECTOR3	vTemp{ fCenterX, fCenterY, 0.f };

		CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture, //출력할 텍스처 컴객체
			nullptr,		// 출력할 이미지 영역에 대한 Rect 주소, null인 경우 이미지의 0, 0기준으로 출력
			&vTemp,		// 출력할 이미지의 중심 좌표 vec3 주소, null인 경우 0, 0 이미지 중심
			nullptr,		// 위치 좌표에 대한 vec3 주소, null인 경우 스크린 상 0, 0 좌표 출력	
			D3DCOLOR_ARGB(255, 255, 255, 255)); // 출력할 이미지와 섞을 색상 값, 0xffffffff를 넘겨주면 섞지 않고 원본 색상 유지
	}
}

void CTerrain::Picking_Tile(const D3DXVECTOR3& mousePoint)
{
	for (size_t i = 0; i < TILEX * TILEY; ++i)
	{
		D3DXVECTOR3 vNewMouse{ 0.f, 0.f, 0.f };
		vNewMouse.x = mousePoint.x / fCameraZoom + vCameraOffset.x;
		vNewMouse.y = mousePoint.y / fCameraZoom + vCameraOffset.y;
		
		bool bInner = true;

		D3DXVECTOR3 upPoint, bottomPoint, leftPoint, rightPoint;
		
		upPoint = { m_vecLine[i][0].x, m_vecLine[i][0].y, 0.f };
		rightPoint = { m_vecLine[i][1].x, m_vecLine[i][1].y, 0.f };
		bottomPoint = { m_vecLine[i][2].x, m_vecLine[i][2].y, 0.f };
		leftPoint = { m_vecLine[i][3].x, m_vecLine[i][3].y, 0.f };
		
		vector<D3DXVECTOR3> vecCWStartPoints{ upPoint ,rightPoint, bottomPoint, leftPoint };
		vector<D3DXVECTOR3> vecCWPolyVec{	rightPoint - upPoint,
											bottomPoint - rightPoint,
											leftPoint - bottomPoint,
											upPoint - leftPoint };

		for (int i = 0; i < (int)vecCWStartPoints.size(); i++)
		{
			D3DXVECTOR3 vMouse = vNewMouse - vecCWStartPoints[i];
			D3DXVECTOR3 tmp(-vecCWPolyVec[i].y, vecCWPolyVec[i].x, vecCWPolyVec[i].z);

			D3DXVec3Normalize(&vMouse, &vMouse);
			D3DXVec3Normalize(&tmp, &tmp);

			if (D3DXVec3Dot(&tmp, &vMouse) > 0.f)
			{
				bInner = false;
				break;
			}
		}

		if (bInner)
		{
			m_vecTile[i]->byDrawID = m_iChangeDrawId;
			m_vecTile[i]->wstrStateKey = m_stChangeFolderName;
			m_vecTile[i]->bChange = true;
			break;
		}
	}
}


void CTerrain::Change_DrawID(int iDrawId, const wstring& stChangeFolderName)
{
	
	m_bCanRender = true;
	m_stChangeFolderName = stChangeFolderName;
	m_dwDrawTileRenderTime = GetTickCount64();
	Set_Picking_DrawId(iDrawId);
}

void CTerrain::Render_Current_Draw_Tile()
{
	if (m_bCanRender)
	{
		auto dwCur = GetTickCount64();
		if (dwCur - m_dwDrawTileRenderTime >= m_dwContinuousTime)
		{
			m_bCanRender = false;
		}
	}

	const TEXINFO* pTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", m_iChangeDrawId);

	D3DXVECTOR3 vCenter(TILECX * 0.5f, TILECY * 0.5f, 0.f);

	D3DXMATRIX matWorld, matRotZ, matScale, matTrans;
	D3DXMatrixIdentity(&matWorld);
	D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
	D3DXMatrixTranslation(&matTrans, 700.f, 50.f, 0.f);
	matWorld = matScale * matTrans;

	CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
	CDevice::Get_Instance()->Get_Sprite()->Draw(pTexInfo->pTexture,
		nullptr,
		&vCenter,   // 텍스처의 중심점    
		nullptr,    // 이미 matWorld로 위치를 설정했으므로 nullptr
		D3DCOLOR_ARGB(255, 255, 255, 255));
}

void CTerrain::Set_Ratio(D3DXMATRIX& pOut, float _fX, float _fY)
{
	pOut._11 *= _fX;
	pOut._21 *= _fX;
	pOut._31 *= _fX;
	pOut._41 *= _fX;
		
	pOut._12 *= _fY;
	pOut._22 *= _fY;
	pOut._32 *= _fY;
	pOut._42 *= _fY;
}

void CTerrain::RenderTileOutline(const TILE* tile)
{
	CDevice::Get_Instance()->Get_Sprite()->End();
	if (!m_pLine) return;

	RECT rc{};
	GetClientRect(m_pMainView->m_hWnd, &rc);
	float fX = WINCX / float(rc.right - rc.left);
	float fY = WINCY / float(rc.bottom - rc.top);

	D3DXVECTOR2 vPos1[5];
	
	float fCX(TILECX / 2.f);
	float fCY(TILECY / 2.f);

	vPos1[0] = D3DXVECTOR2(0.f, fCY);
	vPos1[1] = D3DXVECTOR2(fCX, 0.f);
	vPos1[2] = D3DXVECTOR2(0.f, -fCY);
	vPos1[3] = D3DXVECTOR2(-fCX, 0.f);
	vPos1[4] = vPos1[0];

	D3DXVECTOR2 vPos2[5];

	for (int i = 0; i < 5; ++i)
	{
		vPos2[i] = fCameraZoom * vPos1[i] + vCameraOffset;
	}

	m_pLine->SetWidth(2.0f);
	m_pLine->SetAntialias(TRUE);
	m_pLine->SetGLLines(TRUE);
	m_pLine->Begin();
	m_pLine->Draw(vPos2, 5, D3DCOLOR_ARGB(255, 0, 255, 0));
	m_pLine->End();
}

void CTerrain::DrawDiamondGrid()
{
	CDevice::Get_Instance()->Get_Sprite()->End();
	if (!m_pLine) return;

	m_pLine->SetWidth(2.0f);
	m_pLine->SetAntialias(TRUE); // 안티앨리어싱 키기
	m_pLine->SetGLLines(TRUE); 

	// Draw grid
	m_pLine->Begin();

	for (auto& line : m_vecLine)
	{
		auto arrLine = line;

		for (int i = 0; i < arrLine.size(); ++i)
		{
			arrLine[i] = fCameraZoom * ( arrLine[i] - vCameraOffset);
		}

		m_pLine->Draw(arrLine.data(), 5, D3DCOLOR_ARGB(255, 0, 255, 0));
	}

	m_pLine->End();
}

