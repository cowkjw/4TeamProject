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
	if (FAILED(CTextureMgr::Get_Instance()->Insert_Texture(
		L"../Texture/Stage/Terrain/Tile/Tile%d.png",
		TEX_MULTI, L"Terrain", L"Tile", 36)))
	{
		AfxMessageBox(L"Terrain Texture Insert Failed");
		return;
	}
	D3DXCreateLine(CDevice::Get_Instance()->Get_Device(), &m_pLine);
	m_bCanRender = true;
	m_iChangeDrawId = 0;
	m_dwContinuousTime = 2000ULL; // 2초
	m_dwDrawTileRenderTime = GetTickCount64();
	//m_vecTile.resize(가로 *세로)
	//for (int j = 0; j < WINCY / TILEY; j++)
	//{
	//	for (int i = 0; i < WINCX / TILECX + 1; i++)
	//	{
	//		float fOffsetX = (j % 2 == 0) ? TILECX * 0.5f : 0.0f;

	//		m_vecTile.push_back(new TILE(
	//			{ float(i * TILECX) + fOffsetX , float(j * (TILECY * 0.5f)), 0.f },
	//			0, { TILECX, TILECY }));
	//	}
	//}
// 화면 중앙 좌표
	//vector<TILE> vecTmpTile;
	//CJsonManager<vector<TILE>>::Load_File(vecTmpTile, "TILE_Info");
	//for (auto& tile : vecTmpTile)
	//{
	//	m_vecTile.push_back(new TILE(tile));
	//}
	
	float centerX = WINCX * 0.5f;
	float centerY = WINCY * 0.5f;

	// 타일 개수 계산 (각 방향으로 퍼져나갈 타일 수)
	int halfTilesX = (WINCX / TILECX) / 2;  // 한쪽 방향으로 퍼질 타일 수
	int halfTilesY = (WINCY / TILEY) / 2;

	// 중앙(0,0)에서 시작
	float localX = 0.0f;
	float localY = 0.0f;

	// -halfTiles부터 +halfTiles까지 반복하여 4방향으로 타일 생성
	for (int j = -halfTilesY; j <= halfTilesY; j++)
	{
		for (int i = -halfTilesX-1; i <= halfTilesX; i++)
		{
			float fOffsetX = (j % 2 == 0) ? TILECX * 0.5f : 0.0f;

			float posX = localX + (i * TILECX) + fOffsetX;
			float posY = localY + (j * (TILECY * 0.5f));
			float worldX = posX + centerX;
			float worldY = posY + centerY;
			m_vecTile.push_back(new TILE(
				{ worldX , worldY, 0.f },  // 실제 화면에 그릴 때는 중앙점 더하기
				0,
				{ TILECX, TILECY }));
		}
	}

	m_vecTileWorldMat.resize(m_vecTile.size());
	for (int i = 0; i < (int)m_vecTileWorldMat.size(); i++)
	{
		TILE* tile = m_vecTile[i];
		D3DXMATRIX  matScale, matTrans;
		auto& matWorld = m_vecTileWorldMat[i];

		D3DXMatrixIdentity(&matWorld);
		/*D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, tile->vPos.x, tile->vPos.y, tile->vPos.z);*/

		D3DXMatrixScaling(&matScale, fCameraZoom, fCameraZoom, 1.f);

		// 타일 위치에 카메라 오프셋 적용
		D3DXMatrixTranslation(&matTrans,
			(tile->vPos.x + vCameraOffset.x- m_pMainView->GetScrollPos(0)) * fCameraZoom,
			(tile->vPos.y + vCameraOffset.y- m_pMainView->GetScrollPos(1)) * fCameraZoom,
			tile->vPos.z);

		
		matWorld = matScale * matTrans;

		RECT	rc{};

		GetClientRect(m_pMainView->m_hWnd, &rc);

		float	fX = WINCX / float(rc.right - rc.left);
		float	fY = WINCY / float(rc.bottom - rc.top);

		Set_Ratio(matWorld, fX, fY);
	}
}

void CTerrain::Update()
{

	float screenCenterX = WINCX / 2.0f;
	float screenCenterY = WINCY / 2.0f;

	// 줌 적용 시 화면 중앙 유지를 위한 오프셋 계산
	float zoomOffsetX = screenCenterX - (screenCenterX + vCameraOffset.x + m_pMainView->GetScrollPos(0)) * fCameraZoom;
	float zoomOffsetY = screenCenterY - (screenCenterY + vCameraOffset.y + m_pMainView->GetScrollPos(1)) * fCameraZoom;
	for (int i = 0; i < (int)m_vecTile.size(); i++)
	{
		TILE* tile = m_vecTile[i];
		if (tile->bChange)
		{
			
		}
		D3DXMATRIX matWorld, matScale, matTrans;
		D3DXMatrixIdentity(&matWorld);
		/*D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, tile->vPos.x, tile->vPos.y, tile->vPos.z);*/
		D3DXMatrixScaling(&matScale, fCameraZoom, fCameraZoom, 1.f);

		// 타일 위치에 카메라 오프셋 적용

		D3DXMatrixTranslation(&matTrans,
			(tile->vPos.x  * fCameraZoom) + zoomOffsetX,
			(tile->vPos.y  * fCameraZoom) + zoomOffsetY,
			tile->vPos.z);
		matWorld = matScale * matTrans;
		RECT	rc{};

		GetClientRect(m_pMainView->m_hWnd, &rc);

		float	fX = WINCX / float(rc.right - rc.left);
		float	fY = WINCY / float(rc.bottom - rc.top);

		Set_Ratio(matWorld, fX, fY);
		m_vecTileWorldMat[i] = matWorld;
		tile->bChange = false;
	}
}

void CTerrain::Render()
{
#ifdef _DEBUG
	static CPerformanceTimer timer;
	static int frameCount = 0;
	static double totalRenderTime = 0.0;
	static int drawCalls = 0;
	static int textureSwaps = 0;
	timer.Start();
#endif
	const TEXINFO* lastTexInfo = nullptr;
	BYTE currentDrawID = 255; // 초기값을 잘못된 값으로 설정
	for (int i = 0; i < (int)m_vecTile.size(); i++)
	{
		TILE* tile = m_vecTile[i];
		
#ifdef _DEBUG
		// 텍스처 스왑 카운트
		if (currentDrawID != tile->byDrawID) 
		{
			textureSwaps++;
			currentDrawID = tile->byDrawID;
			lastTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", currentDrawID);
		}
		// 드로우콜 카운트
		drawCalls++;
#endif
		D3DXVECTOR3 vCenter(tile->vSize.x * 0.5f, tile->vSize.y * 0.5f, 0.f);

		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&m_vecTileWorldMat[i]);
		if (lastTexInfo)
		{
		CDevice::Get_Instance()->Get_Sprite()->Draw(lastTexInfo->pTexture,
			nullptr,
			&vCenter,   // 텍스처의 중심점 (중심점을 구해서 이동 시켜야지 잘 그려짐
			nullptr,    // 이미 matWorld로 위치를 설정했으므로 nullptr
			D3DCOLOR_ARGB(255, 255, 255, 255));
		}
		// D3DXMatrixTranslation(&m_vecTileWorldMat[i], 200.f, 200.f, 0.f);



	}

	for (int i = 0; i < (int)m_vecTile.size(); i++)
	{
		TILE* tile = m_vecTile[i];
		TCHAR	szBuf[MIN_STR] = L"";
		swprintf_s(szBuf, L"(%.f)", -(tile->vPos.y - WINCY * 0.5f));// , tile->vPos.y - WINCY * 0.5f);
		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&m_vecTileWorldMat[i]);
		CDevice::Get_Instance()->Get_Font()->DrawTextW(CDevice::Get_Instance()->Get_Sprite(),
			szBuf,		// 출력할 문자열
			lstrlen(szBuf),  // 문자열 버퍼의 크기
			nullptr,	// 출력할 렉트 위치
			0,			// 정렬 기준(옵션)
			D3DCOLOR_ARGB(255, 255, 255, 255));
	}
	if (m_bCanRender)
	{
		Render_Current_Draw_Tile();
	}

#ifdef _DEBUG
	// 성능 정보 출력 (60프레임마다)
	double renderTime = timer.End();
	totalRenderTime += renderTime;
	frameCount++;

	if (frameCount >= 60)
	{
		double averageRenderTime = totalRenderTime / frameCount;
		wchar_t szDebug[256] = {};
		swprintf_s(szDebug,
			L"[Terrain Info]\nAvg Render Time: %.3f ms\nDraw Calls: %d\nTexture Swaps: %d\nTile Count: %d\n\n",
			averageRenderTime, drawCalls / frameCount, textureSwaps / frameCount, (int)m_vecTile.size());
		OutputDebugString(szDebug);

		// 카운터 리셋
		frameCount = 0;
		totalRenderTime = 0.0;
		drawCalls = 0;
		textureSwaps = 0;
	}
#endif
//	CDevice::Get_Instance()->Get_Sprite()->End();
	//for (const auto& tile : m_vecTile) 
	//{
	//	RenderTileOutline(tile);
	//}


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
{  /*  WINDOWINFO wndInfo;
    GetWindowInfo(m_pMiniView->m_hWnd, &wndInfo);
    float miniMapWidth = (float)(wndInfo.rcWindow.right - wndInfo.rcWindow.left);
    float miniMapHeight = (float)(wndInfo.rcWindow.bottom - wndInfo.rcWindow.top);

    float miniMapScale = min(miniMapWidth / WINCX, miniMapHeight / WINCY) * 0.9f;*/
	const TEXINFO* lastTexInfo = nullptr;
	BYTE currentDrawID = 255; // 초기값을 잘못된 값으로 설정
	D3DXMATRIX matWorld,matScale,matTrans;
	for (int i = 0; i < (int)m_vecTile.size(); i++)
	{
		TILE* tile = m_vecTile[i];

		if (currentDrawID != tile->byDrawID)
		{
			currentDrawID = tile->byDrawID;
			lastTexInfo = CTextureMgr::Get_Instance()->Get_Texture(L"Terrain", L"Tile", currentDrawID);
		}
		D3DXVECTOR3 vCenter(tile->vSize.x * 0.5f, tile->vSize.y * 0.5f, 0.f);
		D3DXMatrixScaling(&matScale, 1.f, 1.f, 1.f);
		D3DXMatrixTranslation(&matTrans, tile->vPos.x+ (float)WINCX, tile->vPos.y+(float)WINCY, tile->vPos.z);
		matWorld = matScale* matTrans;
		Set_Ratio(matWorld, 0.3f, 0.3f);
		RECT	rc{};

		GetClientRect(m_pMainView->m_hWnd, &rc);
		CDevice::Get_Instance()->Get_Sprite()->SetTransform(&matWorld);
		if (lastTexInfo)
		{
			CDevice::Get_Instance()->Get_Sprite()->Draw(lastTexInfo->pTexture,
				nullptr,
				&vCenter,   // 텍스처의 중심점 (중심점을 구해서 이동 시켜야지 잘 그려짐
				nullptr,    // 이미 matWorld로 위치를 설정했으므로 nullptr
				D3DCOLOR_ARGB(255, 255, 255, 255));
		}
	}
}

void CTerrain::Picking_Tile(const D3DXVECTOR3& mousePoint)
{
	float screenCenterX = WINCX / 2.0f;
	float screenCenterY = WINCY / 2.0f;

	// 줌 적용 시 화면 중앙 유지를 위한 오프셋 계산
	float zoomOffsetX = screenCenterX - (screenCenterX + vCameraOffset.x) * fCameraZoom;
	float zoomOffsetY = screenCenterY - (screenCenterY + vCameraOffset.y) * fCameraZoom;
	D3DXVECTOR3 adjustedMousePoint = {
	   (mousePoint.x - zoomOffsetX) / fCameraZoom+vCameraOffset.x + m_pMainView->GetScrollPos(0),
	   (mousePoint.y - zoomOffsetY) / fCameraZoom+ vCameraOffset.y +m_pMainView->GetScrollPos(1),
	   mousePoint.z
	};

	for (auto& tile : m_vecTile)
	{
		bool bInner = true;

		D3DXVECTOR3 upPoint, bottomPoint, leftPoint, rightPoint;

		upPoint = { tile->vPos.x, tile->vPos.y + tile->vSize.y * 0.5f, 0.f };
		bottomPoint = { tile->vPos.x, tile->vPos.y - tile->vSize.y * 0.5f,0.f };
		rightPoint = { tile->vPos.x + tile->vSize.x * 0.5f, tile->vPos.y,0.f };
		leftPoint = { tile->vPos.x - tile->vSize.x * 0.5f, tile->vPos.y ,0.f };

		// 시계방향
		//vector<D3DXVECTOR3> vecCWStartPoints{ leftPoint ,upPoint, rightPoint, bottomPoint };
		//vector<D3DXVECTOR3> vecCWPolyVec{ upPoint - leftPoint,rightPoint - upPoint,bottomPoint - rightPoint,
		//leftPoint - bottomPoint };		
		//for (int i = 0; i < (int)vecCWStartPoints.size(); i++)
		//{
		//	D3DXVECTOR3 vMouse = mousePoint - vecCWStartPoints[i];
		//	D3DXVECTOR3 tmp(-vecCWPolyVec[i].y, vecCWPolyVec[i].x, vecCWPolyVec[i].z); //안쪽으로 법선벡터를 뽑음

		//	D3DXVec3Normalize(&vMouse, &vMouse);
		//	D3DXVec3Normalize(&tmp, &tmp);
		//	if (D3DXVec3Dot(&tmp, &vMouse) < 0.f)
		//	{
		//		bInner = false;
		//		break;
		//	}
		//}
		// 반시계 방향
		vector<D3DXVECTOR3> vecCCWStartPoints{ upPoint,leftPoint,  bottomPoint,rightPoint };
		vector<D3DXVECTOR3> vecCCWPolyVec{leftPoint-upPoint,bottomPoint - leftPoint,
	 rightPoint - bottomPoint, upPoint-rightPoint };

		for (int i = 0; i < (int)vecCCWStartPoints.size(); i++)
		{
			D3DXVECTOR3 vMouse = adjustedMousePoint - vecCCWStartPoints[i];
			D3DXVECTOR3 tmp(-vecCCWPolyVec[i].y, vecCCWPolyVec[i].x, vecCCWPolyVec[i].z); // 반시계는 안쪽(왼쪽 회전)으로 법선 벡터를 구함

			D3DXVec3Normalize(&vMouse, &vMouse);
			D3DXVec3Normalize(&tmp, &tmp);
			if (D3DXVec3Dot(&tmp, &vMouse) < 0.f)
			{
				bInner = false;
				break;
			}
		}


		//// 우상
		//float a1 = (rightPoint.y - upPoint.y) / (rightPoint.x - upPoint.x);
		//float b1 = upPoint.y - a1 * upPoint.x; // b = y - ax
		//if (mousePoint.y < a1 * mousePoint.x + b1)
		//{
		//	continue;
		//}

		//// 좌상
		//float a2 = (leftPoint.y - upPoint.y) / (leftPoint.x - upPoint.x);
		//float b2 = upPoint.y - a2 * upPoint.x;
		//if (mousePoint.y < a2 * mousePoint.x + b2)
		//{

		//	continue;
		//}

		//// 우하
		//float a3 = (rightPoint.y - bottomPoint.y) / (rightPoint.x - bottomPoint.x);
		//float b3 = bottomPoint.y - a3 * bottomPoint.x;
		//if (mousePoint.y > a3 * mousePoint.x + b3)
		//{
		//	continue;
		//}

		//// 좌하
		//float a4 = (leftPoint.y - bottomPoint.y) / (leftPoint.x - bottomPoint.x);
		//float b4 = bottomPoint.y - a4 * bottomPoint.x;
		//if (mousePoint.y > a4 * mousePoint.x + b4)
		//{
		//	continue;
		//}

		if (bInner)
		{
			tile->byDrawID = m_iChangeDrawId;
			tile->bChange = true;
			break;
		}
	}
}

void CTerrain::Change_DrawID(int iDrawId)
{
	
	m_bCanRender = true;
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

	D3DXVECTOR2 vertices[5];
	float screenCenterX = WINCX / 2.0f;
	float screenCenterY = WINCY / 2.0f;
	float zoomOffsetX = screenCenterX - (screenCenterX + vCameraOffset.x+ m_pMainView->GetScrollPos(0)) * fCameraZoom;
	float zoomOffsetY = screenCenterY - (screenCenterY + vCameraOffset.y+ m_pMainView->GetScrollPos(1)) * fCameraZoom;

	float halfWidth = (tile->vSize.x * 0.5f) * fCameraZoom * fX;
	float halfHeight = (tile->vSize.y * 0.5f) * fCameraZoom * fY;

	float posX = ((tile->vPos.x + zoomOffsetX) * fX);
	float posY = ((tile->vPos.y + zoomOffsetY) * fY);

	vertices[0] = D3DXVECTOR2(posX - halfWidth, posY);
	vertices[1] = D3DXVECTOR2(posX, posY + halfHeight);
	vertices[2] = D3DXVECTOR2(posX + halfWidth, posY);
	vertices[3] = D3DXVECTOR2(posX, posY - halfHeight);
	vertices[4] = vertices[0];

	m_pLine->SetWidth(2.0f);
	m_pLine->SetAntialias(TRUE);
	m_pLine->SetGLLines(TRUE);
	m_pLine->Begin();
	m_pLine->Draw(vertices, 5, D3DCOLOR_ARGB(255, 0, 255, 0));
	m_pLine->End();
}

void CTerrain::DrawDiamondGrid()
{
	CDevice::Get_Instance()->Get_Sprite()->End();
	if (!m_pLine) return;

	RECT rc{};
	GetClientRect(m_pMainView->m_hWnd, &rc);
	float fX = WINCX / float(rc.right - rc.left);
	float fY = WINCY / float(rc.bottom - rc.top);

	// Grid properties
	float cellWidth = TILECX * fCameraZoom * fX;  // Adjust size as needed
	float cellHeight = TILECY * fCameraZoom * fY; // Adjust size as needed

	// Calculate grid coverage
	int numRows = (WINCY / cellHeight) + 2;
	int numCols = (WINCX / cellWidth) + 2;

	// Calculate offset for camera movement
	float screenCenterX = WINCX / 2.0f;
	float screenCenterY = WINCY / 2.0f;
	float zoomOffsetX = screenCenterX - (screenCenterX + vCameraOffset.x) * fCameraZoom;
	float zoomOffsetY = screenCenterY - (screenCenterY + vCameraOffset.y) * fCameraZoom;

	m_pLine->SetWidth(2.0f);
	m_pLine->SetAntialias(TRUE); // 안티앨리어싱 키기
	m_pLine->SetGLLines(TRUE); 

	// Draw grid
	m_pLine->Begin();

	for (int row = -numRows; row < numRows; row++)
	{
		for (int col = -numCols; col < numCols; col++)
		{
			float baseX = ((col * TILECX) - m_pMainView->GetScrollPos(0) - vCameraOffset.x) * fCameraZoom + zoomOffsetX;
			float baseY = ((row * TILECY) - m_pMainView->GetScrollPos(1) - vCameraOffset.y) * fCameraZoom + zoomOffsetY;

			D3DXVECTOR2 vertices[5];
			vertices[0] = D3DXVECTOR2(baseX, baseY);
			vertices[1] = D3DXVECTOR2(baseX + cellWidth / 2, baseY + cellHeight / 2);
			vertices[2] = D3DXVECTOR2(baseX + cellWidth, baseY);
			vertices[3] = D3DXVECTOR2(baseX + cellWidth / 2, baseY - cellHeight / 2);
			vertices[4] = vertices[0];

			m_pLine->Draw(vertices, 5, D3DCOLOR_ARGB(255, 0, 255, 0));
		}
	}

	m_pLine->End();
}

