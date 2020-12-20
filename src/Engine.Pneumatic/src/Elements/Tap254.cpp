#include "Tap254.h"

#include <cmath>
#include <exception>

double const Tap254::EPSILON = 0.000'001;

double const Tap254::V_NP = 0.0001;
double const Tap254::V_VP = 0.0001;
double const Tap254::V_DK = 0.0003;

Tap254::Tap254()
	: a_pm_mvt(0)
	, a_im_np(0)
	, a_im_vp(0)
	, a_mvt_atm(0)
	, r_pm_mvt(1500.0)
	, r_im_np(1000.0)
	, r_im_vp(200.0)
	, r_mvt_atm(200.0)
	, m_position(2)
	, m_disconnectUnit(true)
	, m_isPMTapOpened(true)
	, m_isIMTapOpened(true)
	, m_isMVTTapOpened(true)
	, pnp(0.0)
	, pnp0(0.0)
	, pvp(0.0)
	, pvp0(0.0)
	, pdk(0.0)
	, pdk0(0.0)
	, pmPressure(0.0)
	, mvtPressure(0.0)
	, imPressure(0.0)
	, a_vp_dk(1)
	, a_vp_atm(0)
	, r_vp_dk(2000.0)
	, r_vp_atm(250.0)
{
}

void Tap254::update(double a_deltaSeconds, double a_pmPressure, double a_mvtPressure, double a_imPressure)
{
	pmPressure = a_pmPressure;
	mvtPressure = a_mvtPressure;
	imPressure = a_imPressure;

	double pressureOnString;
	if (m_position > 2)
		pressureOnString = static_cast<double>(m_position / 25.0);
	else
		pressureOnString = 0.0;
	double pressureOnPiston;
	if (pressureOnString > pdk)
		pressureOnPiston = pressureOnString;
	else
		pressureOnPiston = pdk;

	a_im_np = m_isIMTapOpened;
	if (pvp + 0.1 /*0.6*/ >= pnp)
		a_im_vp = m_isIMTapOpened;
	else
		a_im_vp = 0;

	if (m_position == 1)
		a_vp_atm = 1;
	else
		a_vp_atm = 0;

	if (mvtPressure < pressureOnPiston - 0.001 && mvtPressure < pmPressure)
	{
		a_pm_mvt = m_isPMTapOpened * m_isMVTTapOpened * m_disconnectUnit;
		a_mvt_atm = 0;
	}
	else if (mvtPressure > pressureOnPiston + 0.001)
	{
		a_pm_mvt = 0;
		a_mvt_atm = m_isMVTTapOpened * m_disconnectUnit;
	}
	else
	{
		a_pm_mvt = 0;
		a_mvt_atm = 0;
	}

	dpNP(a_deltaSeconds);
	dpVP(a_deltaSeconds);
	dpDK(a_deltaSeconds);
}

void Tap254::refresh()
{
	pnp0 = pnp;
	pvp0 = pvp;
	pdk0 = pdk;
}

auto Tap254::getPosition() const -> int
{
	return m_position;
}

auto Tap254::setPosition(int a_position) -> void
{
	m_position = a_position;
}

auto Tap254::getDisconnectUnit() const -> bool
{
	return m_disconnectUnit;
}

auto Tap254::setDisconnectUnit(bool a_disconnectUnit) -> void
{
	m_disconnectUnit = a_disconnectUnit;
}

auto Tap254::getIsPMTapOpened() const -> bool
{
	return m_isPMTapOpened;
}

auto Tap254::setIsPMTapOpened(bool a_isOpened) -> void
{
	m_isPMTapOpened = a_isOpened;
}

auto Tap254::getIsIMTapOpened() const -> bool
{
	return m_isIMTapOpened;
}

auto Tap254::setIsIMTapOpened(bool a_isOpened) -> void
{
	m_isIMTapOpened = a_isOpened;
}

auto Tap254::getIsMVTTapOpened() const -> bool
{
	return m_isMVTTapOpened;
}

auto Tap254::setIsMVTTapOpened(bool a_isOpened) -> void
{
	m_isMVTTapOpened = a_isOpened;
}

auto Tap254::getNPPressure() const -> double
{
	return pnp0;
}

auto Tap254::getVPPressure() const -> double
{
	return pvp0;
}

void Tap254::dpNP(double a_deltaSeconds)
{
	pnp = pnp0 + (
		a_im_np / r_im_np * (imPressure - pnp0)
	) * a_deltaSeconds / V_NP;

	checkPressure(pnp);
}

void Tap254::dpVP(double a_deltaSeconds)
{
	pvp = pvp0 + (
		a_im_vp / r_im_vp * (imPressure - pvp0)
		+ a_vp_dk / r_vp_dk * (pdk0 - pvp0)
		- a_vp_atm / r_vp_atm * pvp0
	) * a_deltaSeconds / V_VP;

	checkPressure(pvp);
}

void Tap254::dpDK(double a_deltaSeconds)
{
	pdk = pdk0 + (
		a_vp_dk / r_vp_dk * (pvp0 - pdk0)
	) * a_deltaSeconds / V_DK;

	checkPressure(pdk);
}

auto Tap254::checkPressure(double a_pressure) const -> void
{
	if (std::isnan(a_pressure) || a_pressure > 10.0 || a_pressure < -EPSILON)
		throw std::exception();
}
