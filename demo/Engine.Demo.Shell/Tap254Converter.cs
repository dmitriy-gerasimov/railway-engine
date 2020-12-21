using System;
using System.Globalization;
using System.Windows.Data;

namespace Engine.Demo.Shell
{
	/// <summary>
	///		Конвертер кран 254
	/// </summary>
	public class Tap254Converter : IValueConverter
	{
		public object Convert(object value, Type targetType, object parameter, CultureInfo culture)
		{
			if (value == null)
			{
				throw new ArgumentNullException(nameof(value));
			}

			switch ((int) value)
			{
				case 1:
					return "Отпускное положение (буфер)";
				case 2: 
					return "Поездное";
				case 25:
					return "Тормозное III";
				case 50:
					return "Тормозное IV";
				case 75:
					return "Тормозное V";
				case 100:
					return "Тормозное VI";
				default:
					return $"Тормозное - {value}";
			}
		}

		public object ConvertBack(object value, Type targetType, object parameter, CultureInfo culture)
		{
			return value;
		}
	}
}