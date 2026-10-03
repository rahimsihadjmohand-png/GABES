#pragma once
#include "afxdialogex.h"
#include "HMatrixViewer.h"


// CDlgHMatrixViewer dialog

class CDlgHMatrixViewer : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgHMatrixViewer)

public:
	CDlgHMatrixViewer(const BEM_3D::Model* pModel, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgHMatrixViewer();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_HMATRIX_VIEWER };
#endif

private:
	CHMatrixViewer m_HMatViewer;
	int m_cx, m_cy;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_ViewerFrame;
	virtual BOOL OnInitDialog();

	CButton m_OKbtn;
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
