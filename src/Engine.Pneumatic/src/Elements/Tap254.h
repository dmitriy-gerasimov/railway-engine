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

	// переменные с названиями вида a_x_y: 1 - если открыто соединение  между резервуарами x и y, иначе - 0
	// pm - питательная магистраль
	// mvt - магистраль вспомогательного тормоза
	// im - импульсная магистраль
	// np - полость, к которой подключается импульсная магистраль
	// vp - полость между поршнем и верхним диском
	int		a_pm_mvt, a_im_np, a_im_vp, a_mvt_atm;

	// переменные с названиями вида r_x_y: коэффициент, влияющий на скорость перетекания воздуха между резервуарами x и y
	double	r_pm_mvt, r_im_np, r_im_vp, r_mvt_atm;

private:
	static double const EPSILON;

	// положение крана 254
	int m_position;

	// true - если открыта блокировка 367
	bool m_disconnectUnit;

	// true - если открыто соединение с ПМ
	bool m_isPMTapOpened;

	// true - если открыто соединение с ТМ
	bool m_isIMTapOpened;

	// true - если открыто соединение с МВТ
	bool m_isMVTTapOpened;

	// давление в полости, к которой подключается импульсная магистраль
	double pnp, pnp0;

	// давление в полости между поршнем и верхним диском
	double pvp, pvp0;

	// давление в дополнительной камере
	double pdk, pdk0;

	// давление в ПМ
	double pmPressure;

	// давление в МВТ
	double mvtPressure;

	// давление в ИМ
	double imPressure;

	// переменные с названиями вида a_x_y: 1 - если открыто соединение  между резервуарами x и y, иначе - 0
	// dk - дополнительная камера
	// vp - полость между поршнем и верхним диском
	// atm - атмосфера
	int		a_vp_dk, a_vp_atm;

	// переменные с названиями вида r_x_y: коэффициент, влияющий на скорость перетекания воздуха между резервуарами x и y
	double	r_vp_dk, r_vp_atm;
};