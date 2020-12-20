using System.ComponentModel;
using System.Runtime.CompilerServices;
using Engine.Demo.Infrastructure.Abstractions;
using JetBrains.Annotations;

namespace Engine.Demo.Infrastructure
{
    public class FrameCounter : IFrameCounter, INotifyPropertyChanged
    {
        private double currentHpetCounter;
        private double lastPerformanceCounter;
        private int frameCounter;

        private readonly ITimestampProvider timestampProvider;
        private int fps;

        public int Fps
        {
	        get => fps;
	        private set
	        {
		        fps = value;
                OnPropertyChanged();
	        }
        }

        public double Speed { get; set; }

        public FrameCounter(ITimestampProvider timestampProvider)
        {
            this.timestampProvider = timestampProvider;
        }

        public void UpdateFrameCount()
        {
            currentHpetCounter = timestampProvider.GetTimestamp();
            frameCounter++;
            var elapsedTimeFromUpdatingState = (currentHpetCounter - lastPerformanceCounter) * (Speed / timestampProvider.Frequency);
            //4 раза в секунду обновление счетчика FPS
            if (elapsedTimeFromUpdatingState > 0.25)
            {
                Fps = (int) (frameCounter / elapsedTimeFromUpdatingState);
                lastPerformanceCounter = currentHpetCounter;
                frameCounter = 0;
            }
        }

        public event PropertyChangedEventHandler PropertyChanged;

        [NotifyPropertyChangedInvocator]
        protected virtual void OnPropertyChanged([CallerMemberName] string propertyName = null)
        {
	        PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
        }
    }
}
