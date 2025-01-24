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
		return nullptr;
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
