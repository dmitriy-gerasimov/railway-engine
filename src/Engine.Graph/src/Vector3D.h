#pragma once

class Vector3D final
{
public:
	Vector3D(Vector3D const& a_value) = default;
	Vector3D& operator= (Vector3D const& a_value) = default;

	Vector3D(Vector3D&& a_value) = default;
	Vector3D& operator= (Vector3D&& a_value) = default;

	Vector3D(double x = 0.0, double y = 0.0, double z = 0.0);

	Vector3D operator+ (Vector3D const& a_location) const;
	Vector3D operator- (Vector3D const& a_location) const;
	Vector3D operator* (double a_multiplier) const;
	Vector3D operator/ (double a_multiplier) const;

	// friend Vector3D operator+ (Vector3D const& a_location1, Vector3D const& a_location2);
	// friend Vector3D operator- (Vector3D const& a_location1, Vector3D const& a_location2);

	Vector3D operator+= (Vector3D const& a_location);
	Vector3D operator-= (Vector3D const& a_location);
	Vector3D operator*= (double a_multiplier);
	Vector3D operator/= (double a_multiplier);

	double dotProduct(Vector3D const& a_location) const;
	Vector3D crossProduct(Vector3D const& a_location) const;

	double getEuclideanDistanceTo(Vector3D const& a_vector) const;

	static Vector3D linearInterpolation(Vector3D a_vector1, Vector3D a_vector2, double percent);

	double magnitude() const;

	void normalize();

	double getX() const;
	double getY() const;
	double getZ() const;

private:
	double x;
	double y;
	double z;
};