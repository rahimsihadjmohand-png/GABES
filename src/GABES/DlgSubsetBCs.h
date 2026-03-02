#pragma once
#include "afxdialogex.h"
#include "ElementSubSet.h"

// CDlgSubsetBCs dialog

class CDlgSubsetBCs : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSubsetBCs)

public:
	CDlgSubsetBCs(BEM_3D::ElementSubSet& rSubset, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgSubsetBCs();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SUBSET_BCS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void UpdateControlsState(); // Enables or Disables Controls

	DECLARE_MESSAGE_MAP()
public:
	BEM_3D::ElementSubSet& m_rSubset;

	CString strSubsetName;

	BOOL bNormalDisp;
	BOOL bNormalTrac;
	BOOL bFixedUx;
	BOOL bFixedUy;
	BOOL bFixedUz;
	
	double Un;
	double Ux;
	double Uy;
	double Uz;

	double Tn;
	double Tx;
	double Ty;
	double Tz;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCheckUn();
	afx_msg void OnBnClickedCheckTn();
	afx_msg void OnBnClickedCheckU1();
	afx_msg void OnBnClickedCheckU2();
	afx_msg void OnBnClickedCheckU3();
	virtual void OnOK();
};
