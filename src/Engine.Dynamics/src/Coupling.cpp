#include "Coupling.h"
#include <exception>

double const Coupling::Epsilon = 0.000'01;

Coupling::Coupling(bool a_isFront)
	: connectedCoupling(nullptr)
	, initialDistanceToConnectedCoupling(0.0)
	, k(12'000'000.0)
	, r(1'200'000.0)
	//, k(2'000'000.0)
	//, r(200'000.0)
	, isOpened(false)
	, worldLocation(Vector3D(0.0, 0.0, 0.0))
	, connectedWorldLocation(Vector3D(0.0, 0.0, 0.0))
	, velocity(0.0)
	, mass(0.0)
	, isFront(a_isFront)
	, force(0.0)
	, distanceToConnectedCoupling(0.0)
	, neighborVelocity(0.0)
	, zeroThreshold(0.03)
{
}

Coupling::~Coupling() = default;

auto Coupling::getConnectedCoupling() const -> Coupling*
{
	return this->connectedCoupling;
}

auto Coupling::setConnectedCoupling(Coupling* a_coupling) -> void
{
	this->connectedCoupling = a_coupling;

	// запоминаем расстояние между сцепками во время сцепления
	initialDistanceToConnectedCoupling = (a_coupling->getWorldLocation() - getWorldLocation()).magnitude();
}

auto Coupling::getK() const -> double
{
	return this->k;
}

auto Coupling::setK(double a_k) -> void
{
	this->k = a_k;
}

auto Coupling::getR() const -> double
{
	return this->r;
}

auto Coupling::setR(double a_r) -> void
{
	this->r = a_r;
}

auto Coupling::getIsOpened() const -> bool
{
	return this->isOpened;
}

auto Coupling::setIsOpened(bool a_isOpened) -> void
{
	this->isOpened = a_isOpened;
}

auto Coupling::getForce() const -> double
{
	return force;
}

auto Coupling::update(double /*a_deltaSeconds*/) -> void
{
	if (this->connectedCoupling == nullptr)
	{
		force = 0.0;
		return;
	}

	// 3D координата соседней сцепки
	connectedWorldLocation = connectedCoupling->getWorldLocation();
	
	// расстояние до соседней сцепки
	double distance = getDistanceToConnectedCoupling();
	
	bool const isStretching = distanceToConnectedCoupling > 0.0;

	// если сцепка открыта и растягивается состоянии, то сила через неё не передаётся
	if (isStretching && (this->getIsOpened() || this->connectedCoupling->getIsOpened()))
	{
		force = 0.0;
		return;
	}

	// скорость соседнего вагона относительно нашего вагона
	// (скорость > 0, если соседний вагон удаляется от нашего
	// скорость < 0, если  соседний вагон приближается к нашему)
	double speedDifference = (getIsFront())
		? this->connectedCoupling->getVelocity() - velocity
		: velocity - this->connectedCoupling->getVelocity();
	
	if (abs(distance) < Epsilon)
	{
		distance = 0.0;
	}

	if (abs(speedDifference) < Epsilon)
	{
		speedDifference = 0.0;
	}

	// корректировка distance для имитации свободного хода
	if (distance > zeroThreshold)
	{
		distance -= zeroThreshold;
	}
	else if (distance < -zeroThreshold)
	{
		distance += zeroThreshold;
	}
	else
	{
		distance = 0.0;
		speedDifference = 0.0;
	}

	force = (this->getK() + this->connectedCoupling->getK()) / 2.0 * distance
		+ (this->getR() + this->connectedCoupling->getR()) / 2.0 * speedDifference;
}

auto Coupling::getDistanceToConnectedCoupling() const -> double
{
	if (!hasConnectedCoupling())
	{
		return 0.0;
	}

	return (connectedWorldLocation - worldLocation).magnitude() - initialDistanceToConnectedCoupling;
}

auto Coupling::setCouplingParameters(double a_k, double a_r, double a_freeWheelAmount) -> void
{
	k = a_k;
	r = a_r;
	zeroThreshold = a_freeWheelAmount;
}

auto Coupling::hasConnectedCoupling() const -> bool
{
	return (connectedCoupling != nullptr);
}

auto Coupling::getWorldLocation() const -> Vector3D
{
	return worldLocation;
}

auto Coupling::setWorldLocation(Vector3D a_location) -> void
{
	worldLocation = a_location;
}

auto Coupling::getVelocity() const -> double
{
	return velocity;
}

auto Coupling::setVelocity(double a_velocity) -> void
{
	velocity = a_velocity;
}

auto Coupling::getMass() -> const double
{
	return mass;
}

auto Coupling::setMass(double a_mass) -> void
{
	mass = a_mass;
}

auto Coupling::getIsFront() const -> bool
{
	return isFront;
}

auto Coupling::getIsBack() const -> bool
{
	return !isFront;
}
