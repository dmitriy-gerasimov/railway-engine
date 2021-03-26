#include "RailedVehicle.h"

#include <cmath>

RailedVehicle::RailedVehicle(
	size_t a_number,
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
	, number(a_number)
{
	dynamic->getFrontCoupling()->setVelocity(this->dynamic->getVelocity());
	dynamic->getBackCoupling()->setVelocity(this->dynamic->getVelocity());

	dynamic->getFrontCoupling()->setWorldLocation(front.getWorldLocation());
	dynamic->getBackCoupling()->setWorldLocation(back.getWorldLocation());
}

RailedVehicle::~RailedVehicle() = default;

auto RailedVehicle::update(double a_deltaSeconds) -> void
{
	dynamic->setTractionForce(electric->getTractionForce());

	auto const frontWorldLocation = front.getWorldLocation();
	auto const backWorldLocation = back.getWorldLocation();

	Vector3D const frontWorldLocationProjection(frontWorldLocation.getX(), frontWorldLocation.getY(), 0.0);
	Vector3D const backWorldLocationProjection(backWorldLocation.getX(), backWorldLocation.getY(), 0.0);

	Vector3D distance(frontWorldLocationProjection - backWorldLocationProjection);

	double const gradient = (frontWorldLocation.getZ() - backWorldLocation.getZ()) / distance.magnitude();
	if (std::isnan(gradient))
	{
		// throw std::exception();
	}
	
	dynamic->setGradient(gradient);

	dynamic->setBrakeForce(pneumatic->getBrakeForce() + electric->getBrakeForce());
	
	dynamic->update(a_deltaSeconds);

	dynamic->getFrontCoupling()->setVelocity(dynamic->getVelocity());
	dynamic->getBackCoupling()->setVelocity(dynamic->getVelocity());

	pneumatic->setVelocity(dynamic->getVelocity());

	pneumatic->update(a_deltaSeconds);
	pneumatic->refresh();

	electric->setVelocity(dynamic->getVelocity());

	electric->update(a_deltaSeconds);
	electric->refresh();
}

auto RailedVehicle::move() -> void
{
	double const offset = this->dynamic->getOffset();

	move(offset);
}

auto RailedVehicle::move(double a_offset) -> void
{
	if (a_offset >= 0.0)
	{
		bool const success = front.move(a_offset);

		if (success)
		{
			back.move(a_offset);
		}
	}
	else
	{
		bool const success = back.move(a_offset);

		if (success)
		{
			front.move(a_offset);
		}
	}

	dynamic->getFrontCoupling()->setWorldLocation(front.getWorldLocation());
	dynamic->getBackCoupling()->setWorldLocation(back.getWorldLocation());
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

auto RailedVehicle::getNumber() const -> size_t
{
	return number;
}
