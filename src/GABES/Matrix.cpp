#include "pch.h"
#include "Matrix.h"

using namespace BEM_3D;

Matrix::Matrix()
	: _11(0.0), _12(0.0), _13(0.0)
	, _21(0.0), _22(0.0), _23(0.0)
	, _31(0.0), _32(0.0), _33(0.0)
{

}

Matrix::Matrix(double __11, double __12, double __13,
	double __21, double __22, double __23,
	double __31, double __32, double __33)
	: _11(__11), _12(__12), _13(__13)
	, _21(__21), _22(__22), _23(__23)
	, _31(__31), _32(__32), _33(__33)
{

}

Matrix::Matrix(const Vector& row1, const Vector& row2, const Vector& row3)
	: _11(row1.x), _12(row1.y), _13(row1.z)
	, _21(row2.x), _22(row2.y), _23(row2.z)
	, _31(row3.x), _32(row3.y), _33(row3.z)
{

}



Matrix Matrix::operator + (double val)const
{
	return Matrix(_11 + val, _12 + val, _13 + val,
		_21 + val, _22 + val, _23 + val,
		_31 + val, _32 + val, _33 + val);
}

Matrix Matrix::operator - (double val)const
{
	return Matrix(_11 - val, _12 - val, _13 - val,
		_21 - val, _22 - val, _23 - val,
		_31 - val, _32 - val, _33 - val);
}

Matrix Matrix::operator * (double val)const
{
	return Matrix(_11 * val, _12 * val, _13 * val,
		_21 * val, _22 * val, _23 * val,
		_31 * val, _32 * val, _33 * val);
}

Matrix Matrix::operator / (double val)const
{
	return Matrix(_11 / val, _12 / val, _13 / val,
		_21 / val, _22 / val, _23 / val,
		_31 / val, _32 / val, _33 / val);
}

Matrix Matrix::operator + (const Matrix& rhs)const
{
	return Matrix(_11 + rhs._11, _12 + rhs._12, _13 + rhs._13,
		_21 + rhs._21, _22 + rhs._22, _23 + rhs._23,
		_31 + rhs._31, _32 + rhs._32, _33 + rhs._33);
}


Matrix Matrix::operator - (const Matrix& rhs)const
{
	return Matrix(_11 - rhs._11, _12 - rhs._12, _13 - rhs._13,
		_21 - rhs._21, _22 - rhs._22, _23 - rhs._23,
		_31 - rhs._31, _32 - rhs._32, _33 - rhs._33);
}


Matrix Matrix::operator * (const Matrix& rhs)const
{
	Matrix Res;

	Res._11 = _11 * rhs._11 + _12 * rhs._21 + _13 * rhs._31;
	Res._12 = _11 * rhs._12 + _12 * rhs._22 + _13 * rhs._32;
	Res._13 = _11 * rhs._13 + _12 * rhs._23 + _13 * rhs._33;

	Res._21 = _21 * rhs._11 + _22 * rhs._21 + _23 * rhs._31;
	Res._22 = _21 * rhs._12 + _22 * rhs._22 + _23 * rhs._32;
	Res._23 = _21 * rhs._13 + _22 * rhs._23 + _23 * rhs._33;

	Res._31 = _31 * rhs._11 + _32 * rhs._21 + _33 * rhs._31;
	Res._32 = _31 * rhs._12 + _32 * rhs._22 + _33 * rhs._32;
	Res._33 = _31 * rhs._13 + _32 * rhs._23 + _33 * rhs._33;

	return Res;
}


Vector Matrix::operator * (const Vector& V)const
{
	Vector Res;

	Res.x = _11 * V.x + _12 * V.y + _13 * V.z;
	Res.y = _21 * V.x + _22 * V.y + _23 * V.z;
	Res.z = _31 * V.x + _32 * V.y + _33 * V.z;

	return Res;
}



Matrix Matrix::Transpose()const
{
	return Matrix(_11, _21, _31, _12, _22, _32, _13, _23, _33);
}


Vector Matrix::GetLineVector(int nLine)const
{
	Vector V;

	switch (nLine)
	{
	case 0:
		V.Set(_11, _12, _13);
		break;

	case 1:
		V.Set(_21, _22, _23);
		break;

	case 2:
		V.Set(_31, _32, _33);
		break;
	}

	return V;
}


Vector Matrix::GetColumnVector(int nColumn)const
{
	Vector V;

	switch (nColumn)
	{
	case 0:
		V.Set(_11, _21, _31);
		break;

	case 1:
		V.Set(_12, _22, _32);
		break;

	case 2:
		V.Set(_13, _23, _33);
		break;
	}

	return V;
}




void Matrix::operator += (const Matrix& rhs)
{
	*this = Matrix(_11 + rhs._11, _12 + rhs._12, _13 + rhs._13,
		_21 + rhs._21, _22 + rhs._22, _23 + rhs._23,
		_31 + rhs._31, _32 + rhs._32, _33 + rhs._33);
}


void Matrix::operator -= (const Matrix& rhs)
{
	*this = Matrix(_11 - rhs._11, _12 - rhs._12, _13 - rhs._13,
		_21 - rhs._21, _22 - rhs._22, _23 - rhs._23,
		_31 - rhs._31, _32 - rhs._32, _33 - rhs._33);
}


void Matrix::operator *= (double val)
{
	_11 *= val;
	_12 *= val;
	_13 *= val;

	_21 *= val;
	_22 *= val;
	_23 *= val;
	
    _31 *= val;
	_32 *= val;
	_33 *= val;
}


Matrix Matrix::Identity()
{
	Matrix res;

	res._11 = 1.0;
	res._12 = 0.0;
	res._13 = 0.0;

	res._21 = 0.0;
	res._22 = 1.0;
	res._23 = 0.0;

	res._31 = 0.0;
	res._32 = 0.0;
	res._33 = 1.0;

	return res;
}



Matrix Matrix::TensorProduct(const Vector& V1, const Vector& V2)
{
	Matrix res;

	res._11 = V1.x * V2.x;
	res._12 = V1.x * V2.y;
	res._13 = V1.x * V2.z;

	res._21 = V1.y * V2.x;
	res._22 = V1.y * V2.y;
	res._23 = V1.y * V2.z;

	res._31 = V1.z * V2.x;
	res._32 = V1.z * V2.y;
	res._33 = V1.z * V2.z;

	return res;
}



double Matrix::Frobenius()const
{
	return sqrt(_11 * _11 + _12 * _12 + _13 * _13 +
		_21 * _21 + _22 * _22 + _23 * _23 +
		_31 * _31 + _32 * _32 + _33 * _33);
}


double Matrix::Get(int i, int j)const
{
	switch (i)
	{
	case 0:
	{
		switch (j)
		{
		case 0:
			return _11;

		case 1:
			return _12;

		case 2:
			return _13;
		}
	}
	break;

	case 1:
	{
		switch (j)
		{
		case 0:
			return _21;

		case 1:
			return _22;

		case 2:
			return _23;
		}
	}
	break;

	case 2:
	{
		switch (j)
		{
		case 0:
			return _31;

		case 1:
			return _32;

		case 2:
			return _33;
		}
	}
	break;
	}

	return 0.0; // Impossible value
}