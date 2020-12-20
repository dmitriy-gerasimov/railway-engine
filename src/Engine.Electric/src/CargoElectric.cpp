#include "CargoElectric.h"

CargoElectric::CargoElectric() = default;


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