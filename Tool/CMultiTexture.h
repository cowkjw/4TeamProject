#pragma once

#include "CTexture.h"

class CMultiTexture :    public CTexture
{
public:
	CMultiTexture();
	virtual ~CMultiTexture();

public:
	const TEXINFO* CMultiTexture::Get_Texture(const wstring& pStateKey = L"", const int iCnt = 0);
	
public:
	virtual HRESULT Insert_Texture(const wstring& pFilePath, const wstring& pStateKey = L"", const int iCnt = 0) override;
	virtual void Release() override;

private:
	map<wstring, vector<TEXINFO*>>	m_MapMultiTex;

};

