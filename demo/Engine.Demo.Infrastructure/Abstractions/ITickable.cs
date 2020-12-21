namespace Engine.Demo.Infrastructure.Abstractions
{
    public interface ITickable
    {
        void Start();
        
        void Tick(in LifeCycleContext lifeCycleContext);
    }
}