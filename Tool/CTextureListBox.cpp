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
}

CTextureListBox::~CTextureListBox()
{
}

void CTextureListBox::Load_TextureList(const wstring& folderPath)
{
    SetRedraw(FALSE);
    if (GetCount() != 0)
    {
        ResetContent();
        m_FilePathMap.clear();
    }

    m_stCurFilePath = folderPath;
	CFileFind finder;
    BOOL bWorking = finder.FindFile((folderPath + _T("\\*.*")).c_str());
    vector<pair<wstring, wstring>> files;
    files.reserve(300);
    while (bWorking)
    {
        bWorking = finder.FindNextFile();

        if (!finder.IsDots() && !finder.IsDirectory())
        {
            wstring fileName = finder.GetFileName();
            wstring ext = fileName.substr(fileName.length() - 4);
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);

            if (ext == L".png" || ext == L".jpg")
            {
                wstring nameWithoutExt = fileName.substr(0, fileName.rfind('.'));
                files.push_back({ nameWithoutExt, finder.GetFilePath().GetString()});
            }
        }
    }
    finder.Close();
    const wregex pattern(L"(?:_|^)(\\d+)");
    sort(files.begin(), files.end(), [&pattern](const auto& p1, const auto& p2) {
        wsmatch matches;
        int a = 0, b = 0;

        if (regex_search(p1.first, matches, pattern))
            a = _wtoi(matches[1].str().c_str());
        if (regex_search(p2.first, matches, pattern))
            b = _wtoi(matches[1].str().c_str());

        return a < b;
        });
    wstring tmpPath;
    for (const auto& [name, path] : files)
    {
        int nIndex = AddString(name.c_str());
        if (tmpPath.empty())
        {
            tmpPath = CFileInfo::Convert_RelativePath(path.c_str());
        }
        m_FilePathMap[nIndex] = path;
    }

    SetRedraw(TRUE);   // 리스트박스 갱신 재개
    Invalidate();
    tmpPath = regex_replace(tmpPath, wregex(L"(.*\\\\.*?)(\\d+)([^\\\\]*$)"), L"$1%d$3");
    CTextureMgr::Get_Instance()->Insert_Texture(tmpPath, TEX_MULTI,m_stCategory, m_stFolderName, (int)files.size());
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
