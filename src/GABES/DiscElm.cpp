#include "pch.h"
#include "DiscElm.h"


using namespace BEM_3D;

double DiscElm::Lambda = 0.155;
#define _L Lambda

double DiscElm::N1s_DE[17][208];
double DiscElm::N2s_DE[17][208];
double DiscElm::N3s_DE[17][208];

bool DiscElm::m_bDERsblShpFctsComputed = false;


DiscElm::DiscElm(Vertex* pV1, Vertex* pV2, Vertex* pV3, int I1, int I2, int I3) 
	: Element(pV1, pV2, pV3, I1, I2, I3) 
{
	CalculateControlNodes();
	ComputeReusableShapeFunctions();
	ComputeReusableIntegrationData();
}



void DiscElm::CalculateControlNodes()
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



double DiscElm::N1(double Xi, double Eta)const
{

	return (Xi - _L) / (1.0 - 3.0 * _L);
}

double DiscElm::N2(double Xi, double Eta)const
{
	return (Eta - _L) / (1.0 - 3.0 * _L);
}

double DiscElm::N3(double Xi, double Eta)const
{
	return (1.0 - (Xi + Eta + _L)) / (1.0 - 3.0 * _L);
}


const Vertex& DiscElm::Vdof(int l)const
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

Vertex& DiscElm::Vdof(int l)
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



const Vertex* DiscElm::Vdof_Ptr(int l)const
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

Vertex* DiscElm::Vdof_Ptr(int l)
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



void DiscElm::UpdateGeometricalVertices()
{
	GetFieldPoint(m_pV1, 1.0, 0.0);
	GetFieldPoint(m_pV2, 0.0, 1.0);
	GetFieldPoint(m_pV3, 0.0, 0.0);
}



double DiscElm::Q(const Vertex& SrcPt, int i, int j, int l)const
{
	int N = 0;
	int Idx = 0;
	double I = 0.0;
	// Check for Singular case
	if (&SrcPt == &m_Node1)
	{
		N = RuleToSize(_WS_NODE1);
		Idx = RuleToIndex(_WS_NODE1);

		for (size_t t = 0; t < N; t++)
		{
			double W = ITNDs[Idx][t].W;

			I += U_(SrcPt, Idx, t, i, j, l) * W;
		}

	}
	else if (&SrcPt == &m_Node2)
	{
		N = RuleToSize(_WS_NODE2);
		Idx = RuleToIndex(_WS_NODE2);

		for (size_t t = 0; t < N; t++)
		{
			double W = ITNDs[Idx][t].W;

			I += U_(SrcPt, Idx, t, i, j, l) * W;
		}
	}
	else if (&SrcPt == &m_Node3)
	{
		N = RuleToSize(_WS_NODE3);
		Idx = RuleToIndex(_WS_NODE3);

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



double DiscElm::P(const Vertex& SrcPt, int i, int j, int l)const
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


void DiscElm::GetIntrinsicCoords(int l, double& Xi, double& Eta)const
{
	switch (l)
	{
	case 0:
		Xi = 1.0 - 2.0 * _L;
		Eta = _L;
		break;

	case 1:
		Xi = _L;
		Eta = 1.0 - 2.0 * _L;
		break;

	case 2:
		Xi = _L;
		Eta = _L;
		break;
	}
}





double DiscElm::Ps(int i, int j, int l) const
{

	// We must split the element into 5 triangles
	const Vertex& Vs = Vdof(l);
	const Vertex& V1 = Vgeo(0);
	const Vertex& V2 = Vgeo(1);
	const Vertex& V3 = Vgeo(2);
	const Vertex Vm1 = (V1 + V2) * 0.5;
	const Vertex Vm2 = (V2 + V3) * 0.5;
	const Vertex Vm3 = (V3 + V1) * 0.5;

	double I1 = Element::Ps(Vs, V1, Vm1, i, j);
	double I2 = Element::Ps(Vs, Vm1, V2, i, j);
	double I3 = Element::Ps(Vs, V2, Vm2, i, j);
	double I4 = Element::Ps(Vs, Vm2, V3, i, j);
	double I5 = Element::Ps(Vs, V3, Vm3, i, j);
	double I6 = Element::Ps(Vs, Vm3, V1, i, j);

	return I1 + I2 + I3 + I4 + I5 + I6;
}



// This function is called once !!!!
void DiscElm::ComputeReusableShapeFunctions()
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