#pragma once

#include "Vector3D.h"

class Connector;

class ArcLocation final
{
public:
	enum class MoveResult : size_t
	{
		INVALID,
		PASSED,
		CUT,
		NO_WAY
	};

	ArcLocation(ArcLocation const& a_arcLocation) = default;
	auto operator= (ArcLocation const& a_arcLocation) -> ArcLocation& = default;

	ArcLocation(ArcLocation&& a_arcLocation) = default;
	auto operator= (ArcLocation&& a_arcLocation) -> ArcLocation& = default;

	ArcLocation(Connector const* a_connector, double a_distance, bool a_direction);

	~ArcLocation();

	auto move(double a_deltaDistance, bool a_shouldCut = true) -> bool;

	auto invert() -> void;

	auto getWorldLocation() const -> Vector3D;
	
private:
	Connector const* connector;

	double distance;

	bool direction;
};