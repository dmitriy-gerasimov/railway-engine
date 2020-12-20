namespace Engine.Demo.Infrastructure.Abstractions
{
    public interface IFrameCounter
    {
        /// <summary>
        ///     Текущее значение FPS
        /// </summary>
        int Fps { get; }
        
        /// <summary>
        ///     Обновление счетчика
        /// </summary>
        void UpdateFrameCount();
    }
}
