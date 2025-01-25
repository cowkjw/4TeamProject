#pragma once


// CTextureListBox

class CTextureListBox : public CListBox
{
	DECLARE_DYNAMIC(CTextureListBox)

public:
	CTextureListBox();
	virtual ~CTextureListBox();
	void Load_TextureList(const wstring& folderPath = L"../Texture/Stage/Terrain/Tile"); // 디폴트 타일
	//void Load_TextureList(const CString& folderPath = L"../Texture/Stage/Effect/ElectronicShock");
	void Load_TextureListOfObjcet(const CString& folderPath = L"../Texture/Stage/Effect/ElectronicShock");
	const auto& Get_CurFilePath() const { return m_stCurFilePath; }
	void Set_CurFilePath(const wstring& filePath) { m_stCurFilePath = m_stCurFilePath; }
	const wstring& Get_FilePath(int nIndex) { return m_FilePathMap[nIndex];}
protected:
	DECLARE_MESSAGE_MAP()
public:
	wstring m_stCurFilePath;
	wstring m_stCategory;
	wstring m_stFolderName;
	map<int, wstring> m_FilePathMap;
public:
	afx_msg void OnDropFiles(HDROP hDropInfo);
};


