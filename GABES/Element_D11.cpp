#include "pch.h"
#include "Element_D11.h"


using namespace BEM_3D;

#define _L Element::Lambda

double Element_D11::N1s_D11E[17][208];
double Element_D11::N2s_D11E[17][208];
double Element_D11::N3s_D11E[17][208];

bool Element_D11::m_bD11ERsblShpFctsComputed = false;



Element_D11::Element_D11()
	: Element()
{
	ComputeReusableShapeFunctions();
}


Element_D11::Element_D11(Vertex* pV1, Vertex* pV2, Vertex* pV3, int I1, int I2, int I3)
	: Element(pV1, pV2, pV3, I1, I2, I3)
{
	CalculateControlNodes();
	ComputeReusableShapeFunctions();
}


Element_D11::Element_D11(const Vertex& rV1, const Vertex& rV2, const Vertex& rV3)
{
	m_pV1 = (Vertex*)&rV1;
	m_pV2 = (Vertex*)&rV2;
	m_pV3 = (Vertex*)&rV3;

	CalculateNormal_Jacobian();
	CalculateControlNodes();
	ComputeReusableShapeFunctions();
}



void Element_D11::CalculateControlNodes()
{
	double x1 = m_pV1->x;
	double x2 = m_pV2->x;
	double x3 = m_pV3->x;

	double y1 = m_pV1->y;
	double y2 = m_pV2->y;
	double y3 = m_pV3->y;

	double z1 = m_pV1->z;
	double z2 = m_pV2->z;
	double z3 = m_pV3->z;

	// Calculate coordinates for node 1
	m_Node1.x = (1.0 - 2.0 * _L) * x1 + _L * x2 + _L * x3;
	m_Node1.y = (1.0 - 2.0 * _L) * y1 + _L * y2 + _L * y3;
	m_Node1.z = (1.0 - 2.0 * _L) * z1 + _L * z2 + _L * z3;	
}



double Element_D11::N1(double Xi, double Eta)const
{

	return Xi / (1.0 - 2.0 * _L);
}

double Element_D11::N2(double Xi, double Eta)const
{
	return -_L / (1.0 - 2.0 * _L) * Xi + Eta;
}

double Element_D11::N3(double Xi, double Eta)const
{
	return 1.0 - ((1.0 - _L)/(1.0 - 2.0 * _L) * Xi + Eta);
}


const Vertex& Element_D11::Vdof(int l)const
{
	switch (l)
	{
	case 0:
		return m_Node1;

	case 1:
		return *m_pV2;

	case 2:
		return *m_pV3;

	default:
		break;
	}

	return Vertex(); // Impossible values
}

Vertex& Element_D11::Vdof(int l)
{
	switch (l)
	{
	case 0:
		return m_Node1;

	case 1:
		return *m_pV2;

	case 2:
		return *m_pV3;

	default:
		break;
	}

	return Vertex(); // Impossible values
}



const Vertex* Element_D11::Vdof_Ptr(int l)const
{
	switch (l)
	{
	case 0:
		return &m_Node1;

	case 1:
		return m_pV2;

	case 2:
		return m_pV3;

	default:
		break;
	}

	return nullptr; // Impossible values
}

Vertex* Element_D11::Vdof_Ptr(int l)
{
	switch (l)
	{
	case 0:
		return &m_Node1;

	case 1:
		return m_pV2;

	case 2:
		return m_pV3;

	default:
		break;
	}

	return nullptr; // Impossible values
}







double Element_D11::Q(const Vertex* pSrcPt, int i, int j, int l)const
{
	int N = 0;
	int Idx = 0;
	double I = 0.0;

	// Check for Singular case
	if (pSrcPt == &m_Node1)
	{
		N = RuleToSize(_WS_NODE1);
		Idx = RuleToIndex(_WS_NODE1);

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
		I = RegularIntegral(&Element_D11::U_, *pSrcPt, i, j, l);

	return I;
}



double Element_D11::P(const Vertex* pSrcPt, int i, int j, int l)const
{
	double I = 0.0;


	if (pSrcPt == &m_Node1)
		I = Ps(i, j, 0, l);

	else if (pSrcPt == m_pV2)
		I = Ps(i, j, 1, l);
	
	else if (pSrcPt == m_pV3)
		I = Ps(i, j, 2, l);
	
	else
		I = RegularIntegral(&Element_D11::T_, *pSrcPt, i, j, l);


	return I;
}



// This function is called once !!!!
void Element_D11::ComputeReusableShapeFunctions()
{
	if (m_bD11ERsblShpFctsComputed)
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
			N1s_D11E[nRule][nNode] = N1(Xi, Eta);
			N2s_D11E[nRule][nNode] = N2(Xi, Eta);
			N3s_D11E[nRule][nNode] = N3(Xi, Eta);

		}
	}

	m_bD11ERsblShpFctsComputed = true;
}



void Element_D11::Serialize(CArchive& ar)
{
	Element::Serialize(ar);

	m_Node1.Serialize(ar);
}



double Element_D11::H0(const Vertex& V1, const Vertex& V2, const Vertex& V3, double Theta, int i, int j, int l, int st) const
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

	double Factor = h_ / (1.0 - 2.0 * _L);

	switch (l)
	{
	case 0:
	{
		switch (st)
		{
		case 0:
			return Factor * ((-(1.0 - 2.0 * _L) * S1 + 2.0 * _L * S2)/(S1 + S2) + (1.0 - 2.0 * _L) * log(S0 / (S1 + S2)));

		case 1:
			return h_ * (-1.0 + log(S0 / (S1 + S2)));

		case 2:
			return Factor * ((2.0 * _L * S1 - (1.0 - 2.0 * _L) * S2) / (S1 + S2) + (1.0 - 2.0 * _L) * log(S0 / (S1 + S2)));

		}
	}
	break;


	case 1:
	{
		switch (st)
		{
		case 0:
			return Factor * ((1.0 - 2.0 * _L) * S1 - _L * S2) / (S1 + S2);

		case 1:
			return h_ * S2 / (S1 + S2);

		case 2:
			return Factor * (-_L * S1) / (S1 + S2);
		}
	}
	break;

	case 2:
	{
		switch (st)
		{
		case 0:
			return Factor * (-_L * S2) / (S1 + S2);		

		case 1:
			return h_ * S1 / (S1 + S2);

		case 2:
			return Factor * (-_L * S1 + (1.0 - 2.0 * _L) * S2) / (S1 + S2);
		}
	}
	break;
	}


	return 0.0; // Impossible value
}



double Element_D11::H(const Vertex& V1, const Vertex& V2, const Vertex& V3, double Theta, int i, int j, int s, int l) const
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

	double Factor = h_ / (1.0 - 2.0 * _L);

	switch (s)
	{
	case 1:
	{
		switch (l)
		{
		case 0:
			return Factor * S1 / (S1 + S2);

		case 1:
			return Factor * (-((1.0 - _L) * S1 + (1.0 - 2.0 * _L) * S2) / (S1 + S2) + (1.0 - 2.0 * _L) * log(S0 / (S1 + S2)));

		case 2:
			return Factor * (-_L * S1 + (1.0 - 2.0 * _L) * S2) / (S1 + S2);

		}
	}
	break;

	case 2:
	{
		switch (l)
		{
		case 0:
			return Factor * S2 / (S1 + S2);

		case 1:
			return Factor * ((1.0 - 2.0 * _L) * S1 -_L * S2) / (S1 + S2);

		case 2:
			return Factor * (-((1.0 - 2.0 * _L) * S1 + (1.0 - _L) * S2) / (S1 + S2) + (1.0 - 2.0 * _L) * log(S0 / (S1 + S2)));
		}
	}
	break;
	}


	return 0.0; // Impossible value
}




double Element_D11::ThetaIntegral(
	double (Element_D11::* Func)(const Vertex&, const Vertex&, const Vertex&, double, int, int, int, int)const,
	const Vertex& V1, const Vertex& V2, const Vertex& V3, int i, int j, int k1, int k2)const
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

		I_ += ((this->*Func)(V1, V2, V3, Theta, i, j, k1, k2) * g);
	}

	I_ = I_ * Factor;


	return I_;
}



double Element_D11::Psn(int i, int j, int l)const
{
	const Vertex& V1 = *m_pV1;
	const Vertex& V2 = *m_pV2;
	const Vertex& V3 = *m_pV3;
	
	const Vertex& V4 = m_Node1;
		
	double I1 = ThetaIntegral(&Element_D11::H0,V4, V1, V2, i, j, l, 0);
	double I2 = ThetaIntegral(&Element_D11::H0, V4, V2, V3, i, j, l, 1);
	double I3 = ThetaIntegral(&Element_D11::H0, V4, V3, V1, i, j, l, 2);

	return I1 + I2 + I3;
}


double Element_D11::Ps(int i, int j, int s, int l)const
{
	double I = 0.0;

	switch (s)
	{
	case 0:
	{
		I = Psn(i, j, l);
	}
	break;

	case 1:
	{
		const Vertex& V1 = *m_pV2;
		const Vertex& V2 = *m_pV3;
		const Vertex& V3 = *m_pV1;

		I = ThetaIntegral(&Element_D11::H, V1, V2, V3, i, j, s, l);
	}
	break;

	case 2:
	{
		const Vertex& V1 = *m_pV3;
		const Vertex& V2 = *m_pV1;
		const Vertex& V3 = *m_pV2;

		I = ThetaIntegral(&Element_D11::H, V1, V2, V3, i, j, s, l);
	}
	break;

	}

	return I;
}

