#include "pch.h"
#include "ElementSubSet.h"

using namespace BEM_3D;

UINT ElementSubSet::m_nSubSetIdx = 1;
std::vector<Element*>* ElementSubSet::m_pElements = nullptr;
double ElementSubSet::SpecLength = 10.0;
bool ElementSubSet::m_bShowFixations = true;
bool ElementSubSet::m_bShowDispVectors = true;
bool ElementSubSet::m_bShowTracVectors = true;

ElementSubSet::ElementSubSet()
	: m_bFixed_X(false)
	, m_bFixed_Y(false)
	, m_bFixed_Z(false)
	, m_bNormalDisplacement(false)
	, m_bNormalTraction(false)
	, m_Un(0.0)
	, m_Tn(0.0)
{
	m_strName.Format(_T("Subset N°%d"), m_nSubSetIdx);
	m_nSubSetIdx++;
}

ElementSubSet::ElementSubSet(const std::vector<int> IdxSubSet)
	: m_Indices(IdxSubSet)
	, m_bFixed_X(false)
	, m_bFixed_Y(false)
	, m_bFixed_Z(false)
	, m_bNormalDisplacement(false)
	, m_bNormalTraction(false)
	, m_Un(0.0)
	, m_Tn(0.0)
{
	m_strName.Format(_T("Subset N°%d"), m_nSubSetIdx);
	m_nSubSetIdx++;
}

ElementSubSet::ElementSubSet(const CString& strName) 
	: m_strName(strName)
	, m_bFixed_X(false)
	, m_bFixed_Y(false)
	, m_bFixed_Z(false)
	, m_bNormalDisplacement(false)
	, m_bNormalTraction(false)
	, m_Un(0.0)
	, m_Tn(0.0)
{
	m_nSubSetIdx++;
}

ElementSubSet::ElementSubSet(const std::vector<int> IdxSubSet, const CString& strName) 
	: m_Indices(IdxSubSet)
	, m_strName(strName) 
	, m_bFixed_X(false)
	, m_bFixed_Y(false)
	, m_bFixed_Z(false)
	, m_bNormalDisplacement(false)
	, m_bNormalTraction(false)
	, m_Un(0.0)
	, m_Tn(0.0)
{
	m_nSubSetIdx++;
}

void ElementSubSet::DrawBoundaryConditions(IDirect3DDevice9* pD3ddev, bool bPostTreatment)const
{
	std::vector<Element*>& rElements = *m_pElements;

	// Check if there is a normal displacement
	if (m_bNormalDisplacement && m_Un != 0.0 && m_bShowDispVectors)
	{
		// In this case Draw a normal displacement vector
		for (int Idx : m_Indices)
		{
			Element* pElm = rElements[Idx];

			pElm->DrawNormalDisplacementVector(pD3ddev, m_Un, SpecLength, bPostTreatment);
		}

		return;
	}

	// Check if there is a normal traction
	if (m_bNormalTraction && m_Tn != 0.0 && m_bShowTracVectors)
	{
		// In this case Draw a traction vector
		for (int Idx : m_Indices)
		{
			Element* pElm = rElements[Idx];

			pElm->DrawNormalTractionVector(pD3ddev, m_Tn, SpecLength, bPostTreatment);
		}

		return;
	}


	// Check if there is a Total encastrement
	if ((m_bFixed_X && U.x == 0.0) && (m_bFixed_Y && U.y == 0.0) &&
		(m_bFixed_Z && U.z == 0.0) && !m_bNormalDisplacement && m_bShowFixations)
	{
		// In this case Draw a Total Encastrement
		for (int Idx : m_Indices)
		{
			Element* pElm = rElements[Idx];

			pElm->DrawTotalEncastrement(pD3ddev, bPostTreatment);
		}

		return;
	}

	// Check If there is a Partial encastrement
	if (((m_bFixed_X && U.x == 0.0) || (m_bFixed_Y && U.y == 0.0) || (m_bFixed_Z && U.z == 0.0))
		&& !m_bNormalDisplacement && m_bShowFixations)
	{
		// In this case Draw a partial Encastrement
		for (int Idx : m_Indices)
		{
			Element* pElm = rElements[Idx];

			pElm->DrawPartialEncastrement(pD3ddev, bPostTreatment);
		}
	}

	// Check If there is a Displacement vector
	if (((m_bFixed_X && U.x != 0.0) || (m_bFixed_Y && U.y != 0.0) || (m_bFixed_Z && U.z != 0.0)) && m_bShowDispVectors)
	{
		// In this case Draw a Displacement vector
		for (int Idx : m_Indices)
		{
			Element* pElm = rElements[Idx];

			pElm->DrawDisplacementVector(pD3ddev, U, SpecLength, bPostTreatment);
		}
	}


	// Check If there is a traction vector
	if (((!m_bFixed_X && T.x != 0.0) || (!m_bFixed_Y && T.y != 0.0) || (!m_bFixed_Z && T.z != 0.0)) && m_bShowTracVectors)
	{
		// In this case Draw a traction vector
		for (int Idx : m_Indices)
		{
			Element* pElm = rElements[Idx];

			pElm->DrawTractionVector(pD3ddev, T, SpecLength, bPostTreatment);
		}
	}

}



void ElementSubSet::FullyEncaster()
{
	m_bFixed_X = true;
	m_bFixed_Y = true;
	m_bFixed_Z = true;
	m_bNormalDisplacement = false;
	m_bNormalTraction = false;

	U.Set(0.0, 0.0, 0.0);
	T.Set(0.0, 0.0, 0.0);
	m_Un = 0.0;
	m_Tn = 0.0;
}

void ElementSubSet::SetUx(double Ux)
{
	m_bFixed_X = true;
	m_bNormalDisplacement = false;
	m_bNormalTraction = false;

	U.x = Ux;
	T.x = 0.0;
	m_Un = 0.0;
	m_Tn = 0.0;
}

void ElementSubSet::SetUy(double Uy)
{
	m_bFixed_Y = true;
	m_bNormalDisplacement = false;
	m_bNormalTraction = false;

	U.y = Uy;
	T.y = 0.0;
	m_Un = 0.0;
	m_Tn = 0.0;
}

void ElementSubSet::SetUz(double Uz)
{
	m_bFixed_Z = true;
	m_bNormalDisplacement = false;
	m_bNormalTraction = false;

	U.z = Uz;
	T.z = 0.0;
	m_Un = 0.0;
	m_Tn = 0.0;
}

void ElementSubSet::SetNormalDisplacement(double Un)
{
	m_bFixed_X = true;
	m_bFixed_Y = true;
	m_bFixed_Z = true;
	m_bNormalDisplacement = true;
	m_bNormalTraction = false;

	U.Set(0.0, 0.0, 0.0);
	T.Set(0.0, 0.0, 0.0);
	m_Un = Un;
	m_Tn = 0.0;
}

void ElementSubSet::SetDisplacementVector(const Vector& Uv)
{
	m_bFixed_X = true;
	m_bFixed_Y = true;
	m_bFixed_Z = true;
	m_bNormalDisplacement = false;
	m_bNormalTraction = false;

	U = Uv;
	T.Set(0.0, 0.0, 0.0);
	m_Un = 0.0;
	m_Tn = 0.0;
}

void ElementSubSet::FreeSurface()
{
	m_bFixed_X = false;
	m_bFixed_Y = false;
	m_bFixed_Z = false;
	m_bNormalDisplacement = false;
	m_bNormalTraction = false;

	T.x = 0.0;
	T.y = 0.0;
	T.z = 0.0;

	U.x = 0.0;
	U.y = 0.0;
	U.z = 0.0;

	m_Un = 0.0;
	m_Tn = 0.0;
}

void ElementSubSet::SetTx(double Tx)
{
	m_bFixed_X = false;
	m_bNormalDisplacement = false;
	m_bNormalTraction = false;

	T.x = Tx;
	U.x = 0.0;
	m_Un = 0.0;
	m_Tn = 0.0;
}

void ElementSubSet::SetTy(double Ty)
{
	m_bFixed_Y = false;
	m_bNormalDisplacement = false;
	m_bNormalTraction = false;

	T.y = Ty;
	U.y = 0.0;
	m_Un = 0.0;
	m_Tn = 0.0;
}

void ElementSubSet::SetTz(double Tz)
{
	m_bFixed_Z = false;
	m_bNormalDisplacement = false;
	m_bNormalTraction = false;

	T.z = Tz;
	U.z = 0.0;
	m_Un = 0.0;
	m_Tn = 0.0;
}

void ElementSubSet::SetNormalTraction(double Tn)
{
	m_bFixed_X = false;
	m_bFixed_Y = false;
	m_bFixed_Z = false;
	m_bNormalDisplacement = false;
	m_bNormalTraction = true;

	U.Set(0.0, 0.0, 0.0);
	T.Set(0.0, 0.0, 0.0);
	m_Un = 0.0;
	m_Tn = Tn;
}

void ElementSubSet::SetTractionVector(const Vector& Tv)
{
	m_bFixed_X = false;
	m_bFixed_Y = false;
	m_bFixed_Z = false;
	m_bNormalDisplacement = false;
	m_bNormalTraction = false;

	U.Set(0.0, 0.0, 0.0);
	T = Tv;
	m_Un = 0.0;
	m_Tn = 0.0;
}
