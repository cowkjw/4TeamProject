#pragma once


// CTextureListBox

class CTextureListBox : public CListBox
{
	DECLARE_DYNAMIC(CTextureListBox)

public:
	CTextureListBox();
	virtual ~CTextureListBox();
	void Load_TextureList(const CString& folderPath = L"../Texture/Stage/Terrain/Tile"); // 디폴트 타일
	//void Load_TextureList(const CString& folderPath = L"../Texture/Stage/Effect/ElectronicShock");
	void Load_TextureListOfObjcet(const CString& folderPath = L"../Texture/Stage/Effect/ElectronicShock");
	const auto& Get_CurFilePath() const { return m_stCurFilePath; }
	void Set_CurFilePath(const CString& filePath) { m_stCurFilePath = m_stCurFilePath; }
	const auto& Get_FilePath(int nIndex) { return m_FilePathMap[nIndex]; }
protected:
	DECLARE_MESSAGE_MAP()
private:
	CString m_stCurFilePath;
	map<int, CString> m_FilePathMap;
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
};


