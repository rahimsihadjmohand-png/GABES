#pragma once
#include "Model.h"

#define ID_PALETTE_WINDOW        2002 // ID for the palette sub-window of the internal stress dialog



// CPaletteCtrl


class CPaletteCtrl : public CWnd
{
	DECLARE_DYNAMIC(CPaletteCtrl)

public:
	CPaletteCtrl();
	virtual ~CPaletteCtrl();


	void SetModel(BEM_3D::Model* pModel);

	BOOL Create(CWnd* pParent, const CRect& rc);
	void Update();

protected:
	static CString m_strClassName;
	static BOOL m_bClassRegistrated;
	BEM_3D::Model* m_pModel;
	double m_MaxValue;
	double m_MinValue;

public:
	static int m_nNumDecimals; // The number of decimals
	static int m_nFmt; // 0: Floating Point Format , 1: Exponontial Fromat


protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


