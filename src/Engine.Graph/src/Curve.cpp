#include "Curve.h"

Curve::Curve(std::vector<Vector3D>&& a_vertices)
	: vertices(std::move(a_vertices))
	, length(0.0)
{
	updateDistances();
}

double Curve::getLength() const
{
	return this->length;
}

auto Curve::getConnectorIndex(double a_distance) const -> size_t
{
	auto const numberOfVertices(this->vertices.size());
	if (numberOfVertices <= 1)
		throw std::exception("Curve must have vertices.");

	if (a_distance < 0.0 || this->distances[numberOfVertices - 1] < a_distance)
		throw std::exception("distance is too big");

	size_t connectorIndex;
	size_t connectorIndexLeft(0);
	size_t connectorIndexRight(numberOfVertices - 1);
	
	while (true)
	{
		if (connectorIndexRight - connectorIndexLeft <= 1)
		{
			connectorIndex = connectorIndexLeft;
			break;
		}

		connectorIndex = static_cast<size_t>((connectorIndexLeft + connectorIndexRight) / 2);
		if (a_distance <= this->distances[connectorIndex])
		{
			connectorIndexRight = connectorIndex;
		}
		else if (a_distance >= this->distances[connectorIndex + 1])
		{
			connectorIndexLeft = connectorIndex;
		}
		else
		{
			break;
		}
	}

	return connectorIndex;
}

Vector3D Curve::getLocationFromPercent(double a_percent) const
{
	auto const l_distance = a_percent * this->getLength();
	return getLocationAtDistance(l_distance);
}

Vector3D Curve::getLocationAtDistance(double a_distance) const
{
	size_t const connectorIndex = this->getConnectorIndex(a_distance);

	double const l_delta(this->distances[connectorIndex + 1] - this->distances[connectorIndex]);
	if (l_delta != 0.0)
	{
		return Vector3D::linearInterpolation(this->vertices[connectorIndex], this->vertices[connectorIndex + 1], (a_distance - this->distances[connectorIndex]) / l_delta);
	}
	else
	{
		return this->vertices[connectorIndex];
	}
}

auto Curve::getDirectionAtDistance(double a_distance) const -> Vector3D
{
	Vector3D tangent = { 1, 0, 0 };

	size_t const connectorIndex = this->getConnectorIndex(a_distance);
	double const delta(this->distances[connectorIndex + 1] - this->distances[connectorIndex]);
	if (delta > 0)
		tangent = this->vertices[connectorIndex + 1] - this->vertices[connectorIndex];

	return tangent;
}

std::vector<Vector3D> const& Curve::getVertices() const
{
	return this->vertices;
}

void Curve::updateDistances()
{
	auto const numberOfVertices(this->vertices.size());
	if (numberOfVertices != 0)
	{
		this->distances.clear();
		this->distances.reserve(numberOfVertices);
		double currentLength(0.0);
		for (size_t i = 0, numberOfDifferences = numberOfVertices - 1; i < numberOfDifferences; i++)
		{
			// TODO: восстановить этот код, после того, как будет убрано дублирование точек на Arc
			//if (m_vertices[i].equals(m_vertices[i + 1]))
			//	throw IllegalStateException{ _WFILE, _LINE, L"в списке m_vertices содержатся две одинаковые точки подряд" };

			double const difference = this->vertices[i].getEuclideanDistanceTo(this->vertices[i + 1]);
			this->distances.push_back(currentLength);
			currentLength += difference;
		}
		
		this->distances.push_back(currentLength);
		this->length = currentLength;
	}
}