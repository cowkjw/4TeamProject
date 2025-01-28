#include "pch.h"
#include "CUndoManager.h"
#include "MainFrm.h"
#include "CTerrain.h"
#include "ToolView.h"

IMPLEMENT_SINGLETON(CUndoManager)

void CUndoManager::SaveState(const UndoType eUndoType)
{
	EditorState state;
	state.eType = eUndoType;

	switch (eUndoType)
	{
	case UndoType::TILE:
	{

		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		CToolView* pView = dynamic_cast<CToolView*>(pMainFrm->m_ThirdSplitter.GetPane(0, 0));

		CTerrain* pTerrain = pView->m_pTerrain;
		if (pTerrain)
		{
			state.vecTileInfo = pTerrain->m_vecTile;
		}
		m_UndoStack.push_back(state);
		break;
	}
	case UndoType::OBJ:
		break;
	default:
		break;

	}


	if (MAX_UNDO_SIZE < m_UndoStack.size())
	{
		m_UndoStack.pop_front();
	}
}

void CUndoManager::Undo()
{
	if (m_UndoStack.empty())
	{
		return;
		//throw runtime_error("되돌릴 수 없음");
	}

	EditorState currentState = m_UndoStack.back();
	m_UndoStack.pop_back();
	switch (currentState.eType)
	{
	case UndoType::TILE:
	{

		CMainFrame* pMainFrm = (CMainFrame*)AfxGetMainWnd();
		CToolView* pView = dynamic_cast<CToolView*>(pMainFrm->m_ThirdSplitter.GetPane(0, 0));

		CTerrain* pTerrain = pView->m_pTerrain;
		if (pTerrain)
		{
			pTerrain->m_vecTile = currentState.vecTileInfo;
			pView->Invalidate(FALSE);
		}
		break;
	}
	case UndoType::OBJ:
		// 오브젝트 정보 복원 로직
		break;
	}
}
