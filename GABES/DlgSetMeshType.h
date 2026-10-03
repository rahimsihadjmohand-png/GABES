#pragma once
#include "afxdialogex.h"


// CDlgSetMeshType dialog

class CDlgSetMeshType : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgSetMeshType)

public:
	CDlgSetMeshType(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDlgSetMeshType();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_MESH_TYPE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	virtual void OnOK();
public:
	int nMeshType;
};
