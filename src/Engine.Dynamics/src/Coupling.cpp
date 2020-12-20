#include "Coupling.h"

Coupling::Coupling(bool a_isFront)
	: connectedCoupling(nullptr)
	, initialDistanceToConnectedCoupling(0.0)
	, k(6'000'000.0)
	, r(600'000.0)
	, isOpened(false)
	, worldLocation(Vector3D(0.0, 0.0, 0.0))
	, connectedWorldLocation(Vector3D(0.0, 0.0, 0.0))
	, velocity(0.0)
	, isFront(a_isFront)
	, force(0.0)
	, distanceToConnectedCoupling(0.0)
	, neighborVelocity(0.0)
{
}

Coupling::~Coupling() = default;

auto Coupling::getConnectedCoupling() const -> Coupling const*
{
	return this->connectedCoupling;
}

auto Coupling::setConnectedCoupling(Coupling const* a_coupling) -> void
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

	connectedWorldLocation = connectedCoupling->getWorldLocation();

	// расстояние до соседней автосцепки
	distanceToConnectedCoupling = (connectedWorldLocation - worldLocation).magnitude() - initialDistanceToConnectedCoupling;

	bool const isStretching = distanceToConnectedCoupling > 0.0;

	// если сцепка открыта и находится в растянутом состоянии, то сила через неё не передаётся
	if (isStretching && (this->getIsOpened() || this->connectedCoupling->getIsOpened()))
	{
		force = 0.0;
		return;
	}

	// скорость соседнего вагона относительно нашего вагона
	neighborVelocity = (getIsFront() != connectedCoupling->getIsFront())
		? this->connectedCoupling->getVelocity()
		: -this->connectedCoupling->getVelocity();

	double const speedDifference = neighborVelocity - velocity;
	
	force = (this->getK() + this->connectedCoupling->getK()) / 2.0 * distanceToConnectedCoupling
		+ (this->getR() + this->connectedCoupling->getR()) / 2.0 * speedDifference;

	if (!isStretching)
	{
		force *= 0.8;
	}
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

auto Coupling::getIsFront() const -> bool
{
	return isFront;
}

auto Coupling::getIsBack() const -> bool
{
	return !isFront;
}
