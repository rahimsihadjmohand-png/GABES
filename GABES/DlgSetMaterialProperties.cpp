// DlgSetMaterialProperties.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "afxdialogex.h"
#include "DlgSetMaterialProperties.h"
#include "Element.h"


// CDlgSetMaterialProperties dialog

IMPLEMENT_DYNAMIC(CDlgSetMaterialProperties, CDialogEx)

CDlgSetMaterialProperties::CDlgSetMaterialProperties(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SET_MATERIAL_PROPERTIES, pParent)
	, bE(TRUE)
	, bG(FALSE)
	, E(BEM_3D::Element::E)
	, G(BEM_3D::Element::G)
	, v(BEM_3D::Element::v)
	, bAllowModificationFromSetDoubleNumber(true)
{

}

CDlgSetMaterialProperties::~CDlgSetMaterialProperties()
{
}

void CDlgSetMaterialProperties::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_E, radio_E);
	DDX_Control(pDX, IDC_RADIO_G, radio_G);
	DDX_Control(pDX, IDC_EDIT_E, edit_E);
	DDX_Control(pDX, IDC_EDIT_G, edit_G);
	DDX_Check(pDX, IDC_RADIO_E, bE);
	DDX_Check(pDX, IDC_RADIO_G, bG);
	DDX_Text(pDX, IDC_EDIT_E, E);
	DDX_Text(pDX, IDC_EDIT_G, G);
	DDX_Text(pDX, IDC_EDIT_V, v);

	edit_E.SetReadOnly(radio_E.GetCheck() != BST_CHECKED);
	edit_G.SetReadOnly(radio_G.GetCheck() != BST_CHECKED);
	DDX_Control(pDX, IDC_EDIT_V, edit_v);
}


BEGIN_MESSAGE_MAP(CDlgSetMaterialProperties, CDialogEx)
	ON_BN_CLICKED(IDC_RADIO_E, &CDlgSetMaterialProperties::OnBnClickedRadioE)
	ON_BN_CLICKED(IDC_RADIO_G, &CDlgSetMaterialProperties::OnBnClickedRadioG)
	ON_BN_CLICKED(IDOK, &CDlgSetMaterialProperties::OnBnClickedOk)
	ON_EN_CHANGE(IDC_EDIT_E, &CDlgSetMaterialProperties::OnEnChangeEditE)
	ON_EN_CHANGE(IDC_EDIT_G, &CDlgSetMaterialProperties::OnEnChangeEditG)
	ON_EN_CHANGE(IDC_EDIT_V, &CDlgSetMaterialProperties::OnEnChangeEditV)
END_MESSAGE_MAP()


// CDlgSetMaterialProperties message handlers

void CDlgSetMaterialProperties::OnBnClickedRadioE()
{
	// TODO: Add your control notification handler code here
	edit_E.SetReadOnly(FALSE);
	edit_G.SetReadOnly(TRUE);

	bE = TRUE;
	bG = FALSE;

	UpdateData(TRUE);

	G = E / (2.0 * (1.0 + v));


	UpdateData(FALSE);

}

void CDlgSetMaterialProperties::OnBnClickedRadioG()
{
	// TODO: Add your control notification handler code here
	edit_E.SetReadOnly(TRUE);
	edit_G.SetReadOnly(FALSE);

	bE = FALSE;
	bG = TRUE;

	UpdateData(TRUE);

	E = 2.0 * (1.0 + v) * G;

	UpdateData(FALSE);
}

void CDlgSetMaterialProperties::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	CDialogEx::OnOK();

	if (bE)
	{
		BEM_3D::Element::E = E;
		BEM_3D::Element::v = v;
		BEM_3D::Element::G = E / (2.0 * (1.0 + v));
	}
	else
	{
		BEM_3D::Element::E = 2.0 * (1.0 + v) * G;
		BEM_3D::Element::v = v;
		BEM_3D::Element::G = G;
	}
}

void CDlgSetMaterialProperties::OnEnChangeEditE()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if (!bAllowModificationFromSetDoubleNumber)
		return;

	E = GetDoubleNumber(edit_E);
	v = GetDoubleNumber(edit_v);

	G = E / (2.0 * (1.0 + v));
	SetDoubleNumber(edit_G, G);
}

void CDlgSetMaterialProperties::OnEnChangeEditG()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if (!bAllowModificationFromSetDoubleNumber)
		return;

	G = GetDoubleNumber(edit_G);
	v = GetDoubleNumber(edit_v);

	E = 2.0 * (1.0 + v) * G;
	bAllowModificationFromSetDoubleNumber = true;
	SetDoubleNumber(edit_E, E);

	
}

void CDlgSetMaterialProperties::OnEnChangeEditV()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialogEx::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	if (!bAllowModificationFromSetDoubleNumber)
		return;
	
	if (bE)
	{
		E = GetDoubleNumber(edit_E);
		v = GetDoubleNumber(edit_v);
		G = E / (2.0 * (1.0 + v));
		SetDoubleNumber(edit_G, G);

	}
	else if (bG)
	{
		G = GetDoubleNumber(edit_G);
		v = GetDoubleNumber(edit_v);
		E = 2.0 * (1.0 + v) * G;
		SetDoubleNumber(edit_E, E);
	}
}


double CDlgSetMaterialProperties::GetDoubleNumber(const CEdit& editCtrl)
{
	CString strTxt;
	editCtrl.GetWindowText(strTxt);

	return _tstof(strTxt);
}


void CDlgSetMaterialProperties::SetDoubleNumber(CEdit& editCtrl, double value)
{
	CString strTxt;
	strTxt.Format(_T("%f"), value);

	bAllowModificationFromSetDoubleNumber = false;
	editCtrl.SetWindowText(strTxt);
	bAllowModificationFromSetDoubleNumber = true;
}