#include "pch.h"
#include "Element_C.h"


using namespace BEM_3D;

double Element_C::N1s_CE[17][208];
double Element_C::N2s_CE[17][208];
double Element_C::N3s_CE[17][208];

bool Element_C::m_bCERsblShpFctsComputed = false;


Element_C::Element_C()
	: Element()
{
	ComputeReusableShapeFunctions();
}



Element_C::Element_C(Vertex* pV1, Vertex* pV2, Vertex* pV3, int I1, int I2, int I3)
	: Element(pV1, pV2, pV3, I1, I2, I3)
{
	ComputeReusableShapeFunctions();
}


Element_C::Element_C(Vertex& rV1, Vertex& rV2, Vertex& rV3)
{
	m_pV1 = &rV1;
	m_pV2 = &rV2;
	m_pV3 = &rV3;

	CalculateNormal_Jacobian();
	ComputeReusableShapeFunctions();
}


double Element_C::N1(double Xi, double Eta)const
{
	return Xi;
}

double Element_C::N2(double Xi, double Eta)const
{
	return Eta;
}

double Element_C::N3(double Xi, double Eta)const
{
	return 1.0 - Xi - Eta;
}


const Vertex& Element_C::Vdof(int l)const
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

Vertex& Element_C::Vdof(int l)
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



const Vertex* Element_C::Vdof_Ptr(int l)const
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

Vertex* Element_C::Vdof_Ptr(int l)
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



double Element_C::Q(const Vertex* pSrcPt, int i, int j, int l)const
{
	int N = 0;
	int Idx = 0;
	double I = 0.0;
	// Check for Singular case
	if (pSrcPt == m_pV1)
	{
		N = RuleToSize(_WS_VERT1);
		Idx = RuleToIndex(_WS_VERT1);

		for (size_t t = 0; t < N; t++)
		{
			double W = ITNDs[Idx][t].W;

			I += U_(*pSrcPt, Idx, t, i, j, l) * W;
		}

	}
	else if (pSrcPt == m_pV2)
	{
		N = RuleToSize(_WS_VERT2);
		Idx = RuleToIndex(_WS_VERT2);

		for (size_t t = 0; t < N; t++)
		{
			double W = ITNDs[Idx][t].W;

			I += U_(*pSrcPt, Idx, t, i, j, l) * W;
		}
	}
	else if (pSrcPt == m_pV3)
	{
		N = RuleToSize(_WS_VERT3);
		Idx = RuleToIndex(_WS_VERT3);

		for (size_t t = 0; t < N; t++)
		{
			double W = ITNDs[Idx][t].W;

			I += U_(*pSrcPt, Idx, t, i, j, l) * W;
		}
	}
	else
		return RegularIntegral(&Element_C::U_, *pSrcPt, i, j, l);
	

	return I;
}





double Element_C::P(const Vertex* pSrcPt, int i, int j, int l)const
{
	int N = 0;
	int Idx = 0;
	double I = 0.0;


	bool bCPV = ((pSrcPt == m_pV1) || (pSrcPt == m_pV2) || (pSrcPt == m_pV3));

	if (bCPV)
	{
		Vertex* pV1 = nullptr;
		Vertex* pV2 = nullptr;
		Vertex* pV3 = nullptr;
		int s = -1;

		if (pSrcPt == m_pV1)
		{
			s = 0;

			pV1 = m_pV1;
			pV2 = m_pV2;
			pV3 = m_pV3;
		}
		else if (pSrcPt == m_pV2)
		{
			s = 1;

			pV1 = m_pV2;
			pV2 = m_pV3;
			pV3 = m_pV1;
		}
		else if (pSrcPt == m_pV3)
		{
			s = 2;

			pV1 = m_pV3;
			pV2 = m_pV1;
			pV3 = m_pV2;
		}

		int l_ = (l - s + 3) % 3;

		I = Ps(*pV1, *pV2, *pV3, i, j, l_);
	}	
	else
		I = RegularIntegral(&Element_C::T_, *pSrcPt, i, j, l);


	return I;
}



// This function is called once !!!!
void Element_C::ComputeReusableShapeFunctions()
{
	if (m_bCERsblShpFctsComputed)
		return;

	for (int nRule = 0; nRule < 17; nRule++)
	{
		INTEG_RULE Rule = (INTEG_RULE)nRule;
		int nDim = RuleToSize(Rule);

		for (int nNode = 0; nNode < nDim; nNode++)
		{
			double Xi = ITNDs[nRule][nNode].Xi;
			double Eta = ITNDs[nRule][nNode].Eta;

			// The Shape Functions
			N1s_CE[nRule][nNode] = N1(Xi, Eta);
			N2s_CE[nRule][nNode] = N2(Xi, Eta);
			N3s_CE[nRule][nNode] = N3(Xi, Eta);
		}
	}

	m_bCERsblShpFctsComputed = true;
}


double Element_C::H(const Vertex& V1, const Vertex& V2, const Vertex& V3, double Theta, int i, int j, int l_)const
{

	// Define local vectors
	Vector V12 = V2 - V1;
	Vector V13 = V3 - V1;

	Vector e1 = V12; e1.Normalize();
	const Vector& e3 = m_n;
	Vector e2 = e3 ^ e1;

	double l1 = V12.Magnitude();
	double l2 = V13.Magnitude();

	double Alpha = acos((V12 * V13) / (l1 * l2));

	double S0 = l1 * l2 * sin(Alpha);
	double S1 = l1 * sin(Theta);
	double S2 = l2 * sin(Alpha - Theta);

	double h_ = (e3[j] * e1[i] - e3[i] * e1[j]) * cos(Theta) + (e3[j] * e2[i] - e3[i] * e2[j]) * sin(Theta);


	switch (l_)
	{
	case 0:
		return h_ * (log(S0 / (S1 + S2)) - 1.0);

	case 1:
		return h_ * S2 / (S1 + S2);

	case 2:
		return h_ * S1 / (S1 + S2);
	}


	return 0.0; // Impossible value	
}


double Element_C::Ps(const Vertex& V1, const Vertex& V2, const Vertex& V3, int i, int j, int l_) const
{
	double Factor = (1.0 - 2.0 * v) / (8.0 * M_PI * (1.0 - v));


	// Define triangle angle
	Vector V12 = V2 - V1;
	Vector V13 = V3 - V1;
	double l1 = V12.Magnitude();
	double l2 = V13.Magnitude();
	double Alpha = acos((V12 * V13) / (l1 * l2));


	// Define Gaussian Rule
	double Xi[25];
	double W[25];


	Get1DGaussianRule(Xi, W, 25);



	// One dimension integral
	// Itegrate numerically
	double I_ = 0.0;

	for (int m = 0; m < 25; m++)
	{
		double t = Xi[m];
		double s = 2.0 * t / (1.0 + t * t);
		double ds_dt = 2.0 * (1.0 - t * t) / pow(1.0 + t * t, 2.0);
		double Theta = Alpha / 2.0 * (s + 1.0);
		double dTheta_ds = Alpha / 2.0;
		double g = dTheta_ds * ds_dt * W[m];

		I_ += (H(V1, V2, V3, Theta, i, j, l_) * g);

	}

	I_ = I_ * Factor;


	return I_;
}