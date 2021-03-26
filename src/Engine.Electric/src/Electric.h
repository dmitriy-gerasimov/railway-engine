#pragma once

class Electric abstract
{
public:
	Electric(Electric const& a_electric) = delete;
	auto operator= (Electric const& a_electric)->Electric & = delete;

	Electric(Electric&& a_electric) = delete;
	auto operator= (Electric&& a_electric)->Electric & = delete;

	Electric() = default;
	
	virtual ~Electric() = default;
	
	virtual auto update(double a_deltaSeconds) -> void = 0;

	virtual auto refresh() -> void = 0;

	virtual auto getTractionForce() const -> double = 0;

	virtual auto getBrakeForce() const -> double = 0;

	virtual auto setVelocity(double a_velocity) -> void = 0;
};