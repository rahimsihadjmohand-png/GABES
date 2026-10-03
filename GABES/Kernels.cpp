#include "pch.h"
#include "Element.h"


using namespace BEM_3D;


double Element::U(const Vertex& SrcPt, int nRule, int nNode, int i, int j)const
{
	Vertex FldPt;

	if (m_bRISP)
	{
		INTEG_RULE Rule = IndexToRule(nRule);
		int Offset = RuleToOffset(Rule);
		FldPt = FldPts[Offset + nNode];
	}		
	else
	{
		double Xi = ITNDs[nRule][nNode].Xi;
		double Eta = ITNDs[nRule][nNode].Eta;
		FldPt = GetFieldPoint(Xi, Eta);
	}

	double R = r(SrcPt, FldPt);                    // uses r which is common from element types
	double r_i = dr(SrcPt, FldPt, i);           // uses dr which is common from element types
	double r_j = dr(SrcPt, FldPt, j);
	double ij = KD(i, j);                          // uses KD which is common from element types

	return 1.0 / (16.0 * M_PI * G * (1.0 - v) * R) * ((3.0 - 4.0 * v) * ij + r_i * r_j);
}

double Element::U(const Vertex& SrcPt, const ITND* IntegRule, int nNode, int i, int j)const
{
	double Xi = IntegRule[nNode].Xi;
	double Eta = IntegRule[nNode].Eta;
	Vertex FldPt = GetFieldPoint(Xi, Eta);

	double R = r(SrcPt, FldPt);                    // uses r which is common from element types
	double r_i = dr(SrcPt, FldPt, i);           // uses dr which is common from element types
	double r_j = dr(SrcPt, FldPt, j);
	double ij = KD(i, j);                          // uses KD which is common from element types

	return 1.0 / (16.0 * M_PI * G * (1.0 - v) * R) * ((3.0 - 4.0 * v) * ij + r_i * r_j);
}


double Element::U(const Vertex& SrcPt, const Vertex& FldPt, int i, int j)const
{
	double R = r(SrcPt, FldPt);                    // uses r which is common from element types
	double r_i = dr(SrcPt, FldPt, i);           // uses dr which is common from element types
	double r_j = dr(SrcPt, FldPt, j);
	double ij = KD(i, j);                          // uses KD which is common from element types

	return 1.0 / (16.0 * M_PI * G * (1.0 - v) * R) * ((3.0 - 4.0 * v) * ij + r_i * r_j);
}


double Element::U(const Vertex& SrcPt, double Xi, double Eta, int i, int j)const
{
	Vertex FldPt;
	GetFieldPoint(&FldPt, Xi, Eta);

	double R = r(SrcPt, FldPt);                    // uses r which is common from element types
	double r_i = dr(SrcPt, FldPt, i);           // uses dr which is common from element types
	double r_j = dr(SrcPt, FldPt, j);
	double ij = KD(i, j);                          // uses KD which is common from element types

	return 1.0 / (16.0 * M_PI * G * (1.0 - v) * R) * ((3.0 - 4.0 * v) * ij + r_i * r_j);
}


double Element::T(const Vertex& SrcPt, int nRule, int nNode, int i, int j)const
{
	Vertex FldPt;

	if (m_bRISP)
	{
		INTEG_RULE Rule = IndexToRule(nRule);
		int Offset = RuleToOffset(Rule);
		FldPt = FldPts[Offset + nNode];
	}
	else
	{
		double Xi = ITNDs[nRule][nNode].Xi;
		double Eta = ITNDs[nRule][nNode].Eta;
		FldPt = GetFieldPoint(Xi, Eta);
	}


	double R = r(SrcPt, FldPt);
	double r_i = dr(SrcPt, FldPt, i);
	double r_j = dr(SrcPt, FldPt, j);
	double ij = KD(i, j);
	double dR_dn = dr_dn(SrcPt, FldPt);
	double ni = m_n[i];
	double nj = m_n[j];

	
	return -1.0 / (8.0 * M_PI * (1.0 - v) * R * R) * (dR_dn * ((1.0 - 2.0 * v) * ij + 3.0 * r_i * r_j) - (1.0 - 2.0 * v) * (nj * r_i - ni * r_j));
}



double Element::T(const Vertex& SrcPt, const ITND* IntegRule, int nNode, int i, int j)const
{
	double Xi = IntegRule[nNode].Xi;
	double Eta = IntegRule[nNode].Eta;
	Vertex FldPt = GetFieldPoint(Xi, Eta);


	double R = r(SrcPt, FldPt);
	double r_i = dr(SrcPt, FldPt, i);
	double r_j = dr(SrcPt, FldPt, j);
	double ij = KD(i, j);
	double dR_dn = dr_dn(SrcPt, FldPt);
	double ni = m_n[i];
	double nj = m_n[j];

	
	return -1.0 / (8.0 * M_PI * (1.0 - v) * R * R) * (dR_dn * ((1.0 - 2.0 * v) * ij + 3.0 * r_i * r_j) - (1.0 - 2.0 * v) * (nj * r_i - ni * r_j));
}

double Element::T(const Vertex& SrcPt, const Vertex& FldPt, int i, int j)const
{
	double R = r(SrcPt, FldPt);
	double r_i = dr(SrcPt, FldPt, i);
	double r_j = dr(SrcPt, FldPt, j);
	double ij = KD(i, j);
	double dR_dn = dr_dn(SrcPt, FldPt);
	double ni = m_n[i];
	double nj = m_n[j];


	return -1.0 / (8.0 * M_PI * (1.0 - v) * R * R) * (dR_dn * ((1.0 - 2.0 * v) * ij + 3.0 * r_i * r_j) - (1.0 - 2.0 * v) * (nj * r_i - ni * r_j));
}


double Element::T(const Vertex& SrcPt, double Xi, double Eta, int i, int j)const
{
	Vertex FldPt;
	GetFieldPoint(&FldPt, Xi, Eta);

	double R = r(SrcPt, FldPt);
	double r_i = dr(SrcPt, FldPt, i);
	double r_j = dr(SrcPt, FldPt, j);
	double ij = KD(i, j);
	double dR_dn = dr_dn(SrcPt, FldPt);
	double ni = m_n[i];
	double nj = m_n[j];

	


	return -1.0 / (8.0 * M_PI * (1.0 - v) * R * R) * (dR_dn * ((1.0 - 2.0 * v) * ij + 3.0 * r_i * r_j) - (1.0 - 2.0 * v) * (nj * r_i - ni * r_j));
}

double Element::Ts(int s, double Xi, double Eta, int i, int j)const
{
	const Vertex& SrcPt = Vdof(s);

	Vertex FldPt;
	GetFieldPoint(&FldPt, Xi, Eta);

	double R = r(SrcPt, FldPt);
	double r_i = dr(SrcPt, FldPt, i);
	double r_j = dr(SrcPt, FldPt, j);
	double ni = m_n[i];
	double nj = m_n[j];

	
	return (1.0 - 2.0 * v) / (8.0 * M_PI * (1.0 - v) * R * R) * (nj * r_i - ni * r_j);
}



double Element::D(const Vertex& SrcPt, int nRule, int nNode, int i, int j, int k)const
{
	Vertex FldPt;

	if (m_bRISP)
	{
		INTEG_RULE Rule = IndexToRule(nRule);
		int Offset = RuleToOffset(Rule);
		FldPt = FldPts[Offset + nNode];
	}
	else
	{
		double Xi = ITNDs[nRule][nNode].Xi;
		double Eta = ITNDs[nRule][nNode].Eta;
		FldPt = GetFieldPoint(Xi, Eta);
	}


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

double Element::D(const Vertex& SrcPt, const ITND* IntegRule, int nNode, int i, int j, int k)const
{
	double Xi = IntegRule[nNode].Xi;
	double Eta = IntegRule[nNode].Eta;
	Vertex FldPt = GetFieldPoint(Xi, Eta);


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

double Element::D(const Vertex& SrcPt, const Vertex& FldPt, int i, int j, int k)const
{
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


double Element::D(const Vertex& SrcPt, double Xi, double Eta, int i, int j, int k)const
{
	Vertex FldPt;
	GetFieldPoint(&FldPt, Xi, Eta);

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
	Vertex FldPt;

	if (m_bRISP)
	{
		INTEG_RULE Rule = IndexToRule(nRule);
		int Offset = RuleToOffset(Rule);
		FldPt = FldPts[Offset + nNode];
	}
	else
	{
		double Xi = ITNDs[nRule][nNode].Xi;
		double Eta = ITNDs[nRule][nNode].Eta;
		FldPt = GetFieldPoint(Xi, Eta);
	}


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


	return G / (4.0 * M_PI * (1.0 - v) * r3) * (3.0 * dR_dn * ((1.0 - 2.0 * v) * ij * r_k + v * (ik * r_j + jk * r_i) - 5.0 * r_i * r_j * r_k)
		+ 3.0 * v * (ni * r_j * r_k + nj * r_i * r_k) + (1.0 - 2.0 * v) * (3.0 * nk * r_i * r_j + nj * ik + ni * jk) - (1.0 - 4.0 * v) * nk * ij);
}

double Element::S(const Vertex& SrcPt, const ITND* IntegRule, int nNode, int i, int j, int k)const
{
	double Xi = IntegRule[nNode].Xi;
	double Eta = IntegRule[nNode].Eta;
	Vertex FldPt = GetFieldPoint(Xi, Eta);


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


	return G / (4.0 * M_PI * (1.0 - v) * r3) * (3.0 * dR_dn * ((1.0 - 2.0 * v) * ij * r_k + v * (ik * r_j + jk * r_i) - 5.0 * r_i * r_j * r_k)
		+ 3.0 * v * (ni * r_j * r_k + nj * r_i * r_k) + (1.0 - 2.0 * v) * (3.0 * nk * r_i * r_j + nj * ik + ni * jk) - (1.0 - 4.0 * v) * nk * ij);
}

double Element::S(const Vertex& SrcPt, const Vertex& FldPt, int i, int j, int k)const
{
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


	return G / (4.0 * M_PI * (1.0 - v) * r3) * (3.0 * dR_dn * ((1.0 - 2.0 * v) * ij * r_k + v * (ik * r_j + jk * r_i) - 5.0 * r_i * r_j * r_k)
		+ 3.0 * v * (ni * r_j * r_k + nj * r_i * r_k) + (1.0 - 2.0 * v) * (3.0 * nk * r_i * r_j + nj * ik + ni * jk) - (1.0 - 4.0 * v) * nk * ij);
}

double Element::S(const Vertex& SrcPt, double Xi, double Eta, int i, int j, int k)const
{
	Vertex FldPt;
	GetFieldPoint(&FldPt, Xi, Eta);

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


	return G / (4.0 * M_PI * (1.0 - v) * r3) * (3.0 * dR_dn * ((1.0 - 2.0 * v) * ij * r_k + v * (ik * r_j + jk * r_i) - 5.0 * r_i * r_j * r_k)
		+ 3.0 * v * (ni * r_j * r_k + nj * r_i * r_k) + (1.0 - 2.0 * v) * (3.0 * nk * r_i * r_j + nj * ik + ni * jk) - (1.0 - 4.0 * v) * nk * ij);
}

double Element::W(const Vertex& SrcPt, int nRule, int nNode, int i, int j, int k)const
{
	Vertex FldPt;

	if (m_bRISP)
	{
		INTEG_RULE Rule = IndexToRule(nRule);
		int Offset = RuleToOffset(Rule);
		FldPt = FldPts[Offset + nNode];
	}
	else
	{
		double Xi = ITNDs[nRule][nNode].Xi;
		double Eta = ITNDs[nRule][nNode].Eta;
		FldPt = GetFieldPoint(Xi, Eta);
	}


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

double Element::W(const Vertex& SrcPt, const ITND* IntegRule, int nNode, int i, int j, int k)const
{
	double Xi = IntegRule[nNode].Xi;
	double Eta = IntegRule[nNode].Eta;
	Vertex FldPt = GetFieldPoint(Xi, Eta);


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

double Element::W(const Vertex& SrcPt, const Vertex& FldPt, int i, int j, int k)const
{
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




double Element::W(const Vertex& SrcPt, double Xi, double Eta, int i, int j, int k)const
{
	Vertex FldPt;
	GetFieldPoint(&FldPt, Xi, Eta);

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
	Vertex FldPt;

	if (m_bRISP)
	{
		INTEG_RULE Rule = IndexToRule(nRule);
		int Offset = RuleToOffset(Rule);
		FldPt = FldPts[Offset + nNode];
	}
	else
	{
		double Xi = ITNDs[nRule][nNode].Xi;
		double Eta = ITNDs[nRule][nNode].Eta;
		FldPt = GetFieldPoint(Xi, Eta);
	}


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

double Element::V(const Vertex& SrcPt, const ITND* IntegRule, int nNode, int i, int j, int k)const
{
	double Xi = IntegRule[nNode].Xi;
	double Eta = IntegRule[nNode].Eta;
	Vertex FldPt = GetFieldPoint(Xi, Eta);


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

double Element::V(const Vertex& SrcPt, const Vertex& FldPt, int i, int j, int k)const
{
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



double Element::V(const Vertex& SrcPt, double Xi, double Eta, int i, int j, int k)const
{
	Vertex FldPt;
	GetFieldPoint(&FldPt, Xi, Eta);

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


	

	if (Nl == 0.0 || m_J == 0.0)
		return 0.0;


	return U(SrcPt, nRule, nNode, i, j) * Nl * m_J;
}

//double Element::_U_(const Vertex& SrcPt, const ITND* IntegRule, int nNode, int i, int j, int l)const
//{
//	double Nl = N(l, IntegRule[nNode].Xi, IntegRule[nNode].Eta);
//
//	if (Nl == 0.0 || m_J == 0.0)
//		return 0.0;
//
//	return U(SrcPt, IntegRule, nNode, i, j) * Nl * m_J;
//}
//
//
//double Element::_U_(const Vertex& SrcPt, const Vertex& FldPt, int i, int j, int l)const
//{
//	double Xi, Eta;
//	GetIntrinsicCoords(FldPt, Xi, Eta);
//
//	double Nl = N(l, Xi, Eta);
//
//	if (Nl == 0.0 || m_J == 0.0)
//		return 0.0;
//
//	return U(SrcPt, FldPt, i, j) * Nl * m_J;
//}


double Element::_U_(const Vertex& SrcPt, double Xi, double Eta, int i, int j, int l)const
{
	double Nl = N(l, Xi, Eta);

	if (Nl == 0.0 || m_J == 0.0)
		return 0.0;

	return U(SrcPt, Xi, Eta, i, j) * Nl * m_J;
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



	if (Nl == 0.0 || m_J == 0.0)
		return 0.0;


	return T(SrcPt, nRule, nNode, i, j) * Nl * m_J;
}


//double Element::_T_(const Vertex& SrcPt, const ITND* IntegRule, int nNode, int i, int j, int l)const
//{
//	double Nl = N(l, IntegRule[nNode].Xi, IntegRule[nNode].Eta);
//
//	if (Nl == 0.0 || m_J == 0.0)
//		return 0.0;
//
//	return T(SrcPt, IntegRule, nNode, i, j) * Nl * m_J;
//}
//
//double Element::_T_(const Vertex& SrcPt, const Vertex& FldPt, int i, int j, int l)const
//{
//	double Xi, Eta;
//	GetIntrinsicCoords(FldPt, Xi, Eta);
//
//	double Nl = N(l, Xi, Eta);
//
//	if (Nl == 0.0 || m_J == 0.0)
//		return 0.0;
//
//	return T(SrcPt, FldPt, i, j) * Nl * m_J;
//}


double Element::_T_(const Vertex& SrcPt, double Xi, double Eta, int i, int j, int l)const
{
	double Nl = N(l, Xi, Eta);

	if (Nl == 0.0 || m_J == 0.0)
		return 0.0;

	return T(SrcPt, Xi, Eta, i, j) * Nl * m_J;
}


double Element::_Ts_(int s, double Xi, double Eta, int i, int j, int l)const
{
	double Nl = N(l, Xi, Eta);

	if (Nl == 0.0 || m_J == 0.0)
		return 0.0;

	return Ts(s, Xi, Eta, i, j) * Nl * m_J;
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



	if (Nl == 0.0 || m_J == 0.0)
		return 0.0;

	
	return D(SrcPt, nRule, nNode, i, j, k) * Nl * m_J;
}


//double Element::_D_(const Vertex& SrcPt, const ITND* IntegRule, int nNode, int i, int j, int k, int l)const
//{
//	double Nl = N(l, IntegRule[nNode].Xi, IntegRule[nNode].Eta);
//
//	if (Nl == 0.0 || m_J == 0.0)
//		return 0.0;
//
//	return D(SrcPt, IntegRule, nNode, i, j, k) * Nl * m_J;
//}
//
//double Element::_D_(const Vertex& SrcPt, const Vertex& FldPt, int i, int j, int k, int l)const
//{
//	double Xi, Eta;
//	GetIntrinsicCoords(FldPt, Xi, Eta);
//
//	double Nl = N(l, Xi, Eta);
//
//
//	if (Nl == 0.0 || m_J == 0.0)
//		return 0.0;
//
//	return D(SrcPt, FldPt, i, j, k) * Nl * m_J;
//}

double Element::_D_(const Vertex& SrcPt, double Xi, double Eta, int i, int j, int k, int l)const
{
	double Nl = N(l, Xi, Eta);

	if (Nl == 0.0 || m_J == 0.0)
		return 0.0;

	return D(SrcPt, Xi, Eta, i, j, k) * Nl * m_J;
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


	if (Nl == 0.0 || m_J == 0.0)
		return 0.0;

	return S(SrcPt, nRule, nNode, i, j, k) * Nl * m_J;
}

//double Element::_S_(const Vertex& SrcPt, const ITND* IntegRule, int nNode, int i, int j, int k, int l)const
//{
//	double Nl = N(l, IntegRule[nNode].Xi, IntegRule[nNode].Eta);
//
//	if (Nl == 0.0 || m_J == 0.0)
//		return 0.0;
//
//	return S(SrcPt, IntegRule, nNode, i, j, k) * Nl * m_J;
//}
//
//double Element::_S_(const Vertex& SrcPt, const Vertex& FldPt, int i, int j, int k, int l)const
//{
//	double Xi, Eta;
//	GetIntrinsicCoords(FldPt, Xi, Eta);
//
//	double Nl = N(l, Xi, Eta);
//
//	if (Nl == 0.0 || m_J == 0.0)
//		return 0.0;
//
//	return S(SrcPt, FldPt, i, j, k) * Nl * m_J;
//}

double Element::_S_(const Vertex& SrcPt, double Xi, double Eta, int i, int j, int k, int l)const
{
	double Nl = N(l, Xi, Eta);

	if (Nl == 0.0 || m_J == 0.0)
		return 0.0;

	return S(SrcPt, Xi, Eta, i, j, k) * Nl * m_J;
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


	if (Nl == 0.0 || m_J == 0.0)
		return 0.0;

	return W(SrcPt, nRule, nNode, i, j, k) * Nl * m_J;
}

//double Element::_W_(const Vertex& SrcPt, const ITND* IntegRule, int nNode, int i, int j, int k, int l)const
//{
//	double Nl = N(l, IntegRule[nNode].Xi, IntegRule[nNode].Eta);
//
//	if (Nl == 0.0 || m_J == 0.0)
//		return 0.0;
//
//	return W(SrcPt, IntegRule, nNode, i, j, k) * Nl * m_J;
//}
//
//double Element::_W_(const Vertex& SrcPt, const Vertex& FldPt, int i, int j, int k, int l)const
//{
//	double Xi, Eta;
//	GetIntrinsicCoords(FldPt, Xi, Eta);
//
//	double Nl = N(l, Xi, Eta);
//
//	if (Nl == 0.0 || m_J == 0.0)
//		return 0.0;
//
//	return W(SrcPt, FldPt, i, j, k) * Nl * m_J;
//}

double Element::_W_(const Vertex& SrcPt, double Xi, double Eta, int i, int j, int k, int l)const
{
	double Nl = N(l, Xi, Eta);

	if (Nl == 0.0 || m_J == 0.0)
		return 0.0;

	return W(SrcPt, Xi, Eta, i, j, k) * Nl * m_J;
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


	if (Nl == 0.0 || m_J == 0.0)
		return 0.0;

	return V(SrcPt, nRule, nNode, i, j, k) * Nl * m_J;
}

//double Element::_V_(const Vertex& SrcPt, const ITND* IntegRule, int nNode, int i, int j, int k, int l)const
//{
//	double Nl = N(l, IntegRule[nNode].Xi, IntegRule[nNode].Eta);
//
//	if (Nl == 0.0 || m_J == 0.0)
//		return 0.0;
//
//	return V(SrcPt, IntegRule, nNode, i, j, k) * Nl * m_J;
//}
//
//double Element::_V_(const Vertex& SrcPt, const Vertex& FldPt, int i, int j, int k, int l)const
//{
//	double Xi, Eta;
//	GetIntrinsicCoords(FldPt, Xi, Eta);
//
//	double Nl = N(l, Xi, Eta);
//
//	if (Nl == 0.0 || m_J == 0.0)
//		return 0.0;
//
//	return V(SrcPt, FldPt, i, j, k) * Nl * m_J;
//}

double Element::_V_(const Vertex& SrcPt, double Xi, double Eta, int i, int j, int k, int l)const
{
	double Nl = N(l, Xi, Eta);

	if (Nl == 0.0 || m_J == 0.0)
		return 0.0;

	return V(SrcPt, Xi, Eta, i, j, k) * Nl * m_J;
}