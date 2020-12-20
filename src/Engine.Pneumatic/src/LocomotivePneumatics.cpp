#include "LocomotivePneumatics.h"

#include "PneumaticConnector.h"

#include <cmath>
#include <exception>

double const LocomotivePneumatics::EPSILON = 0.000'001;

LocomotivePneumatics::LocomotivePneumatics()
	: Vgr(1.0)
	, pgr(0.0)
	, pgr0(0.0)
	, Vpm(0.022)
	, ppm(0.0)
	, ppm0(0.0)
	, Vtm(0.044)
	, ptm(0.0)
	, ptm0(0.0)
	, Vim(0.005)
	, pim(0.0)
	, pim0(0.0)
	, Vmvt(0.001)
	, pmvt(0.0)
	, pmvt0(0.0)
	, Vtc(0.005)
	, ptc(0.0)
	, ptc0(0.0)
	, a_gr_pm(1)
	, a_tm_atm_leak(1)
	, a_mvt_tc(1)
	, r_gr_pm(5.0)
	, r_tm_atm_leak(140'000.0)
	, r_mvt_tc(1.0)
	, vr483(true)
	, velocity(0.0)
	, brakeForceFactor(1.0)
	, compressorProductivity(0.1)
	, isCompressorWorking(false)
	, maxCompressorPressure(9.0)
{
	vr483.setTMTapIsOpened(true);

	vr483.setMass(120'000.0);

	vr483.setCargoMass(0.0);

	epk150.setTMTapIsOpened(true);

	epk150.setPMTapIsOpened(true);

#ifdef _DEBUG
	setDefaultPneumaticValues();
#endif
}

LocomotivePneumatics::~LocomotivePneumatics() = default;

auto LocomotivePneumatics::update(double a_deltaSeconds) -> void
{
	dpGR(a_deltaSeconds);
	dpPM(a_deltaSeconds);
	dpTM(a_deltaSeconds);
	dpIM(a_deltaSeconds);
	dpMVT(a_deltaSeconds);
	dpTC(a_deltaSeconds);

	tap254.update(a_deltaSeconds, ppm0, pmvt0, pim0);

	tap395.update(a_deltaSeconds, ppm0, ptm0);

	vr483.update(a_deltaSeconds, ptm0, pim0);

	epk150.update(a_deltaSeconds, ppm0, ptm0);
}

auto LocomotivePneumatics::refresh() -> void
{
	pgr0  = pgr;
	ppm0  = ppm;
	ptm0  = ptm;
	pim0  = pim;
	pmvt0 = pmvt;
	ptc0  = ptc;

	tap254.refresh();

	tap395.refresh();

	vr483.refresh();

	epk150.refresh();
}

auto LocomotivePneumatics::getPMPressure() const -> double
{
	return pgr0;
}

auto LocomotivePneumatics::getTMPressure() const -> double
{
	return ptm0;
}

auto LocomotivePneumatics::getURPressure() const -> double
{
	return tap395.getURPressure();
}

auto LocomotivePneumatics::getTCPressure() const -> double
{
	return ptc0;
}

auto LocomotivePneumatics::getBrakeForce() const -> double
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

	double const sandSupplyFactor = 1.0; // (m_owner->getElectrics().SandSupply() == 0) ? 1.0 : sandSupplyFactor;

	double force = brakeForceFactor * sandSupplyFactor * k * ptc;

	if (velocity < 0.0)
	{
		force *= -1.0;
	}

	return force;
}

auto LocomotivePneumatics::setTap395Position(int a_position) -> void
{
	tap395.setPosition(a_position);
}

auto LocomotivePneumatics::setTap254Position(int a_position) -> void
{
	tap254.setPosition(a_position);
}

auto LocomotivePneumatics::setIsCompressorWorking(bool a_isWorking) -> void
{
	isCompressorWorking = a_isWorking;
}

auto LocomotivePneumatics::setReducer(double a_reducer) -> void
{
	tap395.setTargetURPressure(a_reducer);
}

auto LocomotivePneumatics::setDisconnectUnit(bool a_disconnectUnit) -> void
{
	tap395.setDisconnectUnit(a_disconnectUnit);
	tap254.setDisconnectUnit(a_disconnectUnit);
}

auto LocomotivePneumatics::setCombinedTapPosition(int a_combinedTapPosition) -> void
{
	tap395.setCombinedTapPosition(a_combinedTapPosition);
}

auto LocomotivePneumatics::setVelocity(double a_velocity) -> void
{
	velocity = a_velocity;
}

auto LocomotivePneumatics::getBrakeForceFactor() const -> double
{
	return brakeForceFactor;
}

auto LocomotivePneumatics::setBrakeForceFactor(double a_factor) -> void
{
	brakeForceFactor = a_factor;
}

auto LocomotivePneumatics::getKVVPressure() const -> double
{
	return epk150.getKvvPressure();
}

auto LocomotivePneumatics::setEpkKey(bool a_epkKey) -> void
{
	epk150.setEpkKey(a_epkKey);
}

auto LocomotivePneumatics::setEpkSignal(bool a_epkSignal) -> void
{
	epk150.setEpkSignal(a_epkSignal);
}

auto LocomotivePneumatics::getZRPressure() const -> double
{
	return vr483.getZRPressure();
}

auto LocomotivePneumatics::getVRValveIsOpened() const -> bool
{
	return vr483.getTMTapIsOpened();
}

auto LocomotivePneumatics::setVRValveIsOpened(bool a_value) -> void
{
	vr483.setTMTapIsOpened(a_value);
}

auto LocomotivePneumatics::setDefaultPneumaticValues() -> void
{
	pgr = pgr0 = 9.0;
	ppm = ppm0 = 9.0;
	ptm = ptm0 = 5.0;
	pim = pim0 = 0.0;
	pmvt = pmvt0 = 0.0;
	ptc = ptc0 = 0.0;

	vr483.setDefaultPneumaticValues();

	epk150.setDefaultPneumaticValues();

	tap395.setDefaultPneumaticValues();
}

auto LocomotivePneumatics::setCompressorProductivity(double a_compressorProductivity) -> void
{
	compressorProductivity = a_compressorProductivity;
}

void LocomotivePneumatics::dpGR(double a_deltaSeconds)
{
	double const compressorFlow = (isCompressorWorking)
		? compressorProductivity * (maxCompressorPressure - pgr0)
		: 0.0;

	pgr = pgr0 + (
		compressorFlow	
		+ a_gr_pm / r_gr_pm * (ppm0 - pgr0)
	) * a_deltaSeconds / Vgr;

	checkPressure(pgr);
}

void LocomotivePneumatics::dpPM(double a_deltaSeconds)
{
	ppm = ppm0 + (
		a_gr_pm / r_gr_pm * (pgr0 - ppm0)
		+ (tap395.a_pm_up / tap395.r_pm_up + tap395.a_pm_up_red / tap395.r_pm_up_red) * (tap395.getUPPressure() - ppm0)
		+ (tap395.a_pm_tm / tap395.r_pm_tm + tap395.a_pm_tm_por / tap395.r_pm_tm_por) * (ptm0 - ppm0)
		+ tap254.a_pm_mvt / tap254.r_pm_mvt * (pmvt0 - ppm0)
		+ epk150.a_pm_kvv / epk150.r_pm_kvv * (epk150.getKvvPressure() - ppm0)
	) * a_deltaSeconds / Vpm;

	checkPressure(ppm);
}

void LocomotivePneumatics::dpTM(double a_deltaSeconds)
{
	double const tm_front_flow = getFrontPneumaticConnector()->getConnectorFlow();
	double const tm_back_flow = getBackPneumaticConnector()->getConnectorFlow();

	double const pm_tm_flow = (tap395.a_pm_tm / tap395.r_pm_tm + tap395.a_pm_tm_por / tap395.r_pm_tm_por) * (ppm0 - ptm0);
	double const up_tm_flow = tap395.a_up_tm / tap395.r_up_tm * (tap395.getUPPressure() - ptm0);
	double const tm_mk_flow = vr483.a_tm_mk / vr483.r_tm_mk * (vr483.getMKPressure() - ptm0);
	double const tm_zr_flow = vr483.a_tm_zr / vr483.r_tm_zr * (vr483.getZRPressure() - ptm0);
	double const tm_psk_flow = epk150.a_tm_psk / epk150.r_tm_psk * (epk150.getPskPressure() - ptm0);
	double const tm_atm_flow = (
		tap395.a_tm_atm_por / tap395.r_tm_atm_por
		+ epk150.a_tm_atm / epk150.r_tm_atm
		+ a_tm_atm_leak / r_tm_atm_leak
		+ tap395.a_tm_atm / tap395.r_tm_atm
	) * ptm0;

	ptm = ptm0 + (
		pm_tm_flow
		+ up_tm_flow
		+ tm_mk_flow
		+ tm_zr_flow
		+ tm_psk_flow
		+ tm_front_flow
		+ tm_back_flow
		- tm_atm_flow
	) * a_deltaSeconds / Vtm;

	if (ptm < 0.0)
		ptm = 0.0;

	checkPressure(ptm);
}

void LocomotivePneumatics::dpTC(double a_deltaSeconds)
{
	ptc = ptc0 + (
		a_mvt_tc / r_mvt_tc * (pmvt0 - ptc0)
	) * a_deltaSeconds / Vtc;

	checkPressure(ptc);
}

void LocomotivePneumatics::dpIM(double a_deltaSeconds)
{
	pim = pim0 + (
		vr483.a_kdr_tc / vr483.r_kdr_tc * (vr483.getKDRPressure() - pim0)
		+ vr483.a_zr_tc / vr483.r_zr_tc * (vr483.getZRPressure() - pim0)
		+ tap254.a_im_np / tap254.r_im_np * (tap254.getNPPressure() - pim0)
		+ tap254.a_im_vp / tap254.r_im_vp * (tap254.getVPPressure() - pim0)
		- vr483.a_tc_atm / vr483.r_tc_atm * pim0
	) * a_deltaSeconds / Vim;

	if (pim <= 0.0)
		pim = 0.0;

	checkPressure(pim);
}

void LocomotivePneumatics::dpMVT(double a_deltaSeconds)
{
	pmvt = pmvt0 + (
		tap254.a_pm_mvt / tap254.r_pm_mvt * (ppm0 - pmvt0)
		+ a_mvt_tc / r_mvt_tc * (ptc0 - pmvt0)
		- tap254.a_mvt_atm / tap254.r_mvt_atm * pmvt0
		) * a_deltaSeconds / Vmvt;

	checkPressure(pmvt);
}

auto LocomotivePneumatics::checkPressure(double /*a_pressure*/) const -> void
{
	/*if (std::isnan(a_pressure) || a_pressure > 11.0 || a_pressure < -EPSILON)
		throw std::exception();*/
}