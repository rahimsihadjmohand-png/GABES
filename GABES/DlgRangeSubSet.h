#pragma once
#include "afxdialogex.h"
#include "Model.h"
#include "ReferenceFrame.h"

// CDlgRangeSubSet dialog

class CDlgRangeSubSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgRangeSubSet)

public:
	CDlgRangeSubSet(BEM_3D::ReferenceFrame& rGlobalFrame,
		std::vector<BEM_3D::ReferenceFrame*>& rRefFrames, 
		std::vector<int>& rSelectedElmsIndices,
		BEM_3D::Model& rModel, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgRangeSubSet();


	void InitModelessDialog();


// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_RANGE_SUBSET };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

	void UpdateSliderBars();
	void UpdateElementSelection();

public: 
	BEM_3D::Model& m_rModel;
    BEM_3D::ReferenceFrame& m_rGlobalFrame;
	std::vector<BEM_3D::ReferenceFrame*>& m_rRefFrames;
	BEM_3D::ReferenceFrame* m_pCurrentFrame;
	std::vector<int>& m_rSelectedElmsIndices;

	
	CComboBox cmbCoordSys;
	int nCoordSys;
	CComboBox cmbRefFrames;
	CStatic lblDim1;
	CStatic lblDim2;
	CStatic lblDim3;
	double Min1;
	double Min2;
	double Min3;
	double Max1;
	double Max2;
	double Max3;

	afx_msg void OnCbnSelchangeCmbCoorSysRangeSubset();
	
	

	

	static double nt;         // tolerance value used for near comparaison _ne_ _ng_ _nl_ _nge_ _nle_
	//static bool _ne_(double Val1, double Val2);  // near equal         ==
	//static bool _ng_(double Val1, double Val2); // near greater        >
	//static bool _nl_(double Val1, double Val2); // near lesser         <
	//static bool _nge_(double Val1, double Val2); // near greater_equal >=
	//static bool _nle_(double Val1, double Val2); // near lesser_equal  <=

public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	
	CSliderCtrl sliderMinDim1;
	CSliderCtrl sliderMaxDim1;
	CSliderCtrl sliderMinDim2;
	CSliderCtrl sliderMaxDim2;
	CSliderCtrl sliderMinDim3;
	CSliderCtrl sliderMaxDim3;
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnCbnSelchangeComboRefFrames();
	virtual void OnCancel();
	afx_msg void OnBnClickedBtnApply();

	CString strName;
};
