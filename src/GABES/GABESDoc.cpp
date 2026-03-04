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

// GABESDoc.cpp : implementation of the CGABESDoc class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GABES.h"
#endif

#include "GABESDoc.h"

#include <propkey.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "MainFrm.h"

// CGABESDoc

IMPLEMENT_DYNCREATE(CGABESDoc, CDocument)

BEGIN_MESSAGE_MAP(CGABESDoc, CDocument)
END_MESSAGE_MAP()


// CGABESDoc construction/destruction

CGABESDoc::CGABESDoc() noexcept
	: m_GlobalFrame(_T("Global Frame"))
	//, m_pCurrentFrame(nullptr)
{
	// TODO: add one-time construction code here

}

CGABESDoc::~CGABESDoc()
{
	for (BEM_3D::ReferenceFrame* pRefFrame : m_ReferenceFrames)
	{
		if (pRefFrame)
			delete pRefFrame;
	}

	m_ReferenceFrames.clear();
}

BOOL CGABESDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	m_Model.ClearAll();

	m_Model.SetWorkingDirectory(_T(""));
	m_Model.SetFileName(_T(""));
	m_Model.SetFileType(_T(""));

	// Reset the selections
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CGABESView* pView = (CGABESView*)pMainFrame->GetActiveView();

	pView->ResetSelections();


	// Update the ModelTree  and Info Panels
	pMainFrame->m_ModelTreePanel.UpdateTreeCtrl();
	pMainFrame->m_infoPanel.UpdateInfo();

	return TRUE;
}




void CGABESDoc::UnselectAllRefFrames()
{
	m_GlobalFrame.m_bSelected = false;

	for (BEM_3D::ReferenceFrame* pRefFrame : m_ReferenceFrames)
		pRefFrame->m_bSelected = false;
}

void CGABESDoc::SelectRefFrame(BEM_3D::ReferenceFrame* pRefFrame)
{
	m_GlobalFrame.m_bSelected = false;

	for (BEM_3D::ReferenceFrame* pRefFrame : m_ReferenceFrames)
		pRefFrame->m_bSelected = false;

	if(pRefFrame)
		pRefFrame->m_bSelected = true;
}


void CGABESDoc::SelectRefFrame(int nIdx)
{
	if(nIdx >= 0 && nIdx < m_ReferenceFrames.size())
		SelectRefFrame(m_ReferenceFrames[nIdx]);
}


void CGABESDoc::SelectGlobalFrame()
{
	m_GlobalFrame.m_bSelected = true;

	for (BEM_3D::ReferenceFrame* pRefFrame : m_ReferenceFrames)
		pRefFrame->m_bSelected = false;
}



void CGABESDoc::SetCoordinateRanges()
{
	m_GlobalFrame.SetCoordinateRanges(m_Model);

	for (BEM_3D::ReferenceFrame* pRefFrame : m_ReferenceFrames)
		pRefFrame->SetCoordinateRanges(m_Model);
}




// CGABESDoc serialization

void CGABESDoc::Serialize(CArchive& ar)
{
	m_Model.Serialize(ar);

	if (ar.IsStoring())
	{
		// Store the size of the RefFrames Container
		ar << m_ReferenceFrames.size();
		
		// Store the reference frames
		for (BEM_3D::ReferenceFrame* pRefFrame : m_ReferenceFrames)
			pRefFrame->Serialize(ar);
		
	}
	else
	{
		// Load the size of the RefFrames Container
		size_t Nrf = 0;
		ar >> Nrf;

		// Load the reference frames
		for (size_t i = 0; i < Nrf; i++)
		{
			BEM_3D::ReferenceFrame* pRefFrame = new BEM_3D::ReferenceFrame();
			pRefFrame->Serialize(ar);

			m_ReferenceFrames.push_back(pRefFrame);
		}	
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CGABESDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CGABESDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data.
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CGABESDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = nullptr;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != nullptr)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CGABESDoc diagnostics

#ifdef _DEBUG
void CGABESDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CGABESDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CGABESDoc commands

BOOL CGABESDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
	m_Model.ClearAll();
	
	for (BEM_3D::ReferenceFrame* pRefFrame : m_ReferenceFrames)
	{
		if (pRefFrame)
			delete pRefFrame;
	}

	m_ReferenceFrames.clear();


	if (!CDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// Set the Coordinate ranges for all the frames
	SetCoordinateRanges();

	// Get the main frame and the view
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	CGABESView* pView = (CGABESView*)pMainFrame->GetActiveView();

	// Load the Vertex Node mesh which uses the Specific length
	BEM_3D::Vertex::LoadMesh(pView->GetD3dDevice(), BEM_3D::ElementSubSet::SpecLength);


	// ======================= Update the D3D buffer of the model ===============
	m_Model.UpdateVertexBuffer(pView->GetD3dDevice(), pView->m_bPostProcessing);

	// Reset the selections		
	pView->ResetSelections();



	// Update the ModelTree  and Info Panels
	pMainFrame->m_ModelTreePanel.UpdateTreeCtrl();
	pMainFrame->m_infoPanel.UpdateInfo();

	return TRUE;

}
