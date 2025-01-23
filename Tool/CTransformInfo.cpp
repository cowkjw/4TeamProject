// CTransformInfo.cpp: 구현 파일
//

#include "pch.h"
#include "Tool.h"
#include "CTransformInfo.h"


// CTransformInfo

IMPLEMENT_DYNCREATE(CTransformInfo, CFormView)

CTransformInfo::CTransformInfo()
	: CFormView(IDD_CTransformInfo)
{

}

CTransformInfo::~CTransformInfo()
{
}

void CTransformInfo::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CTransformInfo, CFormView)
END_MESSAGE_MAP()


// CTransformInfo 진단

#ifdef _DEBUG
void CTransformInfo::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CTransformInfo::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CTransformInfo 메시지 처리기


void CTransformInfo::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
		SetScrollSizes(MM_TEXT, CSize(0, 0));
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
}
