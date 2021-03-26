namespace Engine.Demo.Shell.Models
{
	public class DynamicSettings
	{
		/// <summary>
		/// Коэффициент, влияющий на силу между сцепками в зависимости от расстояния между ними
		/// </summary>
		public double K { get; set; }

		/// <summary>
		/// Коэффициент, влияющий на силу между сцепками в зависимости от разности скоростей
		/// </summary>
		public double R { get; set; }

		/// <summary>
		/// Максимально возможное растяжение сцепки
		/// </summary>
		public double MaxTensionCoupling { get; set; }

		/// <summary>
		/// Максимально возможное сжатие сцепки
		/// </summary>
		public double MaxCompressionCoupling { get; set; }

		/// <summary>
		/// Величина свободного хода
		/// </summary>
		public double FreeWheelAmount { get; set; }
	}
}