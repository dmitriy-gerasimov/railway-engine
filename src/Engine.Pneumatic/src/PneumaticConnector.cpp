#include "PneumaticConnector.h"

#include "Pneumatic.h"

PneumaticConnector::PneumaticConnector(Pneumatic const* a_pneumatic)
	: pneumatic(a_pneumatic)
	, otherConnector(nullptr)
	, r_tm_otherTm(1.0)
	, r_tm_atm(1.0)
	, valveIsOpened(true)
	, isTmPipeConnected(false)
{
}

auto PneumaticConnector::getConnectorFlow() const -> double
{
	if (!valveIsOpened)
	{
		// концевой кран закрыт
		// воздух не выходит из ТМ
		return 0.0;
	}

	if (otherConnector == nullptr || !isTmPipeConnected)
	{
		// концевой кран открыт, но у нас нет соседнего вагона или ТМ не соединена с соседним вагоном
		// выпускаем воздух из ТМ в атмосферу
		return -1.0 / r_tm_atm * getTmPressure();
	}

	if (!otherConnector->getValveIsOpened())
	{
		// концевой кран открыт, у нас есть соседний вагон, но у него закрыт концевой кран
		// воздух не выходит из ТМ
		return 0.0;
	}

	// концевой кран открыт, у нас есть соседний вагон и у него открыт концевой кран
	// воздух перетекает из сосденей ТМ в нашу
	return 1.0 / r_tm_otherTm * (otherConnector->getTmPressure() - getTmPressure());
}

auto PneumaticConnector::getTmPressure() const -> double
{
	return pneumatic->getTMPressure();
}

auto PneumaticConnector::getValveIsOpened() const -> bool
{
	return valveIsOpened;
}

auto PneumaticConnector::setValveIsOpened(bool a_value) -> void
{
	valveIsOpened = a_value;
}

auto PneumaticConnector::getOtherConnector() const -> PneumaticConnector const*
{
	return otherConnector;
}

auto PneumaticConnector::setOtherConnector(PneumaticConnector const* a_connector) -> void
{
	otherConnector = a_connector;
}

auto PneumaticConnector::setIsTmPipeConnected(bool a_value) -> void
{
	isTmPipeConnected = a_value;
}
