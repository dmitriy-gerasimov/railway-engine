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
	 * ��������� ����� �������� (����� ���� ���� ��������)
	 */
	auto getLength() const -> double;

	/**
	 * ��������� 3D ��������� �����, ����������� �� ���������� a_distance �� ������ ��������
	 */
	auto getLocationAtDistance(double a_distance) const -> Vector3D;

	/**
	 * ��������� 3D ��������� ����� �� �������� �� ���� (�� 0 �� 1)
	 */
	auto getLocationFromPercent(double a_percent) const -> Vector3D;

	/**
	 * ����������� �� ���������� a_distance
	 */
	auto getDirectionAtDistance(double a_distance) const -> Vector3D;

	/**
	 * ��������� ������ ���� �����
	 */
	auto getVertices() const -> std::vector<Vector3D> const&;

private:
	auto getConnectorIndex(double a_distance) const -> size_t;

	/**
	 * ���������� ������ ���������� (� ������ ���������� ��������� ���������� �� ������ �������� �� ������ �� �����)
	 */
	auto updateDistances() -> void;

	std::vector<Vector3D> vertices;
	std::vector<double> distances;
//	double step;
	double length;
};
