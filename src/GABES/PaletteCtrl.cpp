// PaletteCtrl.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "PaletteCtrl.h"


// CPaletteCtrl

int CPaletteCtrl::m_nFmt = 0;  // Floating point format
int CPaletteCtrl::m_nNumDecimals = 6; // six deciamls 
CString CPaletteCtrl::m_strClassName = _T("_CPaletteCtrl_");
BOOL CPaletteCtrl::m_bClassRegistrated = FALSE;


IMPLEMENT_DYNAMIC(CPaletteCtrl, CWnd)

CPaletteCtrl::CPaletteCtrl()
	: m_pModel(nullptr)
	, m_MinValue(0.0)
	, m_MaxValue(0.0)
{
	if (!m_bClassRegistrated)
	{
		WNDCLASS wc = { 0 };
		wc.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wc.hInstance = ::AfxGetInstanceHandle();
		wc.lpfnWndProc = ::AfxGetAfxWndProc();
		wc.lpszClassName = m_strClassName;
		wc.style = CS_HREDRAW | CS_VREDRAW;

		RegisterClass(&wc);
		m_bClassRegistrated = TRUE;
	}
}

CPaletteCtrl::~CPaletteCtrl()
{
}


BEGIN_MESSAGE_MAP(CPaletteCtrl, CWnd)
	ON_WM_PAINT()
END_MESSAGE_MAP()



BOOL CPaletteCtrl::Create(CWnd* pParent, const CRect& rc)
{
	BOOL bReturn = CWnd::Create(m_strClassName, NULL,
		WS_CHILD | WS_BORDER,
		rc, pParent, ID_PALETTE_WINDOW);



	return bReturn;
}

void CPaletteCtrl::SetModel(BEM_3D::Model* pModel)
{
	m_pModel = pModel;
	Update();
}


void CPaletteCtrl::Update()
{
	if (m_pModel == nullptr)
		return;

	m_MinValue = m_pModel->GetMinValue();
	m_MaxValue = m_pModel->GetMaxValue();

	Invalidate(FALSE);
}

// CPaletteCtrl message handlers



void CPaletteCtrl::OnPaint()
{
	if (!m_pModel)
		return;

	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CWnd::OnPaint() for painting messages

	

	// Get Client Rectangle and name it rcPalette
	CRect rcPalette;
	GetClientRect(&rcPalette);


	// 2 Create a back buffer Bitmap
	CBitmap OffscreenBmp;
	OffscreenBmp.CreateCompatibleBitmap(&dc, rcPalette.Width(), rcPalette.Height());
	// 3 Create a memory device context
	CDC memDc;
	memDc.CreateCompatibleDC(&dc);
	memDc.SelectObject(&OffscreenBmp);
	// Make the Bitmap White
	CBrush WhiteBrush(RGB(255, 255, 255));
	memDc.FillRect(&rcPalette, &WhiteBrush);


	memDc.Rectangle(rcPalette);

	// Draw the Units Text
	CString strOutputMode[] =
	{
		_T("Displacement U1"),
		_T("Displacement U2"),
		_T("Displacement U3"),
		_T("Displacement Magnitude"),
		_T("Sig_11 Stress"),
		_T("Sig_22 Stress"),
		_T("Sig_33 Stress"),
		_T("Sig_12 Stress"),
		_T("Sig_23 Stress"),
		_T("Sig_13 Stress"),
		_T("Von-Mises Stress"),
		_T("Eps_11 Strain"),
		_T("Eps_22 Strain"),
		_T("Eps_33 Strain"),
		_T("Eps_12 Strain"),
		_T("Eps_23 Strain"),
		_T("Eps_13 Strain"),
		_T("Von-Mises Strain"),
	};
	
	int nMode = -1;
	BEM_3D::OUTPUT_MODE outputMode = m_pModel->GetOutputMode();

	switch (outputMode)
	{
	case BEM_3D::NONE:
		nMode = -1;
		break;

	case BEM_3D::U_1:
		nMode = 0;
		break;

	case BEM_3D::U_2:
		nMode = 1;
		break;

	case BEM_3D::U_3:
		nMode = 2;
		break;

	case BEM_3D::U_M:
		nMode = 3;
		break;

	case BEM_3D::S_11:
		nMode = 4;
		break;

	case BEM_3D::S_22:
		nMode = 5;
		break;

	case BEM_3D::S_33:
		nMode = 6;
		break;

	case BEM_3D::S_12:
		nMode = 7;
		break;

	case BEM_3D::S_23:
		nMode = 8;
		break;

	case BEM_3D::S_13:
		nMode = 9;
		break;

	case BEM_3D::S_VON_MISES:
		nMode = 10;
		break;

	case BEM_3D::E_11:
		nMode = 11;
		break;

	case BEM_3D::E_22:
		nMode = 12;
		break;

	case BEM_3D::E_33:
		nMode = 13;
		break;

	case BEM_3D::E_12:
		nMode = 14;
		break;

	case BEM_3D::E_23:
		nMode = 15;
		break;

	case BEM_3D::E_13:
		nMode = 16;
		break;

	case BEM_3D::E_VON_MISES:
		nMode = 17;
		break;

	default:
		break;
	}


	

	CString strLegend_Line1 = nMode >= 0 ? strOutputMode[nMode] : _T('\0');
	
	TEXTMETRIC tm;

	CFont font;
	font.CreateFont(24, 1, 0, 0, FW_BOLD, 0, 0, 0, 0, 0, 0, 0, 0, _T("MS Sans Serif"));
	CFont* pPrevFont = memDc.SelectObject(&font);

	memDc.GetTextMetrics(&tm);

	int h = tm.tmHeight;

	memDc.TextOut(5, 5, strLegend_Line1);
	
	memDc.SelectObject(pPrevFont);
	font.DeleteObject();
	// Fill the Palette
	CPoint Pt_Tl(10, 50);
	CSize Sz(rcPalette.Width() / 5, rcPalette.Height() - 70);
	int nStep = 0;
	for (int y = Pt_Tl.y + Sz.cy; y >= Pt_Tl.y; y--, nStep++)
	{
		// Calculate the ration
		double Ratio = (double)(nStep) / (double)Sz.cy;
		// Calculate the Color From the Ratio
		COLORREF Color = 0;
		if (Ratio < (1.0 / 3.0))
		{
			int i = (int)(255.0 * Ratio * 3.0);
			Color = RGB(0, i, 255 - i);
		}
		else if (Ratio >= (1.0 / 3.0) && Ratio < (2.0 / 3.0))
		{
			double _Ratio = Ratio - (1.0 / 3.0);
			int i = (int)(255.0 * _Ratio * 3.0);
			Color = RGB(i, 255, 0);
		}
		else if (Ratio >= (2.0 / 3.0))
		{
			double _Ratio = Ratio - (2.0 / 3.0);
			int i = (int)(255.0 * _Ratio * 3);
			Color = RGB(255, 255 - i, 0);
		}

		// Create a Solid Colored Pen
		CPen colorPen(PS_SOLID, 1, Color);
		// Select the pen and store the previous One
		CPen* pPrevPen = memDc.SelectObject(&colorPen);
		// Draw a line
		memDc.MoveTo(Pt_Tl.x, y);
		memDc.LineTo(Pt_Tl.x + Sz.cx, y);
		// Restore the Old Pen
		memDc.SelectObject(pPrevPen);
		// Delete the Colored pen
		colorPen.DeleteObject();
	}


	for (int i = 0; i < 11; i++)
	{
		int y = Pt_Tl.y + (10 - i) * Sz.cy / 10;
		CString strValue;
		
		double Offset = (double)i / 10.0 * fabs(m_MaxValue - m_MinValue);

		LPWSTR szFmt = _T("%d");

		switch (m_nFmt)
		{
		case 0:
		{
			switch (m_nNumDecimals)
			{
			case 0:
				szFmt = _T("%.0f");
				break;

			case 1:
				szFmt = _T("%.1f");
				break;

			case 2:
				szFmt = _T("%.2f");
				break;

			case 3:
				szFmt = _T("%.3f");
				break;

			case 4:
				szFmt = _T("%.4f");
				break;

			case 5:
				szFmt = _T("%.5f");
				break;

			case 6:
				szFmt = _T("%.6f");
				break;

			case 7:
				szFmt = _T("%.7f");
				break;

			case 8:
				szFmt = _T("%.8f");
				break;

			case 9:
				szFmt = _T("%.9f");
				break;

			case 10:
				szFmt = _T("%.10f");
				break;

			default:
				break;
			}
		}
		break;

		case 1:
		{
			switch (m_nNumDecimals)
			{
			case 0:
				szFmt = _T("%.0e");
				break;

			case 1:
				szFmt = _T("%.1e");
				break;

			case 2:
				szFmt = _T("%.2e");
				break;

			case 3:
				szFmt = _T("%.3e");
				break;

			case 4:
				szFmt = _T("%.4e");
				break;

			case 5:
				szFmt = _T("%.5e");
				break;

			case 6:
				szFmt = _T("%.6e");
				break;

			case 7:
				szFmt = _T("%.7e");
				break;

			case 8:
				szFmt = _T("%.8e");
				break;

			case 9:
				szFmt = _T("%.9e");
				break;

			case 10:
				szFmt = _T("%.10e");
				break;

			default:
				break;
			}
		}
		break;

		default:
			break;
		}


		strValue.Format(szFmt, m_MinValue + Offset);

		CString str;
		str.Format(_T(" %s"), strValue);



		font.CreateFont(20, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, _T("MS Sans Serif"));
		pPrevFont = memDc.SelectObject(&font);


		memDc.TextOut(Pt_Tl.x + Sz.cx + 5, y - h / 2, str);

		memDc.SelectObject(pPrevFont);
		font.DeleteObject();
	}

	// 8 Do the Bit-Bloc transfer
	memDc.DPtoLP(&rcPalette);
	dc.BitBlt(rcPalette.left, rcPalette.top, rcPalette.Width(), rcPalette.Height(), &memDc, rcPalette.left, rcPalette.top, SRCCOPY);
	// 9 Delete the Bitmap and the memory device context
	OffscreenBmp.DeleteObject();
	memDc.DeleteDC();
}




