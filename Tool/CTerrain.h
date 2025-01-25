#pragma once

#include "Include.h"
#include "ToolView.h"
#include "CMiniView.h"

class CTerrain
{
public:
	CTerrain();
	~CTerrain();

public:
	void		Initialize();
	void		Update();
	void		Render();
	void		Release();
	void Mini_Render();
	void Picking_Tile(const D3DXVECTOR3& mousePoint);
	void Change_DrawID(int iDrawId);
	void	Set_MainView(CToolView* pMainView) { m_pMainView = pMainView; }
	void	Set_MiniView(CMiniView* pMiniView) { m_pMiniView = pMiniView; }
	void Set_CameraOffsetX(float deltaX) { vCameraOffset.x += deltaX; }
	void Set_CameraOffsetY(float deltaY) { vCameraOffset.y += deltaY; }
	void Set_CameraZoom(bool bUp) { fCameraZoom = bUp ? fCameraZoom += 0.1f : max(0.f, fCameraZoom - 0.1f); }

private:
	void Render_Current_Draw_Tile();
	void Set_Picking_DrawId(const int iDrawId) { m_iChangeDrawId = iDrawId; }
	void Set_Ratio(D3DXMATRIX& pOut, float _fX, float _fY);
	void RenderTileOutline(const TILE* tile);
	void DrawDiamondGrid();
private:
	vector<TILE*>		m_vecTile;
	vector<array<D3DXVECTOR2, 5>>	m_vecLine;

	vector<D3DXMATRIX> m_vecTileWorldMat;
	vector<D3DXMATRIX> m_vecMiniTileWorldMat;
	int m_iChangeDrawId;
	bool m_bCanRender;
	DWORD64 m_dwDrawTileRenderTime;
	DWORD64 m_dwContinuousTime;
	CToolView* m_pMainView;
	CMiniView* m_pMiniView;
	LPD3DXLINE m_pLine;
	float fCameraZoom = 1.0f;  // 줌 레벨 (1.0이 기본)
	D3DXVECTOR2 vCameraOffset;
};

