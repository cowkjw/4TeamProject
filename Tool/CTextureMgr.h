#pragma once

#include "CSingleTexture.h"
#include "CMultiTexture.h"

class CTextureMgr
{
	DECLARE_SINGLETON(CTextureMgr)

private:
	CTextureMgr();
	~CTextureMgr();

public:
	const TEXINFO* Get_Texture(const wstring& pObjKey,
		const wstring& pStateKey = L"",
		const int iCnt = 0);

public:
	HRESULT		Insert_Texture(const wstring& pFilePath,
		TEXTYPE eTexture,
		const wstring& pObjKey,
		const wstring& pStateKey = L"",
		const int iCnt = 0);

	void		Release();
	HRESULT		Insert_Texture_Directory(const wstring& pObjKey, const wstring& pStateKey);
	HRESULT Find_Texture_Path(const wstring& basePath, const wstring& pObjKey, const wstring& pStateKey, wstring& outPath);

private:
	map<wstring, map<wstring, CTexture*>> m_mapTex;

};

