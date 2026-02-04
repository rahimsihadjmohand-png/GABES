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
	, nFixedIntegRule((int)BEM_3D::Element::m_FixedIntegRule)
	, bRigidBody(BEM_3D::Element::m_bRigidBodyCPV)
	, bDirectCPV(!BEM_3D::Element::m_bRigidBodyCPV)
	, bAdaptiveCriterion(BEM_3D::Element::m_bUseAdaptiveCriterion)
	, bFixedCubature(!BEM_3D::Element::m_bUseAdaptiveCriterion)
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
		IDB_RULE_208_PTS,
		IDB_ADAPTIVE_CRITERION
	};

	for (size_t i = 0; i < 12; i++)
	{
		m_Bitmaps[i].LoadBitmap(BmpIDs[i]);
	}
}

CDlgSetIntegrationParameters::~CDlgSetIntegrationParameters()
{
	for (size_t i = 0; i < 12; i++)
	{
		m_Bitmaps[i].DeleteObject();
	}
}

void CDlgSetIntegrationParameters::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_INTEGRATION_RULE, cmbFixedIntegRule);
	DDX_CBIndex(pDX, IDC_COMBO_INTEGRATION_RULE, nFixedIntegRule);
	DDX_Control(pDX, IDC_STATIC_IMAGES, statImage);

	DDX_Check(pDX, IDC_RADIO_RIGID_BODY_TRANSLATION, bRigidBody);
	DDX_Check(pDX, IDC_RADIO_DIRECT_EVALUATION, bDirectCPV);
	DDX_Check(pDX, IDC_RADIO_ADAPTIVE_CRITERION, bAdaptiveCriterion);
	DDX_Check(pDX, IDC_RADIO_FIXED_CUBATURE_RULE, bFixedCubature);
}


BEGIN_MESSAGE_MAP(CDlgSetIntegrationParameters, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_INTEGRATION_RULE, &CDlgSetIntegrationParameters::OnCbnSelchangeComboIntegrationRule)
	ON_BN_CLICKED(IDC_RADIO_ADAPTIVE_CRITERION, &CDlgSetIntegrationParameters::OnBnClickedRadioAdaptiveCriterion)
	ON_BN_CLICKED(IDC_RADIO_FIXED_CUBATURE_RULE, &CDlgSetIntegrationParameters::OnBnClickedRadioFixedCubatureRule)
END_MESSAGE_MAP()


// CDlgSetIntegrationParameters message handlers

void CDlgSetIntegrationParameters::OnCbnSelchangeComboIntegrationRule()
{
	// TODO: Add your control notification handler code here
	statImage.SetBitmap(m_Bitmaps[cmbFixedIntegRule.GetCurSel()]);
}

void CDlgSetIntegrationParameters::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialogEx::OnOK();


	BEM_3D::Element::m_FixedIntegRule = (BEM_3D::INTEG_RULE)nFixedIntegRule;
	BEM_3D::Element::m_bRigidBodyCPV = bRigidBody;
	BEM_3D::Element::m_bUseAdaptiveCriterion = bAdaptiveCriterion;
}

void CDlgSetIntegrationParameters::OnBnClickedRadioAdaptiveCriterion()
{
	// TODO: Add your control notification handler code here
	cmbFixedIntegRule.EnableWindow(FALSE);
	statImage.SetBitmap(m_Bitmaps[11]);
}

void CDlgSetIntegrationParameters::OnBnClickedRadioFixedCubatureRule()
{
	// TODO: Add your control notification handler code here
	cmbFixedIntegRule.EnableWindow(TRUE);
	statImage.SetBitmap(m_Bitmaps[cmbFixedIntegRule.GetCurSel()]);
}

BOOL CDlgSetIntegrationParameters::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	cmbFixedIntegRule.EnableWindow(!bAdaptiveCriterion);

	int nBmp = bAdaptiveCriterion ? 11 : nFixedIntegRule;
	
	statImage.SetBitmap(m_Bitmaps[nBmp]);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
