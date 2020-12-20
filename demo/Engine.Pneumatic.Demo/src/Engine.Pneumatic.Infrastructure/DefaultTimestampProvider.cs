using System.Diagnostics;
using System.Runtime.CompilerServices;
using Engine.Demo.Infrastructure.Abstractions;

namespace Engine.Demo.Infrastructure
{
    /// <summary>
    ///     Провайдер счетчика времени на Stopwatch
    /// </summary>
    public class DefaultTimestampProvider : ITimestampProvider
    {
        /// <summary>
        ///     Количества тактов в секунду
        /// </summary>
        public double Frequency { get; }

        /// <summary>
        ///     Текущее число тактов временного механизма
        /// </summary>
        [MethodImpl(MethodImplOptions.AggressiveInlining)]
        public double GetTimestamp() => Stopwatch.GetTimestamp();

        public DefaultTimestampProvider()
        {
            Frequency = Stopwatch.Frequency;
        }
    }
}