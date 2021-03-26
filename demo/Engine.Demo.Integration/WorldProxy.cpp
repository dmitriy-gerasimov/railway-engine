#include "WorldProxy.h"

Engine::WorldProxy::WorldProxy()
{
	world = new World();

	locomotive_output_data = gcnew LocomotivePhysicsOutputData();

	locomotiveInputData = new World::LocomotivePhysicsInputData();

	cargo_output_data = gcnew CargoPhysicsOutputData();

	cargoInputData = new World::CargoPhysicsInputData();

	dynamic_data = new World::DynamicData();
}

Engine::WorldProxy::~WorldProxy()
{
	delete world;

	delete locomotiveInputData;

	delete cargoInputData;
}

auto Engine::WorldProxy::update(double a_deltaSeconds) -> void
{
	world->update(a_deltaSeconds);
}

auto Engine::WorldProxy::init(unsigned int a_railedVehiclesCount) -> void
{
	world->init(a_railedVehiclesCount);
}

auto Engine::WorldProxy::getLocomotivePhysicsOutputData() -> LocomotivePhysicsOutputData^
{
	auto const data = world->getLocomotivePhysicsOutputData(0);

	locomotive_output_data->tmPressure = data.tmPressure;
	locomotive_output_data->tcPressure = data.tcPressure;
	locomotive_output_data->urPressure = data.urPressure;
	locomotive_output_data->pmPressure = data.pmPressure;
	locomotive_output_data->kvvPressure = data.kvvPressure;
	locomotive_output_data->zrPressure = data.zrPressure;

	locomotive_output_data->velocity = data.velocity;
	locomotive_output_data->distance = data.distance;
	locomotive_output_data->couplingDistance = data.couplingDistance;
	locomotive_output_data->couplingForce = data.couplingForce;
	
	return locomotive_output_data;
}

auto Engine::WorldProxy::setLocomotivePhysicsInputData(LocomotivePhysicsInputData^ inputData) -> void
{	
	locomotiveInputData->tap395Position = inputData->tap395Position;
	locomotiveInputData->tap254Position = inputData->tap254Position;
	locomotiveInputData->isCompressorWorking = inputData->isCompressorWorking;
	locomotiveInputData->disconnectUnit = inputData->disconnectUnit;
	locomotiveInputData->reducer = inputData->reducer;
	locomotiveInputData->epkKey = inputData->epkKey;
	locomotiveInputData->epkSignal = inputData->epkSignal;
	locomotiveInputData->controllerPosition = inputData->controllerPosition;
	locomotiveInputData->combinedTapPosition = inputData->combinedTapPosition;
	locomotiveInputData->compressorProductivity = inputData->compressorProductivity;

	world->setLocomotivePhysicsInputData(0, *locomotiveInputData);
}

auto Engine::WorldProxy::setDynamicData(DynamicData^ inputData) -> void
{
	dynamic_data->K = inputData->K;
	dynamic_data->R= inputData->R;
	dynamic_data->MaxTensionCoupling = inputData->MaxTensionCoupling;
	dynamic_data->MaxCompressionCoupling = inputData->MaxCompressionCoupling;
	dynamic_data->FreeWheelAmount = inputData->FreeWheelAmount;
	
	world->setDynamicData(*dynamic_data);
}

auto Engine::WorldProxy::getCargoPhysicsOutputData(unsigned int id) -> CargoPhysicsOutputData^
{
	auto const data = world->getCargoPhysicsOutputData(id);

	cargo_output_data->tmPressure = data.tmPressure;
	cargo_output_data->tcPressure = data.tcPressure;
	cargo_output_data->zrPressure = data.zrPressure;

	cargo_output_data->velocity = data.velocity;
	cargo_output_data->distance = data.distance;
	cargo_output_data->couplingDistance = data.couplingDistance;
	cargo_output_data->couplingForce = data.couplingForce;

	return cargo_output_data;
}

auto Engine::WorldProxy::setCargoPhysicsInputData(unsigned int id, double brakeForceFactor) -> void
{
	cargoInputData->brakeForceFactor = brakeForceFactor;

	world->setCargoPhysicsInputData(id, *cargoInputData);
}

auto Engine::WorldProxy::getRailedVehiclesCount() -> unsigned int
{
	return world->getRailedVehiclesCount();
}

auto Engine::WorldProxy::getFrontValveIsOpened(unsigned int a_id) -> bool
{
	return world->getFrontValveIsOpened(a_id);
}

auto Engine::WorldProxy::setFrontValveIsOpened(unsigned int a_id, bool a_value) -> void
{
	world->setFrontValveIsOpened(a_id, a_value);
}

auto Engine::WorldProxy::getBackValveIsOpened(unsigned int a_id) -> bool
{
	return world->getBackValveIsOpened(a_id);
}

auto Engine::WorldProxy::setBackValveIsOpened(unsigned int a_id, bool a_value) -> void
{
	world->setBackValveIsOpened(a_id, a_value);
}

auto Engine::WorldProxy::getVRValveIsOpened(unsigned int a_id) -> bool
{
	return world->getVRValveIsOpened(a_id);
}

auto Engine::WorldProxy::setVRValveIsOpened(unsigned int a_id, bool a_value) -> void
{
	world->setVRValveIsOpened(a_id, a_value);
}

auto Engine::WorldProxy::setConnectedValve(unsigned int a_id, bool a_value) -> void
{
	world->setConnectedValve(a_id, !a_value);
}
