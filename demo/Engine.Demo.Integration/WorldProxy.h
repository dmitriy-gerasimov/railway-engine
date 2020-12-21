#pragma once

#include "Engine.Example/World.h"

namespace Engine
{
	public ref class WorldProxy sealed
	{
	public:
		ref class LocomotivePhysicsOutputData
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
		};

		ref class CargoPhysicsOutputData
		{
		public:
			double tmPressure;

			double tcPressure;

			double zrPressure;
			
			double velocity;
			
			double distance;
		};
		
		WorldProxy();

		~WorldProxy();

		auto update(double a_deltaSeconds) -> void;

		auto init(unsigned int a_railedVehiclesCount) -> void;

		auto getLocomotivePhysicsOutputData() -> LocomotivePhysicsOutputData^;

		auto setLocomotivePhysicsInputData(
			int tap395Position,
			int tap254Position,
			bool isCompressorWorking,
			bool epkKey,
			bool discUnit,
			double reducer,
			bool epkSignal,
			int controllerPosition,
			int combinedTapPosition,
			double compressorProductivity
		) -> void;

		auto getCargoPhysicsOutputData(unsigned int id) -> CargoPhysicsOutputData^;

		auto setCargoPhysicsInputData(unsigned int id, double brakeForceFactor) -> void;
		
		auto getRailedVehiclesCount() -> unsigned int;

		auto getFrontValveIsOpened(unsigned int a_id) -> bool;

		auto setFrontValveIsOpened(unsigned int a_id, bool a_value) -> void;

		auto getBackValveIsOpened(unsigned int a_id) -> bool;

		auto setBackValveIsOpened(unsigned int a_id, bool a_value) -> void;

		auto getVRValveIsOpened(unsigned int a_id) -> bool;

		auto setVRValveIsOpened(unsigned int a_id, bool a_value) -> void;
		
		auto setConnectedValve(unsigned a_id, bool a_value) -> void;

	private:
		World* world;
		
		LocomotivePhysicsOutputData^ locomotive_output_data;

		World::LocomotivePhysicsInputData* locomotiveInputData;
		
		CargoPhysicsOutputData^ cargo_output_data;

		World::CargoPhysicsInputData* cargoInputData;

	};
}