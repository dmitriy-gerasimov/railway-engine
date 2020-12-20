#include "LocomotiveElectric.h"

LocomotiveElectric::LocomotiveElectric()
	: controllerPosition(0)
	, tractionForce(0)
{
}

LocomotiveElectric::~LocomotiveElectric() = default;

auto LocomotiveElectric::update(double a_deltaSeconds) -> void
{
	tractionForce = (double)controllerPosition * 1000.0;
}

auto LocomotiveElectric::refresh() -> void
{
}

auto LocomotiveElectric::getTractionForce() const -> double
{
	return tractionForce;
}

auto LocomotiveElectric::getBrakeForce() const -> double
{
	return 0.0;
}

auto LocomotiveElectric::setControllerPosition(int a_controllerPosition) -> void
{
	controllerPosition = a_controllerPosition;
}