#pragma once
#include "Model.h"
#include "Solver_H2LIB.h"


using BEM_3D::Model;
using BEM_3D::Vertex;

// HMatrixViewer

class CHMatrixViewer : public CWnd
{
	DECLARE_DYNAMIC(CHMatrixViewer)

public:
	CHMatrixViewer(const BEM_3D::Model* pModel);
	virtual ~CHMatrixViewer();


	void Update();
	void CreateWnd(CWnd* pParent);



protected:
	DECLARE_MESSAGE_MAP()
	const Model* m_pModel;
	pcluster m_pRowCl;
	pcluster m_pColCl;
	pblock m_pRootBlock;
	unsigned m_cx;
	unsigned m_cy;

public:
	void CleanUp();

	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


