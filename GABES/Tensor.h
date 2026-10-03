#pragma once
#include "Matrix.h"

namespace BEM_3D
{
	class Tensor : public Matrix
	{
	public:
		Tensor();
		Tensor(double __11, double __22, double __33, double __12, double __13, double __23);
		Tensor(const Matrix& rhs);
		virtual ~Tensor() {}

		// Calculates the principal stress tensor
		double Principal(int k)const; // k = 0, 1, 2 the index of the principal value
		Vector Principal()const;
		void Principal(Vector& rhs)const;

		// Calculates the Tresca equivalent stress
		double Tresca()const;

		// Calculates the von-Mises equivalent stress
		double VonMises()const;

	private:
		// The tensor invarients
		double _I1()const;
		double _I2()const;
		double _I3()const;
	};
};


