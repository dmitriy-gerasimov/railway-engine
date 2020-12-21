namespace Engine.Demo.Infrastructure.Abstractions
{
    public interface ITimestampProvider
    {
        /// <summary>
        /// Количества тактов в секунду
        /// </summary>
        double Frequency { get; }
        
        /// <summary>
        /// Текущее число тактов временного механизма
        /// </summary>
        double GetTimestamp();
    }

}
