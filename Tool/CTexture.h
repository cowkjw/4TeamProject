#pragma once

#include "Include.h"

class CTexture
{
public:
	CTexture();
	virtual ~CTexture();

public:
	virtual const TEXINFO* Get_Texture(const wstring& pStateKey = L"",const int iCnt = 0)PURE;
	virtual const TEXINFO* Get_Texture2(const wstring& pStateKey = L"", const wstring& FloderKey = L"",const int iCnt = 0)PURE;

public:

	virtual HRESULT		Insert_Texture(const wstring& pFilePath,const wstring& pStateKey = L"",const int iCnt = 0)PURE;
	virtual HRESULT		Insert_Texture2(const wstring& pFilePath,const wstring& pStateKey = L"",
		const wstring& FloderKey = L"",const int iCnt = 0)PURE;

	virtual void		Release()PURE;

};

