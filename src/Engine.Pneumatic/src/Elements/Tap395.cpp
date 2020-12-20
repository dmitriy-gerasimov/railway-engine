#include "Tap395.h"

#include <cmath>
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
	, r_pm_up(/*20.0*/ 1000.0)
	, r_pm_up_red(2000.0)
	, r_pm_tm(20.0)
	, r_pm_tm_por(40.0)
	, r_up_ur(20.0)
	, a_up_tm(0)
	// 16 мм
	, a_up_atm(0)
	// 5 мм
	, a_up_atm_stb(0)
	// 1.6 мм
	, a_ur_atm(0)
	// 0.45 мм
	, a_tm_atm(0)
	// 3 мм
	, a_tm_atm_por(0)
	// 
	, r_up_tm(10.0)
	// 
	, r_up_atm(75.0)
	// 
	, r_up_atm_stb(100'000.0)
	// 0.75 мм (в положении Vа), 2.3 мм (в положении V)
	, r_ur_atm(5000.0)
	// 
	, r_tm_atm(25.0)
	// 
	, r_tm_atm_por(30.0)
	, brakesSignal(0)
	, pup(0.0)
	, pup0(0.0)
	, pur(0.0)
	, pur0(0.0)
	, m_position(2)
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
		a_pm_up = m_position == 1 && m_isPMTapOpened ? 1 : 0;
		a_pm_up_red = (m_position == 1 || m_position == 2) && pur < m_targetURPressure&& m_isPMTapOpened ? 1 : 0;
		a_pm_tm = 0;
		a_up_ur = 1;
		a_up_atm = m_position == 7 ? 1 : 0;
		a_up_atm_stb = (m_position == 1 || m_position == 2) && pup > m_targetURPressure ? 1 : 0;
		a_up_tm = 0;
		a_tm_atm = 1;
		a_ur_atm = m_position == 5 || m_position == 6 || m_position == 7 ? 1 : 0;
		r_ur_atm = m_position == 5 ? 1600.0 : m_position == 6 ? 800.0 : m_position == 7 ? 500.0 : 5000.0;
	}
	break;
	case 0: // Поездное положение
	{
		switch (m_position)
		{
		case 1:
		{
			a_pm_up = m_isPMTapOpened && m_disconnectUnit ? 1 : 0;
			a_pm_up_red = pur < m_targetURPressure&& m_isPMTapOpened&& m_disconnectUnit ? 1 : 0;
			a_pm_tm = m_isPMTapOpened && m_disconnectUnit ? 1 : 0;
			a_up_ur = 1;
			a_up_atm = 0;
			a_up_atm_stb = pup > m_targetURPressure ? 1 : 0;
			a_up_tm = 0;
			a_tm_atm = 0;
			a_ur_atm = 0;

			// r_ur_atm = 5'000.0;
			r_ur_atm = 10'000.0;
		}
		break;
		case 2:
		{
			a_pm_up = 0;
			a_pm_up_red = pur < m_targetURPressure&& m_isPMTapOpened&& m_disconnectUnit ? 1 : 0;
			a_pm_tm = 0;
			a_up_ur = 1;
			a_up_atm = 0;
			a_up_atm_stb = pup > m_targetURPressure ? 1 : 0;
			a_up_tm = 0;
			a_tm_atm = 0;
			a_ur_atm = 0;
			
			// r_ur_atm = 5'000.0;
			r_ur_atm = 10'000.0;
		}
		break;
		case 3:
		{
			a_pm_up = 0;
			a_pm_up_red = 0;
			a_pm_tm = 0;
			a_up_ur = 1;
			a_up_atm = 0;
			a_up_atm_stb = 0;
			a_up_tm = pup > tmPressure ? m_disconnectUnit : 0;
			a_tm_atm = 0;
			a_ur_atm = 0;

			// r_ur_atm = 5'000.0;
			r_ur_atm = 10'000.0;
		}
		break;
		case 4:
		{
			a_pm_up = 0;
			a_pm_up_red = 0;
			a_pm_tm = 0;
			a_up_ur = 1;
			a_up_atm = 0;
			a_up_atm_stb = 0;
			a_up_tm = 0;
			a_tm_atm = 0;
			a_ur_atm = 0;

			// r_ur_atm = 5'000.0;
			r_ur_atm = 10'000.0;
		}
		break;
		case 5:
		{
			a_pm_up = 0;
			a_pm_up_red = 0;
			a_pm_tm = 0;
			a_up_ur = 1;
			a_up_atm = 0;
			a_up_atm_stb = 0;
			a_up_tm = 0;
			a_tm_atm = 0;
			a_ur_atm = 1;
			// r_ur_atm = 1600.0;
			r_ur_atm = 8'000.0;
		}
		break;
		case 6:
		{
			a_pm_up = 0;
			a_pm_up_red = 0;
			a_pm_tm = 0;
			a_up_ur = 1;
			a_up_atm = 0;
			a_up_atm_stb = 0;
			a_up_tm = 0;
			a_tm_atm = 0;
			a_ur_atm = 1;
			// r_ur_atm = 800.0;
			r_ur_atm = 900.0;
		}
		break;
		case 7:
		{
			a_pm_up = 0;
			a_pm_up_red = 0;
			a_pm_tm = 0;
			a_up_ur = 1;
			a_up_atm = 1;
			a_up_atm_stb = 0;
			a_up_tm = 0;
			a_tm_atm = 1;
			a_ur_atm = 1;
			r_ur_atm = 500.0;
		}
		break;
		default:
		{
			a_pm_up = 0;
			a_pm_up_red = 0;
			a_pm_tm = 0;
			a_up_ur = 1;
			a_up_atm = 1;
			a_up_atm_stb = 0;
			a_up_tm = 0;
			a_tm_atm = 1;
			a_ur_atm = 1;
			r_ur_atm = 500.0;
		}
		break;
		}
	}
	break;
	case 1: // Положение двойной тяги
	{
		a_pm_up = m_position == 1 && m_isPMTapOpened && m_disconnectUnit ? 1 : 0;
		a_pm_up_red = (m_position == 1 || m_position == 2) && pur < m_targetURPressure&& m_isPMTapOpened&& m_disconnectUnit ? 1 : 0;
		a_pm_tm = 0;
		a_up_ur = 1;
		a_up_atm = m_position == 7 ? 1 : 0;
		a_up_atm_stb = (m_position == 1 || m_position == 2) && pup > m_targetURPressure ? 1 : 0;
		a_up_tm = 0;
		a_tm_atm = 0;
		a_ur_atm = m_position == 5 || m_position == 6 || m_position == 7 ? 1 : 0;
		r_ur_atm = m_position == 5 ? 1600.0 : m_position == 6 ? 800.0 : m_position == 7 ? 500.0 : 5000.0;
	}
	break;
	default:
	{
		a_pm_up = m_position == 1 && m_isPMTapOpened && m_disconnectUnit ? 1 : 0;
		a_pm_up_red = (m_position == 1 || m_position == 2) && pur < m_targetURPressure&& m_isPMTapOpened&& m_disconnectUnit ? 1 : 0;
		a_pm_tm = 0;
		a_up_ur = 1;
		a_up_atm = m_position == 7 ? 1 : 0;
		a_up_atm_stb = (m_position == 1 || m_position == 2) && pup > m_targetURPressure ? 1 : 0;
		a_up_tm = 0;
		a_tm_atm = 1;
		a_ur_atm = m_position == 5 || m_position == 6 || m_position == 7 ? 1 : 0;
		r_ur_atm = m_position == 5 ? 1600.0 : m_position == 6 ? 800.0 : m_position == 7 ? 500.0 : 5000.0;
	}
	break;
	}

	updateEqualizingPiston();

	dpUP(a_deltaSeconds);

	dpUR(a_deltaSeconds);
}

auto Tap395::refresh() -> void
{
	pup0 = pup;
	pur0 = pur;
}

void Tap395::updateEqualizingPiston()
{
	switch (m_combinedTapPosition)
	{
	case -1: // Положение экстренного торможения
	{
		a_tm_atm_por = 1;
		a_pm_tm_por = 0;
	}
	break;
	case 0: // Поездное положение
	{
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
	break;
	case 1: // Положение двойной тяги
	{
		a_tm_atm_por = 0;
		a_pm_tm_por = 0;
	}
	break;
	default:
	{
		a_tm_atm_por = 1;
		a_pm_tm_por = 0;
	}
	break;
	}
}

auto Tap395::getPosition() const -> int
{
	return m_position;
}

auto Tap395::setPosition(int a_position) -> void
{
	m_position = a_position;
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

	checkPressure(pup);
}

auto Tap395::dpUR(double a_deltaSeconds) -> void
{
	pur = pur0 + (
		a_up_ur / r_up_ur * (pup0 - pur0)
		- a_ur_atm / r_ur_atm * pur0
		) * a_deltaSeconds / V_UR;

	checkPressure(pur);
}

auto Tap395::setDefaultPneumaticValues() -> void
{
	pup = pup0 = 5.0;
	pur = pur0 = 5.0;
}

auto Tap395::checkPressure(double a_pressure) const -> void
{
	if (std::isnan(a_pressure) || a_pressure > 10.0 || a_pressure < -EPSILON)
		throw std::exception();
}
