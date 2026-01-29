#pragma once
#include "Element.h"


namespace BEM_3D
{
	class ContElm : public Element
	{
	public:
		ContElm() : Element() {};
		ContElm(Vertex* pV1, Vertex* pV2, Vertex* pV3, int I1, int I2, int I3);
		virtual ~ContElm() {}

		virtual const Vertex& Vdof(int l)const; // Gets the Node DOF of index l = 0, 1, 2 (constant version) 
		virtual Vertex& Vdof(int l);            // Gets the Node DOF of index l = 0, 1, 2 (non constant version)
		virtual const Vertex* Vdof_Ptr(int l)const; // Get the Node DOF of index l = 0, 1, 2 in pointer form (constant version) 
		virtual Vertex* Vdof_Ptr(int l);            // Get the Node DOF of index l = 0, 1, 2 in pointer form (non constant version)
		virtual ELEMENT_TYPE inline Type()const { return CONTINUOUS; }

		virtual inline double N1(double Xi, double Eta)const;
		virtual inline double N2(double Xi, double Eta)const;
		virtual inline double N3(double Xi, double Eta)const;

		
		virtual double Ps(int i, int j, int l) const;

		//Vector A_(double Theta, int l)const;
		//double f(double Theta, int i, int j, int l)const;
		//double F(double Ro, double Theta, int i, int j, int l)const;
		//double H(double Theta, int i, int j, int l)const;
		void ComputeReusableShapeFunctions();   // This function is called once !!!!!

			

	
		virtual double Q(const Vertex& SrcPt, int i, int j, int l)const;
		virtual double P(const Vertex& SrcPt, int i, int j, int l)const;


	private:
		virtual double N1s(int nRule, int nNode)const { return N1s_CE[nRule][nNode]; }
		virtual double N2s(int nRule, int nNode)const { return N2s_CE[nRule][nNode]; }
		virtual double N3s(int nRule, int nNode)const { return N3s_CE[nRule][nNode]; }

	private:
		// The shape functions
		static double N1s_CE[17][208];
		static double N2s_CE[17][208];
		static double N3s_CE[17][208];

		static bool m_bCERsblShpFctsComputed;
	};
};


