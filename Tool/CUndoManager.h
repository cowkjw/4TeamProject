#pragma once
#include "Include.h"

enum class UndoType
{
	TILE,
	OBJ,
	END
};
struct EditorState
{
	UndoType eType;
	vector<TILE> vecTileInfo;
};
class CUndoManager
{
	DECLARE_SINGLETON(CUndoManager)

public:
	void SaveState(const UndoType eUndoType);
	bool CanUndo() const { return !m_UndoStack.empty(); }
	void Undo();
private:
	CUndoManager(){}
	~CUndoManager() = default;
private:
	const size_t MAX_UNDO_SIZE = 20;
	deque<EditorState> m_UndoStack;
};

