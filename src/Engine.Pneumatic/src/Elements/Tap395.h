#pragma once

class Tap395 final
{
public:
	Tap395(Tap395 const& a_tap) = delete;
	auto operator= (Tap395 const& a_tap) -> Tap395& = delete;

	Tap395(Tap395&& a_tap) = delete;
	auto operator= (Tap395&& a_tap)->Tap395 & = delete;

	Tap395();

	auto update(double a_deltaSeconds, double a_pmPressure, double a_tmPressure) -> void;

	auto refresh() -> void;

	#pragma region "Геттеры/сеттеры для задания новых значений для управляющих элементов крана 395"

	auto getPosition() const -> int;
	auto setPosition(int a_position) -> void;

	auto getCombinedTapPosition() const -> int;
	auto setCombinedTapPosition(int a_combinedTapPosition) -> void;

	auto getIsPMTapOpened() const -> bool;
	auto setIsPMTapOpened(bool a_isPmTapOpened) -> void;

	auto getTargetURPressure() const -> double;
	auto setTargetURPressure(double a_targetURPressure) -> void;

	auto getDisconnectUnit() const -> bool;
	auto setDisconnectUnit(bool a_disconnectUnit) -> void;

	auto getURPressure() const -> double;

	auto getUPPressure() const -> double;

	#pragma endregion

	auto dpUP(double a_deltaSeconds) -> void;

	auto dpUR(double a_deltaSeconds) -> void;

	auto setDefaultPneumaticValues() -> void;
	
	static double const V_UP, V_UR;

	int		a_pm_up, a_pm_up_red, a_pm_tm, a_pm_tm_por, a_up_ur;
	double	r_pm_up, r_pm_up_red, r_pm_tm, r_pm_tm_por, r_up_ur;

	int		a_up_tm, a_up_atm, a_up_atm_stb, a_ur_atm, a_tm_atm, a_tm_atm_por;
	double	r_up_tm, r_up_atm, r_up_atm_stb, r_ur_atm, r_tm_atm, r_tm_atm_por;

	int brakesSignal;

private:
	static double const EPSILON;

	auto checkPressure(double a_pressure) const -> void;

	auto updateEqualizingPiston() -> void;

	double pup, pup0;
	double pur, pur0;

	int m_position;

	int m_combinedTapPosition;

	bool m_isPMTapOpened;

	double m_targetURPressure;

	bool m_disconnectUnit;

	double pmPressure;

	double tmPressure;
};