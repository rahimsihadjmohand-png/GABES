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


double Tensor::Tresca()const
{
	Vector P;
	Principal(P);

	double smax = max(P.x, max(P.y, P.z));
	double smin = min(P.x, min(P.y, P.z));

	return smax - smin;   // Tresca equivalent stress
}


double Tensor::VonMises()const
{
	return sqrt((pow(_11 - _22, 2.0) + pow(_22 - _33, 2.0) + pow(_33 - _11, 2.0) +
		6.0 * (pow(_12, 2.0) + pow(_23, 2.0) + pow(_31, 2.0))) / 2.0);
}


double Tensor::_I1()const
{
	return _11 + _22 + _33;
}

double Tensor::_I2()const
{
	return (_11 * _22 + _22 * _33 + _33 * _11) - (_12 * _12 + _23 * _23 + _31 * _31);
}

double Tensor::_I3()const
{
	return (_11 * _22 * _33 + 2.0 * _12 * _23 * _31) - (_11 * _23 * _23 + _22 * _31 * _31 + _33 * _12 * _12);
}

double Tensor::Principal(int k)const
{
	// Get the invarients
	double I1 = _I1();
	double I2 = _I2();
	double I3 = _I3();

	// Get Q and R
	double Q = (3.0 * I2 - I1 * I1) / 9.0;
	double R = (2.0 * I1 * I1 * I1  - 9.0 * I1 * I2 + 27.0 * I3) / 54.0;

	// Calculate theta
	double theta = acos(R / sqrt(-Q*Q*Q));

	// Return the principal value in direction k
	return 2.0 * sqrt(-Q) * cos((theta + 2.0 * k * M_PI) / 3.0) + I1 / 3.0;
}

Vector Tensor::Principal()const
{
	return Vector(Principal(0), Principal(1), Principal(2));
}

void Tensor::Principal(Vector& rhs)const
{
	rhs.Set(Principal(0), Principal(1), Principal(2));
}