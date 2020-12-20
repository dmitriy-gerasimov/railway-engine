using System;
using Engine.Demo.Infrastructure.Abstractions;

namespace Engine.Demo.Infrastructure.Cycles
{
    public class SubCycle : LifeCycle
    {
        public SubCycle(SubTickable tickableObj, IDeltaTimeCounter deltaTimeCounter, IFrameCounter frameCounter)
            : base(tickableObj, deltaTimeCounter, frameCounter)
        {
        }
    }
    
    public class SubTickable : ITickable
    {
        public void Start()
        {
            Console.WriteLine($"Start - Sub {GetHashCode()}");
        }

        public void Tick(in LifeCycleContext lifeCycleContext)
        {
            //throw new NotImplementedException();
        }
    }
}