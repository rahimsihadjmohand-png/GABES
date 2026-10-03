#pragma once
#include "afxdialogex.h"
#include "Model.h"
#include "ReferenceFrame.h"

// CDlgGeneratePointGrid dialog

class CDlgGeneratePointGrid : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgGeneratePointGrid)

public:
	CDlgGeneratePointGrid(std::vector<BEM_3D::Vertex>& rTrackNodes, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgGeneratePointGrid();


	void InitModlessDialog(BEM_3D::ReferenceFrame* pCurrentFrame, int nCoordSys);

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_GENERATE_POINT_GRID };
#endif

protected:
	void UpdateSliderBars();
	void UpdateTrackNodes();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString strCoordFrame;
	CString strCoordSys;
	CString strDim1Range;
	CString strDim2Range;
	CString strDim3Range;
	CString strDim1Nbr;
	CString strDim2Nbr;
	CString strDim3Nbr;
	double Min1;
	double Max1;
	double Min2;
	double Max2;
	double Min3;
	double Max3;
	int N1;
	int N2;
	int N3;
	CSliderCtrl sliderMinDim1;
	CSliderCtrl sliderMaxDim1;
	CSliderCtrl sliderMinDim2;
	CSliderCtrl sliderMaxDim2;
	CSliderCtrl sliderMinDim3;
	CSliderCtrl sliderMaxDim3;
	CSpinButtonCtrl spinN1;
	CSpinButtonCtrl spinN2;
	CSpinButtonCtrl spinN3;

	BEM_3D::ReferenceFrame* m_pCurrentFrame;
	std::vector<BEM_3D::Vertex>& m_rTrackNodes;	
	int m_nCoordSys;
	virtual BOOL OnInitDialog();

	virtual void OnCancel();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnBnClickedButtonApply();
	afx_msg void OnDeltaposSpin1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDeltaposSpin3(NMHDR* pNMHDR, LRESULT* pResult);
};
