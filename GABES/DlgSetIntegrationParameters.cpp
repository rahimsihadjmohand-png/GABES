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
	, nCriterionPrec((int)BEM_3D::Element::m_CriterionPrec)
	, nFixedIntegRule((int)BEM_3D::Element::m_FixedIntegRule)
	, bAdaptiveCriterion(BEM_3D::Element::m_bUseAdaptiveCriterion)
	, bFixedCubature(!BEM_3D::Element::m_bUseAdaptiveCriterion)
	, bRISP(BEM_3D::Element::m_bRISP)	
{
	

	
}

CDlgSetIntegrationParameters::~CDlgSetIntegrationParameters()
{
}

void CDlgSetIntegrationParameters::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_INTEGRATION_RULE, cmbFixedIntegRule);
	DDX_CBIndex(pDX, IDC_COMBO_INTEGRATION_RULE, nFixedIntegRule);
	DDX_Check(pDX, IDC_RADIO_ADAPTIVE_CRITERION, bAdaptiveCriterion);
	DDX_Check(pDX, IDC_RADIO_FIXED_CUBATURE_RULE, bFixedCubature);
	DDX_Check(pDX, IDC_CHECK_FURID, bRISP);
	DDX_Control(pDX, IDC_COMBO_CRITERION_PRECISION, cmbCriterionPrec);
	DDX_CBIndex(pDX, IDC_COMBO_CRITERION_PRECISION, nCriterionPrec);
}


BEGIN_MESSAGE_MAP(CDlgSetIntegrationParameters, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_ADAPTIVE_CRITERION, &CDlgSetIntegrationParameters::OnBnClickedRadioAdaptiveCriterion)
	ON_BN_CLICKED(IDC_RADIO_FIXED_CUBATURE_RULE, &CDlgSetIntegrationParameters::OnBnClickedRadioFixedCubatureRule)
END_MESSAGE_MAP()


// CDlgSetIntegrationParameters message handlers



void CDlgSetIntegrationParameters::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialogEx::OnOK();

	BEM_3D::Element::m_CriterionPrec = (BEM_3D::CRITERION_PREC)nCriterionPrec;
	BEM_3D::Element::m_FixedIntegRule = (BEM_3D::INTEG_RULE)nFixedIntegRule;
	BEM_3D::Element::m_bUseAdaptiveCriterion = bAdaptiveCriterion;
	BEM_3D::Element::m_bRISP = bRISP;
}

void CDlgSetIntegrationParameters::OnBnClickedRadioAdaptiveCriterion()
{
	// TODO: Add your control notification handler code here
	cmbCriterionPrec.EnableWindow(TRUE);
	cmbFixedIntegRule.EnableWindow(FALSE);
}

void CDlgSetIntegrationParameters::OnBnClickedRadioFixedCubatureRule()
{
	// TODO: Add your control notification handler code here
	cmbFixedIntegRule.EnableWindow(TRUE);
	cmbCriterionPrec.EnableWindow(FALSE);
}

BOOL CDlgSetIntegrationParameters::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	cmbCriterionPrec.EnableWindow(bAdaptiveCriterion);
	cmbFixedIntegRule.EnableWindow(!bAdaptiveCriterion);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
