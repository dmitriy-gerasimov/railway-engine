#include "CargoElectric.h"

CargoElectric::CargoElectric()
	: velocity(0.0)
{
}

auto CargoElectric::update(double a_deltaSeconds) -> void
{
}

auto CargoElectric::refresh() -> void
{
}

auto CargoElectric::getTractionForce() const -> double
{
	return 0.0;
}

auto CargoElectric::getBrakeForce() const -> double
{
	return 0.0;
}

auto CargoElectric::setVelocity(double a_velocity) -> void
{
	velocity = a_velocity;
}