#include "Pneumatic.h"

#include "PneumaticConnector.h"

Pneumatic::Pneumatic()
	: frontPneumaticConnector(this)
	, backPneumaticConnector(this)
{
}

Pneumatic::~Pneumatic() = default;

auto Pneumatic::getFrontPneumaticConnector() -> PneumaticConnector*
{
	return &frontPneumaticConnector;
}

auto Pneumatic::getBackPneumaticConnector() -> PneumaticConnector*
{
	return &backPneumaticConnector;
}

auto Pneumatic::getFrontValveIsOpened() const -> bool
{
	return frontPneumaticConnector.getValveIsOpened();
}

auto Pneumatic::setFrontValveIsOpened(bool a_value) -> void
{
	frontPneumaticConnector.setValveIsOpened(a_value);
}

auto Pneumatic::getBackValveIsOpened() const -> bool
{
	return backPneumaticConnector.getValveIsOpened();
}

auto Pneumatic::setBackValveIsOpened(bool a_value) -> void
{
	backPneumaticConnector.setValveIsOpened(a_value);
}

auto Pneumatic::setIsFrontTmPipeConnected(bool a_value) -> void
{
	frontPneumaticConnector.setIsTmPipeConnected(a_value);
}

auto Pneumatic::setIsBackTmPipeConnected(bool a_value) -> void
{
	backPneumaticConnector.setIsTmPipeConnected(a_value);
}
