using System;

namespace Engine.Demo.Infrastructure
{
    public readonly ref struct LifeCycleContext
    {
        public double DeltaSeconds { get; }

        public DateTime CurrentCycleTime { get; }

        public LifeCycleContext(double deltaSeconds, DateTime currentCycleTime)
        {
            DeltaSeconds = deltaSeconds;
            CurrentCycleTime = currentCycleTime;
        }
    }
}