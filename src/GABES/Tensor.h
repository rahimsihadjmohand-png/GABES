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

		double VonMises()const;
	};
};


