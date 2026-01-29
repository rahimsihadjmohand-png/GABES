#pragma once
#include "afxdialogex.h"


// CDlgSetMaterialProperties dialog

class CDlgSetMaterialProperties : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetMaterialProperties)

public:
	CDlgSetMaterialProperties(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgSetMaterialProperties();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SET_MATERIAL_PROPERTIES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	double GetDoubleNumber(const CEdit& editCtrl);
	void SetDoubleNumber(CEdit& editCtrl, double value);
	bool bAllowModificationFromSetDoubleNumber;

	DECLARE_MESSAGE_MAP()
public:
	CButton radio_E;
	CButton radio_G;
	CEdit edit_E;
	CEdit edit_G;
	BOOL bE;
	BOOL bG;
	double E;
	double G;
	double v;
	afx_msg void OnBnClickedRadioE();
	afx_msg void OnBnClickedRadioG();
	afx_msg void OnBnClickedOk();
	afx_msg void OnEnChangeEditE();
	afx_msg void OnEnChangeEditG();
	afx_msg void OnEnChangeEditV();
	CEdit edit_v;
};
