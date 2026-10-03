#pragma once
#include "afxdialogex.h"
#include "Model.h"

// CDlgSetDeformationScale dialog

class CDlgSetDeformationScale : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetDeformationScale)

public:
	CDlgSetDeformationScale(BEM_3D::Model* _pModel, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgSetDeformationScale();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SET_DEFORMATION_SCALE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton chkAutoCompute;
	CEdit editScale;
	double Scale;
	BEM_3D::Model* pModel;
	afx_msg void OnBnClickedCheckAutoCompute();
	virtual void OnOK();
};
