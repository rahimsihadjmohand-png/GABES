#pragma once
#include "afxdialogex.h"
#include "Model.h"
// CDlgWaitLengthyOperation dialog

#define UM_PRELAUNCH_TIMER (WM_USER + 500)
#define UM_LAUNCH_TIMER (WM_USER + 600)

class CDlgWaitLengthyOperation : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgWaitLengthyOperation)

public:
	CDlgWaitLengthyOperation(BEM_3D::Model* _pModel, int _nOperation, CWnd* pParent = nullptr);   // standard constructor
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
	int nOperation;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	virtual void OnCancel();
	CString strTime;
	CButton btnOk;
};
