#include "pch.h"
#include "Model.h"
#include "resource.h"
#include "Solver_H2LIB.h"






using BEM_3D::Vertex;
using BEM_3D::VtxElmMap;
using BEM_3D::Element;
using BEM_3D::Model;
using BEM_3D::SOLVER;

double Model::m_Eta = 1.5;
uint Model::m_clf = 36U;
double Model::m_ACA_tol = 1.0E-6;
int Model::m_clmode = (int)H2_PCA;   
BOOL Model::m_bSetMaxIter = FALSE;
uint Model::m_maxiter = 500U;
SOLVER Model::m_solver = GMRES;
double Model::m_GMRES_tol = 1.0E-5;     
double Model::m_HLU_tol = 1.0E-5;
double Model::m_Precond_tol = 5.0E-2;   


// The callback function used to compute the block entries: It is used as (entry) callback in H2Lib
void cmpbl(const uint* ridx, const uint* cidx, void* data, const bool ntrans, pamatrix A)
{
	// Transform the data pointer to a model Object
	Model* pObj = (Model*)data;

	// Get the Vertex DOF and Element containers
	const std::vector<Vertex*>& rDOFVertices = pObj->GetDOFVertices();
	const std::vector<Element*>& rElements = pObj->GetElements();


	// Extract the DOF number
	uint NDOF = rDOFVertices.size() * 3;

	// Extract the block size information
	uint arows = !ntrans ? A->rows : A->cols;
	uint acols = !ntrans ? A->cols : A->rows;

	for (int c = 0; c < acols; c++)
	{
		//====== Check for Lengthy job flag ======
		if (!pObj->m_bLengthyJob)
			break;
		//========================================

		// Get the Column DOF index
		uint col = cidx[c];
		// Get the Col Node index
		uint colIdx = col / 3;
		// Get the Col Node
		const Vertex* pColNode = rDOFVertices[colIdx];
		// Get the Column directional index
		uint j = col % 3;


		for (int r = 0; r < arows; r++)
		{
			//====== Check for Lengthy job flag ======
			if (!pObj->m_bLengthyJob)
				break;
			//========================================

			// Get the Row DOF index
			uint row = ridx[r];
			// Get The Row Node index
			uint rowIdx = row / 3;
			// Get the Source Point 
			Vertex* pSrcPt = rDOFVertices[rowIdx];
			// Get the Row directional index
			uint i = row % 3;

			// Set the A storage buffer index
			uint idx = r + arows * c;

			if (!pColNode->IsDispFixed(j))
			{
				A->a[idx] = 0.0;
				// Loop through the Element Map container
				for (const VtxElmMap& vem : pColNode->m_ElmsMap)
				{
					Element* pElm = rElements[vem.ElmIdx];
					int l = vem.LocalIdx;

					A->a[idx] += pElm->P(pSrcPt, i, j, l);
				}

				if (pSrcPt == pColNode)
					A->a[idx] += pSrcPt->C.Get(i, j);
			}
			else
			{
				const VtxElmMap& vem = pColNode->m_ElmsMap.back();
				Element* pElm = rElements[vem.ElmIdx];
				int l = vem.LocalIdx;

				A->a[idx] = -pElm->Q(pSrcPt, i, j, l);
			}

		}
	}
}




void CreateClusterGeometryObjects(void* data,
	                              pclustergeometry* ppRowClGeo,
	                              pclustergeometry* ppColClGeo, 
	                              uint* rowIndices,
	                              uint* colIndices)
{
	Model* pObj = (Model*)data;

	const std::vector<Vertex*>& rDOFVertices = pObj->GetDOFVertices();
	const std::vector<Element*>& rElements = pObj->GetElements();

	uint NVDOF = rDOFVertices.size(); // The Number of DOF Vertices
	uint NDOF =  NVDOF * 3U;            // The Total Number of DOFs 


	if (NDOF == 0)
		return;

	if (*ppRowClGeo)
		del_clustergeometry(*ppRowClGeo);

	if (*ppColClGeo)
		del_clustergeometry(*ppColClGeo);



	// Create the geometry clusters (Row / Column)
	*ppRowClGeo = new_clustergeometry(3, NDOF);
	*ppColClGeo = new_clustergeometry(3, NDOF);

	for (uint idx = 0; idx < NDOF; idx++)
	{
		const Vertex* pV = rDOFVertices[idx / 3U];

		double Xmin, Xmax;
		double Ymin, Ymax;
		double Zmin, Zmax;

		Xmin = Xmax = pV->x;
		Ymin = Ymax = pV->y;
		Zmin = Zmax = pV->z;

		for (VtxElmMap vem : pV->m_ElmsMap)
		{
			const Element* pElm = rElements[vem.ElmIdx];

			for (int l = 0; l < 3; l++)
			{
				const Vertex& Vk = pElm->Vdof(l);
				double x = Vk.x;
				double y = Vk.y;
				double z = Vk.z;

				if (x < Xmin)
					Xmin = x;
				if (y < Ymin)
					Ymin = y;
				if (z < Zmin)
					Zmin = z;

				if (x > Xmax)
					Xmax = x;
				if (y > Ymax)
					Ymax = y;
				if (z > Zmax)
					Zmax = z;
			}
		}


		(*ppColClGeo)->smin[idx][0] = Xmin;
		(*ppColClGeo)->smin[idx][1] = Ymin;
		(*ppColClGeo)->smin[idx][2] = Zmin;

		(*ppColClGeo)->smax[idx][0] = Xmax;
		(*ppColClGeo)->smax[idx][1] = Ymax;
		(*ppColClGeo)->smax[idx][2] = Zmax;


		(*ppColClGeo)->x[idx][0] = pV->x;
		(*ppColClGeo)->x[idx][1] = pV->y;
		(*ppColClGeo)->x[idx][2] = pV->z;



		(*ppRowClGeo)->smin[idx][0] = pV->x;
		(*ppRowClGeo)->smin[idx][1] = pV->y;
		(*ppRowClGeo)->smin[idx][2] = pV->z;

		(*ppRowClGeo)->smax[idx][0] = pV->x;
		(*ppRowClGeo)->smax[idx][1] = pV->y;
		(*ppRowClGeo)->smax[idx][2] = pV->z;


		(*ppRowClGeo)->x[idx][0] = pV->x;
		(*ppRowClGeo)->x[idx][1] = pV->y;
		(*ppRowClGeo)->x[idx][2] = pV->z;
	}


	update_point_bbox_clustergeometry(*ppRowClGeo, NDOF, rowIndices);
	update_point_bbox_clustergeometry(*ppColClGeo, NDOF, colIndices);
}


void CreateClusters(void* data,
	                pcluster* ppRowCl, 
	                pcluster* ppColCl,
	                pclustergeometry pRowClGeo, 
	                pclustergeometry pColClGeo,
	                uint* rowIndices, 
	                uint* colIndices)
{
	Model* pObj = (Model*)data;

	const std::vector<Vertex*>& rDOFVertices = pObj->GetDOFVertices();

	uint NDOF = rDOFVertices.size() * 3;

	if (NDOF == 0)
		return;

	if (*ppRowCl)
		del_cluster(*ppRowCl);

	if (*ppColCl)
		del_cluster(*ppColCl);


	// Build the clusters from the cluster geometry objects
	*ppRowCl = build_cluster(pRowClGeo, NDOF, rowIndices, Model::m_clf, (clustermode)Model::m_clmode);

	*ppColCl = build_cluster(pColClGeo, NDOF, colIndices, Model::m_clf, (clustermode)Model::m_clmode);

	update_support_bbox_cluster(pRowClGeo, *ppRowCl);
	update_support_bbox_cluster(pColClGeo, *ppColCl);
}




void CreateBlockClusterTree(pblock* ppRootBlock,
	                        pcluster pRowCl,
	                        pcluster pColCl)
{
	if (*ppRootBlock)
		del_block(*ppRootBlock);

	*ppRootBlock = build_strict_block(pRowCl, pColCl, (void*)&Model::m_Eta, admissible_2_cluster);
}


void FillLeafBlocks(std::vector<pchmatrix>& rBlockLeaves, pchmatrix pHmat)
{
	if (pHmat == nullptr)
		return;

	if (pHmat->son == nullptr)
	{
		rBlockLeaves.push_back(pHmat);
	}
	else
	{
		uint nSons = pHmat->rsons * pHmat->csons;

		for (uint i = 0; i < nSons; i++)
			FillLeafBlocks(rBlockLeaves, pHmat->son[i]);
	}
}


void BuildH_Matrix(void* data,
	               phmatrix* ppHmatrix,
	               pblock pRootBlock)
{
	if (*ppHmatrix)
		del_hmatrix(*ppHmatrix);

	Model* pObj = (Model*)data;

	// Create the H-Matrix
	*ppHmatrix = build_from_block_hmatrix(pRootBlock, 1U); 

	// Enumerate the Hmatrix leaf blocks
	std::vector<pchmatrix> BlockLeaves;
	FillLeafBlocks(BlockLeaves, *ppHmatrix);
	int nLeaves = BlockLeaves.size();

	// Create a truncation mode object
	ptruncmode tm = new_releucl_truncmode();
    
    #pragma omp parallel for schedule(dynamic)
	for (int i = 0; i < nLeaves; i++)
	{
		
		pchmatrix  G = BlockLeaves[i];

		if (G->r)
		{
			decomp_partialaca_rkmatrix(cmpbl,
				(void*)pObj,
				G->rc->idx,
				G->rc->size,
				G->cc->idx,
				G->cc->size,
				Model::m_ACA_tol,
				nullptr,
				nullptr,
				G->r);

			trunc_rkmatrix(tm, Model::m_ACA_tol, G->r);
		}
		else if (G->f)
		{
			cmpbl(G->rc->idx,
				G->cc->idx,
				(void*)pObj,
				false,
				G->f);
		}

        #pragma omp atomic
		pObj->m_CurrentAdvance += (1.0 / (double)nLeaves);
	}

	// Release the truncation mode object
	del_truncmode(tm);
}

void  H_LU_precond(void* pdata, pavector r)
{
	// parse the data pointer into an H-Matrix
	phmatrix pLU = (phmatrix)pdata;	

	// Modify the vector r
	lrsolve_hmatrix_avector(false, pLU, r);
}


void Model::ResolveLinearSystem_H2LIB(LPVOID pParam)
{
	// Verify the dimension of the system
	if (m_Elements.empty())
		return;


	m_CurrentAdvance = 0.0;
	m_bLengthyJob = true;

	::AfxBeginThread(ResolveLinearSystemThread_H2LIB, pParam);
}




UINT Model::ResolveLinearSystemThread_H2LIB(LPVOID pParam)
{
	// Get the Solver Thread Data object
	SolverThreadData* pSTHD = (SolverThreadData*)pParam;

	// Get the THIS pointer of the model object
	Model* pObj = pSTHD->pObj;


	//====== Check for Lengthy job flag ======
	if (!pObj->m_bLengthyJob)
		return 0;
	//========================================
	if (Element::m_bRISP)
	{
		pObj->m_strCurrentJob = _T("Calculate the reusable integration sample points...");
		pObj->m_CurrentAdvance = 0.0;
		for (Element* pElm : pObj->m_Elements)
			pElm->ComputeReusableIntegrationData();
		pObj->m_CurrentAdvance = 1.0;
	}
	

	pObj->m_strCurrentJob = _T("Copying boundary conditions from subsets...");
	pObj->m_CurrentAdvance = 0.0;
	pObj->CopyBoundaryConditionsFromSubSets();
	pObj->m_CurrentAdvance = 1.0;


	// Update the A matrix and construct the RHS vector using prescribed boundary conditions
	pObj->m_strCurrentJob = _T("Compute the {b} right-hand side vector...");
	pObj->m_CurrentAdvance = 0.0;
	pObj->ConstructRHSvector();
	pObj->m_CurrentAdvance = 1.0;
	// Send a windows user message to the Wait progress dialog to notify that the RHS vector is constructed
	pSTHD->pWaitSolverDlg->SendMessage(WM_USER_RHS_VECTOR_CONSTRUCTED, 0, 0);



	//====== Check for Lengthy job flag ======
	if (!pObj->m_bLengthyJob)
		return 0;
	//========================================


	// Set the RHS vector boolean to true
	pSTHD->bRhsVec = TRUE;
	pSTHD->pWaitSolverDlg->UpdateData(FALSE);


	// The H-matrix construction phase 
	pObj->m_strCurrentJob = _T("Construct the H-Matrix...");
	pObj->m_CurrentAdvance = 0.0;
	// Declare the number of DOFs
	uint NVDOF = pObj->m_DOF_Vertices.size(); // The Number of DOF Vertices
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
	CreateClusterGeometryObjects(pObj, &pRowClGeo, &pColClGeo, rowIndices.data(), colIndices.data());

	// Create the row and column Clusters
	pcluster pRowCl = nullptr;
	pcluster pColCl = nullptr;
	CreateClusters(pObj, &pRowCl, &pColCl, pRowClGeo, pColClGeo, rowIndices.data(), colIndices.data());

	// Create the block cluster tree object (The Skeletton of our H-Matrix)
	pblock pRootBlock = nullptr;
	CreateBlockClusterTree(&pRootBlock, pRowCl, pColCl);

	// Build the H-Matrix
	phmatrix pHmatrix = nullptr;
	BuildH_Matrix(pObj, &pHmatrix, pRootBlock);


	//====== Check for Lengthy job flag ======
	if (!pObj->m_bLengthyJob)
	{
		del_clustergeometry(pRowClGeo);
		del_clustergeometry(pColClGeo);
		del_cluster(pRowCl);
		del_cluster(pColCl);
		del_block(pRootBlock);
		del_hmatrix(pHmatrix);
		return 0;
	}
	//========================================


	// Set the H-Matrix boolean to true and compute the compression ratio
	pSTHD->bHMatrix = TRUE;
	double dNDOF = (double)NDOF;
	double denseSize = dNDOF * dNDOF * (double)sizeof(double);
	double actualSize = getsize_hmatrix(pHmatrix);
	pSTHD->CompRatio = (denseSize - actualSize) / denseSize * 100.0;
	pSTHD->pWaitSolverDlg->UpdateData(FALSE);

	pObj->m_CurrentAdvance = 1.0;

	// Send a windows user message to the Wait progress dialog to notify that the H-Matrix is constructed
	pSTHD->pWaitSolverDlg->SendMessage(WM_USER_HMATRIX_CONSTRUCTED, 0, 0);

	//====== Check for Lengthy job flag ======
	if (!pObj->m_bLengthyJob)
	{
		del_clustergeometry(pRowClGeo);
		del_clustergeometry(pColClGeo);
		del_cluster(pRowCl);
		del_cluster(pColCl);
		del_block(pRootBlock);
		del_hmatrix(pHmatrix);
		return 0;
	}
	//========================================
	

	
	// Solve the linear System
	pObj->m_CurrentAdvance = 0.0;

	avector v_x; 

	// Get the Progress bar control and set Marquee to true 
	CProgressCtrl* pPrgs = (CProgressCtrl*)pSTHD->pWaitSolverDlg->GetDlgItem(IDC_PROGRESS_ADVANCE);
	pPrgs->ModifyStyle(0, PBS_MARQUEE);
	pPrgs->SetMarquee(TRUE, 1000);

	// Get the CANCEL Button and disable it temporarily
	CWnd* pCancel = pSTHD->pWaitSolverDlg->GetDlgItem(IDCANCEL);
	pCancel->EnableWindow(FALSE);


	switch (Model::m_solver)
	{
	case H_LU:
	{
		pObj->m_strCurrentJob = _T("Solve the system [A] {x} = {b} using H-LU decomposition...");
		init_pointer_avector(&v_x, pObj->b, NDOF);

		ptruncmode tm = new_releucl_truncmode();
		lrdecomp_hmatrix(pHmatrix, tm, m_HLU_tol);
		lrsolve_hmatrix_avector(false, pHmatrix, &v_x);
		del_truncmode(tm);
	}
	break;

	case GMRES:
	{
		pObj->m_strCurrentJob = _T("Solve the system [A] {x} = {b} using GMRES iterative solver...");
		
		avector v_b;
		init_pointer_avector(&v_b, pObj->b, NDOF);
		init_zero_avector(&v_x, NDOF);

		// Create a Cheap LU H-Matrix
		phmatrix pLU = clone_hmatrix(pHmatrix);
		ptruncmode tm = new_releucl_truncmode();
		lrdecomp_hmatrix(pLU, tm, m_Precond_tol);
		del_truncmode(tm);

		pSTHD->nIters = solve_pgmres_hmatrix_avector(pHmatrix, H_LU_precond, (void*)pLU, 
			                                    &v_b, &v_x, m_GMRES_tol, m_bSetMaxIter ? m_maxiter : 0U, 50U);

		del_hmatrix(pLU);
		uninit_avector(&v_b);
	}
	break;
	}
	pPrgs->ModifyStyle(PBS_MARQUEE, 0);
	pPrgs->SetMarquee(FALSE, 0);
	pCancel->EnableWindow(TRUE);

	// Set the Solved boolean to true
	pSTHD->bSolved = TRUE;
	pSTHD->pWaitSolverDlg->UpdateData(FALSE);


	// Put the solution into the vector x
	CopyMemory(pObj->x, v_x.v, sizeof(double) * NDOF);


	// Clear the memory resources occupied by H2Lib objects
	del_clustergeometry(pRowClGeo);
	del_clustergeometry(pColClGeo);
	del_cluster(pRowCl);
	del_cluster(pColCl);
	del_block(pRootBlock);
	del_hmatrix(pHmatrix);
	uninit_avector(&v_x);
	
	pObj->m_bLengthyJob = false;
	pObj->m_CurrentAdvance = 1.0;
	return 0;
}