#pragma once
#include "afxdialogex.h"
#include "GABESDoc.h"

// CDlgDomainNodeResponse dialog

class CDlgDomainNodeResponse : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgDomainNodeResponse)

public:
	CDlgDomainNodeResponse(CGABESDoc* pDoc, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgDomainNodeResponse();

	void UpdateTrackNode();
	void InitModelessDialog();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_DOMAIN_NODE_RESPONSE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CGABESDoc* m_pDoc;
	BEM_3D::Model& m_rModel;
	BEM_3D::ReferenceFrame* m_pCurrFrame;
	BEM_3D::Vertex* m_pTrackNode;

	CString strDim1;
	CString strDim2;
	CString strDim3;

	double Dim1;
	double Dim2;
	double Dim3;

	double U1;
	double U2;
	double U3;
	double U;
	
	double Sig_11;
	double Sig_22;
	double Sig_33;
	double Sig_12;
	double Sig_23;
	double Sig_31;
	double Sig_I;
	double Sig_II;
	double Sig_III;
	double Sig_Mises;
	double Sig_Tresca;
	
	double Eps_11;
	double Eps_22;
	double Eps_33;
	double Eps_12;
	double Eps_23;
	double Eps_31;
	double Eps_I;
	double Eps_II;
	double Eps_III;
	

	afx_msg void OnBnClickedCalculate();
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
	BOOL bSig_I;
	BOOL bSig_II;
	BOOL bSig_III;
	BOOL bSig_Tresca;
	BOOL bSig_Mises;

	BOOL bEps_11;
	BOOL bEps_22;
	BOOL bEps_33;
	BOOL bEps_12;
	BOOL bEps_23;
	BOOL bEps_31;
	BOOL bEps_I;
	BOOL bEps_II;
	BOOL bEps_III;

	CButton CheckU1;
	CButton CheckU2;
	CButton CheckU3;
	CButton CheckU;

	CButton CheckSig_11;
	CButton CheckSig_22;
	CButton CheckSig_33;
	CButton CheckSig_12;
	CButton CheckSig_23;
	CButton CheckSig_31;
	CButton CheckSig_I;
	CButton CheckSig_II;
	CButton CheckSig_III;
	CButton CheckSig_Mises;
	CButton CheckSig_Tresca;

	CButton CheckEps_11;
	CButton CheckEps_22;
	CButton CheckEps_33;
	CButton CheckEps_12;
	CButton CheckEps_23;
	CButton CheckEps_31;
	CButton CheckEps_I;
	CButton CheckEps_II;
	CButton CheckEps_III;

	CEdit EditU1;
	CEdit EditU2;
	CEdit EditU3;
	CEdit EditU;

	CEdit EditSig_11;
	CEdit EditSig_22;
	CEdit EditSig_33;
	CEdit EditSig_12;
	CEdit EditSig_23;
	CEdit EditSig_31;
	CEdit EditSig_I;
	CEdit EditSig_II;
	CEdit EditSig_III;
	CEdit EditSig_Mises;
	CEdit EditSig_Tresca;

	CEdit EditEps_11;
	CEdit EditEps_22;
	CEdit EditEps_33;
	CEdit EditEps_12;
	CEdit EditEps_23;
	CEdit EditEps_31;
	CEdit EditEps_I;
	CEdit EditEps_II;
	CEdit EditEps_III;

	virtual BOOL OnInitDialog();
	
	afx_msg void OnBnClickedCheckU1();
	afx_msg void OnBnClickedCheckU2();
	afx_msg void OnBnClickedCheckU3();
	afx_msg void OnBnClickedCheckU();

	afx_msg void OnBnClickedCheckSig11();
	afx_msg void OnBnClickedCheckSig22();
	afx_msg void OnBnClickedCheckSig33();
	afx_msg void OnBnClickedCheckSig12();
	afx_msg void OnBnClickedCheckSig23();
	afx_msg void OnBnClickedCheckSig31();
	afx_msg void OnBnClickedCheckSigI();
	afx_msg void OnBnClickedCheckSigII();
	afx_msg void OnBnClickedCheckSigIII();
	afx_msg void OnBnClickedCheckSigMises();
	afx_msg void OnBnClickedCheckSigTresca();

	
	afx_msg void OnBnClickedCheckEps11();
	afx_msg void OnBnClickedCheckEps22();
	afx_msg void OnBnClickedCheckEps33();
	afx_msg void OnBnClickedCheckEps12();
	afx_msg void OnBnClickedCheckEps23();
	afx_msg void OnBnClickedCheckEps31();
	afx_msg void OnBnClickedCheckEpsI();
	afx_msg void OnBnClickedCheckEpsII();
	afx_msg void OnBnClickedCheckEpsIII();

	int nFrame;
	int nCoordSys;
	CComboBox cmbRefFrames;
	CComboBox cmbCoordSys;
	afx_msg void OnCbnSelchangeComboRefFrame();
	afx_msg void OnCbnSelchangeComboCoordSys();
	virtual void OnCancel();
	afx_msg void OnBnClickedButtonApply();	
};
