#pragma once

#include "Pneumatic.h"

#include "Elements/Tap254.h"
#include "Elements/Tap395.h"
#include "Elements/VR483.h"
#include "Elements/EPK150.h"

class LocomotivePneumatics final : public Pneumatic
{
public:
	LocomotivePneumatics(LocomotivePneumatics const& a_pneumatics) = delete;
	auto operator= (LocomotivePneumatics const& a_pneumatics) -> LocomotivePneumatics& = delete;

	LocomotivePneumatics(LocomotivePneumatics&& a_pneumatics) = delete;
	auto operator= (LocomotivePneumatics&& a_pneumatics) -> LocomotivePneumatics& = delete;

	LocomotivePneumatics();

	~LocomotivePneumatics() override;

	auto update(double a_deltaSeconds) -> void override;

	auto refresh() -> void override;

	auto getBrakeForce() const -> double override;
	
	auto getTMPressure() const -> double override;

	auto getPMPressure() const -> double;

	auto getURPressure() const -> double;

	auto getTCPressure() const -> double;

	auto getTMBeginPressure() const -> double;

	auto setTap395Position(int a_position) -> void;

	auto setTap254Position(int a_position) -> void;

	auto setIsCompressorWorking(bool a_isWorking) -> void;

	auto setEpkKey(bool a_epk) -> void;

	auto setReducer(double a_reducer) -> void;

	auto setDisconnectUnit(bool a_disconnectUnit) -> void;

	auto setCombinedTapPosition(int a_combinedTapPosition) -> void;

	auto setVelocity(double a_velocity) -> void override;

	auto getBrakeForceFactor() const -> double;

	auto setBrakeForceFactor(double a_factor) -> void;

	auto getKVVPressure() const -> double;

	auto setEpkSignal(bool a_epkSignal) -> void;

	auto getZRPressure() const -> double;

	auto getVRValveIsOpened() const -> bool override;

	auto setVRValveIsOpened(bool a_value) -> void override;

	auto setDefaultPneumaticValues() -> void override;

	auto setCompressorProductivity(double a_compressorProductivity) -> void;
	
private:
	static double const EPSILON;

	void dpGR(double a_deltaSeconds);

	void dpPM(double a_deltaSeconds);

	void dpTM(double a_deltaSeconds);

	void dpIM(double a_deltaSeconds);

	void dpMVT(double a_deltaSeconds);

	void dpTC(double a_deltaSeconds);
	
	void dpTMBegin(double a_deltaSeconds);

	// главный резервуар
	double Vgr,  pgr,  pgr0;
	// питательная магистраль
	double Vpm,  ppm,  ppm0;
	// тормозная магистраль
	double Vtm,  ptm,  ptm0;
	// импульсная магистраль
	double Vim,  pim,  pim0;
	// магистраль вспомогательного тормоза
	double Vmvt, pmvt, pmvt0;
	// тормозной цилиндр
	double Vtc,  ptc,  ptc0;
	// начало тормозной магистрали
	double VtmBegin, ptmBegin, ptmBegin0;

	// переменные с названиями вида a_x_y: 1 - если открыто соединение  между резервуарами x и y, иначе - 0
	// gr - главный резервуар
	// pm - питательная магистраль
	// tm - тормозная магистраль
	// atm - атмосфера
	// mvt - магистраль вспомогательного тормоза
	// tc -  тормозной цилиндр
	int		a_gr_pm, a_tm_atm_leak, a_mvt_tc, a_tmBegin_tm;

	// переменные с названиями вида r_x_y: коэффициент, влияющий на скорость перетекания воздуха между резервуарами x и y
	double	r_gr_pm, r_tm_atm_leak, r_mvt_tc, r_tmBegin_tm;

	Tap254 tap254;

	Tap395 tap395;

	VR483 vr483;

	EPK150 epk150;

	// скорость, м/с
	double velocity;

	// коэффициент для корректировки силы торможения
	double brakeForceFactor;

	// коэффициент, определяющий производительность компрессора
	double compressorProductivity;

	// true - если компрессор работает
	bool isCompressorWorking;

	// максимальное давление, которое может создать компрессор
	double maxCompressorPressure;
};