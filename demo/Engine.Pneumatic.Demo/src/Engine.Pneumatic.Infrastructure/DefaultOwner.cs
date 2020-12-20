using System.Threading;
using System.Threading.Tasks;
using Engine.Demo.Infrastructure.Abstractions;

namespace Engine.Demo.Infrastructure
{
    /// <inheritdoc />
    public class DefaultOwner<T> : ILifeCycleOwner<T> where T : ILifeCycle
    {
        public T Cycle { get; }

        public DefaultOwner(T cycle)
        {
            Cycle = cycle;
        }

        /// <inheritdoc />
        public async Task Run(CancellationToken token)
        {
            Cycle.Start();
            await Task.Factory.StartNew(() =>
            {
                while (!token.IsCancellationRequested)
                {
                    Cycle.Update();
                }
            }, token, TaskCreationOptions.LongRunning, TaskScheduler.Default);
        }
    }
}