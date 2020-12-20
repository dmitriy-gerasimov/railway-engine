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

World::World() = default;

World::~World() = default;

auto World::update(double a_deltaSeconds) -> void
{
	for (auto& railedVehicle : this->railedVehicles)
	{
		railedVehicle->update(a_deltaSeconds);
	}

	for (auto& railedVehicle : this->railedVehicles)
	{
		railedVehicle->move();
	}
}

auto World::createLocomotive(ArcLocation& a_location) -> RailedVehicle*
{
	double const locomotiveLength(22.0);
	double const locomotiveMass(120'000.0);

	ArcLocation front(a_location);
	
	ArcLocation back(a_location);
	back.move(-locomotiveLength);

	auto railedVehicle = std::make_unique<RailedVehicle>(
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
	double const cargoLength(22.0);
	double const cargoMass(120'000.0);

	ArcLocation front(a_location);

	ArcLocation back(a_location);
	back.move(-cargoLength);

	auto railedVehicle = std::make_unique<RailedVehicle>(
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
		Vector3D(1000.0, 0.0, 0.0),
		Vector3D(1000.0, 1000.0, 0.0),
		Vector3D(0.0, 1000.0, 0.0),
		Vector3D(0.0, 0.0, 0.0)
	});

	Node* node = graph.createNode();
	node->setLeftConnector(arc->getBegin());
	node->setMainConnector(arc->getEnd());

	ArcLocation location(arc->getBegin(), 900.0, true);

	auto* currentVehicle = createLocomotive(location);
	currentVehicle->getPneumatic()->setFrontValveIsOpened(false);

	for (size_t i = 0; i < a_railedVehiclesCount; i++)
	{
		location.move(2 * -currentVehicle->getDynamic()->getLength());
		
		auto* newVehicle = createCargo(location);

		auto* currentVehiclePneumatic = currentVehicle->getPneumatic();

		auto* newVehiclePneumatic = newVehicle->getPneumatic();

		// создание пневматических соединений
		currentVehiclePneumatic->getBackPneumaticConnector()->setOtherConnector(newVehiclePneumatic->getFrontPneumaticConnector());
		newVehiclePneumatic->getFrontPneumaticConnector()->setOtherConnector(currentVehiclePneumatic->getBackPneumaticConnector());

		// соединение тормозных рукавов
		currentVehiclePneumatic->setIsBackTmPipeConnected(true);
		newVehiclePneumatic->setIsFrontTmPipeConnected(true);

		auto* currentVehicleDynamic = currentVehicle->getDynamic();

		auto* newVehicleDynamic = newVehicle->getDynamic();

		// сцепка вагонов 
		currentVehicleDynamic->getBackCoupling()->setConnectedCoupling(newVehicleDynamic->getFrontCoupling());
		newVehicleDynamic->getFrontCoupling()->setConnectedCoupling(currentVehicleDynamic->getBackCoupling());

		currentVehicle = newVehicle;
	}

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

	data.tmPressure = locomotivePneumatics->getTMPressure();

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

	data.distance = dynamic->getDistance();

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

	locomotivePneumatics->setBrakeForceFactor(a_data.brakeForceFactor);
	
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

	data.distance = dynamic->getDistance();
	
	return data;
}

auto World::setCargoPhysicsInputData(size_t a_id, CargoPhysicsInputData const& a_data) const -> void
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

	cargoPneumatics->setBrakeForceFactor(a_data.brakeForceFactor);
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