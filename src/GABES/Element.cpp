#include "pch.h"
#include "Element.h"

using namespace BEM_3D;




double Element::E = 200.0E3;
double Element::v = 0.3;
double Element::G = E / (2.0 * (1.0 + v));


INTEG_RULE Element::m_FixedIntegRule = _056_PT;
bool Element::m_bRigidBodyCPV = true;         // We use the Rigid body motion By default!!! Because our numerical implementation of Direct CPV Evaluation is not fully implemented!!
bool Element::m_bUseAdaptiveCriterion = true; // The use of the adaptive scheme yields better results and performance

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
{
	CalculateNormal();
	Element::FillIntegrationNodes();	
}



void Element::CalculateNormal()
{
	Vector V31 = *m_pV1 - *m_pV3;
	Vector V32 = *m_pV2 - *m_pV3;
	m_n = V31 ^ V32;
	m_n.Normalize();
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



double Element::KD(int i, int j)
{
	return i == j ? 1.0 : 0.0;
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


double Element::r(const Vertex& SrcPt, const Vertex& FldPt)
{
	return SrcPt.Distance(FldPt);
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


double Element::n(double Xi, double Eta, int k)const
{
	return m_n[k];
}


Vector Element::J(double Xi, double Eta)const
{
	const Vertex& V1 = *m_pV1;
	const Vertex& V2 = *m_pV2;
	const Vertex& V3 = *m_pV3;
	Vector V31 = V1 - V3;
	Vector V32 = V2 - V3;

	Vector J_ = (V31 ^ V32);

	return J_;
}


double Element::Jn(double Xi, double Eta)const
{
	return J(Xi, Eta).Magnitude();
}



double Element::U(const Vertex& SrcPt, int nRule, int nNode, int i, int j)const
{
	const Vertex& FldPt = FldPts[nRule][nNode];    // uses FldPts which is common from element types

	double R = r(SrcPt, FldPt);                    // uses r which is common from element types
	double r_i = dr(SrcPt, FldPt, i);           // uses dr which is common from element types
	double r_j = dr(SrcPt, FldPt, j);           
	double ij = KD(i, j);                          // uses KD which is common from element types

	return 1.0 / (16.0 * M_PI * G * (1.0 - v) * R) * ((3.0 - 4.0 * v) * ij + r_i * r_j);
}

double Element::T(const Vertex& SrcPt, int nRule, int nNode, int i, int j)const
{
	const Vertex& FldPt = FldPts[nRule][nNode];	

	double R = r(SrcPt, FldPt);
	double r_i = dr(SrcPt, FldPt, i);
	double r_j = dr(SrcPt, FldPt, j);
	double ij = KD(i, j);
	double dR_dn = dr_dn(SrcPt, FldPt);
	double ni = m_n[i];
	double nj = m_n[j];

	if (R == 0.0)
		return 0.0;


	return -1.0 / (8.0 * M_PI * (1.0 - v) * R * R) * (dR_dn * ((1.0 - 2.0 * v) * ij + 3.0 * r_i * r_j) - (1.0 - 2.0 * v) * (nj * r_i - ni * r_j));
}


double Element::D(const Vertex& SrcPt, int nRule, int nNode, int i, int j, int k)const
{
	const Vertex& FldPt = FldPts[nRule][nNode];    // uses FldPts which is common from element types

	double R = r(SrcPt, FldPt);                    // uses r which is common from element types
	double r2 = R * R;
	double r_i = dr(SrcPt, FldPt, i);           // uses dr which is common from element types
	double r_j = dr(SrcPt, FldPt, j);
	double r_k = dr(SrcPt, FldPt, k);
	double ij = KD(i, j);                          // uses KD which is common from element types
	double ik = KD(i, k);
	double jk = KD(j, k);

	return 1.0 / (8.0 * M_PI * (1.0 - v) * r2) * ((1.0 - 2.0 * v) * (ik + r_j + jk * r_i - ij * r_k) + 3.0 * r_i * r_j * r_k);
}

double Element::S(const Vertex& SrcPt, int nRule, int nNode, int i, int j, int k)const
{
	const Vertex& FldPt = FldPts[nRule][nNode];

	double R = r(SrcPt, FldPt);
	double r3 = R * R * R;

	double r_i = dr(SrcPt, FldPt, i);
	double r_j = dr(SrcPt, FldPt, j);
	double r_k = dr(SrcPt, FldPt, k);
	double ij = KD(i, j);                          // uses KD which is common from element types
	double ik = KD(i, k);
	double jk = KD(j, k);
	double dR_dn = dr_dn(SrcPt, FldPt);
	double ni = m_n[i];
	double nj = m_n[j];
	double nk = m_n[k];


	return G / (4.0 * M_PI * (1.0 - v) * r3) * (3.0 * dR_dn * ((1.0 - 2.0 * v) * ij * r_k + v * (ik * r_j + jk * r_i) -5.0 * r_i * r_j * r_k)
		+ 3.0 * v * (ni * r_j * r_k + nj * r_i * r_k) + (1.0 - 2.0 * v) * (3.0 * nk * r_i * r_j + nj * ik + ni * jk) - (1.0 - 4.0 * v) * nk * ij);
}



double Element::W(const Vertex& SrcPt, int nRule, int nNode, int i, int j, int k)const
{
	const Vertex& FldPt = FldPts[nRule][nNode];    // uses FldPts which is common from element types

	double R = r(SrcPt, FldPt);                    // uses r which is common from element types
	double r2 = R * R;
	double r_i = dr(SrcPt, FldPt, i);           // uses dr which is common from element types
	double r_j = dr(SrcPt, FldPt, j);
	double r_k = dr(SrcPt, FldPt, k);
	double ij = KD(i, j);                          // uses KD which is common from element types
	double ik = KD(i, k);
	double jk = KD(j, k);

	return 1.0 / (16.0 * M_PI * G * (1.0 - v) * r2) * ((3.0 - 4.0 * v) * r_k * ij - r_i * jk - r_j * ik + 3.0 * r_i * r_j * r_k);
}

double Element::V(const Vertex& SrcPt, int nRule, int nNode, int i, int j, int k)const
{
	const Vertex& FldPt = FldPts[nRule][nNode];

	double R = r(SrcPt, FldPt);
	double r3 = R * R * R;

	double r_i = dr(SrcPt, FldPt, i);
	double r_j = dr(SrcPt, FldPt, j);
	double r_k = dr(SrcPt, FldPt, k);
	double ij = KD(i, j);                          // uses KD which is common from element types
	double ik = KD(i, k);
	double jk = KD(j, k);
	double dR_dn = dr_dn(SrcPt, FldPt);
	double ni = m_n[i];
	double nj = m_n[j];
	double nk = m_n[k];


	return G / (8.0 * M_PI * (1.0 - v) * r3) * (3.0 * dR_dn * ((r_i * jk + r_j * ik) - (1.0 - 2.0 * v) * r_k * ij - 5.0 * r_i * r_j * r_k)
		+ (1.0 - 2.0 * v) * (jk * ni + ij * nk - ik * nj) + 3.0 * r_j * r_i * nk + 3.0 * (1.0 - 2.0 * v) * (r_i * r_k * nj - r_j * r_k * ni));
}



double Element::U_(const Vertex& SrcPt, int nRule, int nNode, int i, int j, int l)const
{
	double Nl = 0.0;

	switch (l)
	{
	case 0:
		Nl = N1s(nRule, nNode);
		break;

	case 1:
		Nl = N2s(nRule, nNode);
		break;

	case 2:
		Nl = N3s(nRule, nNode);
		break;

	}

	return U(SrcPt, nRule, nNode, i, j) * Nl * Js[nRule][nNode];
}

double Element::T_(const Vertex& SrcPt, int nRule, int nNode, int i, int j, int l)const
{
	double Nl = 0.0;

	switch (l)
	{
	case 0:
		Nl = N1s(nRule, nNode);
		break;

	case 1:
		Nl = N2s(nRule, nNode);
		break;

	case 2:
		Nl = N3s(nRule, nNode);
		break;

	}	

	return T(SrcPt, nRule, nNode, i, j) * Nl * Js[nRule][nNode];
}


double Element::D_(const Vertex& SrcPt, int nRule, int nNode, int i, int j, int k, int l)const
{
	double Nl = 0.0;

	switch (l)
	{
	case 0:
		Nl = N1s(nRule, nNode);
		break;

	case 1:
		Nl = N2s(nRule, nNode);
		break;

	case 2:
		Nl = N3s(nRule, nNode);
		break;

	}

	return D(SrcPt, nRule, nNode, i, j, k) * Nl * Js[nRule][nNode];
}

double Element::S_(const Vertex& SrcPt, int nRule, int nNode, int i, int j, int k, int l)const
{
	double Nl = 0.0;

	switch (l)
	{
	case 0:
		Nl = N1s(nRule, nNode);
		break;

	case 1:
		Nl = N2s(nRule, nNode);
		break;

	case 2:
		Nl = N3s(nRule, nNode);
		break;

	}

	return S(SrcPt, nRule, nNode, i, j, k) * Nl * Js[nRule][nNode];
}



double Element::W_(const Vertex& SrcPt, int nRule, int nNode, int i, int j, int k, int l)const
{
	double Nl = 0.0;

	switch (l)
	{
	case 0:
		Nl = N1s(nRule, nNode);
		break;

	case 1:
		Nl = N2s(nRule, nNode);
		break;

	case 2:
		Nl = N3s(nRule, nNode);
		break;

	}

	return W(SrcPt, nRule, nNode, i, j, k) * Nl * Js[nRule][nNode];
}

double Element::V_(const Vertex& SrcPt, int nRule, int nNode, int i, int j, int k, int l)const
{
	double Nl = 0.0;

	switch (l)
	{
	case 0:
		Nl = N1s(nRule, nNode);
		break;

	case 1:
		Nl = N2s(nRule, nNode);
		break;

	case 2:
		Nl = N3s(nRule, nNode);
		break;

	}

	return V(SrcPt, nRule, nNode, i, j, k) * Nl * Js[nRule][nNode];
}


double Element::ID(const Vertex& SrcPt, int i, int j, int k, int l)const
{
	int N = 0;
	int Idx = 0;
	double I = 0.0;

	INTEG_RULE rule = SelectIntegrationRule(SrcPt, 1);

	N = RuleToSize(rule);
	Idx = RuleToIndex(rule);

	for (size_t t = 0; t < N; t++)
	{
		double W = ITNDs[Idx][t].W;

		I += D_(SrcPt, Idx, t, i, j, k, l) * W;
	}

	I *= 0.5;

	return I;
}


double Element::IS(const Vertex& SrcPt, int i, int j, int k, int l)const
{
	int N = 0;
	int Idx = 0;
	double I = 0.0;

	INTEG_RULE rule = SelectIntegrationRule(SrcPt, 1);

	N = RuleToSize(rule);
	Idx = RuleToIndex(rule);

	for (size_t t = 0; t < N; t++)
	{
		double W = ITNDs[Idx][t].W;

		I += S_(SrcPt, Idx, t, i, j, k, l) * W;
	}

	I *= 0.5;

	return I;
}

double Element::IW(const Vertex& SrcPt, int i, int j, int k, int l)const
{
	int N = 0;
	int Idx = 0;
	double I = 0.0;

	INTEG_RULE rule = SelectIntegrationRule(SrcPt, 1);

	N = RuleToSize(rule);
	Idx = RuleToIndex(rule);

	for (size_t t = 0; t < N; t++)
	{
		double W = ITNDs[Idx][t].W;

		I += W_(SrcPt, Idx, t, i, j, k, l) * W;
	}

	I *= 0.5;

	return I;
}


double Element::IV(const Vertex& SrcPt, int i, int j, int k, int l)const
{
	int N = 0;
	int Idx = 0;
	double I = 0.0;

	INTEG_RULE rule = SelectIntegrationRule(SrcPt, 1);

	N = RuleToSize(rule);
	Idx = RuleToIndex(rule);

	for (size_t t = 0; t < N; t++)
	{
		double W = ITNDs[Idx][t].W;

		I += V_(SrcPt, Idx, t, i, j, k, l) * W;
	}

	I *= 0.5;

	return I;
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


void Element::DrawTotalEncastrement(IDirect3DDevice9* pD3ddev, bool bPostTreatment)const
{
	// Create the intrinsic vertices of the fixation
	Vector p1(0.0, 0.0, 0.0);
	Vector p2(- 1.5 / 10.0, 0.0, 1.5 / 5.0);
	Vector p3(1.5 / 10.0, 0.0, 1.5 / 5.0);

    
	Vertex C = GetFieldPoint(1.0 / 3.0, 1.0 / 3.0);


	// Calculate the three Distances to converte intrinsic coordinate to Real Ones

	
	if (bPostTreatment && Vertex::m_bIncludeDisplacements)
	{
		double Scale = Vertex::m_DeformationScale;
		Vector Uc = Vdof(0).U * N1(1.0 / 3.0, 1.0 / 3.0) + 
			        Vdof(1).U * N2(1.0 / 3.0, 1.0 / 3.0) + 
			        Vdof(2).U * N3(1.0 / 3.0, 1.0 / 3.0);

		C +=  Uc * Scale;
	}
	

	Vector V31 = *m_pV1 - *m_pV3;
	Vector V32 = *m_pV2 - *m_pV3;
	Vector Vn = V31 ^ V32;
	double Ln = sqrt(Vn.Magnitude());


	
	p1 = C + V31 * p1.x + V32 * p1.y + m_n * p1.z * Ln;
	p2 = C + V31 * p2.x + V32 * p2.y + m_n * p2.z * Ln;
	p3 = C + V31 * p3.x + V32 * p3.y + m_n * p3.z * Ln;

	// Create the Buffer
	D3D_RGB_Vertex vertices[] = { 
		{(float)p1.x, (float)p1.y, (float)p1.z, D3DCOLOR_XRGB(0, 255, 0)},
		{(float)p2.x, (float)p2.y, (float)p2.z, D3DCOLOR_XRGB(0, 255, 0)},
		{(float)p3.x, (float)p3.y, (float)p3.z, D3DCOLOR_XRGB(0, 255, 0)}
	};

	// Create the vertex Buffer
	IDirect3DVertexBuffer9* tempVertexBuffer = nullptr;
	pD3ddev->CreateVertexBuffer(sizeof(vertices), 0, g_RGB_FVF, D3DPOOL_DEFAULT, &tempVertexBuffer, NULL);

	// Lock the Vertex Buffer and Fill It
	LPVOID pLockMem = nullptr;
	tempVertexBuffer->Lock(0, sizeof(vertices), &pLockMem, D3DLOCK_DISCARD);
	CopyMemory(pLockMem, vertices, sizeof(vertices));
	tempVertexBuffer->Unlock();

	// Set the Stream source and FVF
	pD3ddev->SetStreamSource(0, tempVertexBuffer, 0, sizeof(D3D_RGB_Vertex));
	pD3ddev->SetFVF(g_RGB_FVF);


	//Get The {Lighting,CullMode & FillMode} States
	DWORD dwLightingState, dwCullModeState, dwFillModeState;
	pD3ddev->GetRenderState(D3DRS_LIGHTING, &dwLightingState);
	pD3ddev->GetRenderState(D3DRS_CULLMODE, &dwCullModeState);
	pD3ddev->GetRenderState(D3DRS_FILLMODE, &dwFillModeState);
	// Switch off Lighting Temporarily
	pD3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);
	// Make Cull Mode State to None Temporarily
	pD3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// Make Fill Mode State to Solid Temporarily
	pD3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);


	// Draw the Fixation
	pD3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

	// Restore {Lighting, CullMode & FillMode} states
	pD3ddev->SetRenderState(D3DRS_LIGHTING, dwLightingState);
	pD3ddev->SetRenderState(D3DRS_CULLMODE, dwCullModeState);
	pD3ddev->SetRenderState(D3DRS_FILLMODE, dwFillModeState);

	// delete the temporary vertex Buffer
	tempVertexBuffer->Release();
}

void Element::DrawPartialEncastrement(IDirect3DDevice9* pD3ddev, bool bPostTreatment)const
{
	// Create the intrinsic vertices of the fixation
	Vector p1(0.0, 0.0, 0.0);
	Vector p2(- 1.5 / 10.0, 0.0, 1.5 / 5.0);
	Vector p3( 1.5 / 10.0, 0.0, 1.5 / 5.0 );


	Vertex C = GetFieldPoint(1.0 / 3.0, 1.0 / 3.0);


	// Calculate the three Distances to converte intrinsic coordinate to Real Ones
	if (bPostTreatment && Vertex::m_bIncludeDisplacements)
	{
		double Scale = Vertex::m_DeformationScale;
		Vector Uc = Vdof(0).U * N1(1.0 / 3.0, 1.0 / 3.0) +
			        Vdof(1).U * N2(1.0 / 3.0, 1.0 / 3.0) +
			        Vdof(2).U * N3(1.0 / 3.0, 1.0 / 3.0);

		C += Uc * Scale;
	}


	Vector V31 = *m_pV1 - *m_pV3;
	Vector V32 = *m_pV2 - *m_pV3;
	Vector Vn = V31 ^ V32;
	double Ln = sqrt(Vn.Magnitude());


	p1 = C + V31 * p1.x + V32 * p1.y + m_n * p1.z * Ln;
	p2 = C + V31 * p2.x + V32 * p2.y + m_n * p2.z * Ln;
	p3 = C + V31 * p3.x + V32 * p3.y + m_n * p3.z * Ln;

	// Create the Buffer
	D3D_RGB_Vertex vertices[] = {
		{(float)p1.x, (float)p1.y, (float)p1.z, D3DCOLOR_XRGB(0, 255, 0)},
		{(float)p2.x, (float)p2.y, (float)p2.z, D3DCOLOR_XRGB(0, 255, 0)},
		{(float)p3.x, (float)p3.y, (float)p3.z, D3DCOLOR_XRGB(0, 255, 0)}
	};

	// Create the vertex Buffer
	IDirect3DVertexBuffer9* tempVertexBuffer = nullptr;
	pD3ddev->CreateVertexBuffer(sizeof(vertices), 0, g_RGB_FVF, D3DPOOL_DEFAULT, &tempVertexBuffer, NULL);

	// Lock the Vertex Buffer and Fill It
	LPVOID pLockMem = nullptr;
	tempVertexBuffer->Lock(0, sizeof(vertices), &pLockMem, D3DLOCK_DISCARD);
	CopyMemory(pLockMem, vertices, sizeof(vertices));
	tempVertexBuffer->Unlock();

	// Set the Stream source and FVF
	pD3ddev->SetStreamSource(0, tempVertexBuffer, 0, sizeof(D3D_RGB_Vertex));
	pD3ddev->SetFVF(g_RGB_FVF);


	//Get The {Lighting,CullMode & FillMode} States
	DWORD dwLightingState, dwCullModeState, dwFillModeState;
	pD3ddev->GetRenderState(D3DRS_LIGHTING, &dwLightingState);
	pD3ddev->GetRenderState(D3DRS_CULLMODE, &dwCullModeState);
	pD3ddev->GetRenderState(D3DRS_FILLMODE, &dwFillModeState);
	// Switch off Lighting Temporarily
	pD3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);
	// Make Cull Mode State to None Temporarily
	pD3ddev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
	// Make Fill Mode State to WireFrame Temporarily
	pD3ddev->SetRenderState(D3DRS_FILLMODE, D3DFILL_WIREFRAME);


	// Draw the Fixation
	pD3ddev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 1);

	// Restore {Lighting, CullMode & FillMode} states
	pD3ddev->SetRenderState(D3DRS_LIGHTING, dwLightingState);
	pD3ddev->SetRenderState(D3DRS_CULLMODE, dwCullModeState);
	pD3ddev->SetRenderState(D3DRS_FILLMODE, dwFillModeState);

	// delete the temporary vertex Buffer
	tempVertexBuffer->Release();
}

void Element::DrawDisplacementVector(IDirect3DDevice9* pD3ddev, const Vector& U, double L, bool bPostTreatment)const
{
	// Create the intrinsic vertices of the vector
	Vector p1( 0.0, 0.0, 0.0);
	Vector p2( L, 0.0, 0.0 );
	Vector p3( 0.9 * L, 0.0, 0.05 * L );
	Vector p4( 0.9 * L, 0.0, -0.05 * L );

	Vertex C = GetFieldPoint(1.0 / 3.0, 1.0 / 3.0);

	if (bPostTreatment && Vertex::m_bIncludeDisplacements)
	{
		double Scale = Vertex::m_DeformationScale;
		Vector Uc = Vdof(0).U * N1(1.0 / 3.0, 1.0 / 3.0) +
			        Vdof(1).U * N2(1.0 / 3.0, 1.0 / 3.0) +
			        Vdof(2).U * N3(1.0 / 3.0, 1.0 / 3.0);

		C += Uc * Scale;

	}


	// Calculate the unit vectors
	Vector u = U.UnitVector();
	Vector k(0.0, 0.0, 1.0);
	Vector t = k ^ u;
	if (t.Magnitude() == 0.0)
	{
		k.Set(1.0, 0.0, 0.0);
		t = k ^ u;
	}


	t.Normalize();

	
	// Check the Dot Product to See if the Vector is Beneath the surface and Translate it
	double Dot = m_n * u;
	if (Dot > -1.0e-5)
	{
		p1 = C + u * p1.x + t * p1.z;
		p2 = C + u * p2.x + t * p2.z;
		p3 = C + u * p3.x + t * p3.z;
		p4 = C + u * p4.x + t * p4.z;
	}
	else
	{
		Vector beg = C + u * p1.x + t * p1.z;
		Vector end = C + u * p2.x + t * p2.z;
		Vector d = end - beg;


		p1 = C - d + u * p1.x + t * p1.z;
		p2 = C - d + u * p2.x + t * p2.z;
		p3 = C - d + u * p3.x + t * p3.z;
		p4 = C - d + u * p4.x + t * p4.z;
	}


	// Create the Buffer
	D3D_RGB_Vertex vertices[] = {
		{(float)p1.x, (float)p1.y, (float)p1.z, D3DCOLOR_XRGB(0, 255, 0)},
		{(float)p2.x, (float)p2.y, (float)p2.z, D3DCOLOR_XRGB(0, 255, 0)},
		{(float)p2.x, (float)p2.y, (float)p2.z, D3DCOLOR_XRGB(0, 255, 0)},
		{(float)p3.x, (float)p3.y, (float)p3.z, D3DCOLOR_XRGB(0, 255, 0)},
		{(float)p2.x, (float)p2.y, (float)p2.z, D3DCOLOR_XRGB(0, 255, 0)},
		{(float)p4.x, (float)p4.y, (float)p4.z, D3DCOLOR_XRGB(0, 255, 0)}
	};

	// Create the vertex Buffer
	IDirect3DVertexBuffer9* tempVertexBuffer = nullptr;
	pD3ddev->CreateVertexBuffer(sizeof(vertices), 0, g_RGB_FVF, D3DPOOL_DEFAULT, &tempVertexBuffer, NULL);

	// Lock the Vertex Buffer and Fill It
	LPVOID pLockMem = nullptr;
	tempVertexBuffer->Lock(0, sizeof(vertices), &pLockMem, D3DLOCK_DISCARD);
	CopyMemory(pLockMem, vertices, sizeof(vertices));
	tempVertexBuffer->Unlock();

	// Set the Stream source and FVF
	pD3ddev->SetStreamSource(0, tempVertexBuffer, 0, sizeof(D3D_RGB_Vertex));
	pD3ddev->SetFVF(g_RGB_FVF);


	//Get The Lighting State
	DWORD dwLightingState;
	pD3ddev->GetRenderState(D3DRS_LIGHTING, &dwLightingState);
	
	// Switch off Lighting Temporarily
	pD3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);
	
	// Draw the Displacement vector
	pD3ddev->DrawPrimitive(D3DPT_LINELIST, 0, 3);

	// Restore Lighting state
	pD3ddev->SetRenderState(D3DRS_LIGHTING, dwLightingState);

	// delete the temporary vertex Buffer
	tempVertexBuffer->Release();
}

void Element::DrawTractionVector(IDirect3DDevice9* pD3ddev, const Vector& T, double L, bool bPostTreatment)const
{
	// Create the intrinsic vertices of the vector
	Vector p1( 0.0, 0.0, 0.0 );
	Vector p2( L, 0.0, 0.0 );
	Vector p3( 0.9 * L, 0.0, 0.05 * L );
	Vector p4( 0.9 * L, 0.0, -0.05 * L );

	Vertex C = GetFieldPoint(1.0 / 3.0, 1.0 / 3.0);

	if (bPostTreatment && Vertex::m_bIncludeDisplacements)
	{
		double Scale = Vertex::m_DeformationScale;
		Vector Uc = Vdof(0).U * N1(1.0 / 3.0, 1.0 / 3.0) +
			        Vdof(1).U * N2(1.0 / 3.0, 1.0 / 3.0) +
			        Vdof(2).U * N3(1.0 / 3.0, 1.0 / 3.0);

		C += Uc * Scale;
	}

	// Calculate the unit vectors
	Vector f = T.UnitVector();
	Vector k(0.0, 0.0, 1.0);
	Vector t = k ^ f;
	if (t.Magnitude() == 0.0)
	{
		k.Set(1.0, 0.0, 0.0);
		t = k ^ f;
	}

	t.Normalize();


	// Check the Dot Product to See if the Vector is Beneath the surface and Translate it
	double Dot = m_n * f;
	if (Dot > -1.0e-5)
	{
		p1 = C + f * p1.x + t * p1.z;
		p2 = C + f * p2.x + t * p2.z;
		p3 = C + f * p3.x + t * p3.z;
		p4 = C + f * p4.x + t * p4.z;
	}
	else
	{
		

		Vector beg = C + f * p1.x + t * p1.z;
		Vector end = C + f * p2.x + t * p2.z;
		Vector d = end - beg;


		p1 = C - d + f * p1.x + t * p1.z;
		p2 = C - d + f * p2.x + t * p2.z;
		p3 = C - d + f * p3.x + t * p3.z;
		p4 = C - d + f * p4.x + t * p4.z;
	}


	// Create the Buffer
	D3D_RGB_Vertex vertices[] = {
		{(float)p1.x, (float)p1.y, (float)p1.z, D3DCOLOR_XRGB(255, 0, 0)},
		{(float)p2.x, (float)p2.y, (float)p2.z, D3DCOLOR_XRGB(255, 0, 0)},
		{(float)p2.x, (float)p2.y, (float)p2.z, D3DCOLOR_XRGB(255, 0, 0)},
		{(float)p3.x, (float)p3.y, (float)p3.z, D3DCOLOR_XRGB(255, 0, 0)},
		{(float)p2.x, (float)p2.y, (float)p2.z, D3DCOLOR_XRGB(255, 0, 0)},
		{(float)p4.x, (float)p4.y, (float)p4.z, D3DCOLOR_XRGB(255, 0, 0)}
	};

	// Create the vertex Buffer
	IDirect3DVertexBuffer9* tempVertexBuffer = nullptr;
	pD3ddev->CreateVertexBuffer(sizeof(vertices), 0, g_RGB_FVF, D3DPOOL_DEFAULT, &tempVertexBuffer, NULL);

	// Lock the Vertex Buffer and Fill It
	LPVOID pLockMem = nullptr;
	tempVertexBuffer->Lock(0, sizeof(vertices), &pLockMem, D3DLOCK_DISCARD);
	CopyMemory(pLockMem, vertices, sizeof(vertices));
	tempVertexBuffer->Unlock();

	// Set the Stream source and FVF
	pD3ddev->SetStreamSource(0, tempVertexBuffer, 0, sizeof(D3D_RGB_Vertex));
	pD3ddev->SetFVF(g_RGB_FVF);


	//Get The Lighting State
	DWORD dwLightingState;
	pD3ddev->GetRenderState(D3DRS_LIGHTING, &dwLightingState);

	// Switch off Lighting Temporarily
	pD3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Draw the Displacement vector
	pD3ddev->DrawPrimitive(D3DPT_LINELIST, 0, 3);

	// Restore Lighting state
	pD3ddev->SetRenderState(D3DRS_LIGHTING, dwLightingState);

	// delete the temporary vertex Buffer
	tempVertexBuffer->Release();
}


void Element::DrawNormalDisplacementVector(IDirect3DDevice9* pD3ddev, double Un, double L, bool bPostTreatment)const
{
	// Create the intrinsic vertices of the vector
	Vector p1{ 0.0, 0.0, 0.0 };
	Vector p2{ L, 0.0, 0.0 };
	Vector p3{ 0.9 * L, 0.0, 0.05 * L };
	Vector p4{ 0.9 * L, 0.0, -0.05 * L };

	Vertex C = GetFieldPoint(1.0 / 3.0, 1.0 / 3.0);
	if (bPostTreatment && Vertex::m_bIncludeDisplacements)
	{
		double Scale = Vertex::m_DeformationScale;
		Vector Uc = Vdof(0).U * N1(1.0 / 3.0, 1.0 / 3.0) +
			        Vdof(1).U * N2(1.0 / 3.0, 1.0 / 3.0) +
			        Vdof(2).U * N3(1.0 / 3.0, 1.0 / 3.0);

		C += Uc * Scale;

	}

	// Calculate the unit vectors
	Vector u = m_n * Un;
	u.Normalize();
	Vector k(0.0, 0.0, 1.0);
	Vector t = k ^ u;
	if (t.Magnitude() == 0.0)
	{
		k.Set(1.0, 0.0, 0.0);
		t = k ^ u;
	}

	t.Normalize();


	// Check the Dot Product to See if the Vector is Beneath the surface and Translate it
	double Dot = m_n * u;
	if (Dot < 0)
	{
		Vector beg = C + u * p1.x + t * p1.z;
		Vector end = C + u * p2.x + t * p2.z;
		Vector d = end - beg;


		p1 = C - d + u * p1.x + t * p1.z;
		p2 = C - d + u * p2.x + t * p2.z;
		p3 = C - d + u * p3.x + t * p3.z;
		p4 = C - d + u * p4.x + t * p4.z;
	}
	else
	{
		p1 = C + u * p1.x + t * p1.z;
		p2 = C + u * p2.x + t * p2.z;
		p3 = C + u * p3.x + t * p3.z;
		p4 = C + u * p4.x + t * p4.z;
	}


	// Create the Buffer
	D3D_RGB_Vertex vertices[] = {
		{(float)p1.x, (float)p1.y, (float)p1.z, D3DCOLOR_XRGB(0, 255, 0)},
		{(float)p2.x, (float)p2.y, (float)p2.z, D3DCOLOR_XRGB(0, 255, 0)},
		{(float)p2.x, (float)p2.y, (float)p2.z, D3DCOLOR_XRGB(0, 255, 0)},
		{(float)p3.x, (float)p3.y, (float)p3.z, D3DCOLOR_XRGB(0, 255, 0)},
		{(float)p2.x, (float)p2.y, (float)p2.z, D3DCOLOR_XRGB(0, 255, 0)},
		{(float)p4.x, (float)p4.y, (float)p4.z, D3DCOLOR_XRGB(0, 255, 0)}
	};

	// Create the vertex Buffer
	IDirect3DVertexBuffer9* tempVertexBuffer = nullptr;
	pD3ddev->CreateVertexBuffer(sizeof(vertices), 0, g_RGB_FVF, D3DPOOL_DEFAULT, &tempVertexBuffer, NULL);

	// Lock the Vertex Buffer and Fill It
	LPVOID pLockMem = nullptr;
	tempVertexBuffer->Lock(0, sizeof(vertices), &pLockMem, D3DLOCK_DISCARD);
	CopyMemory(pLockMem, vertices, sizeof(vertices));
	tempVertexBuffer->Unlock();

	// Set the Stream source and FVF
	pD3ddev->SetStreamSource(0, tempVertexBuffer, 0, sizeof(D3D_RGB_Vertex));
	pD3ddev->SetFVF(g_RGB_FVF);


	//Get The Lighting State
	DWORD dwLightingState;
	pD3ddev->GetRenderState(D3DRS_LIGHTING, &dwLightingState);

	// Switch off Lighting Temporarily
	pD3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Draw the Displacement vector
	pD3ddev->DrawPrimitive(D3DPT_LINELIST, 0, 3);

	// Restore Lighting state
	pD3ddev->SetRenderState(D3DRS_LIGHTING, dwLightingState);

	// delete the temporary vertex Buffer
	tempVertexBuffer->Release();
}

void Element::DrawNormalTractionVector(IDirect3DDevice9* pD3ddev, double Tn, double L, bool bPostTreatment)const
{
	// Create the intrinsic vertices of the vector
	Vector p1{ 0.0, 0.0, 0.0 };
	Vector p2{ L, 0.0, 0.0 };
	Vector p3{ 0.9 * L, 0.0, 0.05 * L };
	Vector p4{ 0.9 * L, 0.0, -0.05 * L };

	Vertex C = GetFieldPoint(1.0 / 3.0, 1.0 / 3.0);

	if (bPostTreatment && Vertex::m_bIncludeDisplacements)
	{
		double Scale = Vertex::m_DeformationScale;
		Vector Uc = Vdof(0).U * N1(1.0 / 3.0, 1.0 / 3.0) +
			        Vdof(1).U * N2(1.0 / 3.0, 1.0 / 3.0) +
			        Vdof(2).U * N3(1.0 / 3.0, 1.0 / 3.0);

		C += Uc * Scale;

	}

	// Calculate the unit vectors
	Vector f = m_n * Tn;
	f.Normalize();
	Vector k(0.0, 0.0, 1.0);
	Vector t = k ^ f;
	if (t.Magnitude() == 0.0)
	{
		k.Set(1.0, 0.0, 0.0);
		t = k ^ f;
	}

	t.Normalize();


	// Check the Dot Product to See if the Vector is Beneath the surface and Translate it
	double Dot = m_n * f;
	if (Dot < 0)
	{
		Vector beg = C + f * p1.x + t * p1.z;
		Vector end = C + f * p2.x + t * p2.z;
		Vector d = end - beg;


		p1 = C - d + f * p1.x + t * p1.z;
		p2 = C - d + f * p2.x + t * p2.z;
		p3 = C - d + f * p3.x + t * p3.z;
		p4 = C - d + f * p4.x + t * p4.z;
	}
	else
	{
		p1 = C + f * p1.x + t * p1.z;
		p2 = C + f * p2.x + t * p2.z;
		p3 = C + f * p3.x + t * p3.z;
		p4 = C + f * p4.x + t * p4.z;
	}


	// Create the Buffer
	D3D_RGB_Vertex vertices[] = {
		{(float)p1.x, (float)p1.y, (float)p1.z, D3DCOLOR_XRGB(255, 0, 0)},
		{(float)p2.x, (float)p2.y, (float)p2.z, D3DCOLOR_XRGB(255, 0, 0)},
		{(float)p2.x, (float)p2.y, (float)p2.z, D3DCOLOR_XRGB(255, 0, 0)},
		{(float)p3.x, (float)p3.y, (float)p3.z, D3DCOLOR_XRGB(255, 0, 0)},
		{(float)p2.x, (float)p2.y, (float)p2.z, D3DCOLOR_XRGB(255, 0, 0)},
		{(float)p4.x, (float)p4.y, (float)p4.z, D3DCOLOR_XRGB(255, 0, 0)}
	};

	// Create the vertex Buffer
	IDirect3DVertexBuffer9* tempVertexBuffer = nullptr;
	pD3ddev->CreateVertexBuffer(sizeof(vertices), 0, g_RGB_FVF, D3DPOOL_DEFAULT, &tempVertexBuffer, NULL);

	// Lock the Vertex Buffer and Fill It
	LPVOID pLockMem = nullptr;
	tempVertexBuffer->Lock(0, sizeof(vertices), &pLockMem, D3DLOCK_DISCARD);
	CopyMemory(pLockMem, vertices, sizeof(vertices));
	tempVertexBuffer->Unlock();

	// Set the Stream source and FVF
	pD3ddev->SetStreamSource(0, tempVertexBuffer, 0, sizeof(D3D_RGB_Vertex));
	pD3ddev->SetFVF(g_RGB_FVF);


	//Get The Lighting State
	DWORD dwLightingState;
	pD3ddev->GetRenderState(D3DRS_LIGHTING, &dwLightingState);

	// Switch off Lighting Temporarily
	pD3ddev->SetRenderState(D3DRS_LIGHTING, FALSE);

	// Draw the Displacement vector
	pD3ddev->DrawPrimitive(D3DPT_LINELIST, 0, 3);

	// Restore Lighting state
	pD3ddev->SetRenderState(D3DRS_LIGHTING, dwLightingState);

	// delete the temporary vertex Buffer
	tempVertexBuffer->Release();
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

	case BEM_3D::S_VON_MISES:
		Val = pSig->VonMises();
		break;

	case BEM_3D::S_TRESCA:
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

	case BEM_3D::E_VON_MISES:
		Val = pEps->VonMises();
		break;

	case BEM_3D::E_TRESCA:
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
	double tol = 1.0E-9;

	// 1️⃣ Check if the point coincides with any triangle vertex
	if (m_pV1->Distance(DomainPt) < tol) return true;
	if (m_pV2->Distance(DomainPt) < tol) return true;
	if (m_pV3->Distance(DomainPt) < tol) return true;

	// 2️⃣ Vectors in local triangle system (origin at V3)
	Vector V = DomainPt - *m_pV3;  // vector from V3 to point
	Vector V1 = *m_pV1 - *m_pV3;    // edge V3->V1
	Vector V2 = *m_pV2 - *m_pV3;    // edge V3->V2

	// 3️⃣ Plane check using unit normal
	if (fabs(V * m_n) > tol)
		return false; // point not on triangle plane

	// 4️⃣ Compute intrinsic coordinates (barycentric) relative to V3
	double d00 = V1 * V1;
	double d01 = V1 * V2;
	double d11 = V2 * V2;
	double d20 = V * V1;
	double d21 = V * V2;

	double denom = d00 * d11 - d01 * d01;
	if (fabs(denom) < tol)
		return false; // degenerate triangle

	Xi = (d11 * d20 - d01 * d21) / denom;
	Eta = (d00 * d21 - d01 * d20) / denom;

	// 5️⃣ Point is inside triangle if xi, eta >= 0 and xi + eta <= 1
	return (Xi >= -tol) && (Eta >= -tol) && ((Xi + Eta) <= 1.0 + tol);
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

//double Model::GetSig(int i, int j, const Vector& e1, const Vector& e2, const Vector& e3,
//	const Vector& T, double _Eps11, double _Eps22, double _Eps12)
//{
//	double E = Element::E;
//	double G = Element::G;
//	double v = Element::v;
//	double Sig_ij = 0.0;
//
//	for (int k = 0; k < 3; k++)
//	{
//		Sig_ij += (((e1[i] * e1[j] + e2[i] * e2[j]) * v / (1.0 - v) + e3[i] * e3[j]) * e3[k]
//			+ (e1[i] * e3[j] + e1[j] * e3[i]) * e1[k] + (e2[i] * e3[j] + e2[j] * e3[i]) * e2[k]) * T[k]
//				+ 2.0 * G * (e1[i] * e2[j] + e1[j] * e2[i]) * _Eps12
//					+ 2.0 * G / (1.0 - v) * ((e1[i] * e1[j] + v * e2[i] * e2[j]) * _Eps11
//						+ (v * e1[i] * e1[j] + e2[i] * e2[j]) * _Eps22);
//	}
//
//	return Sig_ij;
//}


//void Model::GetSig(Tensor& rSig, const Vector& e1, const Vector& e2, const Vector& e3,
//	const Vector& T, double _Eps11, double _Eps22, double _Eps12)
//{
//	rSig._11 = GetSig(0, 0, e1, e2, e3, T, _Eps11, _Eps22, _Eps12);
//	rSig._21 = rSig._12 = GetSig(0, 1, e1, e2, e3, T, _Eps11, _Eps22, _Eps12);
//	rSig._31 = rSig._13 = GetSig(0, 2, e1, e2, e3, T, _Eps11, _Eps22, _Eps12);
//	rSig._22 = GetSig(1, 1, e1, e2, e3, T, _Eps11, _Eps22, _Eps12);
//	rSig._32 = rSig._23 = GetSig(1, 2, e1, e2, e3, T, _Eps11, _Eps22, _Eps12);
//	rSig._33 = GetSig(2, 2, e1, e2, e3, T, _Eps11, _Eps22, _Eps12);
//}

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

	double Cte = 1.0 / (1.0 - 3.0 * CollFactor());
		
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
	if (Type() == DISCONTINUOUS)
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


//double Element::Jn(int l)const
//{
//	int l1 = l;
//	int l2 = (l1 + 1) % 3;
//	int l3 = (l2 + 1) % 3;
//
//	const Vertex& P1 = Vdof(l1);
//	const Vertex& P2 = Vdof(l2);
//	const Vertex& P3 = Vdof(l3);
//
//	// Calculate the Jump vectors
//	Vector _V1 = P1 - P3;
//	Vector _V2 = P2 - P3;
//
//	// Calclate the Jacobian vector
//	Vector J_ = _V1 ^ _V2;
//
//	return J_.Magnitude();
//}


double Element::f(const Vertex& V1, const Vertex& V2, const Vertex& V3, double Theta, int i, int j)const
{
	Vector e1 = V2 - V1; e1.Normalize();
	Vector e3 = m_n;
	Vector e2 = e3 ^ e1;

	double Aij = e1[i] * e3[j] - e3[i] * e1[j];
	double Bij = e2[i] * e3[j] - e3[i] * e2[j];

	double K = (1.0 - 2.0 * v) / (8.0 * M_PI * (1.0 - v));


	return K * (Aij * cos(Theta) + Bij * sin(Theta));
}



double Element::GetTheta(const Vertex& V1, const Vertex& V2, const Vertex& V3, double Eta_)const
{
	// Calculate the two vectors
	Vector V12 = V2 - V1;
	Vector V13 = V3 - V1;

	// Calculate L1, L2
	double L1 = V12.Magnitude();
	double L2 = V13.Magnitude();

	// Calculate angle Alpha
	double DotProd = V12 * V13;
	double Alpha = acos(DotProd / (L1 * L2));

	return 0.5 * Alpha * (Eta_ + 1.0);
}


double Element::g(const Vertex& V1, const Vertex& V2, const Vertex& V3, double Eta_)const
{
	// Calculate the two vectors
	Vector V12 = V2 - V1;
	Vector V13 = V3 - V1;

	// Calculate L1, L2
	double L1 = V12.Magnitude();
	double L2 = V13.Magnitude();



	double DotProd = V12 * V13;
	double Alpha = acos(DotProd / (L1 * L2));


	return 0.5 * Alpha;
}


double Element::Ro_(const Vertex& V1, const Vertex& V2, const Vertex& V3, double Theta)const
{
	// Calculate the two vectors
	Vector V12 = V2 - V1;
	Vector V13 = V3 - V1;

	// Calculate L1, L2
	double L1 = V12.Magnitude();
	double L2 = V13.Magnitude();

	// Calculate angle Alpha
	double Alpha = acos((V12 * V13) / (L1 * L2));

	return (L1 * L2 * sin(Alpha)) / (L1 * sin(Theta) + L2 * sin(Alpha - Theta));
}


double Element::H(const Vertex& V1, const Vertex& V2, const Vertex& V3, double Eta_, int i, int j)const
{
	double Theta = GetTheta(V1, V2, V3, Eta_);
	double g_ = g(V1, V2, V3, Eta_);
	double R_ = Ro_(V1, V2, V3, Theta);
	double f_ = f(V1, V2, V3, Theta, i, j);


	double Res = f_ * (log(R_) - 1.0) * g_;


	return Res;
}


double Element::Ps(const Vertex& V1, const Vertex& V2, const Vertex& V3, int i, int j) const
{

	// Define the 12 pts Gaussian Rule
	double Xi[16];
	double W[16];

	Get1DGaussianRule(Xi, W, 16);


	// One dimension integral
	// Itegrate numerically
	double I_ = 0.0;
	for (int t = 0; t < 16; t++)
		I_ += (H(V1, V2, V3, Xi[t], i, j) * W[t]);


	return I_;
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



INTEG_RULE Element::SelectIntegrationRule(const Vertex& SrcPt, int nKernelType) const
{
	if (!m_bUseAdaptiveCriterion)
	{
		return m_FixedIntegRule;
	}
	else
	{
		double d1 = m_pV1->Distance(SrcPt);
		double d2 = m_pV2->Distance(SrcPt);
		double d3 = m_pV3->Distance(SrcPt);

		double L1 = m_pV1->Distance(*m_pV2);
		double L2 = m_pV2->Distance(*m_pV3);
		double L3 = m_pV3->Distance(*m_pV1);

		double dmin = min(d1, min(d2, d3));
		double Lmin = min(L1, min(L2, L3));

		double Lambda = dmin / Lmin;


		switch (nKernelType)
		{
		case 0:
		{
			if (Lambda > 11.33)
				return INTEG_RULE::_001_PT;
			else if (Lambda > 7.0)
				return INTEG_RULE::_003_PT;
			else if (Lambda > 2.85)
				return INTEG_RULE::_004_PT;
			else if (Lambda > 1.83)
				return INTEG_RULE::_007_PT;
			else if (Lambda > 1.40)
				return INTEG_RULE::_013_PT;
			else if (Lambda > 1.20)
				return INTEG_RULE::_028_PT;
			else if (Lambda > 1.073)
				return INTEG_RULE::_052_PT;
			else if (Lambda > 1.062)
				return INTEG_RULE::_104_PT;
			else
				return INTEG_RULE::_208_PT;
		}
		break;

		case 1:
		{
			if (Lambda > 20.67)
				return INTEG_RULE::_001_PT;
			else if (Lambda > 10.0)
				return INTEG_RULE::_003_PT;
			else if (Lambda > 3.60)
				return INTEG_RULE::_004_PT;
			else if (Lambda > 2.29)
				return INTEG_RULE::_007_PT;
			else if (Lambda > 1.71)
				return INTEG_RULE::_013_PT;
			else if (Lambda > 1.42)
				return INTEG_RULE::_028_PT;
			else if (Lambda > 1.21)
				return INTEG_RULE::_052_PT;
			else if (Lambda > 1.17)
				return INTEG_RULE::_104_PT;
			else
				return INTEG_RULE::_208_PT;
		}
		break;
		}
	}
	

	return INTEG_RULE::_208_PT;  // Impossible value
}
