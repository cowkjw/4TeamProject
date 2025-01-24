// CTextureListBox.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "CTextureListBox.h"
#include "CTextureMgr.h"
#include "CFileInfo.h"
#include "CJsonManager.h"


// CTextureListBox

IMPLEMENT_DYNAMIC(CTextureListBox, CListBox)

CTextureListBox::CTextureListBox()
{
    CJsonManager<map<int, wstring>>::Load_File(m_FilePathMap, "TestMap");
}

CTextureListBox::~CTextureListBox()
{
    CJsonManager<map<int, wstring>>::Save_File(m_FilePathMap, "TestMap");
}

void CTextureListBox::Load_TextureList(const wstring& folderPath)
{
    if (GetCount() != 0)
    {
        ResetContent();
    }

    m_stCurFilePath = folderPath;
	CFileFind finder;
    BOOL bWorking = finder.FindFile((folderPath + _T("\\*.*")).c_str());
    vector<pair<CString, CString>> files;
    while (bWorking)
    {
        bWorking = finder.FindNextFile();

        if (!finder.IsDots() && !finder.IsDirectory())
        {
            CString fileName = finder.GetFileName();
            // 특정 확장자만 필터링하려면:
            if (fileName.Right(4).CompareNoCase(_T(".png")) == 0 ||
                fileName.Right(4).CompareNoCase(_T(".jpg")) == 0)
            {
                files.push_back({ fileName.Left(fileName.ReverseFind('.')), finder.GetFilePath()});
               // AddString(fileName);
            }
        }
    }
    finder.Close();

    sort(files.begin(), files.end(), [&](const pair<CString, CString>& p1,const pair<CString, CString>& p2)
        {
            if (p1.first.Find(L"Tile") == 0)
            {
                CString num1 = p1.first.Mid(4);
                CString num2 = p2.first.Mid(4);
                return _ttoi(num1) < _ttoi(num2);
            }
            return p1.first < p2.first;
        });
    for (const auto& file : files)
    {
        int nIndex = AddString(file.first);
        CString tmp = CFileInfo::Convert_RelativePath(file.second);
        m_FilePathMap[nIndex] = file.second;
    }
       // CTextureMgr::Get_Instance()->Insert_Texture(folderPath.GetString(), TEX_MULTI, L"Terrain", L"TILE", (int)files.size());
}

void CTextureListBox::Load_TextureListOfObjcet(const CString& folderPath)
{
    ResetContent();
    m_stCurFilePath = folderPath;
    CFileFind finder;
    BOOL bWorking = finder.FindFile(folderPath + _T("\\*.*"));
    vector<CString> fileNames;
    while (bWorking)
    {
        bWorking = finder.FindNextFile();

        if (!finder.IsDots() && !finder.IsDirectory())
        {
            CString fileName = finder.GetFileName();
            // 특정 확장자만 필터링하려면:
            if (fileName.Right(4).CompareNoCase(_T(".png")) == 0 ||
                fileName.Right(4).CompareNoCase(_T(".jpg")) == 0)
            {
                fileNames.push_back(fileName);
                // AddString(fileName);
            }
        }
    }
    finder.Close();

    sort(fileNames.begin(), fileNames.end(), [&](CString& str1, CString& str2)
        {
            if (str1.Find(L"Tile") == 0)
            {
                CString num1 = str1.Mid(4);
                CString num2 = str2.Mid(4);
                return _ttoi(num1) < _ttoi(num2);
            }
            else
            {
                return str1 < str2;
            }
        });
    for (const auto& name : fileNames)
    {
        AddString(name);
    }
}


BEGIN_MESSAGE_MAP(CTextureListBox, CListBox)
    ON_WM_DROPFILES()
END_MESSAGE_MAP()



// CTextureListBox 메시지 처리기




void CTextureListBox::OnDropFiles(HDROP hDropInfo)
{
    if (GetCount() != 0)
    {
        ResetContent();
    }

    UINT nCount = DragQueryFile(hDropInfo, 0xFFFFFFFF, NULL, 0);
    for (UINT i = 0; i < nCount; i++)
    {
        TCHAR szFilePath[MAX_PATH];
        DragQueryFile(hDropInfo, i, szFilePath, MAX_PATH);

        // 파일 경로에서 파일명만 추출
        CString strFileName = PathFindFileName(szFilePath);
        int nIndex = AddString(strFileName.Left(strFileName.ReverseFind('.')));
        m_FilePathMap[nIndex] = szFilePath;
    }

    DragFinish(hDropInfo);
    CListBox::OnDropFiles(hDropInfo);
}
