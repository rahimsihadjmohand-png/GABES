#pragma once

#include <hmatrix.h>
#include <aca.h>
#include <krylovsolvers.h>
#include <harith.h>


#define WM_USER_RHS_VECTOR_CONSTRUCTED WM_USER+101
#define WM_USER_HMATRIX_CONSTRUCTED    WM_USER+102



// Structure passed as a void pointer to the Solver thread
struct SolverThreadData
{
	BEM_3D::Model* pObj;  // The Model Object
	BOOL bRhsVec;
	BOOL bHMatrix;
	BOOL bSolved;
	double CompRatio;
	uint nIters;
	CDialogEx* pWaitSolverDlg;
};


// The callback function used to compute the block entries: It is used as (entry) callback in H2Lib
void cmpbl(const uint* ridx, 
	const uint* cidx, 
	void* data, 
	const bool ntrans, 
	pamatrix A);

void CreateClusterGeometryObjects(void* data,
	pclustergeometry* ppRowClGeo,
	pclustergeometry* ppColClGeo,
	uint* rowIndices,
	uint* colIndices);

void CreateClusters(void* data,
	pcluster* ppRowCl,
	pcluster* ppColCl,
	pclustergeometry pRowClGeo,
	pclustergeometry pColClGeo,
	uint* rowIndices,
	uint* colIndices);


void CreateBlockClusterTree(pblock* ppRootBlock,
	pcluster pRowCl,
	pcluster pColCl);

void FillLeafBlocks(std::vector<pchmatrix>& rBlockLeaves, 
	pchmatrix pHmat);


void BuildH_Matrix(void* data,
	phmatrix* ppHmatrix,
	pblock pRootBlock);


