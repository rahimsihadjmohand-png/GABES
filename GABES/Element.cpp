#include "pch.h"
#include "Element.h"
#include "BoundarySubset.h"
#include "Element_C.h"
using namespace BEM_3D;



double Element::Lambda = 0.155;
#define _L Element::Lambda

double Element::E = 200.0E3;
double Element::v = 0.3;
double Element::G = E / (2.0 * (1.0 + v));


INTEG_RULE Element::m_FixedIntegRule = _052_PT;
bool Element::m_bUseAdaptiveCriterion = true; // The use of the adaptive scheme yields better results and performance
CRITERION_PREC Element::m_CriterionPrec = _0_001; // The default precision is 1e-3

bool Element::m_bRISP = false; // The use of Reusable Integration Data can slightly accelerate Matrix assembly but
                                                         // Generates Huge memory !!!



double Element::ShortestLength; // This is the Shortest Element Side in the Mesh, used for BC vectors drawing
double Element::LongestLength;  // This is the Longest Element Side in the Mesh, used for BC vectors drawing
double Element::SpecLengthFactor = 0.5; // This factor is used to set up the specific length

double Element::SpecLength()
{
	return ShortestLength + SpecLengthFactor * (LongestLength - ShortestLength);
}

Element::Element()
	: m_pV1(nullptr)
	, m_pV2(nullptr)
	, m_pV3(nullptr)
	, m_I1(-1)
	, m_I2(-1)
	, m_I3(-1)
	, m_J1(-1)
	, m_J2(-1)
	, m_J3(-1)
	, FldPts(nullptr) // Reusable integration points
{
	//CalculateNormal();
	//CalculateControlNodes();            
	Element::FillIntegrationNodes();
	
}

Element::Element(Vertex* pV1, Vertex* pV2, Vertex* pV3, int I1, int I2, int I3)
	: m_pV1(pV1)
	, m_pV2(pV2)
	, m_pV3(pV3)
	, m_I1(I1)
	, m_I2(I2)
	, m_I3(I3)
	, m_J1(-1)
	, m_J2(-1)
	, m_J3(-1)
	, FldPts(nullptr) // Reusable integration points
{
	CalculateNormal_Jacobian();
	Element::FillIntegrationNodes();	
}


Element::~Element()
{
	if (FldPts != nullptr)
	{
		delete[] FldPts;
		FldPts = nullptr;
	}
}


void Element::CalculateNormal_Jacobian()
{
	Vector V31 = *m_pV1 - *m_pV3;
	Vector V32 = *m_pV2 - *m_pV3;
	m_n = V31 ^ V32;
	m_n.Normalize();

	m_J = Jn();
}


const Vector& Element::T(int l)const
{
	switch (l)
	{
	case 0:
		return m_T1;

	case 1:
		return m_T2;

	case 2:
		return m_T3;
	}

	return Vector(); // Impossible value
}

Vector& Element::T(int l)
{
	switch (l)
	{
	case 0:
		return m_T1;

	case 1:
		return m_T2;

	case 2:
		return m_T3;
	}

	return Vector(); // Impossible value
}




const Tensor& Element::Sig(int l)const
{
	switch (l)
	{
	case 0:
		return m_Sig1;

	case 1:
		return m_Sig2;

	case 2:
		return m_Sig3;
	}

	return Tensor(); // Impossible value
}

Tensor& Element::Sig(int l)
{
	switch (l)
	{
	case 0:
		return m_Sig1;

	case 1:
		return m_Sig2;

	case 2:
		return m_Sig3;
	}

	return Tensor(); // Impossible value
}

const Tensor& Element::Eps(int l)const
{
	switch (l)
	{
	case 0:
		return m_Eps1;

	case 1:
		return m_Eps2;

	case 2:
		return m_Eps3;
	}

	return Tensor(); // Impossible value
}

Tensor& Element::Eps(int l)
{
	switch (l)
	{
	case 0:
		return m_Eps1;

	case 1:
		return m_Eps2;

	case 2:
		return m_Eps3;
	}

	return Tensor(); // Impossible value
}



double Element::N(int l, double Xi, double Eta)const
{
	switch (l)
	{
	case 0:
		return N1(Xi, Eta);

	case 1:
		return N2(Xi, Eta);

	case 2:
		return N3(Xi, Eta);

	default:
		break;
	}

	return 0.0; // impossible value
}



Vertex Element::GetFieldPoint(double Xi, double Eta)const
{
	// Prepare the vertex constituants
	double _x = 0.0;
	double _y = 0.0;
	double _z = 0.0;
	Vector n_, U, T;

	// Get the shape functions
	double N1_ = N1(Xi, Eta);
	double N2_ = N2(Xi, Eta);
	double N3_ = N3(Xi, Eta);

	// Get the Vertex DOFs
	const Vertex& V1 = Vdof(0);
	const Vertex& V2 = Vdof(1);
	const Vertex& V3 = Vdof(2);

	// interpolate the position
	_x = N1_ * V1.x + N2_ * V2.x + N3_ * V3.x;
	_y = N1_ * V1.y + N2_ * V2.y + N3_ * V3.y;
	_z = N1_ * V1.z + N2_ * V2.z + N3_ * V3.z;

	// interpolate the displacement
	U = V1.U * N1_ + V2.U * N2_ + V3.U * N3_;

	
	// Prepare the vertex to return
	Vertex V = Vertex(_x, _y, _z);
	V.U = U;

	return V;
}


Vertex Element::GetFieldPoint(const Vertex& V1, const Vertex& V2, const Vertex& V3, double Xi, double Eta)
{
	// Prepare the vertex constituants
	double _x = 0.0;
	double _y = 0.0;
	double _z = 0.0;

	// Get the shape functions
	double N1_ = 1.0 - Xi - Eta;
	double N2_ = Xi;
	double N3_ = Eta;

	
	// interpolate the position
	_x = N1_ * V1.x + N2_ * V2.x + N3_ * V3.x;
	_y = N1_ * V1.y + N2_ * V2.y + N3_ * V3.y;
	_z = N1_ * V1.z + N2_ * V2.z + N3_ * V3.z;

	return Vertex(_x, _y, _z);
}


void Element::GetFieldPoint(const Vertex& V1, const Vertex& V2, const Vertex& V3, double Xi, double Eta, Vertex& Vout)
{
	// Get the shape functions
	double N1_ = 1.0 - Xi - Eta;
	double N2_ = Xi;
	double N3_ = Eta;


	// interpolate the position
	Vout.x = N1_ * V1.x + N2_ * V2.x + N3_ * V3.x;
	Vout.y = N1_ * V1.y + N2_ * V2.y + N3_ * V3.y;
	Vout.z = N1_ * V1.z + N2_ * V2.z + N3_ * V3.z;

}


void Element::GetFieldPoint(Vertex* pFldPt, double Xi, double Eta)const
{
	// Get the shape functions
	double N1_ = N1(Xi, Eta);
	double N2_ = N2(Xi, Eta);
	double N3_ = N3(Xi, Eta);

	// Get the Vertex DOFs
	const Vertex& V1 = Vdof(0);
	const Vertex& V2 = Vdof(1);
	const Vertex& V3 = Vdof(2);

	// interpolate the position
	pFldPt->x = N1_ * V1.x + N2_ * V2.x + N3_ * V3.x;
	pFldPt->y = N1_ * V1.y + N2_ * V2.y + N3_ * V3.y;
	pFldPt->z = N1_ * V1.z + N2_ * V2.z + N3_ * V3.z;

	
	// interpolate the displacement
	pFldPt->U = V1.U * N1_ + V2.U * N2_ + V3.U * N3_;
}


double Element::GetFieldDisp(double Xi, double Eta, int i)const
{
	// Get the shape functions
	double N1_ = N1(Xi, Eta);
	double N2_ = N2(Xi, Eta);
	double N3_ = N3(Xi, Eta);

	// Get the Vertex DOFs
	const Vertex& V1 = Vdof(0);
	const Vertex& V2 = Vdof(1);
	const Vertex& V3 = Vdof(2);

	// interpolate the displacement
	double Ui = N1_ * V1.U[i] + N2_ * V2.U[i] + N3_ * V3.U[i];

	return Ui;
}

double Element::GetFieldStress(double Xi, double Eta, int i, int j)const
{
	// Get the shape functions
	double N1_ = Xi;
	double N2_ = Eta;
	double N3_ = 1.0 - Xi - Eta;

	// Get the Vertex DOFs
	const Vertex& V1 = Vdof(0);
	const Vertex& V2 = Vdof(1);
	const Vertex& V3 = Vdof(2);

	// interpolate the stress
	double Sig_ij = N1_ * m_Sig1.Get(i,j) + N2_ * m_Sig2.Get(i, j) + N3_ * m_Sig3.Get(i, j);

	return Sig_ij;
}

double Element::GetFieldStrain(double Xi, double Eta, int i, int j)const
{
	// Get the shape functions
	double N1_ = Xi;
	double N2_ = Eta;
	double N3_ = 1.0 - Xi - Eta;

	// Get the Vertex DOFs
	const Vertex& V1 = Vdof(0);
	const Vertex& V2 = Vdof(1);
	const Vertex& V3 = Vdof(2);

	// interpolate the strain
	double Eps_ij = N1_ * m_Eps1.Get(i, j) + N2_ * m_Eps2.Get(i, j) + N3_ * m_Eps3.Get(i, j);

	return Eps_ij;
}


void Element::GetIntrinsicCoords(const Vertex& V, double& Xi, double& Eta)const
{
	Vector e1 = *m_pV1 - *m_pV3;
	Vector e2 = *m_pV2 - *m_pV3;
	Vector e = V - *m_pV3;

	// Triangle normal (NOT unit, important!)
	Vector n = e1 ^ e2;

	double denom = n * n;  // squared norm

	// Signed barycentric coordinates
	Xi = ((e ^ e2) * n) / denom;
	Eta = ((e1 ^ e) * n) / denom;
}



double Element::dr(const Vertex& SrcPt, const Vertex& FldPt, int k)
{

	switch (k)
	{
	case 0:
		return (FldPt.x - SrcPt.x) / r(SrcPt, FldPt);

	case 1:
		return (FldPt.y - SrcPt.y) / r(SrcPt, FldPt);

	case 2:
		return (FldPt.z - SrcPt.z) / r(SrcPt, FldPt);

	default:
		break;
	}

	return 0.0; // Impossible value
}

Vector Element::dr(const Vertex& SrcPt, const Vertex& FldPt)
{
	double dr_x = (FldPt.x - SrcPt.x) / r(SrcPt, FldPt);
	double dr_y = (FldPt.y - SrcPt.y) / r(SrcPt, FldPt);
	double dr_z = (FldPt.z - SrcPt.z) / r(SrcPt, FldPt);

	return Vector(dr_x, dr_y, dr_z);
}


double Element::dr_dn(const Vertex& SrcPt, const Vertex& FldPt)const
{
	return dr(SrcPt, FldPt) * m_n;
}


double Element::n(/*double Xi, double Eta,*/ int k)const
{
	return m_n[k];
}


Vector Element::J(/*double Xi, double Eta*/)const
{
	const Vertex& V1 = *m_pV1;
	const Vertex& V2 = *m_pV2;
	const Vertex& V3 = *m_pV3;
	Vector V31 = V1 - V3;
	Vector V32 = V2 - V3;

	Vector J_ = (V31 ^ V32);

	return J_;
}


double Element::Jn(/*double Xi, double Eta*/)const
{
	return J(/*Xi, Eta*/).Magnitude();
}




double Element::IU(const Vertex& SrcPt, int i, int j, int l)const
{
	return RegularIntegral(&Element::_U_, SrcPt, i, j, l);
}

double Element::IT(const Vertex& SrcPt, int i, int j, int l)const
{
	return RegularIntegral(&Element::_T_, SrcPt, i, j, l);
}


double Element::ID(const Vertex& SrcPt, int i, int j, int k, int l)const
{
	return RegularIntegral(&Element::_D_, SrcPt, i, j, k, l);
}


double Element::IS(const Vertex& SrcPt, int i, int j, int k, int l)const
{
	return RegularIntegral(&Element::_S_, SrcPt, i, j, k, l);
}

double Element::IW(const Vertex& SrcPt, int i, int j, int k, int l)const
{
	return RegularIntegral(&Element::_W_, SrcPt, i, j, k, l);
}


double Element::IV(const Vertex& SrcPt, int i, int j, int k, int l)const
{
	return RegularIntegral(&Element::_V_, SrcPt, i, j, k, l);
}


int Element::GeometricalIdx(int l)const
{
	switch (l)
	{
	case 0:
		return m_I1;

	case 1:
		return m_I2;

	case 2:
		return m_I3;

	default:
		break;
	}

	return -1; // Impossible values
}

int Element::DofIdx(int l)const
{
	switch (l)
	{
	case 0:
		return m_J1;

	case 1:
		return m_J2;

	case 2:
		return m_J3;

	default:
		break;
	}

	return -1; // Impossible values
}


void Element::SetDofIdx(int l, int Idx)
{
	switch (l)
	{
	case 0:
		m_J1 = Idx;
		break;

	case 1:
		m_J2 = Idx;
		break;

	case 2:
		m_J3 = Idx;
		break;

	default:
		break;
	}
}


void Element::UpdateGeometricalVertices()
{
	GetFieldPoint(m_pV1, 1.0, 0.0);
	GetFieldPoint(m_pV2, 0.0, 1.0);
	GetFieldPoint(m_pV3, 0.0, 0.0);
}


bool Element::GetIntersectingElm(const D3DXVECTOR3& P1, const D3DXVECTOR3& P2, float* pAlpha, const D3DXMATRIX& rWorld)const
{
	// Calculate the Intersection node between the Ray P1P2 and the Plan of the triangle
	// Calculate the normal vector n
	D3DXVECTOR4 _V1((float)m_pV1->x, (float)m_pV1->y, (float)m_pV1->z, 1.0f);
	D3DXVECTOR4 _V2((float)m_pV2->x, (float)m_pV2->y, (float)m_pV2->z, 1.0f);
	D3DXVECTOR4 _V3((float)m_pV3->x, (float)m_pV3->y, (float)m_pV3->z, 1.0f);


	D3DXVec4Transform(&_V1, &_V1, &rWorld);
	D3DXVec4Transform(&_V2, &_V2, &rWorld);
	D3DXVec4Transform(&_V3, &_V3, &rWorld);

	D3DXVECTOR3 V1(_V1.x, _V1.y, _V1.z);
	D3DXVECTOR3 V2(_V2.x, _V2.y, _V2.z);
	D3DXVECTOR3 V3(_V3.x, _V3.y, _V3.z);
	D3DXVECTOR3 n;

	D3DXVECTOR3 V31 = V1 - V3;
	D3DXVECTOR3 V32 = V2 - V3;

	D3DXVec3Cross(&n, &V31, &V32);


	D3DXVECTOR3 P1V3 = V3 - P1;
	D3DXVECTOR3 P1P2 = P2 - P1;
	float alpha = D3DXVec3Dot(&n, &P1V3) / D3DXVec3Dot(&n, &P1P2);

	if (alpha <= 0 || isnan(alpha))
		return false;

	// Finally calculate the intersection Point
	D3DXVECTOR3 I = P1 + alpha * P1P2;

	// Calculate the intrinsic coordinates
	D3DXVECTOR3 V3I = I - V3;
	float det = V31.x * V32.y - V31.y * V32.x;

	if (det == 0.0)
		return false;

	float Xi = (V3I.x * V32.y - V3I.y * V32.x) / det;
	float Eta = (V31.x * V3I.y - V31.y * V3I.x) / det;

	if (Xi < 0.0f)
		return false;
	else if (Eta < 0.0f)
		return false;
	else if (Xi > 1.0f)
		return false;
	else if (Eta > (1.0 - Xi))
		return false;

	if (*pAlpha <= alpha)
		return false;


	*pAlpha = alpha;
	return true;
}


double Element::GetValue(int l, OUTPUT_MODE om)const
{
	double Val = 0.0;
	const Vertex* pNode = nullptr;
	const Tensor* pSig = nullptr;
	const Tensor* pEps = nullptr;

	switch (l)
	{
	case 0:
		pNode = m_pV1;
		pSig = &m_Sig1;
		pEps = &m_Eps1;
		break;

	case 1:
		pNode = m_pV2;
		pSig = &m_Sig2;
		pEps = &m_Eps2;
		break;

	case 2:
		pNode = m_pV3;
		pSig = &m_Sig3;
		pEps = &m_Eps3;
		break;

	default:
		break;
	}


	if (pNode == nullptr)
		return 0;


	switch (om)
	{
	case BEM_3D::NONE:
		return 0;

	case BEM_3D::U_1:
		Val = pNode->U.x;
		break;

	case BEM_3D::U_2:
		Val = pNode->U.y;
		break;

	case BEM_3D::U_3:
		Val = pNode->U.z;
		break;

	case BEM_3D::U_M:
		Val = pNode->U.Magnitude();
		break;

	case BEM_3D::S_11:
		Val = pSig->_11;
		break;

	case BEM_3D::S_22:
		Val = pSig->_22;
		break;

	case BEM_3D::S_33:
		Val = pSig->_33;
		break;

	case BEM_3D::S_12:
		Val = pSig->_12;
		break;

	case BEM_3D::S_13:
		Val = pSig->_13;
		break;

	case BEM_3D::S_23:
		Val = pSig->_23;
		break;

	case BEM_3D::S_I:
		Val = pSig->Principal(0);
		break;

	case BEM_3D::S_II:
		Val = pSig->Principal(1);
		break;

	case BEM_3D::S_III:
		Val = pSig->Principal(2);
		break;

	case BEM_3D::S_VON_MISES:
		Val = pSig->VonMises();
		break;

	case BEM_3D::S_TRESCA:
		Val = pSig->Tresca();
		break;

	case BEM_3D::E_11:
		Val = pEps->_11;
		break;

	case BEM_3D::E_22:
		Val = pEps->_22;
		break;

	case BEM_3D::E_33:
		Val = pEps->_33;
		break;

	case BEM_3D::E_12:
		Val = pEps->_12;
		break;

	case BEM_3D::E_13:
		Val = pEps->_13;
		break;

	case BEM_3D::E_23:
		Val = pEps->_23;
		break;

	case BEM_3D::E_I:
		Val = pEps->Principal(0);
		break;

	case BEM_3D::E_II:
		Val = pEps->Principal(1);
		break;

	case BEM_3D::E_III:
		Val = pEps->Principal(2);
		break;

	default:
		break;
	}


	return Val;
}


bool Element::IsVertexPartOf(const Vertex* pV)const
{
	for (unsigned l = 0; l < 3; l++)
	{
		const Vertex* pVl = Vdof_Ptr(l);
		if (pVl == pV)
			return true;
	}

	
	return false;
}


bool Element::IsGeometricalVertexPartOf(const Vertex* pV)const
{
	if (pV == m_pV1 || pV == m_pV2 || pV == m_pV3)
		return true;

	return false;
}



bool Element::IsPointInside(const Vertex& DomainPt, double& Xi, double& Eta) const
{
	constexpr double tol = 1.0E-6;

	// 1. Get the longest edge length
	double L1 = m_pV1->Distance(*m_pV2);
	double L2 = m_pV2->Distance(*m_pV3);
	double L3 = m_pV3->Distance(*m_pV1);
	double Lmax = max(L1, max(L2, L3));

	if (Lmax <= 0.0)
		return false;

	// 2. Check vertex coincidence
	if (m_pV1->Distance(DomainPt) < (tol * Lmax)) return true;
	if (m_pV2->Distance(DomainPt) < (tol * Lmax)) return true;
	if (m_pV3->Distance(DomainPt) < (tol * Lmax)) return true;

	// 3. Vectors in local triangle system (origin at V3)
	Vector V = DomainPt - *m_pV3;
	Vector V1 = *m_pV1 - *m_pV3;
	Vector V2 = *m_pV2 - *m_pV3;

	// 4. Check whether the point lies on the triangle plane
	// m_n must be a unit normal
	Vector Vn = V;
	Vn.Normalize();

	if (fabs(Vn * m_n) > tol)
		return false;

	// 5. Compute barycentric coordinates
	double d00 = V1 * V1;
	double d01 = V1 * V2;
	double d11 = V2 * V2;
	double d20 = V * V1;
	double d21 = V * V2;

	double denom = d00 * d11 - d01 * d01;

	// 6. Check for degenerate triangle
	if (d00 <= 0.0 || d11 <= 0.0)
		return false;

	if (denom <= (tol * tol * d00 * d11))
		return false;

	// 7. Calculate intrinsic coordinates
	Xi = (d11 * d20 - d01 * d21) / denom;
	Eta = (d00 * d21 - d01 * d20) / denom;

	// 8. Point-in-triangle test
	return (Xi >= -tol) && (Eta >= -tol) &&	((Xi + Eta) <= (1.0 + tol));
}





int Element::GetVertexLocalIndex(const Vertex* pV)const
{
	for (unsigned l = 0; l < 3; l++)
	{
		const Vertex* pVl = Vdof_Ptr(l);
		if (pVl == pV)
			return l;
	}


	return -1; // impossible value
}


int Element::GetGeometricalVertexLocalIndex(const Vertex* pV)const
{
	if (pV == m_pV1)
		return 0;

	else if (pV == m_pV2)
		return 1;

	else if (pV == m_pV3)
		return 2;


	return -1; // impossible value
}


Vertex Element::GetCentroid()const
{
	double x = (m_pV1->x + m_pV2->x + m_pV3->x) / 3.0;
	double y = (m_pV1->y + m_pV2->y + m_pV3->y) / 3.0;
	double z = (m_pV1->z + m_pV2->z + m_pV3->z) / 3.0;

	return Vertex(x, y, z);
}


double Element::GetRadius2()const
{
	// Cet Centroïd 
	Vertex C = GetCentroid();

	// Calculate the Distances to the triangle nodes
	double R1 = m_pV1->Distance(C);
	double R2 = m_pV2->Distance(C);
	double R3 = m_pV3->Distance(C);
	// Square the distances
	double R1_2 = R1 * R1;
	double R2_2 = R2 * R2;
	double R3_2 = R3 * R3;

	double R23_2 = max(R2_2, R3_2);
	double R_2 = max(R1_2, R23_2);

	return R_2;
}



void Element::GetSig(Tensor& rSig, const Vector& P, double e11, double e22, double e12)
{
	double E = Element::E;
	double G = Element::G;
	double v = Element::v;
	double Lam = E * v / ((1.0 + v) * (1.0 - 2.0 * v));

	rSig._11 = 1.0 / (2.0 * G + Lam) * (4.0 * G * (G + Lam) * e11 + 2.0 * G * Lam * e22 + Lam * P.z);
	rSig._22 = 1.0 / (2.0 * G + Lam) * (4.0 * G * (G + Lam) * e22 + 2.0 * G * Lam * e11 + Lam * P.z);
	rSig._21 = rSig._12 = 2.0 * G * e12;

	rSig._31 = rSig._13 = P.x;
	rSig._32 = rSig._23 = P.y;
	rSig._33 = P.z;
}



void Element::GetEps(Tensor& rEps, const Tensor& rSig)
{
	double Lam = E * v / ((1.0 + v) * (1.0 - 2.0 * v));


	rEps._11 = 1.0 / E * (rSig._11 + v * (rSig._22 + rSig._33));
	rEps._22 = 1.0 / E * (rSig._22 + v * (rSig._11 + rSig._33));
	rEps._33 = 1.0 / E * (rSig._33 + v * (rSig._11 + rSig._22));

	rEps._12 = rEps._21 = (1.0 + v) / E * rSig._12;
	rEps._32 = rEps._23 = (1.0 + v) / E * rSig._23;
	rEps._31 = rEps._13 = (1.0 + v) / E * rSig._13;
}



//void Element::CalculateTensors()
//{
//	// Vertices
//	Vertex& Nd1 = Vgeo(0);
//	Vertex& Nd2 = Vgeo(1);
//	Vertex& Nd3 = Vgeo(2);
//
//	// Get References to displacments
//	const Vector& U1 = Nd1.U;
//	const Vector& U2 = Nd2.U;
//	const Vector& U3 = Nd3.U;
//
//	
//	// The Local unit vectors
//	Vector to = Nd1 - Nd3;
//	to.Normalize();
//	Vector b = m_n ^ to;
//	b.Normalize();
//
//	// Create the Transformation Matrix
//	Matrix Rt(to, b, m_n);  // Rt is the transform matrix from the global frame to local frame
//	Matrix R = Rt.Transpose(); // Rt is the transform matrix from the local frame to global frame 
//
//	// Geometric Jumps
//	Vector V13 = Nd1 - Nd3;
//	Vector V23 = Nd2 - Nd3;
//
//	// Displacement Jumps
//	Vector U13 = U1 - U3;
//	Vector U23 = U2 - U3;
//
//	// The local Geometry
//	Vertex _Z1 = Rt * V13;
//	Vertex _Z2 = Rt * V23;
//	Vertex _Z3(0, 0, 0);
//
//	// The local Displacements
//	Vector _V1 = Rt * U1;
//	Vector _V2 = Rt * U2;
//	Vector _V3 = Rt * U3;
//
//	// The local Displacement Jumps
//	Vector _V13 = _V1 - _V3;
//	Vector _V23 = _V2 - _V3;
//
//		
//	// The J matrix;
//	double j11 = _Z1.x;
//	double j12 = _Z1.y;
//
//	double j21 = _Z2.x;
//	double j22 = _Z2.y;
//	double J = j11 * j22 - j12 * j21;
//
//	// The J^-1 Matrix
//	double _j11 = j22 / J;
//	double _j12 = -j12 / J;
//	double _j21 = -j21 / J;
//	double _j22 = j11 / J;
//
//	// Calculate the local displacement derivatives
//	double dv1_dz1 = (_V13.x * _j11 + _V23.x * _j12);
//	double dv1_dz2 = (_V13.x * _j21 + _V23.x * _j22);
//
//	double dv2_dz1 = (_V13.y * _j11 + _V23.y * _j12);
//	double dv2_dz2 = (_V13.y * _j21 + _V23.y * _j22);
//
//	// Calculate the local strains
//	double e11 = dv1_dz1;
//	double e22 = dv2_dz2;
//	double e12 = (dv1_dz2 + dv2_dz1) / 2.0;
//
//	// Calculate the Local traction vectors
//	Vector P1 = Rt * m_T1;
//	Vector P2 = Rt * m_T2;
//	Vector P3 = Rt * m_T3;
//
//	// Calculate the Local stress Tensors
//	Tensor S1, S2, S3;
//
//	GetSig(S1, P1, e11, e22, e12);
//	GetSig(S2, P2, e11, e22, e12);
//	GetSig(S3, P3, e11, e22, e12);
//
//	// Calculate the Global stress tensors
//	m_Sig1 = R * S1 * Rt;
//	m_Sig2 = R * S2 * Rt;
//	m_Sig3 = R * S3 * Rt;
//
//	// Calculate the Global strain tensors
//	GetEps(m_Eps1, m_Sig1);
//	GetEps(m_Eps2, m_Sig2);
//	GetEps(m_Eps3, m_Sig3);
//}


void Element::CalculateTensors()
{
	// Vertices
	Vertex& Nd1 = Vdof(0);
	Vertex& Nd2 = Vdof(1);
	Vertex& Nd3 = Vdof(2);

	// Get References to displacments
	const Vector& U1 = Nd1.U;
	const Vector& U2 = Nd2.U;
	const Vector& U3 = Nd3.U;


	// The Local unit vectors
	Vector to = Nd1 - Nd3;
	to.Normalize();
	Vector b = m_n ^ to;
	b.Normalize();

	// Create the Transformation Matrix
	Matrix Rt(to, b, m_n);  // Rt is the transform matrix from the global frame to local frame
	Matrix R = Rt.Transpose(); // Rt is the transform matrix from the local frame to global frame 



	// Geometric Jumps
	Vector V13 = Nd1 - Nd3;
	Vector V23 = Nd2 - Nd3;

	// Displacement Jumps
	Vector U13 = U1 - U3;
	Vector U23 = U2 - U3;

	// The local Geometry
	Vertex _Z1 = Rt * V13;
	Vertex _Z2 = Rt * V23;
	Vertex _Z3(0, 0, 0);

	// The local Displacements
	Vector _V1 = Rt * U1;
	Vector _V2 = Rt * U2;
	Vector _V3 = Rt * U3;

	// The local Displacement Jumps
	Vector _V13 = _V1 - _V3;
	Vector _V23 = _V2 - _V3;

	double Cte = 0.0;

	switch (Type())
	{
	case ELEMENT_C:
		Cte = 1.0;
		break;

	case ELEMENT_D:
		Cte = 1.0 / (1.0 - 3.0 * _L);
		break;

	case ELEMENT_D11:
	case ELEMENT_D12:
	case ELEMENT_D13:
		Cte = 1.0 / (1.0 - 2.0 * _L);
		break;

	case ELEMENT_D21:
	case ELEMENT_D22:
	case ELEMENT_D23:
		Cte = 1.0 / ((1.0 - _L) * (1.0 - 3.0 * _L));
		break;
	}


	// The J matrix;
	double j11 = _Z1.x * Cte;
	double j12 = _Z1.y * Cte;
	double j21 = _Z2.x * Cte;
	double j22 = _Z2.y * Cte;

	double J = j11 * j22 - j12 * j21;

	// The J^-1 Matrix
	double _j11 = j22 / J;
	double _j12 = -j12 / J;
	double _j21 = -j21 / J;
	double _j22 = j11 / J;

	// Calculate the local displacement derivatives
	double dv1_dz1 = (_V13.x * _j11 + _V23.x * _j12) * Cte;
	double dv1_dz2 = (_V13.x * _j21 + _V23.x * _j22) * Cte;

	double dv2_dz1 = (_V13.y * _j11 + _V23.y * _j12) * Cte;
	double dv2_dz2 = (_V13.y * _j21 + _V23.y * _j22) * Cte;

	// Calculate the local strains
	double e11 = dv1_dz1;
	double e22 = dv2_dz2;
	double e12 = (dv1_dz2 + dv2_dz1) / 2.0;

	// Calculate the Local traction vectors
	Vector P1 = Rt * m_T1;
	Vector P2 = Rt * m_T2;
	Vector P3 = Rt * m_T3;

	// Calculate the Local stress Tensors
	Tensor S1, S2, S3;

	GetSig(S1, P1, e11, e22, e12);
	GetSig(S2, P2, e11, e22, e12);
	GetSig(S3, P3, e11, e22, e12);

	// Calculate the Global stress tensors
	m_Sig1 = R * S1 * Rt;
	m_Sig2 = R * S2 * Rt;
	m_Sig3 = R * S3 * Rt;

	// if the element is discontinuous Transform the stress tensor from internal nodes to geometrical nodes
	if (Type() == ELEMENT_D)
	{
		Tensor Sig1 = m_Sig1 * N1(0.0, 1.0) + m_Sig2 * N2(0.0, 1.0) + m_Sig3 * N3(0.0, 1.0);
		Tensor Sig2 = m_Sig1 * N1(1.0, 0.0) + m_Sig2 * N2(1.0, 0.0) + m_Sig3 * N3(1.0, 0.0);
		Tensor Sig3 = m_Sig1 * N1(0.0, 0.0) + m_Sig2 * N2(0.0, 0.0) + m_Sig3 * N3(0.0, 0.0);

		m_Sig1 = Sig1;
		m_Sig2 = Sig2;
		m_Sig3 = Sig3;
	}


	// Calculate the Global strain tensors
	GetEps(m_Eps1, m_Sig1);
	GetEps(m_Eps2, m_Sig2);
	GetEps(m_Eps3, m_Sig3);
}


Vector Element::A_Theta(const Vertex& V1, const Vertex& V2, const Vertex& V3, double Theta)const
{
	Vector e1 = V2 - V1;
	Vector e2 = V3 - V1;

	return (e1 * cos(Theta)) + (e2 * sin(Theta));
}


const Vertex& Element::Vgeo(int l) const
{
	switch (l)
	{
	case 0:
		return *m_pV1;

	case 1:
		return *m_pV2;

	case 2:
		return *m_pV3;

	default:
		break;
	}

	return Vertex(); // Impossible values
}

Vertex& Element::Vgeo(int l)
{
	switch (l)
	{
	case 0:
		return *m_pV1;

	case 1:
		return *m_pV2;

	case 2:
		return *m_pV3;

	default:
		break;
	}

	return Vertex(); // Impossible values
}

const Vertex* Element::Vgeo_Ptr(int l) const
{
	switch (l)
	{
	case 0:
		return m_pV1;

	case 1:
		return m_pV2;

	case 2:
		return m_pV3;

	default:
		break;
	}

	return nullptr; // Impossible values
}

Vertex* Element::Vgeo_Ptr(int l)
{
	switch (l)
	{
	case 0:
		return m_pV1;

	case 1:
		return m_pV2;

	case 2:
		return m_pV3;

	default:
		break;
	}

	return nullptr; // Impossible values
}



bool Element::SelectIntegrationRule(const Vertex& SrcPt, int nKernelType, /*OUT*/ INTEG_RULE& rIntegRule)const 
{
	if (!m_bUseAdaptiveCriterion)
	{
		rIntegRule = m_FixedIntegRule;
		return true;
	}
	else
	{
		// Get the Shortest Distance to element
		double Dmin = GetShortestDistance(SrcPt);

		
		// Get the Longest Edge length
		double L1 = m_pV1->Distance(*m_pV2);
		double L2 = m_pV2->Distance(*m_pV3);
		double L3 = m_pV3->Distance(*m_pV1);
		double Lmax = max(L1, max(L2, L3));

		// Calculate Lambda ratio
		double Lambda = Dmin / Lmax;

		INTEG_RULE rule;


		switch (m_CriterionPrec)
		{
		case _0_001:
		{
			switch (nKernelType)
			{
			case 0:
			{
				if (Lambda > 6.482818737) //
					rule = _001_PT;
				else if (Lambda > 2.720150383) //
					rule = _003_PT;
				else if (Lambda > 1.225265571) //
					rule = _004_PT;
				else if (Lambda > 0.559204975) //
					rule = _007_PT;
				else if (Lambda > 0.366938171) //
					rule = _013_PT;
				else if (Lambda > 0.225875632) //
					rule = _028_PT;
				else if (Lambda > 0.153663995) //
					rule = _052_PT;
				else if (Lambda > 0.095455245) //
					rule = _112_PT;
				else if (Lambda > 0.06415003) //
					rule = _208_PT;
				else
					return false;
			}
			break;

			case 1:
			{
				if (Lambda > 9.168322275) //
					rule = _001_PT;
				else if (Lambda > 4.13715789) //
					rule = _003_PT;
				else if (Lambda > 1.581939738) //
					rule = _004_PT;
				else if (Lambda > 0.712065332) //
					rule = _007_PT;
				else if (Lambda > 0.466321371) //
					rule = _013_PT;
				else if (Lambda > 0.303686242) //
					rule = _028_PT;
				else if (Lambda > 0.208808347) //
					rule = _052_PT;
				else if (Lambda > 0.141038423) //
					rule = _112_PT;
				else if (Lambda > 0.099304246) //
					rule = _208_PT;
				else
					return false;
			}
			break;

			case 2:
			{
				if (Lambda > 13.402833) //
					rule = _001_PT;
				else if (Lambda > 5.44863624) //
					rule = _003_PT;
				else if (Lambda > 1.860844329) //
					rule = _004_PT;
				else if (Lambda > 0.829074987) //
					rule = _007_PT;
				else if (Lambda > 0.543924727) //
					rule = _013_PT;
				else if (Lambda > 0.362575969) //
					rule = _028_PT;
				else if (Lambda > 0.251147367) //
					rule = _052_PT;
				else if (Lambda > 0.174167331) //
					rule = _112_PT;
				else if (Lambda > 0.122893129) //
					rule = _208_PT;
				else
					return false;
			}
			break;
			}

			rIntegRule = rule;
			return true;
		}
		break;

		case _0_0001:
		{
			switch (nKernelType)
			{
			case 0:
			{
				if (Lambda > 20.49734626)
					rule = _001_PT;
				else if (Lambda > 6.194320367)
					rule = _003_PT;
				else if (Lambda > 2.220174217)
					rule = _004_PT;
				else if (Lambda > 0.896047618)
					rule = _007_PT;
				else if (Lambda > 0.560637498)
					rule = _013_PT;
				else if (Lambda > 0.362768419)
					rule = _028_PT;
				else if (Lambda > 0.240837541)
					rule = _052_PT;
				else if (Lambda > 0.154441197)
					rule = _112_PT;
				else if (Lambda > 0.105206049)
					rule = _208_PT;
				else
					return false;
			}
			break;

			case 1:
			{
				if (Lambda > 29.01185103)
					rule = _001_PT;
				else if (Lambda > 9.258460969)
					rule = _003_PT;
				else if (Lambda > 2.845997209)
					rule = _004_PT;
				else if (Lambda > 1.110087108)
					rule = _007_PT;
				else if (Lambda > 0.687871606)
					rule = _013_PT;
				else if (Lambda > 0.457261413)
					rule = _028_PT;
				else if (Lambda > 0.306820429)
					rule = _052_PT;
				else if (Lambda > 0.209289473)
					rule = _112_PT;
				else if (Lambda > 0.144914918)
					rule = _208_PT;
				else
					return false;
			}
			break;

			case 2:
			{
				if (Lambda > 43.16330168)
					rule = _001_PT;
				else if (Lambda > 12.10052284)
					rule = _003_PT;
				else if (Lambda > 3.551173515)
					rule = _004_PT;
				else if (Lambda > 1.311614234)
					rule = _007_PT;
				else if (Lambda > 0.787120867)
					rule = _013_PT;
				else if (Lambda > 0.573761664)
					rule = _028_PT;
				else if (Lambda > 0.358845398)
					rule = _052_PT;
				else if (Lambda > 0.26522028)
					rule = _112_PT;
				else if (Lambda > 0.173884316)
					rule = _208_PT;
				else
					return false;
			}
			break;
			}

			rIntegRule = rule;
			return true;
		}
		break;
		}

	}

	return false;  // Impossible value
}


double Element::RegularIntegral(double (Element::* Func)(const Vertex&, int, int, int, int, int)const,
	const Vertex& SrcPt, int i, int j, int l)const
{  
	double I = 0.0; 

	int nKernelType = 0;

	if (Func == (&Element::U_))
		nKernelType = 0;
	else if (Func == (&Element::T_))
		nKernelType = 1;
	

	INTEG_RULE rule;
	if (SelectIntegrationRule(SrcPt, nKernelType, rule))
	{
		int N = RuleToSize(rule);
		int Idx = RuleToIndex(rule);

		for (size_t t = 0; t < N; t++)
		{
			double W = ITNDs[Idx][t].W;

			I += (this->*Func)(SrcPt, Idx, t, i, j, l) * W;
		}

		I *= 0.5;
	}
	else
	{
		for (size_t t = 0; t < 448; t++)
		{
			double W = Max_ITND[t].W;
			double Xi = Max_ITND[t].Xi;
			double Eta = Max_ITND[t].Eta;

			if (Func == (&Element::U_))
				I += _U_(SrcPt, Xi, Eta, i, j, l) * W;
			else if (Func == (&Element::T_))
				I += _T_(SrcPt, Xi, Eta, i, j, l) * W;
		}

		I *= 0.5;
	}
	/*else
	{
		if (Func == (&Element::U_))
			I = RegularIntegral(&Element::_U_, SrcPt, i, j, l);
		else if (Func == (&Element::T_))
			I = RegularIntegral(&Element::_T_, SrcPt, i, j, l);
	}*/	

	return I;
}


double Element::RegularIntegral(double (Element::* Func)(const Vertex&, int, int, int, int, int, int)const,
	const Vertex& SrcPt, int i, int j, int k, int l)const
{
	double I = 0.0;
	

	int nKernelType = 0;

	if (Func == (&Element::D_))
		nKernelType = 1;
	else if (Func == (&Element::S_))
		nKernelType = 2;
	else if (Func == (&Element::W_))
		nKernelType = 1;
	else if (Func == (&Element::V_))
		nKernelType = 2;

	INTEG_RULE rule;
	if (SelectIntegrationRule(SrcPt, nKernelType, rule))
	{
		int N = RuleToSize(rule);
		int Idx = RuleToIndex(rule);

		for (size_t t = 0; t < N; t++)
		{
			double W = ITNDs[Idx][t].W;

			I += (this->*Func)(SrcPt, Idx, t, i, j, k, l) * W;
		}

		I *= 0.5;
	}
	else
	{
		for (size_t t = 0; t < 448; t++)
		{
			double W = Max_ITND[t].W;
			double Xi = Max_ITND[t].Xi;
			double Eta = Max_ITND[t].Eta;


			if (Func == (&Element::D_))
				I += _D_(SrcPt, Xi, Eta, i, j, k, l) * W;
			else if (Func == (&Element::S_))
				I += _S_(SrcPt, Xi, Eta, i, j, k, l) * W;
			else if (Func == (&Element::W_))
				I += _W_(SrcPt, Xi, Eta, i, j, k, l) * W;
			else if (Func == (&Element::V_))
				I += _V_(SrcPt, Xi, Eta, i, j, k, l) * W;
		}

		I *= 0.5;
	}
	/*else
	{
		if (Func == (&Element::D_))
			I = RegularIntegral(&Element::_D_, SrcPt, i, j, k, l);
		else if (Func == (&Element::S_))
			I = RegularIntegral(&Element::_S_, SrcPt, i, j, k, l);
		else if (Func == (&Element::W_))
			I = RegularIntegral(&Element::_W_, SrcPt, i, j, k, l);
		else if (Func == (&Element::V_))
			I = RegularIntegral(&Element::_V_, SrcPt, i, j, k, l);
	}*/

	return I;
}



double Element::RegularIntegral(double (Element::* Func)(const Vertex&, double, double, int, int, int)const,
	const Vertex& SrcPt, int i, int j, int l)const
{
	double I = 0.0;

	int nKernelType = 0;

	if (Func == (&Element::_U_))
		nKernelType = 0;
	else if (Func == (&Element::_T_))
		nKernelType = 1;


	INTEG_RULE rule;
	if (SelectIntegrationRule(SrcPt, nKernelType, rule))
	{
		int N = RuleToSize(rule);
		int Idx = RuleToIndex(rule);

		for (size_t t = 0; t < N; t++)
		{
			double Xi = ITNDs[Idx][t].Xi;
			double Eta = ITNDs[Idx][t].Eta;
			double W = ITNDs[Idx][t].W;

			I += (this->*Func)(SrcPt, Xi, Eta, i, j, l) * W;
		}

		I *= 0.5;
	}
	else // Subdivide the element
	{
		Vertex& V1 = *m_pV1;
		Vertex& V2 = *m_pV2;
		Vertex& V3 = *m_pV3;
		Vertex Va = (V1 + V2) / 2.0;
		Vertex Vb = (V2 + V3) / 2.0;
		Vertex Vc = (V3 + V1) / 2.0;

		Element_C E1(V1, Va, Vc);
		Element_C E2(Va, V2, Vb);
		Element_C E3(Vc, Vb, V3);
		Element_C E4(Va, Vb, Vc);

		I += E1.RegularIntegral(Func, SrcPt, i, j, l);
		I += E2.RegularIntegral(Func, SrcPt, i, j, l);
		I += E3.RegularIntegral(Func, SrcPt, i, j, l);
		I += E4.RegularIntegral(Func, SrcPt, i, j, l);
	}

	return I;
}

double Element::RegularIntegral(double (Element::* Func)(const Vertex&, double, double, int, int, int, int)const,
	const Vertex& SrcPt, int i, int j, int k, int l)const
{
	double I = 0.0;


	int nKernelType = 0;

	if (Func == (&Element::_D_))
		nKernelType = 1;
	else if (Func == (&Element::_S_))
		nKernelType = 2;
	else if (Func == (&Element::_W_))
		nKernelType = 1;
	else if (Func == (&Element::_V_))
		nKernelType = 2;

	INTEG_RULE rule;
	if (SelectIntegrationRule(SrcPt, nKernelType, rule))
	{
		int N = RuleToSize(rule);
		int Idx = RuleToIndex(rule);

		for (size_t t = 0; t < N; t++)
		{
			double Xi = ITNDs[Idx][t].Xi;
			double Eta = ITNDs[Idx][t].Eta;
			double W = ITNDs[Idx][t].W;

			I += (this->*Func)(SrcPt, Xi, Eta, i, j, k, l) * W;
		}

		I *= 0.5;
	}
	else // Subdivide the element
	{
		Vertex& V1 = *m_pV1;
		Vertex& V2 = *m_pV2;
		Vertex& V3 = *m_pV3;
		Vertex Va = (V1 + V2) / 2.0;
		Vertex Vb = (V2 + V3) / 2.0;
		Vertex Vc = (V3 + V1) / 2.0;

		Element_C E1(V1, Va, Vc);
		Element_C E2(Va, V2, Vb);
		Element_C E3(Vc, Vb, V3);
		Element_C E4(Va, Vb, Vc);

		I += E1.RegularIntegral(Func, SrcPt, i, j, k, l);
		I += E2.RegularIntegral(Func, SrcPt, i, j, k, l);
		I += E3.RegularIntegral(Func, SrcPt, i, j, k, l);
		I += E4.RegularIntegral(Func, SrcPt, i, j, k, l);
	}

	return I;
}




Vertex Element::GetClosestPointOnElement(const Vertex& p) const
{
	// Prepare the needed vectors
	const Vertex& a = *m_pV1;
	const Vertex& b = *m_pV2;
	const Vertex& c = *m_pV3;

	// Check if P in vertex region outside A
	Vector ab = b - a;
	Vector ac = c - a;
	Vector ap = p - a;
	double d1 = ab * ap;
	double d2 = ac * ap;
	if (d1 <= 0.0 && d2 <= 0.0) return a; // barycentric coordinates (1,0,0)
	// Check if P in vertex region outside B
	Vector bp = p - b;
	double d3 = ab * bp;
	double d4 = ac * bp;
	if (d3 >= 0.0 && d4 <= d3) return b; // barycentric coordinates (0,1,0)
	// Check if P in edge region of AB, if so return projection of P onto AB
	double vc = d1 * d4 - d3 * d2;
	if (vc <= 0.0 && d1 >= 0.0 && d3 <= 0.0) {
		double v = d1 / (d1 - d3);
		return a + ab * v; // barycentric coordinates (1-v,v,0)
	}
	// Check if P in vertex region outside C
	Vector cp = p - c;
	double d5 = ab * cp;
	double d6 = ac * cp;
	if (d6 >= 0.0 && d5 <= d6) return c; // barycentric coordinates (0,0,1)

	// Check if P in edge region of AC, if so return projection of P onto AC
	double vb = d5 * d2 - d1 * d6;
	if (vb <= 0.0 && d2 >= 0.0 && d6 <= 0.0) {
		double w = d2 / (d2 - d6);
		return a + ac * w; // barycentric coordinates (1-w,0,w)
	}
	// Check if P in edge region of BC, if so return projection of P onto BC
	double va = d3 * d6 - d5 * d4;
	if (va <= 0.0 && (d4 - d3) >= 0.0 && (d5 - d6) >= 0.0) {
		double w = (d4 - d3) / ((d4 - d3) + (d5 - d6));
		return b + (c - b) * w; // barycentric coordinates (0,1-w,w)
	}
	// P inside face region. Compute Q through its barycentric coordinates (u,v,w)
	double denom = 1.0 / (va + vb + vc);
	double v = vb * denom;
	double w = vc * denom;
	return a + ab * v + ac * w; // = u*a + v*b + w*c, u = va * denom = 1.0 - v - w
}

double Element::GetShortestDistance(const Vertex& P) const
{
	Vertex Q = GetClosestPointOnElement(P);
	return (P - Q).Magnitude();
}


//===================================================================================================
void Element::Serialize(CArchive& ar)
{
	m_n.Serialize(ar);
	m_T1.Serialize(ar);
	m_T2.Serialize(ar);
	m_T3.Serialize(ar);
	m_Sig1.Serialize(ar);
	m_Sig2.Serialize(ar);
	m_Sig3.Serialize(ar);
	m_Eps1.Serialize(ar);
	m_Eps2.Serialize(ar);
	m_Eps3.Serialize(ar);


	if (ar.IsStoring())
	{
		ar << m_I1;
		ar << m_I2;
		ar << m_I3;
		ar << m_J;
	}
	else
	{
		// TODO: add loading code here
		ar >> m_I1;
		ar >> m_I2;
		ar >> m_I3;
		ar >> m_J;
	}
}