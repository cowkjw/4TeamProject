#pragma once

#include "Include.h"

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
	void Picking_Tile(const D3DXVECTOR3& mousePoint);
	void Change_DrawID(bool bUp);

private:
	void Render_Current_Draw_Tile();
	void Set_Picking_DrawId(const int iDrawId) { m_iChangeDrawId = iDrawId; }

private:
	vector<TILE*>		m_vecTile;
	vector<D3DXMATRIX> m_vecTileWorldMat;
	int m_iChangeDrawId;
	bool m_bCanRender;
	DWORD64 m_dwDrawTileRenderTime;
	DWORD64 m_dwContinuousTime;


	float fCameraZoom = 1.0f;  // 줌 레벨 (1.0이 기본)
	D3DXVECTOR2 vCameraOffset = { 0.0f, 0.0f };  // 카메라 이동 오프셋
};

