// DlgWaitLengthyOperation.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "afxdialogex.h"
#include "DlgWaitLengthyOperation.h"

#define TIME_ELAPSE_MS 10

// CDlgWaitLengthyOperation dialog

IMPLEMENT_DYNAMIC(CDlgWaitLengthyOperation, CDialogEx)

CDlgWaitLengthyOperation::CDlgWaitLengthyOperation(BEM_3D::Model* _pModel, int _nOperation, CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DLG_WAIT_LENGTHY_OPERATION, pParent)
	, pModel(_pModel)
	, nOperation(_nOperation)
	, strTime(_T(""))
{

}

CDlgWaitLengthyOperation::~CDlgWaitLengthyOperation()
{
}

void CDlgWaitLengthyOperation::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS_ADVANCE, PrgsAdvance);
	DDX_Text(pDX, IDC_STATIC_TIME, strTime);
	DDX_Control(pDX, IDOK, btnOk);
}


BEGIN_MESSAGE_MAP(CDlgWaitLengthyOperation, CDialogEx)
	ON_WM_TIMER()
END_MESSAGE_MAP()


void CDlgWaitLengthyOperation::FormatTime()
{
	// Get the Actual ticks
	LARGE_INTEGER ticksCurr;
	QueryPerformanceCounter(&ticksCurr);

	// Get time in seconds
	double totalSec = (double)(ticksCurr.QuadPart - ticksStart.QuadPart) / (double) ticksFreq.QuadPart;
	
	int hrs = (int)(totalSec / 3600.0);
	int min = (int)((totalSec - hrs * 3600.0) / 60.0);
	double sec = totalSec - hrs * 3600.0 - min * 60.0;
	int secInt = (int)sec;
	int frac = (int)((sec - secInt) * 100.0);  // 2 digits after comma

	strTime.Format(_T("%02u : %02u : %02d,%02d"), hrs, min, secInt, frac);
}

// CDlgWaitLengthyOperation message handlers

void CDlgWaitLengthyOperation::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default

	double Advance = pModel->m_CurrentAdvance * 100.0;

	PrgsAdvance.SetPos((int)(Advance));
	CDialogEx::OnTimer(nIDEvent);

	CString strAdvance;
	strAdvance.Format(_T("%.2f %%"), Advance);

	SetDlgItemText(IDC_STATIC_ADVANCE, strAdvance);
	SetDlgItemText(IDC_STATIC_CURRENT_JOB, pModel->m_strCurrentJob);


	if (pModel->m_bLengthyJob)
	{

		FormatTime();
		SetDlgItemText(IDC_STATIC_TIME, strTime);
	}
	else
	{
		if (nOperation == 0)
			EndDialog(0);
		else
		{
			PrgsAdvance.ModifyStyle(PBS_MARQUEE, 0);
			PrgsAdvance.SetMarquee(false, 100);

			btnOk.EnableWindow(TRUE);
		}		
	}
}



BOOL CDlgWaitLengthyOperation::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	
	switch (nOperation)
	{
	case 0:
		SetWindowText(_T("Calaculating Coefficient Matrices"));
		/*PrgsAdvance.ModifyStyle(PBS_MARQUEE, 0);
		PrgsAdvance.SetMarquee(false, 100);*/
		pModel->ConstructCoefficientMatrices();
		break;

	case 1:
		SetWindowText(_T("Solving Linear System"));
		/*PrgsAdvance.ModifyStyle(0, PBS_MARQUEE);
		PrgsAdvance.SetMarquee(true, 100);*/
		pModel->ConstructAndResolveLinearSystem();
		break;

	default:
		break;
	}
	
	QueryPerformanceCounter(&ticksStart);
	QueryPerformanceFrequency(&ticksFreq);

	btnOk.EnableWindow(FALSE);
	
	SetTimer(0, TIME_ELAPSE_MS, NULL);


	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CDlgWaitLengthyOperation::OnCancel()
{
	// TODO: Add your specialized code here and/or call the base class
	pModel->m_bLengthyJob = false;

	CDialogEx::OnCancel();
}
