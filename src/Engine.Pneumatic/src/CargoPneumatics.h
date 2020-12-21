#pragma once

#include "Pneumatic.h"

#include "Elements/VR483.h"

class CargoPneumatics : public Pneumatic
{
public:
	CargoPneumatics(CargoPneumatics const& a_pneumatics) = delete;
	auto operator= (CargoPneumatics const& a_pneumatics) -> CargoPneumatics& = delete;

	CargoPneumatics(CargoPneumatics&& a_pneumatics) = delete;
	auto operator= (CargoPneumatics&& a_pneumatics) -> CargoPneumatics& = delete;

	CargoPneumatics();

	~CargoPneumatics() override;

	auto update(double a_deltaSeconds) -> void override;

	auto refresh() -> void override;

	auto getBrakeForce() const -> double override;
	
	auto getTMPressure() const -> double override;
	
	auto getPMPressure() const -> double;

	auto getURPressure() const -> double;

	auto getTCPressure() const -> double;

	auto setVelocity(double a_velocity);

	auto getBrakeForceFactor() const -> double;

	auto setBrakeForceFactor(double a_factor) -> void;

	auto getZRPressure() const -> double;

	auto getVRValveIsOpened() const -> bool override;

	auto setVRValveIsOpened(bool a_value) -> void override;

	auto setDefaultPneumaticValues() -> void override;

private:
	static double const EPSILON;

	void dpTM(double a_deltaSeconds);

	void dpTC(double a_deltaSeconds);

	// питательная магистраль
	double Vpm, ppm, ppm0;
	// тормозная магистраль
	double Vtm, ptm, ptm0;
	// тормозной цилиндр
	double Vtc, ptc, ptc0;

	// переменные с названиями вида a_x_y: 1 - если открыто соединение  между резервуарами x и y, иначе - 0
	// tm - тормозная магистраль
	// atm - атмосфера
	int		a_tm_atm_leak;

	// переменные с названиями вида r_x_y: коэффициент, влияющий на скорость перетекания воздуха между резервуарами x и y
	double	r_tm_atm_leak;

	VR483 vr483;

	// скорость, м/с
	double velocity;

	// коэффициент для корректировки силы торможения
	double brakeForceFactor;
};