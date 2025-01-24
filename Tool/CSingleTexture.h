#pragma once

#include "CTexture.h"

class CSingleTexture :    public CTexture
{
public:
	CSingleTexture();
	virtual ~CSingleTexture();

public:
	const TEXINFO* CSingleTexture::Get_Texture(const wstring& pStateKey = L"", const int iCnt =0)
	{
		return m_pTexInfo;
	}

public:
	HRESULT Insert_Texture(const wstring& pFilePath, const wstring& pStateKey = L"", const int iCnt = 0) override;
	void Release() override;

private:
	TEXINFO*	m_pTexInfo;


	// CTexture을(를) 통해 상속됨
	const TEXINFO* Get_Texture2(const wstring& pStateKey, const wstring& FloderKey, const int iCnt) override;

	HRESULT Insert_Texture2(const wstring& pFilePath, const wstring& pStateKey, const wstring& FloderKey, const int iCnt) override;

};

