#pragma once
#include "afxdialogex.h"


// CDlgSetRamQuota dialog

class CDlgSetRamQuota : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetRamQuota)

public:
	CDlgSetRamQuota(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgSetRamQuota();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_SET_RAM_BAIL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	unsigned RAM_Quota;
	virtual BOOL OnInitDialog();
	int nTotalRam;
	CSliderCtrl sldrRamQuota;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	BOOL bAutoQuotas;
	int Q_Quota;
	int R_Quota;
	int A_Quota;
	CSliderCtrl sldrQQuota;
	CSliderCtrl sldrRQuota;
	CSliderCtrl sldrAQuota;
	afx_msg void OnBnClickedCheckAutoMatrixQuotas();
};
