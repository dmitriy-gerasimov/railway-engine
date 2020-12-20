#include "Arc.h"

Arc::Arc(std::vector<Vector3D>&& a_vertices)
	: begin(this)
	, end(this)
	, curve(std::move(a_vertices))
{
}

Arc::~Arc() = default;

auto Arc::getBegin() const -> Connector const*
{
	return &(this->begin);
}

auto Arc::getBegin() -> Connector*
{
	return &(this->begin);
}

auto Arc::getEnd() const -> Connector const*
{
	return &(this->end);
}

auto Arc::getEnd() -> Connector*
{
	return &(this->end);
}

auto Arc::getLength() const -> double
{
	return this->curve.getLength();
}

auto Arc::getCurve() const -> Curve const&
{
	return curve;
}
