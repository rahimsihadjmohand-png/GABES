#pragma once
#include "Vector.h"

namespace BEM_3D
{
	class Matrix
	{
	public:
		Matrix();
		Matrix(double __11, double __12, double __13,
			   double __21, double __22, double __23,
			   double __31, double __32, double __33);

		Matrix(const Vector& row1, const Vector& row2, const Vector& row3);


		// Operators 
		Matrix operator + (double val)const;
		Matrix operator - (double val)const;
		Matrix operator * (double val)const;
		Matrix operator / (double val)const;

		Matrix operator + (const Matrix& rhs)const;
		Matrix operator - (const Matrix& rhs)const;
		Matrix operator * (const Matrix& rhs)const;

		void operator += (const Matrix& rhs);
		void operator -= (const Matrix& rhs);
		void operator *= (double val);

		Vector operator * (const Vector& V)const;


		static Matrix Identity();
		static Matrix TensorProduct(const Vector& V1, const Vector& V2);
		Matrix Transpose()const;
		Vector GetLineVector(int nLine)const;   // nLine is 0 based index
		Vector GetColumnVector(int nColumn)const; // nLine is 0 based index

		double Get(int i, int j)const; // Get the entry by line and column zero based indices

		double Frobenius()const;

	public:
		double _11, _12, _13;
		double _21, _22, _23;
		double _31, _32, _33;
	};
};


