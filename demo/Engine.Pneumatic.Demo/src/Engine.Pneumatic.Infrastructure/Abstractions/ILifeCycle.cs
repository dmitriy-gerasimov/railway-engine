namespace Engine.Demo.Infrastructure.Abstractions
{
    public interface ILifeCycle
    {
        ILifeCycleState State { get; }

        void Start();

        void Update();

        void Play();

        void Pause();
    }
}