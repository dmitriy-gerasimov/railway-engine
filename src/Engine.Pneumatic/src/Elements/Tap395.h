#pragma once

class Tap395 final
{
public:
	enum class Position : int
	{
		I	= 1,
		II	= 2,
		III	= 3,
		IV	= 4,
		Va	= 5,
		V	= 6,
		VI	= 7
	};

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

	// переменные с названиями вида a_x_y: 1 - если открыто соединение  между резервуарами x и y, иначе - 0
	// pm - питательная магистраль
	// up - полость над уравнительным поршнем
	// tm - тормозная магистраль
	// ur - уравнительный резервуар
	int		a_pm_up, a_pm_up_red, a_pm_tm, a_pm_tm_por, a_up_ur;

	// переменные с названиями вида r_x_y: коэффициент, влияющий на скорость перетекания воздуха между резервуарами x и y
	double	r_pm_up, r_pm_up_red, r_pm_tm, r_pm_tm_por, r_up_ur;

	// переменные с названиями вида a_x_y: 1 - если открыто соединение  между резервуарами x и y, иначе - 0
	// up - полость над уравнительным поршнем
	// tm - тормозная магистраль
	// atm - атмосфера
	// ur - уравнительный резервуар	
	int		a_up_tm, a_up_atm, a_up_atm_stb, a_ur_atm, a_tm_atm, a_tm_atm_por;

	// переменные с названиями вида r_x_y: коэффициент, влияющий на скорость перетекания воздуха между резервуарами x и y
	double	r_up_tm, r_up_atm, r_up_atm_stb, r_ur_atm, r_tm_atm, r_tm_atm_por;

	int brakesSignal;

private:
	static double const EPSILON;

	// обновление состояния крана 395 при комбинированном кране в положении экстренного торможения
	auto updateEmergencyBrakePosition() -> void;

	// обновление состояния крана 395 при комбинированном кране в поездном положении
	auto updateTrainPosition() -> void;

	// обновление состояния крана 395 при комбинированном кране в положении двойной тяги
	auto updateDoubleTractionPosition() -> void;

	// возвращает true, если редуктор пропускает воздух из ПМ в полость над уравнительным поршнем
	auto isReducerOpened() const -> bool;

	// возвращает true, если крна 395 находится в отпускном положении (положение I или II)
	auto isReleasePosition() const -> bool;

	// возвращает true, если крна 395 находится в перекрыше (положение III или IV)
	auto isDisconnectPosition() const -> bool;

	// возвращает true, если крна 395 находится в тормозном положении (положение Va, V или VI)
	auto isBrakeingPosition() const -> bool;

	// давление в полости над уравнительным поршнем
	double pup, pup0;

	// давление в уравнительном резервуаре
	double pur, pur0;

	// положение крана 395
	Position m_position;

	// положение комбинированного крана (-1 - экстренное торможение, 0 - поездное, 1 - положение двойной тяги)
	int m_combinedTapPosition;

	// положение крана, соединяющего ПМ с краном 395 (открыт/закрыт)
	bool m_isPMTapOpened;

	// зарядное давление, установленное с помощью редуктора
	double m_targetURPressure;

	// true - если открыта блокировка 367
	bool m_disconnectUnit;

	// давление в ПМ
	double pmPressure;

	// давление в ТМ
	double tmPressure;
};