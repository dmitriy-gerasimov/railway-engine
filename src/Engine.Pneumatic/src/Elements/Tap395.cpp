#include "Tap395.h"

#include <exception>

double const Tap395::EPSILON = 0.000'001;

double const Tap395::V_UP = 0.001;
double const Tap395::V_UR = 0.02;

Tap395::Tap395()
	: a_pm_up(0)
	, a_pm_up_red(0)
	, a_pm_tm(0)
	, a_pm_tm_por(0)
	, a_up_ur(1)
	, r_pm_up(1000.0)
	, r_pm_up_red(2000.0)
	, r_pm_tm(20.0)
	, r_pm_tm_por(40.0)
	, r_up_ur(20.0)
	, a_up_tm(0)
	, a_up_atm(0)
	, a_up_atm_stb(0)
	, a_ur_atm(0)
	, a_tm_atm(0)
	, a_tm_atm_por(0)
	, r_up_tm(10.0)
	, r_up_atm(75.0)
	, r_up_atm_stb(100'000.0)
	, r_ur_atm(5000.0)
	, r_tm_atm(25.0)
	, r_tm_atm_por(30.0)
	, brakesSignal(0)
	, pup(0.0)
	, pup0(0.0)
	, pur(0.0)
	, pur0(0.0)
	, m_position(Position::II)
	, m_combinedTapPosition(0)
	, m_isPMTapOpened(true)
	, m_targetURPressure(5.5)
	, m_disconnectUnit(false)
	, pmPressure(0)
	, tmPressure(0.0)
{
}

void Tap395::update(double a_deltaSeconds, double a_pmPressure, double a_tmPressure)
{
	pmPressure = a_pmPressure;
	tmPressure = a_tmPressure;

	switch (m_combinedTapPosition)
	{
		case -1: // Положение экстренного торможения
		{
			updateEmergencyBrakePosition();
		}
		break;
		case 0: // Поездное положение
		{
			updateTrainPosition();
		}
		break;
		case 1: // Положение двойной тяги
		{
			updateDoubleTractionPosition();
		}
		break;
		default:
		{
			updateEmergencyBrakePosition();
		}
		break;
	}

	dpUP(a_deltaSeconds);

	dpUR(a_deltaSeconds);
}

auto Tap395::refresh() -> void
{
	pup0 = pup;
	pur0 = pur;
}

auto Tap395::updateEmergencyBrakePosition() -> void
{
	a_pm_up = m_position == Position::I && m_isPMTapOpened ? 1 : 0;
	a_pm_up_red = isReleasePosition() && pur < m_targetURPressure && m_isPMTapOpened ? 1 : 0;
	a_pm_tm = 0;
	a_up_ur = 1;
	a_up_atm = m_position == Position::VI ? 1 : 0;
	a_up_atm_stb = isReleasePosition() && pup > m_targetURPressure ? 1 : 0;
	a_up_tm = 0;
	a_tm_atm = 1;
	a_ur_atm = isBrakeingPosition() ? 1 : 0;
	r_ur_atm = m_position == Position::Va ? 1600.0 : m_position == Position::V ? 800.0 : m_position == Position::VI ? 500.0 : 5000.0;

	// обновление состояния уравнительного поршня
	a_tm_atm_por = 1;
	a_pm_tm_por = 0;
}

auto Tap395::updateTrainPosition() -> void
{
	// прямые каналы им ПМ в УП и из ПМ в ТМ
	if (m_position == Position::I)
	{
		a_pm_up = m_isPMTapOpened && m_disconnectUnit ? 1 : 0;
		a_pm_tm = m_isPMTapOpened && m_disconnectUnit ? 1 : 0;
	}
	else
	{
		a_pm_up = 0;
		a_pm_tm = 0;
	}

	// перетекание воздуха из ПМ в УП через редуктор,
	// выход воздуха из УП в атмосферу через стабилизатор
	if (isReleasePosition())
	{
		a_pm_up_red = pur < m_targetURPressure && m_isPMTapOpened && m_disconnectUnit ? 1 : 0;
		a_up_atm_stb = pup > m_targetURPressure ? 1 : 0;
	}
	else
	{
		a_pm_up_red = 0;
		a_up_atm_stb = 0;
	}

	// перетекание воздуха из УП в ТМ через обратный клапан
	if (m_position == Position::III)
	{
		a_up_tm = pup > tmPressure ? m_disconnectUnit : 0;
	}
	else
	{
		a_up_tm = 0;
	}

	// выход воздуха из УР в атмосферу
	if (isBrakeingPosition())
	{
		a_ur_atm = 1;

		// коэффициенты для выхода воздуха из УР в атмосферу
		if (m_position == Position::Va)
		{
			r_ur_atm = 8'000.0;
		}
		else if (m_position == Position::V)
		{
			r_ur_atm = 900.0;
		}
		else if (m_position == Position::VI)
		{
			r_ur_atm = 500.0;
		}
	}
	else
	{
		a_ur_atm = 0;
	}

	// выход воздуха из УП и ТМ в атмосферу
	if (m_position == Position::VI)
	{
		a_up_atm = 1;
		a_tm_atm = 1;
	}
	else
	{
		a_up_atm = 0;
		a_tm_atm = 0;
	}

	// обновление состояния уравнительного поршня
	if (tmPressure > pup + 0.05)
	{
		a_tm_atm_por = 1;
		a_pm_tm_por = 0;
	}
	else if (tmPressure < pup - 0.01)
	{
		a_tm_atm_por = 0;
		a_pm_tm_por = m_isPMTapOpened ? 1 : 0;
	}
	else
	{
		a_tm_atm_por = 0;
		a_pm_tm_por = 0;
	}
}

auto Tap395::updateDoubleTractionPosition() -> void
{
	a_pm_up = m_position == Position::I && m_isPMTapOpened && m_disconnectUnit ? 1 : 0;
	a_pm_up_red = isReleasePosition() && isReducerOpened() ? 1 : 0;
	a_pm_tm = 0;
	a_up_ur = 1;
	a_up_atm = m_position == Position::VI ? 1 : 0;
	a_up_atm_stb = isReleasePosition() && pup > m_targetURPressure ? 1 : 0;
	a_up_tm = 0;
	a_tm_atm = 0;
	a_ur_atm = isBrakeingPosition() ? 1 : 0;
	r_ur_atm = m_position == Position::Va ? 1600.0 : m_position == Position::V ? 800.0 : m_position == Position::VI ? 500.0 : 5000.0;

	// обновление состояния уравнительного поршня
	a_tm_atm_por = 0;
	a_pm_tm_por = 0;
}

auto Tap395::isReducerOpened() const -> bool
{
	return pur < m_targetURPressure && m_isPMTapOpened && m_disconnectUnit;
}

auto Tap395::isReleasePosition() const -> bool
{
	return m_position == Position::I || m_position == Position::II;
}

auto Tap395::isDisconnectPosition() const -> bool
{
	return m_position == Position::III || m_position == Position::IV;
}

auto Tap395::isBrakeingPosition() const -> bool
{
	return m_position == Position::Va || m_position == Position::V || m_position == Position::VI;
}

auto Tap395::getPosition() const -> int
{
	return static_cast<int>(m_position);
}

auto Tap395::setPosition(int a_position) -> void
{
	if (a_position < 1 || a_position > 7)
		throw std::exception();
	
	m_position = static_cast<Position>(a_position);
}

auto Tap395::getCombinedTapPosition() const -> int
{
	return m_combinedTapPosition;
}

auto Tap395::setCombinedTapPosition(int a_combinedTapPosition) -> void
{
	m_combinedTapPosition = a_combinedTapPosition;
}

auto Tap395::getIsPMTapOpened() const -> bool
{
	return m_isPMTapOpened;
}

auto Tap395::setIsPMTapOpened(bool a_isPmTapOpened) -> void
{
	m_isPMTapOpened = a_isPmTapOpened;
}

auto Tap395::getTargetURPressure() const -> double
{
	return m_targetURPressure;
}

auto Tap395::setTargetURPressure(double a_targetURPressure) -> void
{
	m_targetURPressure = a_targetURPressure;
}

auto Tap395::getDisconnectUnit() const -> bool
{
	return m_disconnectUnit;
}

auto Tap395::setDisconnectUnit(bool a_disconnectUnit) -> void
{
	m_disconnectUnit = a_disconnectUnit;
}

auto Tap395::getURPressure() const -> double
{
	return pur0;
}

auto Tap395::getUPPressure() const -> double
{
	return pup0;
}

auto Tap395::dpUP(double a_deltaSeconds) -> void
{
	pup = pup0 + (
		a_pm_up / r_pm_up * (pmPressure - pup0)
		+ a_pm_up_red / r_pm_up_red * (pmPressure - pup0)
		+ a_up_ur / r_up_ur * (pur0 - pup0)
		+ a_up_tm / r_up_tm * (tmPressure - pup0)
		- a_up_atm / r_up_atm * pup0
		- a_up_atm_stb / r_up_atm_stb * pup0
		) * a_deltaSeconds / V_UP;
}

auto Tap395::dpUR(double a_deltaSeconds) -> void
{
	pur = pur0 + (
		a_up_ur / r_up_ur * (pup0 - pur0)
		- a_ur_atm / r_ur_atm * pur0
		) * a_deltaSeconds / V_UR;
}

auto Tap395::setDefaultPneumaticValues() -> void
{
	pup = pup0 = 5.0;
	pur = pur0 = 5.0;
}
