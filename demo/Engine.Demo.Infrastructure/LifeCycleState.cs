using System;
using Engine.Demo.Infrastructure.Abstractions;

namespace Engine.Demo.Infrastructure
{
    /// <inheritdoc />
    internal class LifeCycleState : ILifeCycleState
    {
        /// <inheritdoc />
        public double Speed { get; set; }

        /// <inheritdoc />
        public DateTime CycleTime { get; set; }

        /// <inheritdoc />
        public int Fps { get; set; }

        public LifeCycleState()
        {
            Speed = 1;
        }
    }
}