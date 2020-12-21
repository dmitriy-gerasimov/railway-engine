using System.Threading;
using System.Threading.Tasks;

namespace Engine.Demo.Infrastructure.Abstractions
{
    /// <summary>
    ///     Отвечает за запуск жизненого цикла
    /// </summary>
    public interface ILifeCycleOwner
    {
        /// <summary>
        ///     Запуск
        /// </summary>
        /// <param name="token"></param>
        /// <returns></returns>
        Task Run(CancellationToken token);
    }

    public interface ILifeCycleOwner<out T> : ILifeCycleOwner where T : ILifeCycle
    {
        T Cycle { get; }
    }
}