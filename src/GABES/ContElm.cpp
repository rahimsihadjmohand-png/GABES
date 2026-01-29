#include "pch.h"
#include "ContElm.h"


using namespace BEM_3D;

double ContElm::N1s_CE[17][208];
double ContElm::N2s_CE[17][208];
double ContElm::N3s_CE[17][208];

bool ContElm::m_bCERsblShpFctsComputed = false;

ContElm::ContElm(Vertex* pV1, Vertex* pV2, Vertex* pV3, int I1, int I2, int I3)
	: Element(pV1, pV2, pV3, I1, I2, I3)
{
	ComputeReusableShapeFunctions();
	ComputeReusableIntegrationData();
}



double ContElm::N1(double Xi, double Eta)const
{

	return Xi;
}

double ContElm::N2(double Xi, double Eta)const
{
	return Eta;
}

double ContElm::N3(double Xi, double Eta)const
{
	return 1.0 - Xi - Eta;
}


const Vertex& ContElm::Vdof(int l)const
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

Vertex& ContElm::Vdof(int l)
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



const Vertex* ContElm::Vdof_Ptr(int l)const
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

Vertex* ContElm::Vdof_Ptr(int l)
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



double ContElm::Q(const Vertex& SrcPt, int i, int j, int l)const
{
	int N = 0;
	int Idx = 0;
	double I = 0.0;
	// Check for Singular case
	if (&SrcPt == m_pV1)
	{
		N = RuleToSize(_WS_VERT1);
		Idx = RuleToIndex(_WS_VERT1);

		for (size_t t = 0; t < N; t++)
		{
			double W = ITNDs[Idx][t].W;

			I += U_(SrcPt, Idx, t, i, j, l) * W;
		}

	}
	else if (&SrcPt == m_pV2)
	{
		N = RuleToSize(_WS_VERT2);
		Idx = RuleToIndex(_WS_VERT2);

		for (size_t t = 0; t < N; t++)
		{
			double W = ITNDs[Idx][t].W;

			I += U_(SrcPt, Idx, t, i, j, l) * W;
		}
	}
	else if (&SrcPt == m_pV3)
	{
		N = RuleToSize(_WS_VERT3);
		Idx = RuleToIndex(_WS_VERT3);

		for (size_t t = 0; t < N; t++)
		{
			double W = ITNDs[Idx][t].W;

			I += U_(SrcPt, Idx, t, i, j, l) * W;
		}
	}
	else
	{
		INTEG_RULE rule = SelectIntegrationRule(SrcPt, 0);

		N = RuleToSize(rule);
		Idx = RuleToIndex(rule);

		for (size_t t = 0; t < N; t++)
		{
			double W = ITNDs[Idx][t].W;

			I += U_(SrcPt, Idx, t, i, j, l) * W;
		}

		I *= 0.5;
	}

	return I;
}





double ContElm::P(const Vertex& SrcPt, int i, int j, int l)const
{
	int N = 0;
	int Idx = 0;
	double I = 0.0;


	// Check for Singular case
	if (&SrcPt == Vdof_Ptr(l))
	{
		if (!m_bRigidBodyCPV)
			I = Ps(i, j, l);
		else
			I = 0.0;

	}	
	else
	{
		INTEG_RULE rule = SelectIntegrationRule(SrcPt, 1);

		N = RuleToSize(rule);
		Idx = RuleToIndex(rule);

		for (size_t t = 0; t < N; t++)
		{
			double W = ITNDs[Idx][t].W;

			I += T_(SrcPt, Idx, t, i, j, l) * W;
		}

		I *= 0.5;
	}

	return I;
}




double ContElm::Ps(int i, int j, int l) const
{
	int l1 = l;
	int l2 = (l1 + 1) % 3;
	int l3 = (l2 + 1) % 3;

	const Vertex& V1 = Vdof(l1);
	const Vertex& V2 = Vdof(l2);
	const Vertex& V3 = Vdof(l3);

	return Element::Ps(V1, V2, V3, i, j);
}



// This function is called once !!!!
void ContElm::ComputeReusableShapeFunctions()
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