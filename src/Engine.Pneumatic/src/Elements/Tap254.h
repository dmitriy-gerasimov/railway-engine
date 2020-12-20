#pragma once

class Tap254 final
{
public:
	Tap254();

	void update(double a_deltaSeconds, double a_pmPressure, double a_mvtPressure, double a_imPressure);

	void refresh();

	static double const V_NP, V_VP, V_DK;

	auto getPosition() const -> int;
	auto setPosition(int a_position) -> void;

	auto getDisconnectUnit() const -> bool;
	auto setDisconnectUnit(bool a_disconnectUnit) -> void;

	auto getIsPMTapOpened() const -> bool;
	auto setIsPMTapOpened(bool a_isOpened) -> void;

	auto getIsIMTapOpened() const -> bool;
	auto setIsIMTapOpened(bool a_isOpened) -> void;

	auto getIsMVTTapOpened() const -> bool;
	auto setIsMVTTapOpened(bool a_isOpened) -> void;

	auto getNPPressure() const -> double;

	auto getVPPressure() const -> double;

	void dpNP(double a_deltaSeconds);
	
	void dpVP(double a_deltaSeconds);
	
	void dpDK(double a_deltaSeconds);

	int		a_pm_mvt, a_im_np, a_im_vp, a_mvt_atm;
	double	r_pm_mvt, r_im_np, r_im_vp, r_mvt_atm;

private:
	static double const EPSILON;

	void checkPressure(double a_pressure) const;

	int m_position;

	bool m_disconnectUnit;

	bool m_isPMTapOpened;

	bool m_isIMTapOpened;

	bool m_isMVTTapOpened;

	double pnp, pnp0;
	
	double pvp, pvp0;
	
	double pdk, pdk0;

	double pmPressure;

	double mvtPressure;

	double imPressure;

	int		a_vp_dk, a_vp_atm;
	double	r_vp_dk, r_vp_atm;
};