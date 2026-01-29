// DlgSetDeformationScale.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "afxdialogex.h"
#include "DlgSetDeformationScale.h"


// CDlgSetDeformationScale dialog

IMPLEMENT_DYNAMIC(CDlgSetDeformationScale, CDialogEx)

CDlgSetDeformationScale::CDlgSetDeformationScale(BEM_3D::Model* _pModel, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_SET_DEFORMATION_SCALE, pParent)
	, pModel(_pModel)
	, Scale(BEM_3D::Vertex::m_DeformationScale)
{

}

CDlgSetDeformationScale::~CDlgSetDeformationScale()
{
}

void CDlgSetDeformationScale::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHECK_AUTO_COMPUTE, chkAutoCompute);
	DDX_Control(pDX, IDC_EDIT_SCALE, editScale);
	DDX_Text(pDX, IDC_EDIT_SCALE, Scale);

	editScale.SetReadOnly(chkAutoCompute.GetCheck() == BST_CHECKED);
}


BEGIN_MESSAGE_MAP(CDlgSetDeformationScale, CDialogEx)
	ON_BN_CLICKED(IDC_CHECK_AUTO_COMPUTE, &CDlgSetDeformationScale::OnBnClickedCheckAutoCompute)
END_MESSAGE_MAP()


// CDlgSetDeformationScale message handlers

void CDlgSetDeformationScale::OnBnClickedCheckAutoCompute()
{
	// TODO: Add your control notification handler code here
	editScale.SetReadOnly(chkAutoCompute.GetCheck() == BST_CHECKED);

	if (chkAutoCompute.GetCheck() != BST_CHECKED)
		return;

	// Loop through the vertices and Calculate the Highest Displacement
	double U = 0.0;
	for (BEM_3D::Vertex* pV : pModel->GetVertices())
	{
		if (U < pV->U.Magnitude())
			U = pV->U.Magnitude();
	}

	if (U == 0.0)
		return;

	Scale = 10.0 / U;

	UpdateData(false);
}

void CDlgSetDeformationScale::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialogEx::OnOK();


	BEM_3D::Vertex::m_DeformationScale = Scale;
}
