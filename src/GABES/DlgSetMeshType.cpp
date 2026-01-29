// DlgSetMeshType.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "afxdialogex.h"
#include "DlgSetMeshType.h"
#include "Model.h"

// CDlgSetMeshType dialog

IMPLEMENT_DYNAMIC(CDlgSetMeshType, CDialogEx)

CDlgSetMeshType::CDlgSetMeshType(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_MESH_TYPE, pParent)
	, nMeshType((int)BEM_3D::Model::m_MeshType)
{

}

CDlgSetMeshType::~CDlgSetMeshType()
{
}

void CDlgSetMeshType::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_CBIndex(pDX, IDC_COMBO_MESH_TYPE, nMeshType);
}


BEGIN_MESSAGE_MAP(CDlgSetMeshType, CDialogEx)
END_MESSAGE_MAP()


// CDlgSetMeshType message handlers

void CDlgSetMeshType::OnOK()
{
	// TODO: Add your specialized code here and/or call the base class

	CDialogEx::OnOK();


	BEM_3D::Model::m_MeshType = (BEM_3D::MESH_TYPE)nMeshType;
}
