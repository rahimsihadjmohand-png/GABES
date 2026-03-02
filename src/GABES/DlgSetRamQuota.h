#pragma once
#include "afxdialogex.h"
#include "Model.h"


// CDlgSetRamQuota dialog

class CDlgSetRamQuota : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetRamQuota)

public:
	CDlgSetRamQuota(BEM_3D::Model& rModel, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgSetRamQuota();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SET_RAM_QUOTA };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	BEM_3D::Model& m_rModel;
	virtual BOOL OnInitDialog();
	int nTotalRam;

	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	BOOL bAutoQuota;
	int A_Quota;
	CSliderCtrl sldrAQuota;
	afx_msg void OnBnClickedCheckAutoMatrixQuotas();
};
