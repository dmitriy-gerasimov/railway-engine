#include "VR483.h"

#include <cmath>
#include <exception>

double const VR483::EPSILON = 0.000'001;

double const VR483::V_MK = 0.002;
double const VR483::V_ZK = 0.006;
double const VR483::V_RK = 0.006;
double const VR483::V_ZR = 0.055;
double const VR483::V_KDR = 0.001;

VR483::VR483(bool a_withAutoMode)
	: a_tm_mk(0)
	, a_mk_zk(0)
	, a_mk_rk(0)
	, a_mk_kdr(0)
	, a_mk_atm(0)
	, a_zk_rk(0)
	, a_kdr_tc(0)
	, a_tc_atm(0)
	, a_rk_atm(0)
	, a_zr_tc(0)
	, a_tm_zr(0)
	, a_kdr_atm(0)
	, r_tm_mk(10.0)
	, r_mk_kdr(400.0)
	, r_mk_atm(1000.0)
	, r_zk_rk(1000.0)
	, r_kdr_tc(1000.0)
	, r_tc_atm(500.0)
	, r_zr_tc(1500.0)
	, r_rk_atm(500.0)
	, r_tm_zr(1500.0)
	, r_kdr_atm(500.0)
	, r_mk_zk(1000.0)
	, r_mk_rk(1000.0)
	, tmPressure(0.0)
	, tcPressure(0.0)
	, pmk(0.0)
	, pzk(0.0)
	, prk(0.0)
	, pzr(0.0)
	, pkdr(0.0)
	, pmk0(0.0)
	, pzk0(0.0)
	, prk0(0.0)
	, pzr0(0.0)
	, pkdr0(0.0)
	, withAutoMode(a_withAutoMode)
	, maxTCPressure(4.0)
	, mass(0.0)
	, cargoMass(0.0)
	, load(3)
	, tmTapIsOpened(true)
{
}

double VR483::minP(double p1, double p2) const
{
	if (p1 < p2)
		return p1;
	else
		return p2;
}

void VR483::update(double a_deltaSeconds, double a_tmPressure, double a_tcPressure)
{
	tmPressure = a_tmPressure;
	tcPressure = a_tcPressure;

	double const totalMass = (mass + cargoMass) / 1000.0;

	double pmin = 1.6, paverage = 3.0, pmax = 4.0;
	double mmin = 55.0, mmax = 100.0;
	double a = (pmax - pmin) / (mmax - mmin), b = (mmax*pmin - mmin*pmax) / (mmax - mmin);

	if (withAutoMode)
	{
		if (totalMass < mmin)
			maxTCPressure = pmin;
		else if (totalMass > mmax)
			maxTCPressure = load == 1 ? pmin : load == 2 ? paverage : pmax;
		else
			maxTCPressure = load == 1 ? minP(b + a * totalMass, pmin) : load == 2 ? minP(b + a * totalMass, paverage) : minP(b + a * totalMass, pmax);
	}
	else
	{
		if (load == 1)	// порожний
			maxTCPressure = pmin;
		else if (load == 2)	// средний
			maxTCPressure = paverage;
		else	// груженый
			maxTCPressure = pmax;
	}

	a_tm_mk = tmTapIsOpened ? 1 : 0;

	if (tmPressure > pzr)
	{
		a_tm_zr = tmTapIsOpened ? 1 : 0;
	}
	else
	{
		a_tm_zr = 0;
	}

	if (pmk < pzk - 0.5)
	{
		a_mk_zk = 1;
		a_mk_atm = 1;
		a_mk_kdr = 1;
		a_mk_rk = 0;
		r_mk_zk = 1000.0;
		r_mk_rk = 1000.0;
	}
	else if (pmk < pzk - 0.15)
	{
		a_mk_zk = 1;
		a_mk_atm = 0;
		a_mk_kdr = 1;
		a_mk_rk = 0;
		r_mk_zk = 500.0;
		r_mk_rk = 1000.0;
	}
	else if (pmk > pzk + 0.2)
	{
		a_mk_zk = 1;
		a_mk_atm = 0;
		a_mk_kdr = 0;
		a_mk_rk = 1;
		r_mk_zk = 1500.0;
		r_mk_rk = 40'000.0;
	}
	else if (pmk > pzk + 0.05)
	{
		a_mk_zk = 1;
		a_mk_atm = 0;
		a_mk_kdr = 0;
		a_mk_rk = 1;
		r_mk_zk = 500.0;
		r_mk_rk = 20000.0;
	}
	else
	{
		a_mk_zk = 1;
		a_mk_atm = 0;
		a_mk_kdr = 0;
		a_mk_rk = 0;
		r_mk_zk = 1000.0;
		r_mk_rk = 1000.0;
	}

	if (prk > pzk + 0.15)
	{
		a_zk_rk = 0;
		a_kdr_tc = 0;
		a_kdr_atm = 0;
		if (tcPressure < 2.5 * (prk - pzk) - 0.05 && tcPressure < maxTCPressure)
		{
			a_tc_atm = 0;
			a_zr_tc = 1;
		}
		else if (tcPressure > 2.5 * (prk - pzk))
		{
			a_tc_atm = 1;
			a_zr_tc = 0;
		}
		else
		{
			a_tc_atm = 0;
			a_zr_tc = 0;
		}
	}
	else if (prk > pzk + 0.1)
	{
		a_zk_rk = 0;
		a_kdr_tc = 0;
		a_tc_atm = 0;
		a_zr_tc = 0;
		a_kdr_atm = 1;
	}
	else if (prk < pzk - 0.1)
	{
		a_zk_rk = 1;
		a_kdr_tc = 1;
		a_tc_atm = 1;
		a_zr_tc = 0;
		a_kdr_atm = 0;
	}
	else
	{
		a_zk_rk = 1;
		a_kdr_tc = 1;
		a_tc_atm = 1;
		a_zr_tc = 0;
		a_kdr_atm = 1;
	}

	dpMK(a_deltaSeconds);

	dpZK(a_deltaSeconds);

	dpRK(a_deltaSeconds);

	dpZR(a_deltaSeconds);

	dpKDR(a_deltaSeconds);
}

auto VR483::refresh() -> void
{
	pmk0 = pmk;
	
	pzk0 = pzk;
	
	prk0 = prk;
	
	pzr0 = pzr;
	
	pkdr0 = pkdr;
}

auto VR483::getTMTapIsOpened() const -> bool
{
	return tmTapIsOpened;
}

auto VR483::setTMTapIsOpened(bool a_value) -> void
{
	tmTapIsOpened = a_value;
}

auto VR483::getLoad() const -> int
{
	return load;
}

auto VR483::setLoad(int a_value) -> void
{
	load = a_value;
}

auto VR483::getMass() const -> double
{
	return mass;
}

auto VR483::setMass(double a_value) -> void
{
	mass = a_value;
}

auto VR483::getCargoMass() const -> double
{
	return cargoMass;
}

auto VR483::setCargoMass(double a_value) -> void
{
	cargoMass = a_value;
}

auto VR483::getMKPressure() const -> double
{
	return pmk0;
}

auto VR483::getZRPressure() const -> double
{
	return pzr0;
}

auto VR483::getKDRPressure() const -> double
{
	return pkdr0;
}

auto VR483::setDefaultPneumaticValues() -> void
{
	pmk = pmk0 = 5.0;
	pzk = pzk0 = 5.0;
	prk = prk0 = 5.0;
	pzr = pzr0 = 5.0;
	pkdr = pkdr0 = 5.0;
}

void VR483::dpMK(double a_deltaSeconds)
{
	pmk = pmk0 + (
		a_tm_mk / r_tm_mk * (tmPressure - pmk0)
		+ a_mk_zk / r_mk_zk * (pzk0 - pmk0)
		+ a_mk_kdr / r_mk_kdr * (pkdr0 - pmk0)
		+ a_mk_rk / r_mk_rk * (prk0 - pmk0)
		- a_mk_atm / r_mk_atm * pmk0
	) * a_deltaSeconds / V_MK;

	checkPressure(pmk);
}

void VR483::dpZK(double a_deltaSeconds)
{
	pzk = pzk0 + (
		a_mk_zk / r_mk_zk * (pmk0 - pzk0)
		+ a_zk_rk / r_zk_rk * (prk0 - pzk0)
	) * a_deltaSeconds / V_ZK;

	checkPressure(pzk);
}

void VR483::dpRK(double a_deltaSeconds)
{
	prk = prk0 + (
		a_zk_rk / r_zk_rk * (pzk0 - prk0)
		+ a_mk_rk / r_mk_rk * (pmk0 - prk0)
		- a_rk_atm / r_rk_atm * prk0
		) * a_deltaSeconds / V_RK;

	checkPressure(prk);
}

void VR483::dpZR(double a_deltaSeconds)
{
	pzr = pzr0 + (
		a_tm_zr / r_tm_zr * (tmPressure - pzr0)
		+ a_zr_tc / r_zr_tc * (tcPressure - pzr0)
	) * a_deltaSeconds / V_ZR;

	checkPressure(pzr);
}

void VR483::dpKDR(double a_deltaSeconds)
{
	pkdr = pkdr0 + (
		a_mk_kdr / r_mk_kdr * (pmk0 - pkdr0)
		+ a_kdr_tc / r_kdr_tc * (tcPressure - pkdr0)
	) * a_deltaSeconds / V_KDR;

	checkPressure(pkdr);
}

auto VR483::checkPressure(double /*a_pressure*/) const -> void
{
	/*if (std::isnan(a_pressure) || a_pressure > 10.0 || a_pressure < -EPSILON)
		throw std::exception();*/
}