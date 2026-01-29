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

// BEM3DDoc.h : interface of the CBEM3DDoc class
//


#pragma once
#include "Model.h"
#include "ReferenceFrame.h"


class CGABESDoc : public CDocument
{
protected: // create from serialization only
	CGABESDoc() noexcept;
	DECLARE_DYNCREATE(CGABESDoc)

// Attributes
public:
	BEM_3D::Model m_Model;
	BEM_3D::ReferenceFrame m_GlobalFrame;
	std::vector<BEM_3D::ReferenceFrame*> m_ReferenceFrames;
	BEM_3D::ReferenceFrame* m_pCurrentFrame;

// Operations
public:
	void UnselectAllRefFrames();
	void SelectRefFrame(BEM_3D::ReferenceFrame* pRefFrame); // Select the reference frame and unselect all the others
	                                                        // Selecting the frame makes it more appearent in the 3D Scene 
	void SelectGlobalFrame();

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CGABESDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
};
