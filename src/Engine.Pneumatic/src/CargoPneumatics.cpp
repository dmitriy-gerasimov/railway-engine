#include "CargoPneumatics.h"

#include <cmath>

double const CargoPneumatics::EPSILON = 0.000'001;

CargoPneumatics::CargoPneumatics()
	: Vpm(0.022)
	, ppm(0.0)
	, ppm0(0.0)
	, Vtm(0.044)
	, ptm(0.0)
	, ptm0(0.0)
	, Vtc(0.005)
	, ptc(0.0)
	, ptc0(0.0)
	, a_tm_atm_leak(1)
	, r_tm_atm_leak(140'000.0)
	, vr483(true)
	, velocity(0.0)
	, brakeForceFactor(10'000.0)
{
	vr483.setTMTapIsOpened(true);

	vr483.setMass(120'000.0);

	vr483.setCargoMass(0.0);

#ifdef _DEBUG
	// CargoPneumatics::setDefaultPneumaticValues();
#endif
}

CargoPneumatics::~CargoPneumatics() = default;

auto CargoPneumatics::update(double a_deltaSeconds) -> void
{
	dpTM(a_deltaSeconds);
	dpTC(a_deltaSeconds);

	vr483.update(a_deltaSeconds, ptm0, ptc0);
}

auto CargoPneumatics::refresh() -> void
{
	ppm0 = ppm;
	ptm0 = ptm;
	ptc0 = ptc;

	vr483.refresh();
}

auto CargoPneumatics::getTMPressure() const -> double
{
	return ptm0;
}

auto CargoPneumatics::getTCPressure() const -> double
{
	return ptc0;
}

auto CargoPneumatics::getBrakeForce() const -> double
{
	if (ptc < 0.1)
	{
		return 0.0;
	}

	double const velocityKmpH = abs(this->velocity * 3.6);

	double const k = -0.6
		* (16.0 * ptc / 9806.65 + 100.0) / (80.0 * ptc / 9806.65 + 100.0)
		* (1003.0 * velocityKmpH + 19000.0) / (3217.0 * velocityKmpH + 760.0)
		/ 25.0;

	double const sandSupplyFactor = 1.0;

	double force = brakeForceFactor * sandSupplyFactor * k * ptc;

	if (velocity < 0.0)
	{
		force *= -1.0;
	}

	return force;
}

auto CargoPneumatics::setVelocity(double a_velocity) -> void
{
	velocity = a_velocity;
}

auto CargoPneumatics::getBrakeForceFactor() const -> double
{
	return brakeForceFactor;
}

auto CargoPneumatics::setBrakeForceFactor(double a_factor) -> void
{
	brakeForceFactor = a_factor;
}

auto CargoPneumatics::getZRPressure() const -> double
{
	return vr483.getZRPressure();
}

auto CargoPneumatics::getVRValveIsOpened() const -> bool
{
	return vr483.getTMTapIsOpened();
}

auto CargoPneumatics::setVRValveIsOpened(bool a_value) -> void
{
	vr483.setTMTapIsOpened(a_value);
}

auto CargoPneumatics::setDefaultPneumaticValues() -> void
{
	ppm = ppm0 = 9.0;
	ptm = ptm0 = 5.0;
	ptc = ptc0 = 0.0;
}

void CargoPneumatics::dpTM(double a_deltaSeconds)
{
	double const tm_front_flow = getFrontPneumaticConnector()->getConnectorFlow();
	double const tm_back_flow = getBackPneumaticConnector()->getConnectorFlow();

	double const tm_mk_flow = vr483.a_tm_mk / vr483.r_tm_mk * (vr483.getMKPressure() - ptm0);
	double const tm_zr_flow = vr483.a_tm_zr / vr483.r_tm_zr * (vr483.getZRPressure() - ptm0);
	double const tm_atm_flow = a_tm_atm_leak / r_tm_atm_leak * ptm0;

	ptm = ptm0 + (
		tm_mk_flow
		+ tm_zr_flow
		+ tm_front_flow
		+ tm_back_flow
		- tm_atm_flow
	) * a_deltaSeconds / Vtm;

	if (ptm < 0.0)
		ptm = 0.0;
}

void CargoPneumatics::dpTC(double a_deltaSeconds)
{
	ptc = ptc0 + (
		vr483.a_kdr_tc / vr483.r_kdr_tc * (vr483.getKDRPressure() - ptc0)
		+ vr483.a_zr_tc / vr483.r_zr_tc * (vr483.getZRPressure() - ptc0)
		- vr483.a_tc_atm / vr483.r_tc_atm * ptc0
	) * a_deltaSeconds / Vtc;
}