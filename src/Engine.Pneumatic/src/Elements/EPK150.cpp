#include "EPK150.h"

double const EPK150::EPSILON = 0.000'001;

double const EPK150::V_KVV = 0.004;
double const EPK150::V_PSK = 0.001;

EPK150::EPK150()
	: a_pm_kvv(0)
	, a_tm_psk(0)
	, a_tm_atm(0)
	, r_pm_kvv(800.0)
	, r_tm_psk(200.0)
	, r_tm_atm(10.0)
	, pmTapIsOpened(false)
	, tmTapIsOpened(false)
	, epkKey(false)
	, epkSignal(false)
	, epkValve(false)
	, membrane(false)
	, stallingValve(false)
	, reverseValve(false)
	, pmPressure(0.0)
	, tmPressure(0.0)
	, pkvv(0.0)
	, pkvv0(0.0)
	, ppsk(0.0)
	, ppsk0(0.0)
	, a_kvv_atm(0)
	, a_psk_atm(0)
	, r_kvv_atm(1200.0)
	, r_psk_atm(50.0)
{
}

void EPK150::update(double a_deltaSeconds, double a_pmPressure, double a_tmPressure)
{
	pmPressure = a_pmPressure;
	tmPressure = a_tmPressure;

	bool tempEpkKey(false);
	
	if (pkvv0 <= 1.5)
		tempEpkKey = false;
	
	if (epkKey)
		tempEpkKey = true;
		
	epkValve = pkvv0 > 1.5 || tempEpkKey ? epkSignal : false;
	
	reverseValve = pmPressure > pkvv ? true : false;

	membrane = pkvv > 1.5 ? true : false;

	stallingValve = ppsk + 1.5 < tmPressure ? true : false;

	a_pm_kvv = pmTapIsOpened && reverseValve && epkValve ? 1 : 0;

	a_kvv_atm = epkValve == 0 ? 1 : 0;

	a_tm_atm = tmTapIsOpened && stallingValve ? 1 : 0;

	a_tm_psk = tmTapIsOpened;
	
	a_psk_atm = membrane == 0 ? 1 : 0;

	dpKVV(a_deltaSeconds);

	dpPSK(a_deltaSeconds);
}

auto EPK150::refresh() -> void
{
	pkvv0 = pkvv;
	ppsk0 = ppsk;
}

auto EPK150::getPMTapIsOpened() const -> bool
{
	return pmTapIsOpened;
}

auto EPK150::setPMTapIsOpened(bool a_value) -> void
{
	pmTapIsOpened = a_value;
}

auto EPK150::getTMTapIsOpened() const -> bool
{
	return tmTapIsOpened;
}

auto EPK150::setTMTapIsOpened(bool a_value) -> void
{
	tmTapIsOpened = a_value;
}

auto EPK150::getEpkKey() const -> bool
{
	return epkKey;
}

auto EPK150::setEpkKey(bool a_value) -> void
{
	epkKey = a_value;
}

auto EPK150::getEpkSignal() const -> bool
{
	return epkSignal;
}

auto EPK150::setEpkSignal(bool a_value) -> void
{
	epkSignal = a_value;
}

auto EPK150::getKvvPressure() const -> double
{
	return pkvv0;
}

auto EPK150::getPskPressure() const -> double
{
	return ppsk0;
}

auto EPK150::setDefaultPneumaticValues() -> void
{
	pkvv = pkvv0 = 9.0;
	ppsk = ppsk0 = 5.0;
}

void EPK150::dpKVV(double a_deltaSeconds)
{
	pkvv = pkvv0 + (
		a_pm_kvv / r_pm_kvv * (pmPressure - pkvv0)
		- a_kvv_atm / r_kvv_atm * pkvv0
	) * a_deltaSeconds / V_KVV;
}
void EPK150::dpPSK(double a_deltaSeconds)
{
	ppsk = ppsk0 + (
		a_tm_psk / r_tm_psk * (tmPressure - ppsk0)
		- a_psk_atm / r_psk_atm * ppsk0
	) * a_deltaSeconds / V_PSK;
}