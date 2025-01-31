#include "pch.h"
#include "CTextureMgr.h"

IMPLEMENT_SINGLETON(CTextureMgr)

CTextureMgr::CTextureMgr()
{
}

CTextureMgr::~CTextureMgr()
{
	Release();
}

const TEXINFO* CTextureMgr::Get_Texture(const wstring& pObjKey, const wstring& pStateKey, const int iCnt)
{
	auto& innerMap = m_mapTex[pObjKey];
	auto innerIter = innerMap.find(pStateKey);
	if (innerIter == innerMap.end() && innerMap.find(pObjKey) == innerMap.end()) // 멀티도 싱글도 텍스쳐가 없으면 nullptr
	{
		if (Insert_Texture_Directory(pObjKey, pStateKey) == E_FAIL)
			return nullptr;
        else
        {
            innerMap = m_mapTex[pObjKey];
            innerIter = innerMap.find(pStateKey);
        }
	}
	return innerIter->second->Get_Texture(pStateKey, iCnt);
}

HRESULT CTextureMgr::Insert_Texture(const wstring& pFilePath, TEXTYPE eTexture, const wstring& pObjKey, const wstring& pStateKey, const int iCnt)
{
	auto& innerMap = m_mapTex[pObjKey];
	auto innerIter = innerMap.find(pStateKey);

	if (innerIter == innerMap.end())
	{
		CTexture* pTexture = nullptr;
		switch (eTexture)
		{
		case TEX_SINGLE:
			pTexture = new CSingleTexture;
			break;
		case TEX_MULTI:
			pTexture = new CMultiTexture;
			break;
		}

		if (FAILED(pTexture->Insert_Texture(pFilePath, pStateKey, iCnt)))
		{
			AfxMessageBox(pFilePath.c_str());
			return E_FAIL;
		}
		switch (eTexture)
		{
		case TEX_SINGLE:
			innerMap.insert({ pObjKey, pTexture });
			break;
		case TEX_MULTI:
			innerMap.insert({ pStateKey, pTexture });
			break;
		}
	}
	return S_OK;
}

void CTextureMgr::Release()
{
	for_each(m_mapTex.begin(), m_mapTex.end(), [](auto& MyPair)
		{
			for (auto& innerPair : MyPair.second)
			{
				Safe_Delete<CTexture*>(innerPair.second);
			}
		});
	m_mapTex.clear();

}

HRESULT CTextureMgr::Insert_Texture_Directory(const wstring& pObjKey, const wstring& pStateKey)
{
    wstring foundPath;
    // 기본 경로에서 시작해서 해당 폴더들을 찾음
    if (FAILED(Find_Texture_Path(L"../Texture/", pObjKey, pStateKey, foundPath)))
        return E_FAIL;

    // 마지막 슬래시까지만 경로 추출
    size_t lastSlash = foundPath.find_last_of(L"/\\");
    wstring folderPath = foundPath.substr(0, lastSlash);

    // 파일 개수 확인
    WIN32_FIND_DATA findData;
    HANDLE hFind = FindFirstFile((folderPath + L"/*.*").c_str(), &findData);

    if (hFind == INVALID_HANDLE_VALUE)
        return E_FAIL;

    int fileCount = 0;
    do {
        if (!(findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
            fileCount++;
        }
    } while (FindNextFile(hFind, &findData));

    FindClose(hFind);
    if (fileCount == 0)
        return E_FAIL;

    auto& innerMap = m_mapTex[pObjKey];
    auto innerIter = innerMap.find(pStateKey);

    if (innerIter == innerMap.end())
    {
        foundPath = regex_replace(foundPath, wregex(L"(.*/.+?)(\\d+)([^/]*$)"), L"$1%d$3");
        CTexture* pTexture = nullptr;
        TEXTYPE eTexture = (fileCount > 1) ? TEX_MULTI : TEX_SINGLE;

        switch (eTexture)
        {
        case TEX_SINGLE:
            pTexture = new CSingleTexture;
            break;
        case TEX_MULTI:
            pTexture = new CMultiTexture;
            break;
        }

        if (eTexture == TEX_SINGLE)
        {
            if (FAILED(pTexture->Insert_Texture(foundPath , pStateKey, 1)))
            {
                Safe_Delete(pTexture);
                return E_FAIL;
            }
            innerMap.insert({ pObjKey, pTexture });
        }
        else
        {
            if (FAILED(pTexture->Insert_Texture(foundPath , pStateKey, fileCount)))
            {
                Safe_Delete(pTexture);
                return E_FAIL;
            }
            innerMap.insert({ pStateKey, pTexture });
        }
    }

    return S_OK;
}

HRESULT CTextureMgr::Find_Texture_Path(const wstring& basePath, const wstring& pObjKey, const wstring& pStateKey, wstring& outPath)
{
    WIN32_FIND_DATA findData;
    wstring searchPath = basePath + L"/*.*";
    HANDLE hFind = FindFirstFile(searchPath.c_str(), &findData);
    if (hFind == INVALID_HANDLE_VALUE)
    {
        return E_FAIL;
    }
    do {
        if (wcscmp(findData.cFileName, L".") == 0 || wcscmp(findData.cFileName, L"..") == 0)
            continue;
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            wstring currentPath = basePath + L"/" + findData.cFileName;
            wstring texturePath = currentPath + L"/" + pObjKey;
            WIN32_FIND_DATA textureData;
            HANDLE hTileFind = FindFirstFile(texturePath.c_str(), &textureData);
            if (hTileFind != INVALID_HANDLE_VALUE)
            {
                FindClose(hTileFind);
                wstring statePath = texturePath + L"/" + pStateKey;
                WIN32_FIND_DATA stateData;
                HANDLE hStateFind = FindFirstFile((statePath + L"/*.*").c_str(), &stateData);
                if (hStateFind != INVALID_HANDLE_VALUE)
                {
                    // 첫 번째 파일을 찾을 때까지 검색
                    do {
                        if (!(stateData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
                        {
                            wstring filePath = statePath + L"/" + stateData.cFileName;
                            outPath = filePath;
                            FindClose(hStateFind);
                            FindClose(hFind);
                            return S_OK;
                        }
                    } while (FindNextFile(hStateFind, &stateData));
                    FindClose(hStateFind);
                }
            }
            else
            {
                if (SUCCEEDED(Find_Texture_Path(currentPath, pObjKey, pStateKey, outPath)))
                {
                    FindClose(hFind);
                    return S_OK;
                }
            }
        }
    } while (FindNextFile(hFind, &findData));
    FindClose(hFind);
    return E_FAIL;
}
