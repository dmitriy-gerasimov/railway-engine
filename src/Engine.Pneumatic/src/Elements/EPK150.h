#pragma once

class EPK150 final
{
public:
	EPK150(EPK150 const& a_epk150) = delete;
	auto operator= (EPK150 const& a_epk150) -> EPK150& = delete;

	EPK150(EPK150&& a_epk150) = delete;
	auto operator= (EPK150&& a_epk150) -> EPK150& = delete;
	
	EPK150();
	
	auto update(double a_deltaSeconds, double a_pmPressure, double a_tmPressure) -> void;

	auto refresh() -> void;
	
	auto getPMTapIsOpened() const -> bool;
	auto setPMTapIsOpened(bool a_value) -> void;

	auto getTMTapIsOpened() const -> bool;
	auto setTMTapIsOpened(bool a_value) -> void;

	auto getEpkKey() const -> bool;
	auto setEpkKey(bool a_value) -> void;

	auto getEpkSignal() const -> bool;
	auto setEpkSignal(bool a_value) -> void;

	auto getKvvPressure() const -> double;

	auto getPskPressure() const -> double;

	auto setDefaultPneumaticValues() -> void;
	
	//Объем полости над срывным клапаном
	static double const V_PSK;
	
	//Объем камеры выдержки времени
	static double const V_KVV;

    //переменные с названиями вида a_x_y: 1 - если открыто соединение  между резервуарами x и y, иначе - 0
  	//tm - тормозная магистраль
    //pm - питательная магистраль
    //atm - атмосфера
    //kvv - камера выдержки времени
    //psk - полости над срывным клапаном
	int		a_pm_kvv, a_tm_psk, a_tm_atm;
	//переменные с названиями вида r_x_y: коэффициент, влияющий на скорость перетекания воздуха между резервуарами x и y
	//tm - тормозная магистраль
    //pm - питательная магистраль
    //atm - атмосфера
    //kvv - камера выдержки времени
    //psk - полости над срывным клапаном
	double	r_pm_kvv, r_tm_psk, r_tm_atm;
	
private:
	static double const EPSILON;

	auto checkPressure(double a_pressure) const -> void;

	auto dpKVV(double a_deltaSeconds) -> void;
	
	auto dpPSK(double a_deltaSeconds) -> void;
	
	//кран трубопровода от Питательной магистрали
	bool pmTapIsOpened;
    //кран трубопровода от Тормозной магистрали
	bool tmTapIsOpened;
    // Ключ ЭПК True - зарядное положение False - поездное положение
	bool epkKey;
    //напряжение электромагнита
	bool epkSignal;
	//TODO
	bool epkValve;
	//TODO
	bool membrane;
    //TODO
	bool stallingValve;
    //TODO
	bool reverseValve;
	//Давление Питательной магистрали
	double pmPressure;
    //Давление Тормозной магистрали
	double tmPressure;
    //Давление камеры выдержки времени
	double pkvv;
	//Давление камеры выдержки времени на предыдущем кадре
	double pkvv0;
    //Давление полости над срывным клапаном
	double ppsk;
	//Давление полости над срывным клапаном на предыдущем кадре
	double ppsk0;
    //соединение между камерой выдержки времени и атмосферой
	int	a_kvv_atm;
	//соединение между полости над срывным клапаном и атмосферой
	int a_psk_atm;
	//коэффициент, влияющий на скорость перетекания воздуха между камерой выдержки времени и атмосферой
	double r_kvv_atm;
	//коэффициент, влияющий на скорость перетекания воздуха между полостью над срывным клапаном и атмосферой
	double r_psk_atm;
};

