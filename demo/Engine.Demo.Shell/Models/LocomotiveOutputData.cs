using System.ComponentModel;
using System.Runtime.CompilerServices;
using JetBrains.Annotations;

namespace Engine.Demo.Shell.Models
{
	public sealed class LocomotiveOutputData : INotifyPropertyChanged
	{
		public double TM { get; set; }
		public double TC { get; set; }
		public double UR { get; set; }
		public double Pm { get; set; }
		public double KVV { get; set; }
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