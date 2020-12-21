using Engine.Demo.Infrastructure.Abstractions;

namespace Engine.Demo.Infrastructure
{
    public class DeltaTimeCounter : IDeltaTimeCounter
    {
        private double previousHpetCounter;
        private double currentHpetCounter;

        private readonly ITimestampProvider timestampProvider;

        public DeltaTimeCounter(ITimestampProvider timestampProvider)
        {
            this.timestampProvider = timestampProvider;
            previousHpetCounter = timestampProvider.GetTimestamp();
            currentHpetCounter = timestampProvider.GetTimestamp();
        }

        public double UpdateDelta(double speed, double minDelta)
        {
            currentHpetCounter = timestampProvider.GetTimestamp();
            var deltaSeconds = speed * (currentHpetCounter - previousHpetCounter) / timestampProvider.Frequency;

            if (deltaSeconds > minDelta)
            {
                previousHpetCounter = currentHpetCounter;
            }
            else
            {
                return 0;
            }

            if (deltaSeconds > 0.01)
            {
                deltaSeconds = 0.01;
            }

            return deltaSeconds;
        }

    }
}
