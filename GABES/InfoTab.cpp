// InfoTab.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "InfoTab.h"
#include "cluster.h"


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

	// Update the information
	UpdateInfo();
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

	m_DOF = m_Nv_dof * 3;
	//=======================================================================================
	//3. Material information
	m_E = BEM_3D::Element::E;                     
	m_G = BEM_3D::Element::G;
	m_v = BEM_3D::Element::v;
	//=======================================================================================

	//4. Analysis information
	if (BEM_3D::Element::m_bUseAdaptiveCriterion)
	{
		CString strCriterionPrec; 
		switch (BEM_3D::Element::m_CriterionPrec)
		{
		case BEM_3D::_0_001:
			m_strIntegScheme = _T("Adaptive Criterion - eps = 1e-3");
			break;

		case BEM_3D::_0_0001:
			m_strIntegScheme = _T("Adaptive Criterion - eps = 1e-4");
			break;
		}
	}
	else
	{

		if(BEM_3D::Element::m_FixedIntegRule == BEM_3D::_001_PT)
			m_strIntegScheme.Format(_T("Fixed Cubature - %d Point"), 1);
		else
			m_strIntegScheme.Format(_T("Fixed Cubature - %d Points"), BEM_3D::Element::RuleToSize(BEM_3D::Element::m_FixedIntegRule));
	}
	
	if (BEM_3D::Element::m_bRISP)
		m_strRISP = _T("True");
	else
		m_strRISP = _T("False");

	switch (BEM_3D::Model::m_solver)
	{
	case BEM_3D::H_LU:
		m_strSolver = _T("H-LU");
		break;

	case BEM_3D::GMRES:
		m_strSolver = _T("GMRES");
		break;
	}


	switch (BEM_3D::Model::m_clmode)
	{
	case H2_ADAPTIVE:
		m_strClusterMode = _T("ADAPTIVE");
		break;

	case H2_REGULAR:
		m_strClusterMode = _T("REGULAR");
		break;

	case H2_SIMSUB:
		m_strClusterMode = _T("SIMSUB");
		break;

	case H2_PCA:
		m_strClusterMode = _T("PCA");
	}

	if (BEM_3D::Model::m_bSetMaxIter)
		m_strMaxIterations.Format(_T("True - %d iterations"), BEM_3D::Model::m_maxiter);
	else
		m_strMaxIterations = _T("False");

	m_strErrorThreshold.Format(_T("%.2e"), BEM_3D::Model::m_accur);

	m_strEta.Format(_T("%.2f"), BEM_3D::Model::m_Eta);

	m_strClf.Format(_T("%d"), BEM_3D::Model::m_clf);
	// ===================================================================================

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
		_T("Total DOFs:"),
	};


	CString strNv_dof, strNce, strNde, strDOF;
	strNv_dof.Format(_T("%d"), m_Nv_dof);
	strNce.Format(_T("%d"), m_Nce);
	strNde.Format(_T("%d"), m_Nde);
	strDOF.Format(_T("%d"), m_DOF);

	CString strValues[] = {
		m_strMeshType,
		strNv_dof,
		strNce,
		strNde,
		strDOF,
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
		  _T("Integ. Scheme:"),
		  _T("RISP Algorithm:"),
		  _T("Solver:"),
		  _T("Clustering Mode:"),
		  _T("Use of Maxiter:"),
		  _T("Error Threshold:"),
		  _T("Eta:"),
		  _T("Max Leaf Size:"),
	};


	CString strValues[] = {
		m_strIntegScheme,
		m_strRISP,
		m_strSolver,
		m_strClusterMode,
		m_strMaxIterations,
		m_strErrorThreshold,
		m_strEta,
		m_strClf
	};

	OutputInfo(pDC, strProperties, strValues, 8);
}


