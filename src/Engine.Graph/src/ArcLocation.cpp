#include "ArcLocation.h"

#include "Connector.h"
#include "Arc.h"
#include "Node.h"

ArcLocation::ArcLocation(Connector const* a_connector, double a_distance, bool a_direction)
	: connector(a_connector)
	, distance(a_distance)
	, direction(a_direction)
{
}

ArcLocation::~ArcLocation() = default;

auto ArcLocation::move(double a_deltaDistance, bool a_shouldCut) -> bool
{
	bool isInverted(false);

	// если a_deltaDistance отрицательный, то разворачиваем данный GraphLocation в противоположную сторону
	if (a_deltaDistance < 0.0)
	{
		invert();
		a_deltaDistance = -a_deltaDistance;
		isInverted = true;
	}

	// текущий Connector
	Connector const* currentConnector(this->connector);

	// расстояние от точки начала поиска до текущего Connector'а
	double currentDistance(-distance);

	bool success(false);

	while (true)
	{
		// рассчёт значения l_currentDistance на следующей итерации
		double const nextCurrentDistance(currentConnector->getArc()->getLength() + currentDistance);

		// проверка на необходимость дальнейшего поиска
		if (nextCurrentDistance > a_deltaDistance)
		{
			this->connector = currentConnector;
			this->distance = a_deltaDistance - currentDistance;
			success = true;
			break;
		}

		// Получение очередного Connector'а
		Connector const* anotherConnector = currentConnector->getAnother();
		Connector const* nextConnector = anotherConnector->getLinkedConnector();
		if (nextConnector == nullptr && a_shouldCut)
		{
			auto* node = anotherConnector->getNode();
			if (node != nullptr)
			{
				node->switchToOther();
				nextConnector = anotherConnector->getLinkedConnector();
			}
		}

		// Если не удалось получить очередной Connector
		if (nextConnector == nullptr)
		{
			this->connector = currentConnector;
			this->distance = this->connector->getArc()->getLength();
			success = false;
			break;
		}

		currentConnector = nextConnector;
		currentDistance = nextCurrentDistance;
	}

	// восстановление изначального направления данного GraphLocation'а
	if (isInverted)
	{
		invert();
	}

	return success;
}

auto ArcLocation::invert() -> void
{
	this->distance = this->connector->getArc()->getLength() - this->distance;
	this->connector = this->connector->getAnother();
}

auto ArcLocation::getWorldLocation() const -> Vector3D
{
	Curve const& curve = connector->getArc()->getCurve();
	
	double const distanceFromBegin = connector->isBegin() ? distance : curve.getLength() - distance;

	return curve.getLocationAtDistance(distanceFromBegin);
}
