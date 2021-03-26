#pragma once

#include "PneumaticConnector.h"

class Pneumatic
{
public:
	Pneumatic(Pneumatic const& a_pneumatic) = delete;
	auto operator= (Pneumatic const& a_pneumatic) -> Pneumatic& = delete;

	Pneumatic(Pneumatic&& a_pneumatic) = delete;
	auto operator= (Pneumatic&& a_pneumatic)->Pneumatic & = delete;

	Pneumatic();

	virtual ~Pneumatic();

	virtual auto update(double a_deltaSeconds) -> void = 0;

	virtual auto refresh() -> void = 0;

	virtual auto getBrakeForce() const -> double = 0;

	virtual auto getTMPressure() const -> double = 0;

	auto getFrontPneumaticConnector() -> PneumaticConnector*;

	auto getBackPneumaticConnector() -> PneumaticConnector*;

	auto getFrontValveIsOpened() const -> bool;

	auto setFrontValveIsOpened(bool a_value) -> void;

	auto getBackValveIsOpened() const -> bool;

	auto setBackValveIsOpened(bool a_value) -> void;

	virtual auto getVRValveIsOpened() const -> bool = 0;

	virtual auto setVRValveIsOpened(bool a_value) -> void = 0;

	auto setIsFrontTmPipeConnected(bool a_value) -> void;

	auto setIsBackTmPipeConnected(bool a_value) -> void;

	virtual auto setDefaultPneumaticValues() -> void = 0;

	virtual auto setVelocity(double a_velocity) -> void = 0;
	
private:
	// передний пневматический соединитель
	PneumaticConnector frontPneumaticConnector;

	// задний пневматический соединитель
	PneumaticConnector backPneumaticConnector;
};