// HMatrixViewer.cpp : implementation file
//

#include "pch.h"
#include "GABES.h"
#include "HMatrixViewer.h"
#include <cairo.h>
#include <cairo-win32.h>

// HMatrixViewer

IMPLEMENT_DYNAMIC(CHMatrixViewer, CWnd)

CHMatrixViewer::CHMatrixViewer(const BEM_3D::Model* pModel)
    : m_pModel(pModel)
    , m_pRowCl(nullptr)
    , m_pColCl(nullptr)
    , m_pRootBlock(nullptr)
{
    
}

CHMatrixViewer::~CHMatrixViewer()
{
    CleanUp();
}



void CHMatrixViewer::CleanUp()
{
    if (m_pRowCl)
    {
        del_cluster(m_pRowCl);
        m_pRowCl = nullptr;
    }


    if (m_pColCl)
    {
        del_cluster(m_pColCl);
        m_pColCl = nullptr;
    }


    if (m_pRootBlock)
    {
        del_block(m_pRootBlock);
        m_pRootBlock = nullptr;
    }
}



void CHMatrixViewer::Update()
{
    CleanUp();

    // Get a reference to the DOF vertices
    const std::vector<Vertex*>& rDOFVertices = m_pModel->GetDOFVertices();

    // Declare the number of DOFs
    uint NVDOF = rDOFVertices.size(); // The Number of DOF Vertices
    uint NDOF = NVDOF * 3U;            // The Total Number of DOFs 


    // Create the row and column index sets and fill them
    std::vector<uint> rowIndices;
    std::vector<uint> colIndices;
    for (uint i = 0; i < NDOF; i++)
    {
        rowIndices.push_back(i);
        colIndices.push_back(i);
    }

    // Create the row and column Cluster Geometry objects
    pclustergeometry pRowClGeo = nullptr;
    pclustergeometry pColClGeo = nullptr;
    CreateClusterGeometryObjects((void*)m_pModel, &pRowClGeo, &pColClGeo, rowIndices.data(), colIndices.data());

    // Create the row and column Clusters
   
    CreateClusters((void*)m_pModel, &m_pRowCl, &m_pColCl, pRowClGeo, pColClGeo, rowIndices.data(), colIndices.data());

    // Create the block cluster tree object (The Skeletton of our H-Matrix)
  
    CreateBlockClusterTree(&m_pRootBlock, m_pRowCl, m_pColCl);

       
    del_clustergeometry(pRowClGeo);
    del_clustergeometry(pColClGeo);
   

    Invalidate();
}




BEGIN_MESSAGE_MAP(CHMatrixViewer, CWnd)
    ON_WM_PAINT()
    ON_WM_SIZE()
END_MESSAGE_MAP()





// HMatrixViewer message handlers



void CHMatrixViewer::OnPaint()
{
    CPaintDC dc(this); // device context for painting
    // TODO: Add your message handler code here
    // Do not call CWnd::OnPaint() for painting messages
    
    // Create cairo surface
    cairo_surface_t* surface = cairo_win32_surface_create(dc.m_hDC);

    // Create cairo context
    cairo_t* cr = cairo_create(surface);

    // Draw the H matrix on this cairo surface 
    draw_cairo_block(cr, m_pRootBlock, 0);
       
    // Cleanup
    cairo_destroy(cr);
    cairo_surface_destroy(surface);
}

void CHMatrixViewer::OnSize(UINT nType, int cx, int cy)
{
    CWnd::OnSize(nType, cx, cy);

    // TODO: Add your message handler code here
    m_cx = (unsigned)cx;
    m_cy = (unsigned)cy;
}


void CHMatrixViewer::CreateWnd(CWnd* pParent)
{
    CRect rcWnd;
    pParent->GetClientRect(rcWnd);

    static CString clsName = AfxRegisterWndClass(
        CS_HREDRAW | CS_VREDRAW,
        ::LoadCursor(nullptr, IDC_ARROW),
        (HBRUSH)GetStockObject(WHITE_BRUSH),
        nullptr);

    Create(
        clsName,
        _T("GABES: H_Matrix Viewer"),
        WS_CHILD | WS_VISIBLE, // The problem is when I use WS_POPUP !!! 
        rcWnd,
        pParent,         
        123456789);    

    Update();
    
}

