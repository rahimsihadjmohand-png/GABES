#include "pch.h"
#include "Element_D.h"


using namespace BEM_3D;

#define _L Element::Lambda

double Element_D::N1s_DE[17][208];
double Element_D::N2s_DE[17][208];
double Element_D::N3s_DE[17][208];

bool Element_D::m_bDERsblShpFctsComputed = false;



Element_D::Element_D()
	: Element()
{
	ComputeReusableShapeFunctions();
}



Element_D::Element_D(Vertex* pV1, Vertex* pV2, Vertex* pV3, int I1, int I2, int I3) 
	: Element(pV1, pV2, pV3, I1, I2, I3) 
{
	CalculateControlNodes();
	ComputeReusableShapeFunctions();
}


Element_D::Element_D(const Vertex& rV1, const Vertex& rV2, const Vertex& rV3)
{
	m_pV1 = (Vertex*)&rV1;
	m_pV2 = (Vertex*)&rV2;
	m_pV3 = (Vertex*)&rV3;

	CalculateNormal_Jacobian();
	CalculateControlNodes();
	ComputeReusableShapeFunctions();
}



void Element_D::CalculateControlNodes()
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

	// Calculate coordinates for node 2
	m_Node2.x = _L * x1 + (1.0 - 2.0 * _L) * x2 + _L * x3;
	m_Node2.y = _L * y1 + (1.0 - 2.0 * _L) * y2 + _L * y3;
	m_Node2.z = _L * z1 + (1.0 - 2.0 * _L) * z2 + _L * z3;

	// Calculate coordinates for node 3
	m_Node3.x = _L * x1 + _L * x2 + (1.0 - 2.0 * _L) * x3;
	m_Node3.y = _L * y1 + _L * y2 + (1.0 - 2.0 * _L) * y3;
	m_Node3.z = _L * z1 + _L * z2 + (1.0 - 2.0 * _L) * z3;
}



double Element_D::N1(double Xi, double Eta)const
{

	return (Xi - _L) / (1.0 - 3.0 * _L);
}

double Element_D::N2(double Xi, double Eta)const
{
	return (Eta - _L) / (1.0 - 3.0 * _L);
}

double Element_D::N3(double Xi, double Eta)const
{
	return (1.0 - (Xi + Eta + _L)) / (1.0 - 3.0 * _L);
}


const Vertex& Element_D::Vdof(int l)const
{
	switch (l)
	{
	case 0:
		return m_Node1;

	case 1:
		return m_Node2;

	case 2:
		return m_Node3;

	default:
		break;
	}

	return Vertex(); // Impossible values
}

Vertex& Element_D::Vdof(int l)
{
	switch (l)
	{
	case 0:
		return m_Node1;

	case 1:
		return m_Node2;

	case 2:
		return m_Node3;

	default:
		break;
	}

	return Vertex(); // Impossible values
}



const Vertex* Element_D::Vdof_Ptr(int l)const
{
	switch (l)
	{
	case 0:
		return &m_Node1;

	case 1:
		return &m_Node2;

	case 2:
		return &m_Node3;

	default:
		break;
	}

	return nullptr; // Impossible values
}

Vertex* Element_D::Vdof_Ptr(int l)
{
	switch (l)
	{
	case 0:
		return &m_Node1;

	case 1:
		return &m_Node2;

	case 2:
		return &m_Node3;

	default:
		break;
	}

	return nullptr; // Impossible values
}



double Element_D::Q(const Vertex* pSrcPt, int i, int j, int l)const
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
	else if (pSrcPt == &m_Node2)
	{
		N = RuleToSize(_WS_NODE2);
		Idx = RuleToIndex(_WS_NODE2);

		for (size_t t = 0; t < N; t++)
		{
			double W = ITNDs[Idx][t].W;

			I += U_(*pSrcPt, Idx, t, i, j, l) * W;
		}
	}
	else if (pSrcPt == &m_Node3)
	{
		N = RuleToSize(_WS_NODE3);
		Idx = RuleToIndex(_WS_NODE3);

		for (size_t t = 0; t < N; t++)
		{
			double W = ITNDs[Idx][t].W;

			I += U_(*pSrcPt, Idx, t, i, j, l) * W;
		}
	}
	else
		I = RegularIntegral(&Element_D::U_, *pSrcPt, i, j, l);

	return I;
}



double Element_D::P(const Vertex* pSrcPt, int i, int j, int l)const
{
	int N = 0;
	int Idx = 0;
	double I = 0.0;


	bool bNode_CPV = ((pSrcPt == &m_Node1) || (pSrcPt == &m_Node2) || (pSrcPt == &m_Node3));

	if (bNode_CPV)
	{
		Vertex* pV1 = nullptr;
		Vertex* pV2 = nullptr;
		Vertex* pV3 = nullptr;
		
		Vertex const* pNd1 = nullptr;
		


		int s = -1;

		if (pSrcPt == &m_Node1)
		{
			s = 0;

			pV1 = m_pV1;
			pV2 = m_pV2;
			pV3 = m_pV3;
			
			pNd1 = &m_Node1;
			
		}
		else if (pSrcPt == &m_Node2)
		{
			s = 1;

			pV1 = m_pV2;
			pV2 = m_pV3;
			pV3 = m_pV1;

			pNd1 = &m_Node2;
			
		}
		else if (pSrcPt == &m_Node3)
		{
			s = 2;

			pV1 = m_pV3;
			pV2 = m_pV1;
			pV3 = m_pV2;

			pNd1 = &m_Node3;			
		}

		int l_ = (l - s + 3) % 3;

		I = Ps(*pV1, *pV2, *pV3, *pNd1, i, j, l_); // Singular part
	}
	else
		I = RegularIntegral(&Element_D::T_, *pSrcPt, i, j, l);


	return I;
}


// This function is called once !!!!
void Element_D::ComputeReusableShapeFunctions()
{
	if (m_bDERsblShpFctsComputed)
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
			N1s_DE[nRule][nNode] = N1(Xi, Eta);
			N2s_DE[nRule][nNode] = N2(Xi, Eta);
			N3s_DE[nRule][nNode] = N3(Xi, Eta);

		}
	}

	m_bDERsblShpFctsComputed = true;
}



void Element_D::Serialize(CArchive& ar)
{
	Element::Serialize(ar);

	m_Node1.Serialize(ar);
	m_Node2.Serialize(ar);
	m_Node3.Serialize(ar);
}



double Element_D::H(const Vertex& V1, const Vertex& V2, const Vertex& V3, double Theta, int i, int j, int l_, int st)const
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

	double Factor = h_ / (1.0 - 3.0 * _L);

	switch (st)
	{
	case 0:
	{
		switch (l_)
		{
		case 0:
			return Factor * ((-(1.0 - 2.0 * _L) * S1 + 2.0 * _L * S2) / (S1 + S2) + (1.0 - 3.0 * _L) * log(S0 / (S1 + S2)));

		case 1:
			return Factor * ((1.0 - _L) * S1 - _L * S2) / (S1 + S2);

		case 2:
			return Factor * -_L;	

		}
	}
	break;


	case 1:
	{
		switch (l_)
		{
		case 0:
			return Factor * (-(1.0 - 2.0 * _L) + (1.0 - 3.0 * _L) * log(S0 / (S1 + S2)));

		case 1:
			return Factor * (-_L * S1 + (1.0 - _L) * S2) / (S1 + S2);

		case 2:
			return  Factor * ((1.0 - _L) * S1 - _L * S2) / (S1 + S2);

		}
	}
	break;

	case 2:
	{
		switch (l_)
		{
		case 0:
			return Factor * ((2.0 * _L * S1 - (1.0 - 2.0 * _L) * S2) / (S1 + S2) + (1.0 - 3.0 * _L) * log(S0 / (S1 + S2)));

		case 1:
			return Factor * -_L;

		case 2:
			return Factor * (-_L * S1 + (1.0 - _L) * S2) / (S1 + S2);

		}
	}
	break;
	}


	return 0.0; // Impossible value
}



double Element_D::Ps(const Vertex& V1, const Vertex& V2, const Vertex& V3, int i, int j, int l_, int st) const
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

		I_ += (H(V1, V2, V3, Theta, i, j, l_, st) * g);

	}

	I_ = I_ * Factor;

	
	return I_;
}





double Element_D::Ps(const Vertex& V1, const Vertex& V2, const Vertex& V3, const Vertex& Node, int i, int j, int l_)const
{
	const Vertex& V4 = Node;
	
	double I1 = Ps(V4, V1, V2, i, j, l_, 0);
	double I2 = Ps(V4, V2, V3, i, j, l_, 1);
	double I3 = Ps(V4, V3, V1, i, j, l_, 2);

	return I1 + I2 + I3;
}