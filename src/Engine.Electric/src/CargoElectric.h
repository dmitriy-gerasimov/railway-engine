#pragma once

#include "Electric.h"

class CargoElectric final : public Electric
{
public:
	CargoElectric(CargoElectric const& a_electric) = delete;
	auto operator= (CargoElectric const& a_electric) -> CargoElectric& = delete;

	CargoElectric(CargoElectric&& a_electric) = default;
	auto operator= (CargoElectric&& a_electric) -> CargoElectric & = default;

	CargoElectric();

	~CargoElectric() = default;

	auto update(double a_deltaSeconds) -> void override;

	auto refresh() -> void override;

	auto getTractionForce() const -> double override;

	auto getBrakeForce() const -> double override;
};