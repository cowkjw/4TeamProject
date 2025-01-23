// CTextureListBox.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "CTextureListBox.h"


// CTextureListBox

IMPLEMENT_DYNAMIC(CTextureListBox, CListBox)

CTextureListBox::CTextureListBox()
{

}

CTextureListBox::~CTextureListBox()
{
}

void CTextureListBox::Load_TextureList(const CString& folderPath)
{
    if (GetCount() != 0)
    {
        ResetContent();
    }

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
END_MESSAGE_MAP()



// CTextureListBox 메시지 처리기


