using System;
using Engine.Demo.Infrastructure.Abstractions;

namespace Engine.Demo.Infrastructure.Cycles
{
    public class MainCycle : LifeCycle
    {
        public MainCycle(MainTickable tickableObj, IDeltaTimeCounter deltaTimeCounter,
            IFrameCounter frameCounter) : base(tickableObj, deltaTimeCounter, frameCounter)
        {
        }
    }

    public class MainTickable : ITickable
    {
	    public double DeltaSeconds { get; set; }

	    public DateTime CurrentCycleTime { get; set; }

        public void Start()
        {
            Console.WriteLine($"Start - Main {GetHashCode()}");
        }

        public void Tick(in LifeCycleContext lifeCycleContext)
        {
	        DeltaSeconds = lifeCycleContext.DeltaSeconds;
	        CurrentCycleTime = lifeCycleContext.CurrentCycleTime;
        }
    }
}