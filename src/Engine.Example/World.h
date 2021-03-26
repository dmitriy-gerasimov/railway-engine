#pragma once

#include "Engine.Graph/src/Graph.h"
#include "Engine.Graph/src/ArcLocation.h"

class RailedVehicle;

class Pneumatic;
class LocomotivePneumatics;

class World final
{
public:
	struct LocomotivePhysicsOutputData
	{
		double tmPressure;

		double pmPressure;

		double tcPressure;

		double urPressure;

		double kvvPressure;

		double zrPressure;

		double velocity;

		double distance;

		double couplingDistance;

		double couplingForce;
	};

	struct LocomotivePhysicsInputData
	{
		int tap395Position;

		int tap254Position;

		bool isCompressorWorking;

		bool epkKey;

		bool epkSignal;

		bool disconnectUnit;

		double reducer;

		double brakeForceFactor;

		int controllerPosition;

		int combinedTapPosition;
		
		double compressorProductivity;
	};

	struct CargoPhysicsOutputData
	{
		double tmPressure;

		double tcPressure;

		double zrPressure;

		double velocity;

		double distance;

		double couplingDistance;

		double couplingForce;
	};

	struct CargoPhysicsInputData
	{
		double brakeForceFactor;
	};
	
	struct DynamicData
	{
		double K;
		double R;
		double MaxTensionCoupling;
		double MaxCompressionCoupling;
		double FreeWheelAmount;
	};

	World(World const& a_world) = delete;
	auto operator= (World const& a_world) -> World& = delete;

	World(World&& a_world) = delete;
	auto operator= (World&& a_world)->World & = delete;

	World();

	~World();

	auto update(double a_deltaSeconds) -> void;

	auto init(size_t a_railedVehiclesCount) -> void;

	auto getLocomotivePhysicsOutputData(size_t a_id) const -> LocomotivePhysicsOutputData;

	auto setLocomotivePhysicsInputData(size_t a_id, LocomotivePhysicsInputData const& a_data) const -> void;

	auto getCargoPhysicsOutputData(size_t a_id) const -> CargoPhysicsOutputData;

	auto setCargoPhysicsInputData(size_t a_id, CargoPhysicsInputData const& a_data) const -> void;

	auto getRailedVehiclesCount() const -> size_t;

	auto getFrontValveIsOpened(size_t a_id) const -> bool;

	auto setFrontValveIsOpened(size_t a_id, bool a_value) -> void;

	auto getBackValveIsOpened(size_t a_id) const -> bool;

	auto setBackValveIsOpened(size_t a_id, bool a_value) -> void;

	auto getVRValveIsOpened(size_t a_id) const -> bool;

	auto setVRValveIsOpened(size_t a_id, bool a_value) -> void;

	auto setConnectedValve(size_t a_id, bool a_value) -> void;
	
	auto setDynamicData(DynamicData const& a_dynamicData) -> void;

private:
	static double const Epsilon;

	auto createLocomotive(ArcLocation& a_location) -> RailedVehicle*;

	auto createCargo(ArcLocation& a_location) -> RailedVehicle*;
	
	auto findRailedVehicle(size_t a_id) -> RailedVehicle*;

	auto findRailedVehicle(size_t a_id) const -> RailedVehicle const*;

	auto findRailedVehiclePneumatic(size_t a_id) -> Pneumatic*;

	auto findRailedVehiclePneumatic(size_t a_id) const -> Pneumatic const*;
	
	Graph graph;

	std::vector<std::unique_ptr<RailedVehicle>> railedVehicles;

	double maxTensionCoupling;

	double maxCompressionCoupling;
};