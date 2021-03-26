#include "World.h"

#include "Engine.Graph/src/RailedVehicle.h"
#include "Engine.Graph/src/Arc.h"
#include "Engine.Graph/src/Node.h"

#include "Engine.Pneumatic/src/LocomotivePneumatics.h"
#include "Engine.Pneumatic/src/CargoPneumatics.h"

#include "Engine.Electric/src/LocomotiveElectric.h"
#include "Engine.Electric/src/CargoElectric.h"

#include "Engine.Pneumatic/src/PneumaticConnector.h"

#include <memory>

double const World::Epsilon = 0.000'001;

World::World()
	: maxTensionCoupling(0.06)
	, maxCompressionCoupling(-0.06)
{
}

World::~World() = default;

auto World::update(double a_deltaSeconds) -> void
{
	for (size_t i = 0; i < railedVehicles.size(); i++)
	{
		railedVehicles[i]->update(a_deltaSeconds);
	}

	for (size_t i = 0; i < railedVehicles.size(); i++)
	{
		railedVehicles[i]->move();
	}

	for (size_t i = 1; i < railedVehicles.size(); i++)
	{
		double const distance1 = railedVehicles[i]->getDynamic()->getFrontCoupling()->getDistanceToConnectedCoupling();

		Coupling const* coupling = railedVehicles[i]->getDynamic()->getFrontCoupling();

		double const distanceToConnectedCoupling = coupling->getDistanceToConnectedCoupling();

		if (distanceToConnectedCoupling > maxTensionCoupling)
		{
			railedVehicles[i]->move(distanceToConnectedCoupling - maxTensionCoupling + Epsilon);
		}
		else if (distanceToConnectedCoupling < maxCompressionCoupling)
		{
			railedVehicles[i]->move(distanceToConnectedCoupling - maxCompressionCoupling - Epsilon);
		}

		double const newDistanceToConnectedCoupling = coupling->getDistanceToConnectedCoupling();

		if (newDistanceToConnectedCoupling > maxTensionCoupling || newDistanceToConnectedCoupling < maxCompressionCoupling)
		{
			throw std::exception();
		}
	}
}

auto World::createLocomotive(ArcLocation& a_location) -> RailedVehicle*
{
	// длина вагона
	double const locomotiveLength(22.0);

	// масса вагона
	double const locomotiveMass(120'000.0);
	// double const locomotiveMass(33'000.0);

	// расстояние от тележки до торца вагона
	double const railroadTruckOffset(2.0);

	ArcLocation front(a_location);
	front.move(-railroadTruckOffset);
	
	ArcLocation back(a_location);
	back.move(-locomotiveLength);
	back.move(railroadTruckOffset);

	auto railedVehicle = std::make_unique<RailedVehicle>(
		railedVehicles.size(),
		front,
		back,
		std::make_unique<Dynamic>(locomotiveMass, locomotiveLength),
		std::make_unique<LocomotivePneumatics>(),
		std::make_unique<LocomotiveElectric>()
	);

	railedVehicles.push_back(std::move(railedVehicle));

	return railedVehicles.back().get();
}

auto World::createCargo(ArcLocation& a_location) -> RailedVehicle*
{
	// длина вагона
	double const cargoLength(22.0);

	// масса вагона
	double const cargoMass(33'000.0);
	
	// расстояние от тележки до торца вагона
	double const railroadTruckOffset(2.0);

	ArcLocation front(a_location);
	front.move(-railroadTruckOffset);

	ArcLocation back(a_location);
	back.move(-cargoLength);
	back.move(railroadTruckOffset);

	auto railedVehicle = std::make_unique<RailedVehicle>(
		railedVehicles.size(),
		front,
		back,
		std::make_unique<Dynamic>(cargoMass, cargoLength),
		std::make_unique<CargoPneumatics>(),
		std::make_unique<CargoElectric>()
	);

	railedVehicles.push_back(std::move(railedVehicle));

	return railedVehicles.back().get();
}

auto World::init(size_t a_railedVehiclesCount) -> void
{
	Arc* arc = graph.createArc({
		Vector3D(0.0, 0.0, 0.0),
		Vector3D(10'000.0, 0.0, 0.0),
		Vector3D(11'000.0, 0.0, 25.0),
		Vector3D(12'000.0, 0.0, 0.0),
		Vector3D(1'000'000.0, 0.0, -10'000.0)
	});

	Node* node = graph.createNode();
	node->setLeftConnector(arc->getBegin());
	node->setMainConnector(arc->getEnd());

	ArcLocation location(arc->getBegin(), 9'000.0, true);

	auto* currentVehicle = createLocomotive(location);
	currentVehicle->getPneumatic()->setFrontValveIsOpened(false);

	for (size_t i = 0; i < a_railedVehiclesCount; i++)
	{
		location.move(-currentVehicle->getDynamic()->getLength());
		
		auto* newVehicle = createCargo(location);

		auto* currentVehiclePneumatic = currentVehicle->getPneumatic();

		auto* newVehiclePneumatic = newVehicle->getPneumatic();

		// пневматическое соединение между вагонами
		currentVehiclePneumatic->getBackPneumaticConnector()->setOtherConnector(newVehiclePneumatic->getFrontPneumaticConnector());
		newVehiclePneumatic->getFrontPneumaticConnector()->setOtherConnector(currentVehiclePneumatic->getBackPneumaticConnector());

		// открытие концевых кранов
		currentVehiclePneumatic->setIsBackTmPipeConnected(true);
		newVehiclePneumatic->setIsFrontTmPipeConnected(true);

		auto* currentVehicleDynamic = currentVehicle->getDynamic();

		auto* newVehicleDynamic = newVehicle->getDynamic();

		// соединение сцепок между вагонами
		currentVehicleDynamic->getBackCoupling()->setConnectedCoupling(newVehicleDynamic->getFrontCoupling());
		newVehicleDynamic->getFrontCoupling()->setConnectedCoupling(currentVehicleDynamic->getBackCoupling());

		currentVehicle = newVehicle;
	}

	// закрытие концевого крана в хвосте состава
	currentVehicle->getPneumatic()->setBackValveIsOpened(false);
}

auto World::getLocomotivePhysicsOutputData(size_t a_id) const -> LocomotivePhysicsOutputData
{
	LocomotivePhysicsOutputData data = LocomotivePhysicsOutputData();

	RailedVehicle const* railedVehicle(findRailedVehicle(a_id));
	if (railedVehicle == nullptr)
	{
		return data;
	}

	Pneumatic* pneumatics(railedVehicle->getPneumatic());
	if (pneumatics == nullptr)
	{
		return data;
	}

	auto const* const locomotivePneumatics = dynamic_cast<LocomotivePneumatics const*>(pneumatics);
	if (locomotivePneumatics == nullptr)
	{
		return data;
	}

	data.pmPressure = locomotivePneumatics->getPMPressure();

	// data.tmPressure = locomotivePneumatics->getTMPressure();
	data.tmPressure = locomotivePneumatics->getTMBeginPressure();

	data.urPressure = locomotivePneumatics->getURPressure();

	data.tcPressure = locomotivePneumatics->getTCPressure();

	data.kvvPressure = locomotivePneumatics->getKVVPressure();

	data.zrPressure = locomotivePneumatics->getZRPressure();

	Dynamic* dynamic(railedVehicle->getDynamic());
	if (dynamic == nullptr)
	{
		return data;
	}

	data.velocity = dynamic->getVelocityKmPH();

	// data.distance = dynamic->getDistance();
	data.distance = dynamic->getFrontCoupling()->getWorldLocation().getX();

	// расстояние до соседней сцепки в миллиметрах
	data.couplingDistance = dynamic->getFrontCoupling()->getDistanceToConnectedCoupling() * 1000.0;

	// усилие на сцепке в кН
	data.couplingForce = dynamic->getFrontCoupling()->getForce() / 1000.0;

	return data;
}

auto World::setLocomotivePhysicsInputData(size_t a_id, LocomotivePhysicsInputData const& a_data) const -> void
{
	RailedVehicle const* railedVehicle(findRailedVehicle(a_id));
	if (railedVehicle == nullptr)
	{
		throw std::exception();
	}

	Pneumatic* pneumatics(railedVehicle->getPneumatic());
	if (pneumatics == nullptr)
	{
		throw std::exception();
	}

	auto* const locomotivePneumatics = dynamic_cast<LocomotivePneumatics*>(pneumatics);
	if (locomotivePneumatics == nullptr)
	{
		throw std::exception();
	}

	locomotivePneumatics->setTap395Position(a_data.tap395Position);

	locomotivePneumatics->setTap254Position(a_data.tap254Position);

	locomotivePneumatics->setIsCompressorWorking(a_data.isCompressorWorking);

	locomotivePneumatics->setEpkKey(a_data.epkKey);

	locomotivePneumatics->setEpkSignal(a_data.epkSignal);

	locomotivePneumatics->setDisconnectUnit(a_data.disconnectUnit);

	locomotivePneumatics->setReducer(a_data.reducer);

	locomotivePneumatics->setCombinedTapPosition(a_data.combinedTapPosition);

	locomotivePneumatics->setCompressorProductivity(a_data.compressorProductivity);

	Electric* electric(railedVehicle->getElectric());
	if (electric == nullptr)
	{
		throw std::exception();
	}
	
	auto* const locomotiveElectric = dynamic_cast<LocomotiveElectric*>(electric);
	if (locomotiveElectric == nullptr)
	{
		throw std::exception();
	}

	locomotiveElectric->setControllerPosition(a_data.controllerPosition);
}

auto World::getCargoPhysicsOutputData(size_t a_id) const -> CargoPhysicsOutputData
{
	CargoPhysicsOutputData data = CargoPhysicsOutputData();

	RailedVehicle const* railedVehicle(findRailedVehicle(a_id));
	if (railedVehicle == nullptr)
	{
		return data;
	}

	Pneumatic* pneumatics(railedVehicle->getPneumatic());
	if (pneumatics == nullptr)
	{
		return data;
	}

	auto const* const cargoPneumatics = dynamic_cast<CargoPneumatics const*>(pneumatics);
	if (cargoPneumatics == nullptr)
	{
		return data;
	}

	data.tmPressure = cargoPneumatics->getTMPressure();

	data.tcPressure = cargoPneumatics->getTCPressure();

	data.zrPressure = cargoPneumatics->getZRPressure();

	Dynamic* dynamic(railedVehicle->getDynamic());
	if (dynamic == nullptr)
	{
		return data;
	}

	data.velocity = dynamic->getVelocityKmPH();
	
	// data.distance = dynamic->getDistance();
	data.distance = dynamic->getFrontCoupling()->getWorldLocation().getX();

	// расстояние до соседней сцепки в миллиметрах
	data.couplingDistance = dynamic->getFrontCoupling()->getDistanceToConnectedCoupling() * 1000.0;

	// усилие на сцепке в кН
	data.couplingForce = dynamic->getFrontCoupling()->getForce() / 1000.0;
	
	return data;
}

auto World::setCargoPhysicsInputData(size_t a_id, CargoPhysicsInputData const& /*a_data*/) const -> void
{
	RailedVehicle const* railedVehicle(findRailedVehicle(a_id));
	if (railedVehicle == nullptr)
	{
		throw std::exception();
	}

	Pneumatic* pneumatics(railedVehicle->getPneumatic());
	if (pneumatics == nullptr)
	{
		throw std::exception();
	}

	auto* const cargoPneumatics = dynamic_cast<CargoPneumatics*>(pneumatics);
	if (cargoPneumatics == nullptr)
	{
		throw std::exception();
	}
}

auto World::getRailedVehiclesCount() const -> size_t
{
	return railedVehicles.size();
}

auto World::getFrontValveIsOpened(size_t a_id) const -> bool
{
	return findRailedVehiclePneumatic(a_id)->getFrontValveIsOpened();
}

auto World::setFrontValveIsOpened(size_t a_id, bool a_value) -> void
{
	findRailedVehiclePneumatic(a_id)->setFrontValveIsOpened(a_value);
}

auto World::getBackValveIsOpened(size_t a_id) const -> bool
{
	return findRailedVehiclePneumatic(a_id)->getBackValveIsOpened();
}

auto World::setBackValveIsOpened(size_t a_id, bool a_value) -> void
{
	findRailedVehiclePneumatic(a_id)->setBackValveIsOpened(a_value);
}

auto World::getVRValveIsOpened(size_t a_id) const -> bool
{
	return findRailedVehiclePneumatic(a_id)->getVRValveIsOpened();
}

auto World::setVRValveIsOpened(size_t a_id, bool a_value) -> void
{
	findRailedVehiclePneumatic(a_id)->setVRValveIsOpened(a_value);
}

auto World::setConnectedValve(size_t a_id, bool a_value) -> void
{
	RailedVehicle const* railedVehicle(findRailedVehicle(a_id));
	if (railedVehicle == nullptr)
	{
		throw std::exception();
	}

	Pneumatic* pneumatics(railedVehicle->getPneumatic());
	if (pneumatics == nullptr)
	{
		throw std::exception();
	}

	pneumatics->setIsBackTmPipeConnected(a_value);
}

auto World::setDynamicData(DynamicData const& a_dynamicData) -> void
{
	maxTensionCoupling = a_dynamicData.MaxTensionCoupling;
	maxCompressionCoupling = a_dynamicData.MaxCompressionCoupling;
	
	for (auto& vehicle : railedVehicles)
	{
		vehicle->getDynamic()->setCouplingParameters(a_dynamicData.K, a_dynamicData.R, a_dynamicData.FreeWheelAmount);
	}
}

auto World::findRailedVehicle(size_t a_id) -> RailedVehicle*
{
	if (a_id >= railedVehicles.size())
	{
		return nullptr;
	}

	return  railedVehicles[a_id].get();
}

auto World::findRailedVehicle(size_t a_id) const -> RailedVehicle const*
{
	if (a_id >= railedVehicles.size())
	{
		return nullptr;
	}

	return  railedVehicles[a_id].get();
}

auto World::findRailedVehiclePneumatic(size_t a_id) -> Pneumatic*
{
	RailedVehicle const* railedVehicle(findRailedVehicle(a_id));
	if (railedVehicle == nullptr)
	{
		throw std::exception();
	}

	Pneumatic* pneumatics(railedVehicle->getPneumatic());
	if (pneumatics == nullptr)
	{
		throw std::exception();
	}

	return pneumatics;
}

auto World::findRailedVehiclePneumatic(size_t a_id) const->Pneumatic const*
{
	RailedVehicle const* railedVehicle(findRailedVehicle(a_id));
	if (railedVehicle == nullptr)
	{
		throw std::exception();
	}

	Pneumatic* pneumatics(railedVehicle->getPneumatic());
	if (pneumatics == nullptr)
	{
		throw std::exception();
	}

	return pneumatics;
}
