using System;

namespace Engine.Demo.Infrastructure.Abstractions
{
    /// <summary>
    ///     Состояние жизненого цикла
    /// </summary>
    public interface ILifeCycleState
    {
        /// <summary>
        ///     Скорость обновления
        /// </summary>
        double Speed { get; set; }

        /// <summary>
        ///     Время в жизненном цикле
        /// </summary>
        DateTime CycleTime { get; }

        /// <summary>
        ///     Счетчик обновлений в секунду
        /// </summary>
        int Fps { get; }
    }
}