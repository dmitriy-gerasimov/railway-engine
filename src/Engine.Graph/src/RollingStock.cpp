#include "RollingStock.h"

RollingStock::RollingStock(std::vector<std::unique_ptr<Dynamic>>&& a_dynamicses)
	: dynamicses(std::move(a_dynamicses))
{
}

RollingStock::~RollingStock()
{
}

auto RollingStock::getFrontCoupling() const -> Coupling const*
{
	auto frontDynamics(dynamicses.front().get());

	auto frontDynamicsFrontCoupling(frontDynamics->getFrontCoupling());

	if (frontDynamicsFrontCoupling->hasConnectedCoupling())
	{
		return frontDynamicsFrontCoupling;
	}
	else
	{
		return frontDynamics->getBackCoupling();
	}
}

auto RollingStock::getBackCoupling() const -> Coupling const*
{
	auto backDynamics(dynamicses.front().get());

	auto backDynamicsBackCoupling(backDynamics->getBackCoupling());

	if (backDynamicsBackCoupling->hasConnectedCoupling())
	{
		return backDynamicsBackCoupling;
	}
	else
	{
		return backDynamics->getFrontCoupling();
	}
}

auto RollingStock::update(double a_deltaSeconds) -> void
{
	for (auto& dynamics : this->dynamicses)
	{
		dynamics->update(a_deltaSeconds);
	}
}

/*auto RollingStock::split(Coupling* a_coupling) -> std::unique_ptr<RollingStock>
{
	auto coupling(getFrontCoupling());

	size_t i = 0;
	
	while(coupling != nullptr)
	{
		if (coupling == a_coupling)
			break;

		coupling = this->dynamicses[i]->getOtherCoupling(coupling);

		if (coupling == a_coupling)
			break;

		coupling = coupling->getConnectedCoupling();

		i++;
	}

	std::vector<std::unique_ptr<Dynamics>> dynamicses;

	for (size_t j = i + 1; j <= this->dynamicses.size(); j++)
	{
		dynamicses.push_back(std::move(this->dynamicses[j]));
	}

	this->dynamicses.erase(this->dynamicses.begin() + i + 1);
	
	return std::make_unique<RollingStock>(std::move(dynamicses));
}*/