#include "Dynamic.h"

#include "Engine.Graph/src/RailedVehicle.h"

#include <cmath>
#include <exception>

double const Dynamic::G = 9.8;
double const Dynamic::Epsilon = 0.000'001;

Dynamic::Dynamic(double a_mass, double a_length)
	: frontCoupling(true)
	, backCoupling(false)
	, mass(a_mass)
	, acceleration(0.0)
	, velocity(0.0)
	, offset(0.0)
	, tractionForce(0.0)
	, brakeForce(0.0)
	, gradient(0.0)
	, frictionForce(0.0)
	, frictionForceFactor(0.3)
	, distance(0.0)
	, length(a_length)
{
}

auto Dynamic::getFrontCoupling() const -> Coupling const*
{
	return &this->frontCoupling;
}

auto Dynamic::getFrontCoupling() -> Coupling*
{
	return &this->frontCoupling;
}

auto Dynamic::getBackCoupling() const -> Coupling const*
{
	return &this->backCoupling;
}

auto Dynamic::getBackCoupling() -> Coupling*
{
	return &this->backCoupling;
}

auto Dynamic::getOtherCoupling(Coupling const* a_coupling) const->Coupling const*
{
	if (a_coupling == &frontCoupling)
	{
		return &backCoupling;
	}

	if (a_coupling == &backCoupling)
	{
		return &frontCoupling;
	}

	throw std::exception("illegal argument");
}

auto Dynamic::getMass() const -> double
{
	return this->mass;
}

auto Dynamic::getLength() const -> double
{
	return length;
}

auto Dynamic::getAcceleration() const -> double
{
	return this->acceleration;
}

auto Dynamic::getVelocity() const -> double
{
	return this->velocity;
}

auto Dynamic::getVelocityKmPH() const -> double
{
	return this->velocity * 3.6;
}

auto Dynamic::getOffset() const -> double
{
	return this->offset;
}

auto Dynamic::setMass(double a_mass) -> void
{
	this->mass = a_mass;
}

auto Dynamic::getTractionForce() const -> double
{
	return this->tractionForce;
}

auto Dynamic::setTractionForce(double a_force) -> void
{
	this->tractionForce = a_force;
}

auto Dynamic::getBrakeForce() const -> double
{
	return this->brakeForce;
}

auto Dynamic::setBrakeForce(double a_force) -> void
{
	this->brakeForce = a_force;
}

auto Dynamic::getGradient() const -> double
{
	return this->gradient;
}

auto Dynamic::setGradient(double a_gradient) -> void
{
	this->gradient = a_gradient;
}

auto Dynamic::getFrictionForce() const -> double
{
	double FS;

	double const velocityKmpH = abs(this->getVelocityKmPH());

	if (velocity <= -Epsilon || velocity >= Epsilon)
		// сила трения при движении
		FS = (1.8 + 0.011 * velocityKmpH + 0.00026 * velocityKmpH * velocityKmpH) * G * this->getMass() * 0.001;
	else
		// сила трения без движения
		FS = (2.5 + 0.035 * velocityKmpH + 0.0003 * velocityKmpH * velocityKmpH) * G * this->getMass() * 0.001;

	 FS *= frictionForceFactor;

	if (velocity >= 0.0)
	{
		FS *= -1;
	}

	return FS;
	
}

auto Dynamic::update(double a_deltaSeconds) -> void
{
	// сила, возникающая на уклоне
	double const gradientForce = -gradient * mass * G;

	// результирующая сила
	double const resultTractionForce = tractionForce + gradientForce + frontCoupling.getForce() - backCoupling.getForce();

	// сила трения
	frictionForce = getFrictionForce();

	// результирующая сила торможения
	double const resultBrakeForce = brakeForce + frictionForce;

	// ускорение
	acceleration = (resultTractionForce + resultBrakeForce) / mass;

	double const previousVelocity = velocity;

	// расчёт скороси
	velocity = velocity + acceleration * a_deltaSeconds;

	// если силя тяги больше силы трения
	if (velocity * previousVelocity <= 0.0 && abs(resultTractionForce) <= abs(resultBrakeForce))
	{
		// вагон не едет
		velocity = 0.0;
		acceleration = 0.0;
	}

	// смещение вагона за текущий кадр
	offset = velocity * a_deltaSeconds;

	// путь, пройденный с момента начала движения (для отладки)
	distance += offset;

	// обновления сил, приложенных к передней автосцепке
	if (frontCoupling.hasConnectedCoupling())
		frontCoupling.update(a_deltaSeconds);

	// обновления сил, приложенных к задней автосцепке
	if (backCoupling.hasConnectedCoupling())
		backCoupling.update(a_deltaSeconds);
}

auto Dynamic::disconnectFrontCoupling() -> void
{
	this->frontCoupling.setConnectedCoupling(nullptr);
}

auto Dynamic::disconnectBackCoupling() -> void
{
	this->backCoupling.setConnectedCoupling(nullptr);
}

auto Dynamic::getFrictionForceFactor() const -> double
{
	return this->frictionForceFactor;
}

auto Dynamic::setFrictionForceFactor(double a_factor) -> void
{
	this->frictionForceFactor = a_factor;
}

auto Dynamic::getDistance() const -> double
{
	return distance;
}