using System;
using System.Collections.Generic;
using Engine.Demo.Infrastructure;
using Engine.Demo.Infrastructure.Abstractions;

namespace Engine.Demo.Shell.Models
{
	public sealed class WorldTickable : ITickable, IDisposable
	{
		private readonly WorldProxy worldProxy = new WorldProxy();

		public int Tap395Position { get; set; }
		public int Tap254Position { get; set; }
		public bool EpkKey { get; set; }
		public bool EpkSignal { get; set; }
		public bool Compressor { get; set; }
		public bool DiscUnit { get; set; }
		public double Reducer { get; set; }
		public int Controller { get; set; }
		public int CombinedTapPosition { get; set; }
		public double CompressorProductivity { get; set; } = 100;

		public LocomotiveOutputData LocomotiveOutputData { get; set; } = new LocomotiveOutputData();
		public List<CargoOutputData> CargoPhysicsOutputData { get; set; } = new List<CargoOutputData>();

		public List<RailedVehiclesInputData> VehiclesInputData { get; set; } = new List<RailedVehiclesInputData>();

		public const uint CargoVehiclesCount = 20;
		private const uint RailedVehiclesCount = CargoVehiclesCount + 1;

		public WorldTickable()
		{
			for (uint i = 0; i < CargoVehiclesCount; i++)
			{
				CargoPhysicsOutputData.Add(new CargoOutputData());
			}
			for (var i = 0; i < RailedVehiclesCount; i++)
			{
				VehiclesInputData.Add(new RailedVehiclesInputData
				{
					Id = i
				});
			}
		}

		public void Start()
		{
			worldProxy.init(CargoVehiclesCount);

			foreach (var inputData in VehiclesInputData)
			{
				var id = (uint) inputData.Id;
				inputData.FrontValve = worldProxy.getFrontValveIsOpened(id);
				inputData.BackValve = worldProxy.getBackValveIsOpened(id);
				inputData.VrValve = worldProxy.getVRValveIsOpened(id);
			}
		}

		public void Tick(in LifeCycleContext lifeCycleContext)
		{
			worldProxy.setLocomotivePhysicsInputData(
				Tap395Position,
				Tap254Position,
				Compressor,
				EpkKey,
				DiscUnit,
				Reducer,
				EpkSignal,
				Controller,
				CombinedTapPosition,
				CompressorProductivity / 1000.0
			);

			foreach (var inputData in VehiclesInputData)
			{
				var id = (uint)inputData.Id;
				worldProxy.setFrontValveIsOpened(id, inputData.FrontValve);
				worldProxy.setBackValveIsOpened(id, inputData.BackValve);
				worldProxy.setVRValveIsOpened(id, inputData.VrValve);
				worldProxy.setConnectedValve(id, inputData.IsConnected);
			}

			worldProxy.update(lifeCycleContext.DeltaSeconds);

			for (var i = 0; i < CargoVehiclesCount; i++)
			{
				var info = worldProxy.getCargoPhysicsOutputData((uint) i + 1);
				CargoPhysicsOutputData[i].TM = info.tmPressure;
				CargoPhysicsOutputData[i].TC = info.tcPressure;
				CargoPhysicsOutputData[i].ZR = info.zrPressure;
				CargoPhysicsOutputData[i].Velocity = info.velocity;
				CargoPhysicsOutputData[i].Distance = info.distance;
			}

			var outputData = worldProxy.getLocomotivePhysicsOutputData();

			LocomotiveOutputData.TM = outputData.tmPressure;
			LocomotiveOutputData.UR = outputData.urPressure;
			LocomotiveOutputData.TC = outputData.tcPressure;
			LocomotiveOutputData.Pm = outputData.pmPressure;
			LocomotiveOutputData.KVV = outputData.kvvPressure;
			LocomotiveOutputData.ZR = outputData.zrPressure;
			LocomotiveOutputData.Velocity = outputData.velocity;
			LocomotiveOutputData.Distance = outputData.distance;
		}

		public void Dispose()
		{
			worldProxy?.Dispose();
		}
	}
}