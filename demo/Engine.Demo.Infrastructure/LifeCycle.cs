using System;
using Engine.Demo.Infrastructure.Abstractions;

namespace Engine.Demo.Infrastructure
{ 
    public abstract class LifeCycle : ILifeCycle
    {
        private const double MinDelta = 0.001;

        private bool isPause;
        private readonly LifeCycleState state;
        
        private readonly ITickable tickableObj;
        private readonly IDeltaTimeCounter deltaTimeCounter;
        private readonly IFrameCounter frameCounter;

        public ILifeCycleState State => state;

        public LifeCycle(ITickable tickableObj, IDeltaTimeCounter deltaTimeCounter,
            IFrameCounter frameCounter)
        {
            this.tickableObj = tickableObj;
            this.deltaTimeCounter = deltaTimeCounter;
            this.frameCounter = frameCounter;
            state = new LifeCycleState();
        }

        public void Start()
        {
            tickableObj.Start();
            state.CycleTime = DateTime.Now;
        }

        public void Update()
        {
            var deltaSeconds = deltaTimeCounter.UpdateDelta(state.Speed, MinDelta);

            if (Math.Abs(deltaSeconds) <= double.Epsilon)
                return;

            if (isPause)
                return;

            UpdateState(deltaSeconds);
            Step(deltaSeconds);
        }
        
        public void Play()
        {
            isPause = false;
        }

        public void Pause()
        {
            isPause = true;
        }
        
        private void Step(double deltaSeconds)
        {
            var context = new LifeCycleContext(deltaSeconds, state.CycleTime);
            tickableObj.Tick(in context);
        }

        private void UpdateState(double deltaSeconds)
        {
            frameCounter.UpdateFrameCount();
            state.Fps = frameCounter.Fps;
            state.CycleTime = state.CycleTime.AddSeconds(deltaSeconds);
        }

    }
}