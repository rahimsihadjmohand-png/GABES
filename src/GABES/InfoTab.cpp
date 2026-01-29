// InfoTab.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "InfoTab.h"


// CInfoTab

IMPLEMENT_DYNAMIC(CInfoTab, CTabCtrl)

CInfoTab::CInfoTab()
{

}

CInfoTab::~CInfoTab()
{
}


BEGIN_MESSAGE_MAP(CInfoTab, CTabCtrl)
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CInfoTab message handlers


void CInfoTab::InitTabCtrl()
{
	// Add the Mesh Tab
	InsertItem(0, _T("Mesh"));

	// Add the Model Tab
	InsertItem(1, _T("Model"));

	// Add the Material Tab
    InsertItem(2, _T("Material"));

	// Add the Analysis Tab
	InsertItem(3, _T("Analysis")); 

	// Add the Memory Tab
	InsertItem(4, _T("Memory"));

	// Update the information
	UpdateInfo();
}


void CInfoTab::MemoryToString(CString& strMem, double Mem)
{
	double Tb = pow(1024.0, 4.0);
	double Gb = pow(1024.0, 3.0);
	double Mb = pow(1024.0, 2.0);
	double Kb = 1024.0;

	if (Mem >= Tb)
		strMem.Format(_T("%.2f Tb"), Mem / Tb);
	else if (Mem >= Gb)
		strMem.Format(_T("%.2f Gb"), Mem / Gb);
	else if (Mem >= Mb)
		strMem.Format(_T("%.2f Mb"), Mem / Mb);
	else if (Mem >= Kb)
		strMem.Format(_T("%.2f Kb"), Mem / Kb);
	else 
		strMem.Format(_T("%d byte"), Mem);
}


void CInfoTab::RAM_OOC_ToString(CString& strMem, double RAM, double OOC)
{
	double Tb = pow(1024.0, 4.0);
	double Gb = pow(1024.0, 3.0);
	double Mb = pow(1024.0, 2.0);
	double Kb = 1024.0;

	// For RAM
	CString strRam;
	if (RAM >= Tb)
		strRam.Format(_T("%.2f Tb"), RAM / Tb);
	else if (RAM >= Gb)
		strRam.Format(_T("%.2f Gb"), RAM / Gb);
	else if (RAM >= Mb)
		strRam.Format(_T("%.2f Mb"), RAM / Mb);
	else if (RAM >= Kb)
		strRam.Format(_T("%.2f Kb"), RAM / Kb);
	else
		strRam.Format(_T("%d byte"), RAM);

	// For OOC
	CString strOoc;
	if (OOC >= Tb)
		strOoc.Format(_T("%.2f Tb"), OOC / Tb);
	else if (OOC >= Gb)
		strOoc.Format(_T("%.2f Gb"), OOC / Gb);
	else if (OOC >= Mb)
		strOoc.Format(_T("%.2f Mb"), OOC / Mb);
	else if (OOC >= Kb)
		strOoc.Format(_T("%.2f Kb"), OOC / Kb);
	else
		strOoc.Format(_T("%d byte"), OOC);

	// Resultante
	strMem = strRam + _T(" / ") + strOoc;
}


void CInfoTab::UpdateInfo()
{
	// Get a reference to the Model object
	const BEM_3D::Model& rModel = m_pDoc->m_Model;
	// Get a reference to the Vertex, Element and Vertex DOF container
	const std::vector<BEM_3D::Vertex*>& rVertices = rModel.GetVertices();
	const std::vector<BEM_3D::Vertex*>& rDOFVertices = rModel.GetDOFVertices();
	const std::vector<BEM_3D::Element*>& rElements = rModel.GetElements();


	// 1. Update mesh information
	
	m_strWorkingDirectory = rModel.GetWorkingDirectory();  
    m_strMeshFileTitle = rModel.GetFileName();     
	m_strFileType = rModel.GetFileType();          
	
	m_Ne = rElements.size();                
	m_Nv = rVertices.size();                 
	//=========================================================================================================

	// 2. Update Model information
	
	switch (rModel.m_MeshType)
	{
	case BEM_3D::PURE_CONTINUOUS:
		m_strMeshType = _T("Continuous");
		break;

	case BEM_3D::PURE_DISCONTINUOUS:
		m_strMeshType = _T("Discontinuous");
		break;

	case BEM_3D::DISCONTINUOUS_SHARP_EDGES:
		m_strMeshType = _T("Disc. Sharp Edges");
		break;
	}
	
	m_Nv_dof = rDOFVertices.size();                
	
	m_Nce = rModel.GetNumContElms();                 
	m_Nde = rModel.GetNumDiscElms();	

	m_N = m_Nv_dof * 3;
	m_M = m_Ne * 9;                   
	//=======================================================================================
	//3. Material information
	m_E = BEM_3D::Element::E;                     
	m_G = BEM_3D::Element::G;
	m_v = BEM_3D::Element::v;
	//=======================================================================================

	//4. Analysis information
	switch (BEM_3D::Element::m_IntegrationRule)
	{
	case BEM_3D::_001_PT:
		m_strIntegRule.Format(_T("%d Point"), 1);
		break;

	case BEM_3D::_003_PT:
		m_strIntegRule.Format(_T("%d Points"), 3);
		break;

	case BEM_3D::_004_PT:
		m_strIntegRule.Format(_T("%d Point"), 4);
		break;

	case BEM_3D::_007_PT:
		m_strIntegRule.Format(_T("%d Points"), 7);
		break;

	case BEM_3D::_013_PT:
		m_strIntegRule.Format(_T("%d Points"), 13);
		break;

	case BEM_3D::_028_PT:
		m_strIntegRule.Format(_T("%d Points"), 28);
		break;

	case BEM_3D::_052_PT:
		m_strIntegRule.Format(_T("%d Points"), 52);
		break;

	case BEM_3D::_056_PT:
		m_strIntegRule.Format(_T("%d Points"), 56);
		break;

	case BEM_3D::_104_PT:
		m_strIntegRule.Format(_T("%d Points"), 104);
		break;

	case BEM_3D::_112_PT:
		m_strIntegRule.Format(_T("%d Points"), 112);
		break;

	case BEM_3D::_208_PT:
		m_strIntegRule.Format(_T("%d Points"), 208);
		break;
	}

	m_str_Q_Matrix.Format(_T("[%d x %d]"), m_N, m_M);
	m_str_RA_Matrix.Format(_T("[%d x %d]"), m_N, m_N);
	m_str_bx_Vector.Format(_T("[%d]"), m_N);
	// ===================================================================================
	
	// 5. Memory information
	// Compute Memory requirements
	// Model Memory
	double Elems_Mem = (double)(m_Ne * sizeof(BEM_3D::Element));
	double Vertex_Mem = (double)(m_Nv * sizeof(BEM_3D::Vertex));
	double V_Dof_Mem = (double)(m_Nv_dof * sizeof(BEM_3D::Vertex));
	double Model_Mem = Elems_Mem + Vertex_Mem + V_Dof_Mem;

	// Mat-Vec Memory
	double Q_Mem = (double)(m_N * m_M * sizeof(double));
	double RA_Mem = (double)(m_N * m_N * sizeof(double));
	double bx_Mem = (double)(m_N * sizeof(double));

	// Total Memory
	double Total_Mem = Model_Mem + Q_Mem + 2.0 * (RA_Mem + bx_Mem);

	// Remainig Matrix Memory
	double Ram_Quota = (double)rModel.m_RAM_Quota_Gb * pow(1024.0, 3.0);
	double Rem_Mat_Mem = max(0.0, Ram_Quota - Model_Mem - 2.0 * bx_Mem);
	double Req_Mat_Mem = Q_Mem + 2.0 * RA_Mem;

	// Calculate the Quotas for matrices Q, R and A
	double Gb = 1024.0 * 1024.0 * 1024.0;
	double Q_Quota = (double)rModel.GetQ_Quota() * Gb;
	double R_Quota = (double)rModel.GetR_Quota() * Gb;
	double A_Quota = (double)rModel.GetA_Quota() * Gb;

	double Q_Ram = min(Q_Quota, Q_Mem);
	double R_Ram = min(R_Quota, RA_Mem);
	double A_Ram = min(A_Quota, RA_Mem);

	double Q_Ooc = max(Q_Mem - Q_Ram, 0);
	double R_Ooc = max(RA_Mem - R_Ram, 0);
	double A_Ooc = max(RA_Mem - A_Ram, 0);

	// Set the memory strings 
	MemoryToString(m_str_Total_Req, Total_Mem);
	MemoryToString(m_str_Model_Req, Model_Mem);
	MemoryToString(m_str_Q_Req, Q_Mem);
	MemoryToString(m_str_RA_Req, RA_Mem);
	MemoryToString(m_str_Quota_Mem, Ram_Quota);
	RAM_OOC_ToString(m_str_Q_Mem, Q_Ram, Q_Ooc);
	RAM_OOC_ToString(m_str_R_Mem, R_Ram, R_Ooc);
	RAM_OOC_ToString(m_str_A_Mem, A_Ram, A_Ooc);

	Invalidate();
}



void CInfoTab::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CTabCtrl::OnPaint() for painting messages

	// Let Windows/MFC draw the tabs and borders first
	DefWindowProc(WM_PAINT, (WPARAM)dc.m_hDC, 0);

	dc.SetBkMode(TRANSPARENT);

	// Get the selected Tab 
	switch (GetCurSel())
	{
	case 0:
		OutputMeshInfo(&dc);
		break;

	case 1:
		OutputModelInfo(&dc);
		break;

	case 2:
		OutputMaterialInfo(&dc);
		break;

	case 3:
		OutputAnalysisInfo(&dc);
		break;

	case 4:
		OutputMemoryInfo(&dc);
		break;
	}

}



void CInfoTab::OutputInfo(CDC* pDC,
	const CString* const strProperties,
	const CString* const strValues,
	int count)
{
	// Create the bold font
	CFont fontBold;

	// Get the system GUI font
	NONCLIENTMETRICS ncm = { sizeof(NONCLIENTMETRICS) };
	SystemParametersInfo(SPI_GETNONCLIENTMETRICS, sizeof(NONCLIENTMETRICS), &ncm, 0);

	LOGFONT lf = ncm.lfMessageFont;
	lf.lfWeight = FW_BOLD;     // Make it bold

	// Create both fonts
	fontBold.CreateFontIndirect(&lf);


	CFont* pOldFont = (CFont*)pDC->SelectObject(&fontBold);

	
	// Get the text metrics
	TEXTMETRIC tm = { 0 };
	pDC->GetTextMetrics(&tm);

	// Get the text characters Dimension
	int dy = tm.tmHeight + tm.tmDescent + 5;
	int dx = tm.tmAveCharWidth;

	// Compute drawing start position BELOW the tabs
	CRect rcClient;
	GetClientRect(&rcClient);
	AdjustRect(FALSE, &rcClient);

	int x0 = rcClient.left + 4;
	int y0 = rcClient.top + 10;
	int W = rcClient.right - rcClient.left;

	int x = x0;
	int y = y0;

	// detect max length of property text to align the values
	int maxLen = 0;
	for (unsigned i = 0; i < count; i++)
	{
		int Len = strProperties[i].GetLength();
		if (Len > maxLen)
			maxLen = Len;
	}


	DWORD DefColor = pDC->GetTextColor();

	for (unsigned i = 0; i < count; i++)
	{
		// Set text color to default
		pDC->SetTextColor(DefColor);
		x = x0;

		pDC->TextOutW(x, y, strProperties[i]);

		// Set Text color To Red
		pDC->SetTextColor(RGB(255, 0, 0));
		x = x0 + dx * maxLen;
		pDC->TextOutW(x, y, strValues[i]);
		y += dy;

		
	}

	// Set text color to default
	pDC->SetTextColor(DefColor);

	// Restore old font
	pDC->SelectObject(pOldFont);

	// Delete Bold font object
	fontBold.DeleteObject();
}


void CInfoTab::OutputMeshInfo(CDC* pDC)
{
	CString strProperties[] = {
		_T("Directory:"),
		_T("File Name:"),
		_T("File type:"),
		_T("Triangles:"),
		_T("Vertices:")
	};
	

	CString strNe, strNv;
	strNe.Format(_T("%d"), m_Ne);
	strNv.Format(_T("%d"), m_Nv);

	CString strValues[] = {
		m_strWorkingDirectory,
		m_strMeshFileTitle,
		m_strFileType,
		strNe,
		strNv
	};

	OutputInfo(pDC, strProperties, strValues, 5);
}

void CInfoTab::OutputModelInfo(CDC* pDC)
{
	CString strProperties[] = {
		_T("Disc. Type:"),
		_T("DOF Vertices:"),
		_T("Cont. Elements:"),
		_T("Disc. Elements:"),
		_T("Algebraic DOFs:"),
	};


	CString strNv_dof, strNce, strNde, strN, strM;
	strNv_dof.Format(_T("%d"), m_Nv_dof);
	strNce.Format(_T("%d"), m_Nce);
	strNde.Format(_T("%d"), m_Nde);
	strN.Format(_T("%d"), m_N);

	CString strValues[] = {
		m_strMeshType,
		strNv_dof,
		strNce,
		strNde,
		strN,
		strM,
	};

	OutputInfo(pDC, strProperties, strValues, 5);
}


void CInfoTab::OutputMaterialInfo(CDC* pDC)
{
	CString strProperties[] = {
		_T("Young Modulus:"),
		_T("Shear Modulus:"),
		_T("Poisson's Ratio:"),
	};


	CString str_E, str_G, str_v;
	str_E.Format(_T("%e"), m_E);
	str_G.Format(_T("%e"), m_G);
	str_v.Format(_T("%.2f"), m_v);
	

	CString strValues[] = {
		str_E,
		str_G,
		str_v
	};

	OutputInfo(pDC, strProperties, strValues, 3);
}



void CInfoTab::OutputAnalysisInfo(CDC* pDC)
{
	CString strProperties[] = {
		  _T("Integ. Rule:"),
		  _T("[Q] matrix:"),
		  _T("[R] & [A] matrices:"),
		  _T("{b} & {x} vectors:")
	};



	CString strValues[] = {
		m_strIntegRule,
		m_str_Q_Matrix,
		m_str_RA_Matrix,
		m_str_bx_Vector
	};

	OutputInfo(pDC, strProperties, strValues, 4);
}


void CInfoTab::OutputMemoryInfo(CDC* pDC)
{
	CString strProperties[] = {
		  _T("Total Requirement:"),
		  _T("Model Requirement:"),
		  _T("[Q] Requirement:"),
		  _T("[R]/[A] Requirement:"),
		  _T("Fixed RAM Quota :"),
		  _T("[Q] Storage RAM/OOC:"),
		  _T("[R] Storage RAM/OOC:"),
		  _T("[A] Storage RAM/OOC:"),
	};



	CString strValues[] = {
		m_str_Total_Req,
		m_str_Model_Req,
		m_str_Q_Req,
		m_str_RA_Req,
		m_str_Quota_Mem,
		m_str_Q_Mem,
		m_str_R_Mem,
		m_str_A_Mem,
	};

	OutputInfo(pDC, strProperties, strValues, 8);
}