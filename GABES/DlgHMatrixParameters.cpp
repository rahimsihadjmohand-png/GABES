// DlgHMatrixParameters.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "afxdialogex.h"
#include "DlgHMatrixParameters.h"


using BEM_3D::Model;


// CDlgHMatrixParameters dialog

IMPLEMENT_DYNAMIC(CDlgHMatrixParameters, CDialogEx)

CDlgHMatrixParameters::CDlgHMatrixParameters(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_HMATRIX_PARAMETERS, pParent)
{

}

CDlgHMatrixParameters::~CDlgHMatrixParameters()
{
}

void CDlgHMatrixParameters::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_CBIndex(pDX, IDC_COMBO_CLMODE, Model::m_clmode);
	DDX_Text(pDX, IDC_EDIT_ACA_TOL, Model::m_ACA_tol);
	DDX_Text(pDX, IDC_EDIT_ETA, Model::m_Eta);
	DDX_Text(pDX, IDC_EDIT_CLF, Model::m_clf);
}


BEGIN_MESSAGE_MAP(CDlgHMatrixParameters, CDialogEx)
END_MESSAGE_MAP()


// CDlgHMatrixParameters message handlers
