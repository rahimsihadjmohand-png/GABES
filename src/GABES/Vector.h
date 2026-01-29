#pragma once


namespace BEM_3D
{
	class Vector
	{
	public:
		// Constructors;
		Vector();
		Vector(double x_, double y_, double z_);
		
		virtual ~Vector() {}

		// getters
		double Magnitude()const;
		Vector UnitVector()const;

		// Modifiers
		void Normalize();
		void Set(double x_, double y_, double z_);

		// operators
		// binary operators
		Vector operator * (double fac)const;        // multiplication by a real
		Vector operator / (double fac)const;        // division by a real
		Vector operator + (const Vector& rhs)const; // vector addition
		Vector operator - (const Vector& rhs)const; // vector subtraction
		Vector& operator += (const Vector& rhs);    // vector incrementation
		Vector& operator -= (const Vector& rhs);    // vector decrementation
		double operator * (const Vector& rhs)const; // dot product
		Vector operator ^ (const Vector& rhs)const; // cross product
		// index
		double operator [] (int k)const;
		double& operator [] (int k);
		// logilal
		bool operator == (const Vector& rhs)const; // Check if V1 == V2
		bool operator != (const Vector& rhs)const; // Check if V1 != V2




	public:
		double x;
		double y;
		double z;
	};
};



