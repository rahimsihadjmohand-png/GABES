#pragma once
#include "Element.h"

namespace BEM_3D
{
	class DiscElm : public Element
	{
	public:
		DiscElm() : Element() {};
		DiscElm(Vertex* pV1, Vertex* pV2, Vertex* pV3, int I1, int I2, int I3);
		virtual ~DiscElm() {}

		virtual const Vertex& Vdof(int l)const; // Gets the Node DOF of index l = 0, 1, 2 (constant version) 
		virtual Vertex& Vdof(int l);            // Gets the Node DOF of index l = 0, 1, 2 (non constant version)
		virtual const Vertex* Vdof_Ptr(int l)const; // Get the Node DOF of index l = 0, 1, 2 in pointer form (constant version) 
		virtual Vertex* Vdof_Ptr(int l);            // Get the Node DOF of index l = 0, 1, 2 in pointer form (non constant version)
		virtual ELEMENT_TYPE inline Type()const { return DISCONTINUOUS; }
		virtual double CollFactor()const { return Lambda; }
		virtual inline double N1(double Xi, double Eta)const;    
		virtual inline double N2(double Xi, double Eta)const;  
		virtual inline double N3(double Xi, double Eta)const;  

		virtual void UpdateGeometricalVertices();
		virtual void GetIntrinsicCoords(int l, double& Xi, double& Eta)const;
		
		virtual double Ps(int i, int j, int l) const;
		virtual double Q(const Vertex& SrcPt, int i, int j, int l)const;
		virtual double P(const Vertex& SrcPt, int i, int j, int l)const;

		void ComputeReusableShapeFunctions();   // This function is called once !!!!!

		void CalculateControlNodes(); // This method calculate the internal control nodes from the geometrical boundary nodes

	private:
		virtual double N1s(int nRule, int nNode)const { return N1s_DE[nRule][nNode]; }
		virtual double N2s(int nRule, int nNode)const { return N2s_DE[nRule][nNode]; }
		virtual double N3s(int nRule, int nNode)const { return N3s_DE[nRule][nNode]; }


	private:
		// Static geometrical Data
		static double Lambda;  // The collocation factor

		// The three control Nodes
		Vertex m_Node1; // node 1
		Vertex m_Node2; // node 2
		Vertex m_Node3; // node 3

		// The shape functions
		static double N1s_DE[17][208];
		static double N2s_DE[17][208];
		static double N3s_DE[17][208];

		static bool m_bDERsblShpFctsComputed;
	};
};
