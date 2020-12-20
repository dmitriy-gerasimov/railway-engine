#pragma once

#include "Engine.Dynamics/src/Dynamic.h"

#include <vector>
#include <memory>

class Coupling;

class RollingStock
{
public:
	RollingStock(RollingStock const& a_rollingStock) = delete;
	auto operator= (RollingStock const& a_rollingStock) -> RollingStock& = delete;

	RollingStock(RollingStock&& a_rollingStock) = delete;
	auto operator= (RollingStock&& a_rollingStock) -> RollingStock& = delete;

	RollingStock(std::vector<std::unique_ptr<Dynamic>>&& a_dynamicses);

	~RollingStock();

	auto getFrontCoupling() const -> Coupling const*;

	auto getBackCoupling() const -> Coupling const*;

	auto update(double a_deltaSeconds) -> void;

	// auto split(Coupling* a_coupling) -> std::unique_ptr<RollingStock>;

	// auto merge(Coupling* a_coupling)->std::unique_ptr<RollingStock>;

private:
	std::vector<std::unique_ptr<Dynamic>> dynamicses;
};