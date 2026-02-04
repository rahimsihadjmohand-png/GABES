#pragma once
#include "afxdialogex.h"


// CDlgSetIntegrationParameters dialog

class CDlgSetIntegrationParameters : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetIntegrationParameters)

public:
	CDlgSetIntegrationParameters(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgSetIntegrationParameters();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SET_INTEGRATION_PARAMETERS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CComboBox cmbFixedIntegRule;
	int nFixedIntegRule;
	CStatic statImage;
	CBitmap m_Bitmaps[12];
	int m_BmpIDs[12];
	afx_msg void OnCbnSelchangeComboIntegrationRule();
	virtual void OnOK();
	BOOL bRigidBody;
	BOOL bDirectCPV;
	BOOL bAdaptiveCriterion;
	BOOL bFixedCubature;

	afx_msg void OnBnClickedRadioAdaptiveCriterion();
	afx_msg void OnBnClickedRadioFixedCubatureRule();
	virtual BOOL OnInitDialog();
};
