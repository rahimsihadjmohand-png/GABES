// SubSetsPanel.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "ModelTreePanel.h"
#include "GABESView.h"


// CModelTreePanel

IMPLEMENT_DYNAMIC(CModelTreePanel, CDockablePane)

CModelTreePanel::CModelTreePanel()
	: m_hFramesItem(nullptr)
	, m_hModelItem(nullptr)
{

}

CModelTreePanel::~CModelTreePanel()
{
}


BEGIN_MESSAGE_MAP(CModelTreePanel, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_SUBSET_RENAME, &CModelTreePanel::OnSubsetRename)
	ON_COMMAND(ID_SUBSET_DELETE, &CModelTreePanel::OnSubsetDelete)
END_MESSAGE_MAP()



// CModelTreePanel message handlers



int CModelTreePanel::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;


	// TODO:  Add your specialized creation code here
	m_TreeCtrl.Create(WS_CHILD | WS_BORDER | WS_VISIBLE | TVS_EDITLABELS |
		TVS_HASLINES | TVS_LINESATROOT | TVS_HASBUTTONS,
		CRect(0, 0, 0, 0), this, ID_TREE_CTRL);


	return 0;
}

void CModelTreePanel::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	// Resize the Tree Control to occupy the Panel client area
	m_TreeCtrl.MoveWindow(0, 0, cx, cy);
}



void CModelTreePanel::InitTreeCtrl()
{
	// Create the Image List and fill it with icons
	m_ImageList.Create(32, 32, ILC_COLOR32, 4, 0);
	
	HICON hIcon = LoadIcon(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_REFFRAMES));
	m_ImageList.Add(hIcon);
	hIcon = LoadIcon(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_REFFRAME));
	m_ImageList.Add(hIcon);
	hIcon = LoadIcon(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_MODEL));
	m_ImageList.Add(hIcon);
	hIcon = LoadIcon(::AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_SUBSET));
	m_ImageList.Add(hIcon);

	// Set the Image list to the TreeCtrl
	m_TreeCtrl.SetImageList(&m_ImageList, TVSIL_NORMAL);

	// Update the Tree Control
	UpdateTreeCtrl();
}



void CModelTreePanel::UpdateTreeCtrl()
{
	// Empty the Tree Control
	m_TreeCtrl.DeleteAllItems();

	// Verify the Document Pointer
	if (m_pDoc == nullptr)
		return;

	
	//================================== The Coordinate Frames ========================================
	// Create the Frames Root Item
	CString strText = _T("Reference frames");
	TV_ITEM tvItem;
	tvItem.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE;
	tvItem.pszText = (LPWSTR)strText.GetString();
	tvItem.cchTextMax = strText.GetLength();
	tvItem.iImage = 0;
	tvItem.iSelectedImage = 0;
	tvItem.lParam = (LPARAM)nullptr;

	// Insert the Root Item
	TVINSERTSTRUCT tvInsert;
	tvInsert.hParent = TVI_ROOT;
	tvInsert.hInsertAfter = TVI_LAST;
	tvInsert.item = tvItem;
	m_hFramesItem = m_TreeCtrl.InsertItem(&tvInsert);


	// Create the Global Frame TvItem
	strText = m_pDoc->m_GlobalFrame.m_strName;
	tvItem.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE;
	tvItem.pszText = (LPWSTR)strText.GetString();
	tvItem.cchTextMax = strText.GetLength();
	tvItem.iImage = 1;
	tvItem.iSelectedImage = 1;
	tvItem.lParam = (LPARAM)(& m_pDoc->m_GlobalFrame);  // The pointer to the global frame

	// Insert the Global Frame TvItem
	tvInsert.hParent = m_hFramesItem;
	tvInsert.hInsertAfter = TVI_LAST;
	tvInsert.item = tvItem;
	HTREEITEM hGlobalFrameItem = m_TreeCtrl.InsertItem(&tvInsert);


	// Create and insert the User frames TvItems
	std::vector <BEM_3D::ReferenceFrame*>& rRefFrames = m_pDoc->m_ReferenceFrames;

	for (BEM_3D::ReferenceFrame* pRefFrame : rRefFrames)
	{
		strText = pRefFrame->m_strName;
		tvItem.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE;
		tvItem.pszText = (LPWSTR)strText.GetString();
		tvItem.cchTextMax = strText.GetLength();
		tvItem.iImage = 1;
		tvItem.iSelectedImage = 1;
		tvItem.lParam = (LPARAM)pRefFrame; // A pointer to the frame
		tvInsert.hParent = m_hFramesItem;
		tvInsert.hInsertAfter = TVI_LAST;
		tvInsert.item = tvItem;
		
		m_TreeCtrl.InsertItem(&tvInsert);
	}

	m_TreeCtrl.Expand(m_hFramesItem, TVE_EXPAND);

	//===================================================================================================


	// Verify if the Model is Empty
	if (m_pDoc->m_Model.IsModelEmpty())
		return;


	//===================================== The Model Subsets ==============================================

	CString strMeshName = m_pDoc->m_Model.GetFileName();
    // Remove the extension from the file title
	// Look for the last dot in the filename
	int dotPos = strMeshName.ReverseFind('.');

	if (dotPos != -1)
	{
		CString ext = strMeshName.Mid(dotPos);

		if (ext.CompareNoCase(_T(".obj")) == 0)
		{
			strMeshName = strMeshName.Left(dotPos);
		}

		else if (ext.CompareNoCase(_T(".stl")) == 0)
		{
			strMeshName = strMeshName.Left(dotPos);
		}
	}


	// Create the Model Item
	tvItem.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE;
	tvItem.pszText = (LPWSTR)strMeshName.GetString();
	tvItem.cchTextMax = strMeshName.GetLength();
	tvItem.iImage = 2;
	tvItem.iSelectedImage = 2;
	tvItem.lParam = (LPARAM)nullptr;

	// Insert the Root Item
	tvInsert.hParent = TVI_ROOT;
	tvInsert.hInsertAfter = TVI_LAST;
	tvInsert.item = tvItem;
	m_hModelItem = m_TreeCtrl.InsertItem(&tvInsert);

	

	// Get a reference to the Body Object 
	BEM_3D::Model& rModel = m_pDoc->m_Model;

	// Get a reference to the subsets Container
	std::list<BEM_3D::ElementSubSet*>& rSubSets = rModel.GetSubSets();


	// Loop through the SubSets Container
	for (auto iter = rSubSets.begin(); iter != rSubSets.end(); iter++)
	{
		BEM_3D::ElementSubSet* pSubSet = *iter;
		// Create the Child item on the Tree corresponding to this Shape
		TVITEM tvItem;
		tvItem.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_PARAM | TVIF_SELECTEDIMAGE;

		TVINSERTSTRUCT tvInsert;

		tvItem.iImage = 3;
		tvItem.iSelectedImage = 3;
		tvItem.pszText = pSubSet->m_strName.GetBuffer();
		tvItem.cchTextMax = pSubSet->m_strName.GetLength();
		tvItem.lParam = (LPARAM)pSubSet;
		tvInsert.hParent = m_hModelItem;
		tvInsert.hInsertAfter = TVI_LAST;
		tvInsert.item = tvItem;
		m_TreeCtrl.InsertItem(&tvInsert);
	}

	// Expand the Tree Control
	m_TreeCtrl.Expand(m_hModelItem, TVE_EXPAND);

}

BOOL CModelTreePanel::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	// TODO: Add your specialized code here and/or call the base class
	LPNMHDR pNmhdr = (LPNMHDR)lParam;

	if (pNmhdr->hwndFrom == m_TreeCtrl.GetSafeHwnd())
	{
		// Verify the notification code
		switch (pNmhdr->code)
		{
		case TVN_ENDLABELEDIT:      // Renaming
		{
			// Transform the LPARAM Parameter to an LPNMTVDISPINFO structure
			LPNMTVDISPINFO lpNmtvdi = (LPNMTVDISPINFO)pNmhdr;

			// Get the Shape adress
			HTREEITEM hItem = lpNmtvdi->item.hItem;

			if ((hItem != NULL) && (hItem != TVI_ROOT) && (hItem != m_hFramesItem) && (hItem != m_hModelItem))
			{
				HTREEITEM hParent = m_TreeCtrl.GetParentItem(hItem);

				if (hParent == m_hFramesItem)
				{
					BEM_3D::ReferenceFrame* pRefFrame = (BEM_3D::ReferenceFrame*)(lpNmtvdi->item.lParam);
					// Set the New name

					if ((pRefFrame != nullptr) && (pRefFrame != &m_pDoc->m_GlobalFrame))
					{
						m_TreeCtrl.SetItemText(lpNmtvdi->item.hItem, lpNmtvdi->item.pszText);
						pRefFrame->m_strName = lpNmtvdi->item.pszText;
					}
				}
				else if (hParent == m_hModelItem)
				{
					BEM_3D::ElementSubSet* pSubSet = (BEM_3D::ElementSubSet*)(lpNmtvdi->item.lParam);
					// Set the New name

					if ((pSubSet != nullptr))
					{
						m_TreeCtrl.SetItemText(lpNmtvdi->item.hItem, lpNmtvdi->item.pszText);
						pSubSet->m_strName = lpNmtvdi->item.pszText;
					}
				}
			}			
		}
		break;


		case TVN_SELCHANGED:
		{
			m_pView->ResetSelections();

			if (m_pDoc->m_Model.GetSubSets().empty())
				return CDockablePane::OnNotify(wParam, lParam, pResult);

			// Transform the lParam to a NMTREEVIEW* structure
			NMTREEVIEW* lpNmtv = (NMTREEVIEW*)lParam;

			// Get the selected item
			HTREEITEM hSelected = lpNmtv->itemNew.hItem;

		    if ((hSelected != NULL) && (hSelected != TVI_ROOT) && (hSelected != m_hFramesItem) && (hSelected != m_hModelItem))
			{
				HTREEITEM hParent = m_TreeCtrl.GetParentItem(hSelected);

				if (hParent == m_hModelItem)
				{
					BEM_3D::ElementSubSet* pSubSet = (BEM_3D::ElementSubSet*)lpNmtv->itemNew.lParam;
					if (pSubSet)
						m_pView->m_pSelectedSubSet = pSubSet;
				}				
			}				
		}
		break;

		case NM_RCLICK:
		{
			CPoint ptScreen;
			GetCursorPos(&ptScreen);

			// Step 2: Convert to client coordinates for hit testing
			CPoint ptClient = ptScreen;
			m_TreeCtrl.ScreenToClient(&ptClient);

			// Step 3: Hit test the item under the cursor
			UINT uFlags;
			HTREEITEM hItem = m_TreeCtrl.HitTest(ptClient, &uFlags);

			if ((hItem != NULL) && (hItem != TVI_ROOT) && (hItem != m_hFramesItem) && (hItem != m_hModelItem))
			{
				// Optional: Select the item under right-click
				m_TreeCtrl.SelectItem(hItem);

				// Step 4: Load your popup menu from resource
				CMenu menu;
				if (menu.LoadMenu(IDR_SUBSETS_MENU)) // IDR_SUBSETS_MENU is a menu resource
				{
					CMenu* pPopup = menu.GetSubMenu(0); // first submenu

					// Step 5: Show the menu
					if (pPopup)
					{
						pPopup->TrackPopupMenu(
							TPM_RIGHTBUTTON | TPM_LEFTALIGN,
							ptScreen.x,
							ptScreen.y,
							this); // the owner window
					}
				}
			}

			*pResult = 0;
		}
		break;


		case NM_DBLCLK:
		{
			DWORD dwPos = GetMessagePos();
			CPoint pt(GET_X_LPARAM(dwPos), GET_Y_LPARAM(dwPos));
			m_TreeCtrl.ScreenToClient(&pt);

			UINT uFlags = 0;
			HTREEITEM hItem = m_TreeCtrl.HitTest(pt, &uFlags);


			if ((hItem != NULL) && (hItem != TVI_ROOT) && (hItem != m_hFramesItem) && (hItem != m_hModelItem) && (uFlags & TVHT_ONITEM))
			{
				// Optional: Select the item under right-click
				m_TreeCtrl.SelectItem(hItem);

				// Get the parent Item
				HTREEITEM hParent = m_TreeCtrl.GetParentItem(hItem);

				if (hParent == m_hFramesItem)
				{
					

					BEM_3D::ReferenceFrame* pRefFrame = (BEM_3D::ReferenceFrame*)m_TreeCtrl.GetItemData(hItem);

					// Invoke the Modify frame Dialog
					if ((pRefFrame != nullptr) && (pRefFrame != &m_pDoc->m_GlobalFrame))
					{

						pRefFrame->m_bSelected = true;
						m_pView->m_DlgReferenceFrame.m_pCurrRefFrame = pRefFrame;
						m_pView->m_DlgReferenceFrame.UpdateControlsFromCurrentRefFrame();
						m_pView->m_DlgReferenceFrame.ShowWindow(SW_SHOWNORMAL);
					}
				}
			}

			*pResult = 0;
		}
		break;

		default:
			break;
		}
	}

	return CDockablePane::OnNotify(wParam, lParam, pResult);
}

void CModelTreePanel::OnSubsetRename()
{
	// TODO: Add your command handler code here
	// Get Selected Item 
	HTREEITEM hItem = m_TreeCtrl.GetSelectedItem();

	if (hItem != NULL && hItem != TVI_ROOT)
	{
		m_TreeCtrl.EditLabel(hItem);
	}

}

void CModelTreePanel::OnSubsetDelete()
{
	// TODO: Add your command handler code here
	// Get Selected Item 
	HTREEITEM hItem = m_TreeCtrl.GetSelectedItem();

	if ((hItem != NULL) && (hItem != TVI_ROOT) && (hItem != m_hFramesItem) && (hItem != m_hModelItem))
	{
		// Get the parent Item
		HTREEITEM hParentItem = m_TreeCtrl.GetParentItem(hItem);

		if (hParentItem == m_hFramesItem)
		{
			BEM_3D::ReferenceFrame* pRefFrame = (BEM_3D::ReferenceFrame*)m_TreeCtrl.GetItemData(hItem);

			if ((pRefFrame != nullptr) && (pRefFrame != &m_pDoc->m_GlobalFrame))
			{
				if (m_pView->m_DlgReferenceFrame.m_pCurrRefFrame == pRefFrame)
				{
					m_pView->m_DlgReferenceFrame.m_pCurrRefFrame = nullptr;
					m_pView->m_DlgReferenceFrame.ShowWindow(SW_HIDE);
				}


				std::vector<BEM_3D::ReferenceFrame*>& rRefFrames = m_pDoc->m_ReferenceFrames;
				
				auto iter = std::find(rRefFrames.begin(), rRefFrames.end(), pRefFrame);
				rRefFrames.erase(iter);
				delete pRefFrame;
				m_TreeCtrl.DeleteItem(hItem);				
			}			
		}
		else if (hParentItem == m_hModelItem)
		{
			BEM_3D::ElementSubSet* pSubSet = (BEM_3D::ElementSubSet*)m_TreeCtrl.GetItemData(hItem);
			std::list<BEM_3D::ElementSubSet*>& rSubSets = m_pDoc->m_Model.GetSubSets();

			if (pSubSet)
			{
				auto iter = std::find(rSubSets.begin(), rSubSets.end(), pSubSet);
				rSubSets.erase(iter);
				delete pSubSet;
				m_TreeCtrl.DeleteItem(hItem);
				m_pView->ResetSelections();
			}
		}	
		
	}
}




BOOL CModelTreePanel::CanBeClosed() const
{
	return FALSE;   // 🔒 prevents closing (docked AND floating)
}