#pragma once
#include "afxdialogex.h"
#include "Model.h"


// CDlgHMatrixParameters dialog

class CDlgHMatrixParameters : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHMatrixParameters)

public:
	CDlgHMatrixParameters(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgHMatrixParameters();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_HMATRIX_PARAMETERS };
#endif


public:
	const BEM_3D::Model* m_pModel;



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
