using System.ComponentModel;
using System.Runtime.CompilerServices;
using JetBrains.Annotations;

namespace Engine.Demo.Shell.Models
{
	public sealed class CargoOutputData : INotifyPropertyChanged
	{
		public double TM { get; set; }
		public double TC { get; set; }
		public double ZR { get; set; }
		public double Velocity { get; set; }
		public double Distance { get; set; }

		public event PropertyChangedEventHandler PropertyChanged;

		[NotifyPropertyChangedInvocator]
		// ReSharper disable once UnusedMember.Local
		private void OnPropertyChanged([CallerMemberName] string propertyName = null)
		{
			PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
		}
	}
}