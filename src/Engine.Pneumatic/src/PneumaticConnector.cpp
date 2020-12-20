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
		// �������� ���� ������
		// ������ �� ������� �� ��
		return 0.0;
	}

	if (otherConnector == nullptr || !isTmPipeConnected)
	{
		// �������� ���� ������, �� � ��� ��� ��������� ������ ��� �� �� ��������� � �������� �������
		// ��������� ������ �� �� � ���������
		return -1.0 / r_tm_atm * getTmPressure();
	}

	if (!otherConnector->getValveIsOpened())
	{
		// �������� ���� ������, � ��� ���� �������� �����, �� � ���� ������ �������� ����
		// ������ �� ������� �� ��
		return 0.0;
	}

	// �������� ���� ������, � ��� ���� �������� ����� � � ���� ������ �������� ����
	// ������ ���������� �� �������� �� � ����
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
