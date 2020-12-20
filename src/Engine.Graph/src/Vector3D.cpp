#include "Vector3D.h"

#include <cmath>

Vector3D::Vector3D(double x, double y, double z) :
	x(x),
	y(y),
	z(z)
{
}

Vector3D Vector3D::operator+ (Vector3D const& a_location) const
{
	return {this->x + a_location.getX(), this->y + a_location.getY(), this->z + a_location.getZ()};
}

Vector3D Vector3D::operator- (Vector3D const& a_location) const
{
	return {this->x - a_location.getX(), this->y - a_location.getY(), this->z - a_location.getZ()};
}

Vector3D Vector3D::operator* (double a_multiplier) const
{
	return {this->x * a_multiplier, this->y * a_multiplier, this->z * a_multiplier};
}

Vector3D Vector3D::operator/ (double a_multiplier) const
{
	return {this->x / a_multiplier, this->y / a_multiplier, this->z / a_multiplier};
}

Vector3D Vector3D::operator+= (Vector3D const& a_location)
{
	this->x += a_location.getX();
	this->y += a_location.getY();
	this->z += a_location.getZ();
	return *this;
}

Vector3D Vector3D::operator-= (Vector3D const& a_location)
{
	this->x -= a_location.getX();
	this->y -= a_location.getY();
	this->z -= a_location.getZ();
	return *this;
}

Vector3D Vector3D::operator*= (double a_multiplier)
{
	this->x *= a_multiplier;
	this->y *= a_multiplier;
	this->z *= a_multiplier;
	return *this;
}

Vector3D Vector3D::operator/= (double a_multiplier)
{
	this->x /= a_multiplier;
	this->y /= a_multiplier;
	this->z /= a_multiplier;
	return *this;
}

double Vector3D::dotProduct(Vector3D const& a_location) const
{
	return this->x * a_location.getX() + this->y * a_location.getY() + this->z * a_location.getZ();
}

Vector3D Vector3D::crossProduct(Vector3D const& a_location) const
{
	return Vector3D(
		this->y * a_location.getZ() - this->z * a_location.getY(),
		this->z * a_location.getX() - this->x * a_location.getZ(),
		this->x * a_location.getY() - this->y * a_location.getZ()
	);
}

auto Vector3D::getEuclideanDistanceTo(Vector3D const& a_vector) const -> double
{
	Vector3D const vector = a_vector - *this;
	return sqrt(vector.dotProduct(vector));
}

auto Vector3D::linearInterpolation(Vector3D a_vector1, Vector3D a_vector2, double a_percent) -> Vector3D
{
	return a_vector1 + (a_vector2 - a_vector1) * a_percent;
}

double Vector3D::magnitude() const
{
	return sqrt(this->dotProduct(*this));
}

void Vector3D::normalize()
{
	*this /= this->magnitude();
}

double Vector3D::getX() const
{
	return this->x;
}

double Vector3D::getY() const
{
	return this->y;
}

double Vector3D::getZ() const
{
	return this->z;
}

/*Vector3D operator+(Vector3D const& a_location1, Vector3D const& a_location2)
{
	
}*/
