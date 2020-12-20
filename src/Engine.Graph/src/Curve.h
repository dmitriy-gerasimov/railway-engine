#pragma once

#include "Vector3D.h"

#include <vector>

class Curve final
{
public:
	Curve(Curve const& a_curve) = delete;
	auto operator =(Curve const& a_curve) -> Curve& = delete;

	Curve(Curve&& a_curve) = delete;
	auto operator =(Curve&& a_curve) -> Curve& = delete;

	Curve(std::vector<Vector3D>&& a_vertices);

	/**
	 * ѕолучение длины ломанной (сумма длин всех отрезков)
	 */
	auto getLength() const -> double;

	/**
	 * ѕолучение 3D координат точки, наход€щейс€ на рассто€нии a_distance от начала ломанной
	 */
	auto getLocationAtDistance(double a_distance) const -> Vector3D;

	/**
	 * ѕолучение 3D координат точки по проценту на арке (от 0 до 1)
	 */
	auto getLocationFromPercent(double a_percent) const -> Vector3D;

	/**
	 * Ќаправление на рассто€нии a_distance
	 */
	auto getDirectionAtDistance(double a_distance) const -> Vector3D;

	/**
	 * ѕолучение списка всех точек
	 */
	auto getVertices() const -> std::vector<Vector3D> const&;

private:
	auto getConnectorIndex(double a_distance) const -> size_t;

	/**
	 * ќбновление списка рассто€ний (в списке рассто€ний наход€тс€ рассто€ни€ от начала ломанной до каждой из точек)
	 */
	auto updateDistances() -> void;

	std::vector<Vector3D> vertices;
	std::vector<double> distances;
//	double step;
	double length;
};
