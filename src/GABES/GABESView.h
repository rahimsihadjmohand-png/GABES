// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface
// (the "Fluent UI") and is provided only as referential material to supplement the
// Microsoft Foundation Classes Reference and related electronic documentation
// included with the MFC C++ library software.
// License terms to copy, use or distribute the Fluent UI are available separately.
// To learn more about our Fluent UI licensing program, please visit
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// GABESView.h : interface of the CGABESView class
//

#pragma once
#include "PaletteCtrl.h"
#include "DlgReferenceFrame.h"
#include "DlgRangeSubSet.h"
#include "GizmoWnd.h"





enum MOUSE_ACTION
{
	NONE,
	ROTATE_VIEW,
	ELEMENT_SELECTION,
	POINT_SELECTION,
};



class CGABESView : public CView
{
protected: // create from serialization only
	CGABESView() noexcept;
	DECLARE_DYNCREATE(CGABESView)

// Attributes
public:
	CGABESDoc* GetDocument() const;
	BEM_3D::Model* pModel; // a reference to the Model Object Residing in the Document
	CPoint m_GrabPoint;
	float m_Width, m_W2;
	float m_Height, m_H2;
	const float m_fNear, m_fFar;
	IDirect3DDevice9* GetD3dDevice() { return m_pD3ddev; }


//==========Direct3D Variables ===========
private:
	IDirect3DDevice9* m_pD3ddev;
	CGizmoWnd m_GizmoWnd;
	BEM_3D::ReferenceFrame* m_pTempRefFrame; // This is used for visualization while we create a new frame

	float m_CameraDistance;  // The distance from origne to the camera
	float m_Theta;           // The Longitude angle of the camera
	float m_Phi;             // The Latitude angle of the camera
	float m_XRotAng;         // Angle of rotation about X axis
	float m_YRotAng;         // Angle of rotation about Y axis
//=========================================

//============== GUI Variables =================
public:
	BEM_3D::FILL_MODE m_FillMode;
	BEM_3D::SHOW_POINTS_MODE m_ShowPointsMode;

	MOUSE_ACTION m_MouseAction;

	int m_HitTestedElmIdx;
	std::vector<int> m_SelectedElmsIndices;

	BEM_3D::Vertex* m_pHitTestedPoint;
	std::vector<BEM_3D::Vertex*> m_SelectedPoints;
	

	BEM_3D::ElementSubSet* m_pSelectedSubSet;
	bool m_bPostTreatment;
	CPaletteCtrl m_PaletteCtrl; 
	CDlgReferenceFrame m_DlgReferenceFrame;
	CDlgRangeSubSet* m_pDlgRangeSubSet;  // Created as pointer in order to instanciate it in the OnInitialUpdate() Messagehandler
	                                     // Because some constructor arguments are not ready at the View Creation
//=================================================

// Operations
public:
	void ResetSelections(); // Resets all the Selections in case of Greate Modifications (New File, New Geometry ...etc)

private:
	void HitTestElements(float xScreenPos, float yScreenPos);  // Returns the index of the closest element traversed by the generated ray
	void HitTestPoints(float xScreenPos, float yScreenPos);  // Returns the index of the closest Vertex traversed by the generated ray

	
//=============== Direct 3D Methods ==========================
private:
	void InitializeD3D();
	void CleanUpD3D();
	void SetOptions();
	void SetGeometryPipeline();
	void SetLighting();
	void SetMaterial();
	void DrawHitTestedElement();
	void DrawHitTestedPoint();
	void DrawSelectedElements();
	void DrawSelectedPoints();
	void DrawSelectedSubSet();


public:
	void RenderFrameD3D();
	void RecreateD3dComponents();
//============================================================	

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	

public:
	virtual void OnUpdateCmdUI(CFrameWnd* pTarget, BOOL bDisableIfNoHndler);

// Implementation
public:
	virtual ~CGABESView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:

	// Message Handlers
	virtual void OnInitialUpdate();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);


	// Command mesages
	// FILE Tab
	afx_msg void OnFileLoadMeshFromFile();
	afx_msg void OnFileDeleteModel();
	afx_msg void OnUpdateFileDeleteModel(CCmdUI* pCmdUI);
	// MODELING Tab
	afx_msg void OnModelingCreateElementSetFromSelection();
	afx_msg void OnUpdateModelingCreateElementSetFromSelection(CCmdUI* pCmdUI);
	afx_msg void OnModelingImportElementSetFromObjFile();
	afx_msg void OnUpdateModelingImportElementSetFromObjFile(CCmdUI* pCmdUI);
	afx_msg void OnModelingCreateRangeElementSet();
	afx_msg void OnUpdateModelingCreateRangeElementSet(CCmdUI* pCmdUI);
	// BOUNDARY CONDITIONS Tab
	afx_msg void OnBcsFullyEncaster();
	afx_msg void OnBcsPartialEncastrement();
	afx_msg void OnBcsDisplacementVector();
	afx_msg void OnBcsNormalDisplacement();
	afx_msg void OnBcsTractionVector();
	afx_msg void OnBcsNormalTraction();
	afx_msg void OnBcsPressure();
	afx_msg void OnUpdateBcsFullyEncaster(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBcsPartialEncastrement(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBcsDisplacementVector(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBcsNormalDisplacement(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBcsTractionVector(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBcsNormalTraction(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBcsPressure(CCmdUI* pCmdUI);
	afx_msg void OnBcsCheckShowFixations();
	afx_msg void OnBcsCheckShowDisplacementVectors();
	afx_msg void OnBcsCheckShowTractionVectors();
	afx_msg void OnUpdateBcsCheckShowFixations(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBcsCheckShowDisplacementVectors(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBcsCheckShowTractionVectors(CCmdUI* pCmdUI);
	// BEM ANALYSIS Tab
	afx_msg void OnBemSolveBoundaryValueProblem();
	afx_msg void OnBemShowDeformation();
	afx_msg void OnUpdateBemShowDeformation(CCmdUI* pCmdUI);
	afx_msg void OnBemSetMaterialProperties();
	afx_msg void OnBemSetIntegrationParameters();
	afx_msg void OnBemSetDeformationScale();
	afx_msg void OnBemPosttreatmentMode();
	afx_msg void OnUpdateBemPosttreatmentMode(CCmdUI* pCmdUI);
	afx_msg void OnUpdateBemSetDeformationScale(CCmdUI* pCmdUI);
	afx_msg void OnBemPaletteSettings();
	afx_msg void OnBemSetMemoryQuota();
	afx_msg void OnBemGetDomainNodeResponse();
	afx_msg void OnBemGenerateReport();
	// View toolbar
	afx_msg void OnViewFront();
	afx_msg void OnViewRight();
	afx_msg void OnViewTop();
	afx_msg void OnViewBack();
	afx_msg void OnViewLeft();
	afx_msg void OnViewBottom();
	afx_msg void OnViewMouseactionRotate();
	afx_msg void OnViewMouseactionElementSelection();
	afx_msg void OnUpdateViewMouseactionRotate(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewMouseactionElementSelection(CCmdUI* pCmdUI);
	
	afx_msg void OnModelingAddFrame();
	afx_msg void OnViewMouseactionPointSelection();
	afx_msg void OnUpdateViewMouseactionPointSelection(CCmdUI* pCmdUI);
	afx_msg void OnViewIsometric();
	afx_msg void OnBcsTractionFree();
	afx_msg void OnUpdateBcsTractionFree(CCmdUI* pCmdUI);
};

#ifndef _DEBUG  // debug version in GABESView.cpp
inline CGABESDoc* CGABESView::GetDocument() const
   { return reinterpret_cast<CGABESDoc*>(m_pDocument); }
#endif

