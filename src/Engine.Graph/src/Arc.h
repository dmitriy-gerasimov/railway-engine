#pragma once

#include "Connector.h"

#include "Curve.h"

class Arc final
{
public:
	Arc(Arc const& a_arc) = delete;
	auto operator= (Arc const& a_arc) -> Arc& = delete;

	Arc(Arc&& a_arc) = delete;
	auto operator= (Arc&& a_arc)->Arc & = delete;

	Arc(std::vector<Vector3D>&& a_vertices);

	~Arc();

	auto getBegin() const -> Connector const*;

	auto getBegin() -> Connector*;

	auto getEnd() const -> Connector const*;

	auto getEnd() -> Connector*;

	auto getLength() const -> double;

	auto getCurve() const -> Curve const&;
	
private:
	Connector begin;

	Connector end;

	Curve curve;
};