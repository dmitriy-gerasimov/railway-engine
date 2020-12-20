#pragma once

class VR483 final
{
public:
	VR483(VR483 const& a_vr483) = delete;
	auto operator= (VR483 const& a_vr483) -> VR483& = delete;

	VR483(VR483&& a_vr483) = delete;
	auto operator= (VR483&& a_vr483) -> VR483& = delete;

	VR483(bool a_withAutoMode);

	auto update(double a_deltaSeconds, double a_tmPressure, double a_tcPressure) -> void;

	auto refresh() -> void;

	auto getTMTapIsOpened() const -> bool;

	auto setTMTapIsOpened(bool a_value) -> void;

	auto getLoad() const -> int;

	auto setLoad(int a_value) -> void;

	auto getMass() const -> double;

	auto setMass(double a_value) -> void;

	auto getCargoMass() const -> double;

	auto setCargoMass(double a_value) -> void;

	auto getMKPressure() const -> double;

	auto getZRPressure() const -> double;

	auto getKDRPressure() const -> double;

	auto setDefaultPneumaticValues() -> void;
	
	static double const V_MK;

	static double const V_ZK;

	static double const V_RK;

	static double const V_ZR;

	static double const V_KDR;

	int		a_kdr_tc, a_tc_atm, a_zr_tc, a_tm_zr, a_tm_mk;
	double	r_kdr_tc, r_tc_atm, r_zr_tc, r_tm_zr, r_tm_mk;

private:
	static double const EPSILON;

	auto checkPressure(double a_pressure) const -> void;

	double minP(double p1, double p2) const;

	auto dpMK(double a_deltaSeconds) -> void;

	auto dpZK(double a_deltaSeconds) -> void;

	auto dpRK(double a_deltaSeconds) -> void;

	auto dpZR(double a_deltaSeconds) -> void;

	auto dpKDR(double a_deltaSeconds) -> void;

	// магистральная камера
	double pmk,  pmk0;
	// золотниковая камера
	double pzk,  pzk0;
	// рабочая камера
	double prk,  prk0;
	// запасной резервуар
	double pzr,  pzr0;
	// камера дополнительной разрядки
	double pkdr, pkdr0;

	double maxTCPressure;

	int		a_mk_zk, a_mk_rk, a_mk_kdr, a_mk_atm, a_zk_rk;
	double	r_mk_zk, r_mk_rk, r_mk_kdr, r_mk_atm, r_zk_rk;

	int		a_rk_atm, a_kdr_atm;
	double	r_rk_atm, r_kdr_atm;

	bool withAutoMode;

	double tmPressure;
	
	double tcPressure;

	bool tmTapIsOpened;

	// 1 - порожний, 2 - средний, 3 - гружёный
	int load;

	// масса вагона, на котором установлен данный воздухораспределитель
	double mass;
	
	// масса груза в вагоне, на котором установлен данный воздухораспределитель
	double cargoMass;
};
