#pragma once

#include "Electric.h"

class LocomotiveElectric final : public Electric
{
public:
	LocomotiveElectric(LocomotiveElectric const& a_electric) = delete;
	auto operator= (LocomotiveElectric const& a_electric) -> LocomotiveElectric& = delete;

	LocomotiveElectric(LocomotiveElectric&& a_electric) = delete;
	auto operator= (LocomotiveElectric&& a_electric) -> LocomotiveElectric& = delete;

	LocomotiveElectric();

	virtual ~LocomotiveElectric();

	auto update(double a_deltaSeconds) -> void override;

	auto refresh() -> void  override;

	auto getTractionForce() const -> double override;

	auto getBrakeForce() const -> double override;

	auto setControllerPosition(int a_controllerPosition) -> void;

	auto setVelocity(double a_velocity) -> void override;

private:
	int controllerPosition;

	double tractionForce;

	double velocity;
};