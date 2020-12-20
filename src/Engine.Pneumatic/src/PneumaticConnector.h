#pragma once

class Pneumatic;

class PneumaticConnector final
{
public:
	PneumaticConnector(PneumaticConnector const& a_connector) = delete;
	auto operator= (PneumaticConnector const& a_connector) -> PneumaticConnector& = delete;

	PneumaticConnector(PneumaticConnector&& a_connector) = delete;
	auto operator= (PneumaticConnector&& a_connector) -> PneumaticConnector & = delete;

	PneumaticConnector(Pneumatic const* a_pneumatic);
	
	auto getConnectorFlow() const -> double;

	auto getTmPressure() const -> double;

	auto getValveIsOpened() const -> bool;

	auto setValveIsOpened(bool a_value) -> void;

	auto getOtherConnector() const ->PneumaticConnector const*;

	auto setOtherConnector(PneumaticConnector const* a_connector) -> void;

	auto setIsTmPipeConnected(bool a_value) -> void;
	
private:
	Pneumatic const* pneumatic;
	
	PneumaticConnector const* otherConnector;

	double r_tm_otherTm;

	double r_tm_atm;

	bool valveIsOpened;

	bool isTmPipeConnected;
};