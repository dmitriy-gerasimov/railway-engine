namespace Engine.Demo.Infrastructure.Abstractions
{
    public interface IDeltaTimeCounter
    {
        /// <summary>
        ///     Расчет дельты времени с учетом абстрактной скорости и минимального показателя
        /// </summary>
        /// <param name="speed">Показатель скорости</param>
        /// <param name="minDelta">Минимальная дельта для расчета</param>
        /// <returns></returns>
        double UpdateDelta(double speed, double minDelta);
    }
}
