#pragma once
#include "afxdialogex.h"
#include "Model.h"


// CDlgGenerateDomainResponseReport dialog

class CDlgGenerateDomainResponseReport : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgGenerateDomainResponseReport)

public:
	CDlgGenerateDomainResponseReport(BEM_3D::Model* pModel, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgGenerateDomainResponseReport();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_GENERATE_DOMAIN_RESPONSE_REPORT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton RadioInputFile;
	CButton RadioGenerateGrid;
	BOOL    bUseInputFile;
	CEdit   EditInputFile;
	CButton BtnInputFile;
	CButton BtnGridGeneration;
	CButton CheckGridGenerated;
	CString strInputFile;
	CString strOutputFile;
	BEM_3D::Model* m_pModel;
	std::vector<BEM_3D::Vertex> TrackNodes;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioInputFile();
	afx_msg void OnBnClickedRadioGenerateGrid();
	afx_msg void OnBnClickedButtonInputFile();
	afx_msg void OnBnClickedButtonOutputFile();
	virtual void OnOK();

	void GetTrackPointsFromInputFile();

	BOOL bU1;
	BOOL bU2;
	BOOL bU3;
	BOOL bU;
	BOOL bSig_11;
	BOOL bSig_22;
	BOOL bSig_33;
	BOOL bSig_12;
	BOOL bSig_23;
	BOOL bSig_31;
	BOOL bSig_VM;
	BOOL bEps_11;
	BOOL bEps_22;
	BOOL bEps_33;
	BOOL bEps_12;
	BOOL bEps_23;
	BOOL bEps_31;
};
