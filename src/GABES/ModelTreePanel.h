#pragma once
#include "GABESDoc.h"
#include "GABESView.h"

// CSubSetsPanel

#define ID_SUBSETS_PANEL 14071990
#define ID_TREE_CTRL     14071991


class CModelTreePanel : public CDockablePane
{
	DECLARE_DYNAMIC(CModelTreePanel)

public:
	CModelTreePanel();
	virtual ~CModelTreePanel();



	void InitTreeCtrl();
	void UpdateTreeCtrl();

public:
	CTreeCtrl m_TreeCtrl;   // The Tree control of the Panel
	CImageList m_ImageList; // This is the Image list of the Tree Control
	CGABESDoc* m_pDoc;
	CGABESView* m_pView;


protected:
	DECLARE_MESSAGE_MAP()

	virtual BOOL CanBeClosed() const override;

private:
	HTREEITEM m_hModelItem;
	HTREEITEM m_hFramesItem;

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	afx_msg void OnSubsetRename();
	afx_msg void OnSubsetDelete();
};


