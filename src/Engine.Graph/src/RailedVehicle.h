#pragma once

#include "ArcLocation.h"
#include "Engine.Dynamics/src/Dynamic.h"
#include "Engine.Pneumatic/src/Pneumatic.h"
#include "Engine.Electric/src/Electric.h"

#include <memory>

class RailedVehicle final
{
public:
	RailedVehicle(RailedVehicle const& a_railedVehicle) = delete;
	auto operator= (RailedVehicle const& a_railedVehicle) -> RailedVehicle& = delete;

	RailedVehicle(RailedVehicle&& a_railedVehicle) = delete;
	auto operator= (RailedVehicle&& a_railedVehicle) -> RailedVehicle& = delete;

	RailedVehicle(
		ArcLocation a_front,
		ArcLocation a_back,
		std::unique_ptr<Dynamic>&& a_dynamic,
		std::unique_ptr<Pneumatic>&& a_pneumatic,
		std::unique_ptr<Electric>&& a_electric
	);

	~RailedVehicle() = default;

	auto update(double a_deltaSeconds) -> void;

	auto move() -> void;

	auto getGradient() const -> double;

	auto setGradient(double a_gradient) const -> void;

	auto getDynamic() const -> Dynamic*;

	auto getPneumatic() const -> Pneumatic*;

	auto getElectric() const -> Electric*;

private:
	auto updateCouplings() -> void;
	
	ArcLocation front;

	ArcLocation back;

	std::unique_ptr<Dynamic> dynamic;

	std::unique_ptr<Pneumatic> pneumatic;

	std::unique_ptr<Electric> electric;
};
