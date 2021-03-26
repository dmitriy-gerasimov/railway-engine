#pragma once

#include "Engine.Example/World.h"

namespace Engine
{
	public ref class WorldProxy sealed
	{
	public:
		ref struct LocomotivePhysicsInputData
		{
		public:
			int tap395Position;
			int tap254Position;
			bool isCompressorWorking;
			bool epkKey;
			bool disconnectUnit;
			double reducer;
			bool epkSignal;
			int controllerPosition;
			int combinedTapPosition;
			double compressorProductivity;
		};

		ref struct LocomotivePhysicsOutputData
		{
		public:
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

		ref struct CargoPhysicsOutputData
		{
		public:
			double tmPressure;
			double tcPressure;
			double zrPressure;
			double velocity;
			double distance;
			double couplingDistance;
			double couplingForce;
		};
		
		ref struct DynamicData
		{
			double K;
			double R;
			double MaxTensionCoupling;
			double MaxCompressionCoupling;
			double FreeWheelAmount;
		};

		WorldProxy();

		~WorldProxy();

		auto update(double a_deltaSeconds) -> void;

		auto init(unsigned int a_railedVehiclesCount) -> void;

		auto getLocomotivePhysicsOutputData() -> LocomotivePhysicsOutputData^;

		auto setLocomotivePhysicsInputData(LocomotivePhysicsInputData^ inputData) -> void;

		auto setDynamicData(DynamicData^ inputData) -> void;

		auto getCargoPhysicsOutputData(unsigned int id) -> CargoPhysicsOutputData^;

		auto setCargoPhysicsInputData(unsigned int id, double brakeForceFactor) -> void;
		
		auto getRailedVehiclesCount() -> unsigned int;

		auto getFrontValveIsOpened(unsigned int a_id) -> bool;

		auto setFrontValveIsOpened(unsigned int a_id, bool a_value) -> void;

		auto getBackValveIsOpened(unsigned int a_id) -> bool;

		auto setBackValveIsOpened(unsigned int a_id, bool a_value) -> void;

		auto getVRValveIsOpened(unsigned int a_id) -> bool;

		auto setVRValveIsOpened(unsigned int a_id, bool a_value) -> void;
		
		auto setConnectedValve(unsigned int a_id, bool a_value) -> void;
	private:
		World* world;
		
		LocomotivePhysicsOutputData^ locomotive_output_data;

		World::LocomotivePhysicsInputData* locomotiveInputData;
		
		CargoPhysicsOutputData^ cargo_output_data;

		World::CargoPhysicsInputData* cargoInputData;

		World::DynamicData* dynamic_data;
	};
}