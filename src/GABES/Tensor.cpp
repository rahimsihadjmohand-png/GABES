#include "pch.h"
#include "Tensor.h"

using namespace BEM_3D;

Tensor::Tensor()
	: Matrix()
{

}


Tensor::Tensor(double __11, double __12, double __13, double __22, double __23, double __33)
	: Matrix(__11, __12, __13,
		__12, __22, __23,
		__13, __23, __33)
{

}



Tensor::Tensor(const Matrix& rhs)
	: Matrix(rhs._11, rhs._12, rhs._13,
		rhs._21, rhs._22, rhs._23,
		rhs._31, rhs._32, rhs._33)
{

}



double Tensor::VonMises()const
{
	return sqrt((pow(_11 - _22, 2.0) + pow(_22 - _33, 2.0) + pow(_33 - _11, 2.0) +
		6.0 * (pow(_12, 2.0) + pow(_23, 2.0) + pow(_31, 2.0))) / 2.0);
}