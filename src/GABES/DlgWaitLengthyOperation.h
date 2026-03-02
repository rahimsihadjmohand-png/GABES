#pragma once
#include "afxdialogex.h"
#include "Model.h"
// CDlgWaitLengthyOperation dialog



class CDlgWaitLengthyOperation : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgWaitLengthyOperation)

public:
	CDlgWaitLengthyOperation(BEM_3D::Model* _pModel, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgWaitLengthyOperation();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_WAIT_LENGTHY_OPERATION };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void FormatTime();
	LARGE_INTEGER ticksStart, ticksFreq;

	DECLARE_MESSAGE_MAP()
public:
	CProgressCtrl PrgsAdvance;
	BEM_3D::Model* pModel;
	bool m_bConstructionStage;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	CString strTime;
	CButton btnOk;
};
