#pragma once
#include "afxdialogex.h"
#include "GABESDoc.h"
#include "DlgGeneratePointGrid.h"



// CDlgGenerateDomainResponseReport dialog

class CDlgGenerateDomainResponseReport : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgGenerateDomainResponseReport)

public:
	CDlgGenerateDomainResponseReport(CGABESDoc* pDoc, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgGenerateDomainResponseReport();

	// This function is called before showing the dialog 
	// It does nearly the same Job as the constructor in case of a modal dialog box
	void InitModlessDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_GENERATE_DOMAIN_RESPONSE_REPORT };
#endif

protected:
	CDlgGeneratePointGrid m_DlgGenPtsGrid;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CButton RadioInputFile;
	CButton RadioGenerateGrid;
	BOOL    bUseInputFile;
	CEdit   EditInputFile;
	CButton BtnInputFile;
	CButton BtnGridGeneration;
	CString strInputFile;
	CString strOutputFile;
	CGABESDoc* m_pDoc;
	BEM_3D::Model& m_rModel;
	BEM_3D::ReferenceFrame* m_pCurrFrame;
	std::vector<BEM_3D::Vertex> m_TrackNodes;

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedRadioInputFile();
	afx_msg void OnBnClickedRadioGenerateGrid();
	afx_msg void OnBnClickedButtonInputFile();
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
	int nFrame;
	int nCoordSys;
	CComboBox cmbRefFrames;
	CComboBox cmbCoordSys;
	afx_msg void OnCbnSelchangeComboRefFrames();
	virtual void OnCancel();
	afx_msg void OnBnClickedButtonOpenGridGenerationDialog();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
