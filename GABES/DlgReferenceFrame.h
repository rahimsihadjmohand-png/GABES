#pragma once
#include "afxdialogex.h"
#include "GABESDoc.h"


// CDlgReferenceFrame dialog

enum PLANE
{
	XY,
	YZ,
	ZX
};



class CDlgReferenceFrame : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgReferenceFrame)

public:
	CDlgReferenceFrame(std::vector<int>& rSelectedElmsIndices, std::vector<BEM_3D::Vertex*>& rSelectedPoints,	CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgReferenceFrame();

	void UpdateControlsFromCurrentRefFrame();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_REFFRAME };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	DECLARE_MESSAGE_MAP()
public:
	CGABESDoc* pDoc;
	std::vector<BEM_3D::Vertex*>& m_rSelectedPoints;
	std::vector<int>& m_rSelectedElmsIndices;
	BEM_3D::ReferenceFrame* m_pCurrRefFrame;
	BEM_3D::ReferenceFrame m_PrevStateRefFrame; // The previous state of the reference frame

	CString strName;
	PLANE Plane;
		
	double Ox;
	double Oy;
	double Oz;

	double V1x;
	double V1y;
	double V1z;

	double V2x;
	double V2y;
	double V2z;

	afx_msg void OnBnClickedButtonApply();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnCbnSelchangeComboPlane();
	CComboBox CmbPlane;
	CString strVec1;
	CString strVec2;
	afx_msg void OnBnClickedBtnOriginPickSelPoint();
	afx_msg void OnBnClickedBtnOriginPickCenter3pts();
	afx_msg void OnBnClickedBtnVector1PickSelPoint();
	afx_msg void OnBnClickedBtnVector1PickSelectedElementNormal();
	afx_msg void OnBnClickedBtnVector2PickSelPoint();
	afx_msg void OnBnClickedBtnVector2PickSelectedElementNormal();
};
