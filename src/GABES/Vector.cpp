#include "pch.h"
#include "Vector.h"


using namespace BEM_3D;


Vector::Vector()
	: x(0.0)
	, y(0.0)
	, z(0.0)
{

}


Vector::Vector(double x_, double y_, double z_)
	: x(x_)
	, y(y_)
	, z(z_)
{

}


double Vector::Magnitude()const
{
	return sqrt((x * x) + (y * y) + (z * z));
}


Vector Vector::UnitVector()const
{
	// Compute the Magnitude
	double V = Magnitude();

	return Vector(x / V, y / V, z / V);
}



void Vector::Normalize()
{
	double V = Magnitude();

	x = x / V;
	y = y / V;
	z = z / V;
}


void Vector::Set(double x_, double y_, double z_)
{
	x = x_;
	y = y_;
	z = z_;
}


Vector Vector::operator * (double fac)const
{
	return Vector(x * fac, y * fac, z * fac);
}

Vector Vector::operator / (double fac)const
{
	return Vector(x / fac, y / fac, z / fac);
}

Vector Vector::operator + (const Vector& rhs)const
{
	return Vector(x + rhs.x, y + rhs.y, z + rhs.z);
}

Vector Vector::operator - (const Vector& rhs)const
{
	return Vector(x - rhs.x, y - rhs.y, z - rhs.z);
}

Vector& Vector::operator += (const Vector& rhs)
{
	Set(x + rhs.x, y + rhs.y, z + rhs.z);
	return *this;
}

Vector& Vector::operator -= (const Vector& rhs)
{
	Set(x - rhs.x, y - rhs.y, z - rhs.z);
	return *this;
}

double Vector::operator * (const Vector& rhs)const
{
	return (x * rhs.x + y * rhs.y + z * rhs.z);
}

Vector Vector::operator ^ (const Vector& rhs)const
{
	double X = y * rhs.z - rhs.y * z;
	double Y = z * rhs.x - rhs.z * x;
	double Z = x * rhs.y - rhs.x * y;

	return Vector(X, Y, Z);
}


double Vector::operator [] (int k)const
{
	switch (k)
	{
	case 0:
		return x;

	case 1:
		return y;

	case 2:
		return z;


	default:
		break;
	}

	return 0.0; // impossible value
}

double& Vector::operator [] (int k)
{
	switch (k)
	{
	case 0:
		return x;

	case 1:
		return y;

	case 2:
		return z;


	default:
		break;
	}

	return x; // impossible value
}


bool Vector::operator == (const Vector& rhs)const
{
	return x == rhs.x && y == rhs.y && z == rhs.z;
}

bool Vector::operator != (const Vector& rhs)const
{
	return x != rhs.x || y != rhs.y || z != rhs.z;
}