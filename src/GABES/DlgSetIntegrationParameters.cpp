// DlgSetIntegrationParameters.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "afxdialogex.h"
#include "DlgSetIntegrationParameters.h"
#include "Element.h"


// CDlgSetIntegrationParameters dialog

IMPLEMENT_DYNAMIC(CDlgSetIntegrationParameters, CDialogEx)

CDlgSetIntegrationParameters::CDlgSetIntegrationParameters(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SET_INTEGRATION_PARAMETERS, pParent)
	, nIntegrationRule((int)BEM_3D::Element::m_IntegrationRule)
	, bRigidBody(BEM_3D::Element::m_bRigidBodyCPV)
	, bDirectCPV(!BEM_3D::Element::m_bRigidBodyCPV)
{
	int BmpIDs[] = {
		IDB_RULE_001_PTS,
		IDB_RULE_003_PTS,
		IDB_RULE_004_PTS,
		IDB_RULE_007_PTS,
		IDB_RULE_013_PTS,
		IDB_RULE_028_PTS,
		IDB_RULE_052_PTS,
		IDB_RULE_056_PTS,
		IDB_RULE_104_PTS,
		IDB_RULE_112_PTS,
		IDB_RULE_208_PTS
	};

	for (size_t i = 0; i < 11; i++)
	{
		m_Bitmaps[i].LoadBitmap(BmpIDs[i]);
	}
}

CDlgSetIntegrationParameters::~CDlgSetIntegrationParameters()
{
	for (size_t i = 0; i < 11; i++)
	{
		m_Bitmaps[i].DeleteObject();
	}
}

void CDlgSetIntegrationParameters::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_INTEGRATION_RULE, cmbIntegRule);
	DDX_CBIndex(pDX, IDC_COMBO_INTEGRATION_RULE, nIntegrationRule);
	DDX_Control(pDX, IDC_STATIC_IMAGES, statImage);

	statImage.SetBitmap(m_Bitmaps[cmbIntegRule.GetCurSel()]);
	DDX_Check(pDX, IDC_RADIO_RIGID_BODY_TRANSLATION, bRigidBody);
	DDX_Check(pDX, IDC_RADIO_DIRECT_EVALUATION, bDirectCPV);
}


BEGIN_MESSAGE_MAP(CDlgSetIntegrationParameters, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_INTEGRATION_RULE, &CDlgSetIntegrationParameters::OnCbnSelchangeComboIntegrationRule)
END_MESSAGE_MAP()


// CDlgSetIntegrationParameters message handlers

void CDlgSetIntegrationParameters::OnCbnSelchangeComboIntegrationRule()
{
	// TODO: Add your control notification handler code here
	statImage.SetBitmap(m_Bitmaps[cmbIntegRule.GetCurSel()]);
}

void CDlgSetIntegrationParameters::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialogEx::OnOK();


	BEM_3D::Element::m_IntegrationRule = (BEM_3D::INTEG_RULE)nIntegrationRule;
	BEM_3D::Element::m_bRigidBodyCPV = bRigidBody;
}
