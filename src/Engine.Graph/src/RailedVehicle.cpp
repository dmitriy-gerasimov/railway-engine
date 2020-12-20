#include "RailedVehicle.h"

#include <cmath>

RailedVehicle::RailedVehicle(
	ArcLocation a_front,
	ArcLocation a_back,
	std::unique_ptr<Dynamic>&& a_dynamic,
	std::unique_ptr<Pneumatic>&& a_pneumatic,
	std::unique_ptr<Electric>&& a_electric
)
	: front(std::move(a_front))
	, back(std::move(a_back))
	, dynamic(std::move(a_dynamic))
	, pneumatic(std::move(a_pneumatic))
	, electric(std::move(a_electric))
{
	updateCouplings();
}

auto RailedVehicle::update(double a_deltaSeconds) -> void
{
	this->dynamic->setTractionForce(electric->getTractionForce());

	auto const frontWorldLocation = front.getWorldLocation();
	auto const backWorldLocation = back.getWorldLocation();

	Vector3D const frontWorldLocationProjection(frontWorldLocation.getX(), frontWorldLocation.getY(), 0.0);
	Vector3D const backWorldLocationProjection(backWorldLocation.getX(), backWorldLocation.getY(), 0.0);

	Vector3D distance(frontWorldLocationProjection - backWorldLocationProjection);

	double const gradient = (frontWorldLocation.getZ() - backWorldLocation.getZ()) / distance.magnitude();
	if (std::isnan(gradient))
	{
		throw std::exception();
	}
	
	this->dynamic->setGradient(gradient);

	double const brakeForce = pneumatic->getBrakeForce() + electric->getBrakeForce();
	this->dynamic->setBrakeForce(brakeForce);

	updateCouplings();
	
	this->dynamic->update(a_deltaSeconds);
	
	this->pneumatic->update(a_deltaSeconds);
	this->pneumatic->refresh();

	this->electric->update(a_deltaSeconds);
	this->electric->refresh();
}

auto RailedVehicle::move() -> void
{
	double const offset = this->dynamic->getOffset();

	if (offset >= 0.0)
	{
		bool const success = front.move(offset);

		if (success)
		{
			back.move(offset);
		}
	}
	else
	{
		bool const success = back.move(offset);

		if (success)
		{
			front.move(offset);
		}
	}
}

auto RailedVehicle::getGradient() const -> double
{
	return this->dynamic->getGradient();
}

auto RailedVehicle::setGradient(double a_gradient) const -> void
{
	this->dynamic->setGradient(a_gradient);
}

auto RailedVehicle::getDynamic() const -> Dynamic*
{
	return dynamic.get();
}

auto RailedVehicle::getPneumatic() const -> Pneumatic*
{
	return pneumatic.get();
}

auto RailedVehicle::getElectric() const -> Electric*
{
	return electric.get();
}

auto RailedVehicle::updateCouplings() -> void
{
	auto* frontCoupling = this->dynamic->getFrontCoupling();
	frontCoupling->setWorldLocation(front.getWorldLocation());
	frontCoupling->setVelocity(this->dynamic->getVelocity());

	auto* backCoupling = this->dynamic->getBackCoupling();
	backCoupling->setWorldLocation(back.getWorldLocation());
	backCoupling->setVelocity(this->dynamic->getVelocity());
}