using System;
using System.Globalization;
using System.Windows.Data;

namespace Engine.Demo.Shell
{
	/// <summary>
	///		Конвертер Блокировки усл.№ 367м
	/// </summary>
	public class Lock367Converter : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			if (value == null)
			{
				throw new ArgumentNullException(nameof(value));
			}

			return (int) value switch
			{
				-1 => "Экстренное торможение",
				0 => "Поездное положение",
				1 => "Положение двойной тяги",
				_ => value.ToString()
			};
		}

		public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
		{
			return value;
		}
	}
}