using System.ComponentModel;
using System.Runtime.CompilerServices;
using JetBrains.Annotations;

namespace Engine.Demo.Shell.Models
{
	public sealed class RailedVehiclesInputData : INotifyPropertyChanged
	{
		public int Id { get; set; }
		public bool FrontValve { get; set; }
		public bool BackValve { get; set; }
		public bool VrValve { get; set; }
		public bool IsConnected { get; set; }

		public event PropertyChangedEventHandler PropertyChanged;

		[NotifyPropertyChangedInvocator]
		// ReSharper disable once UnusedMember.Local
		private void OnPropertyChanged([CallerMemberName] string propertyName = null)
		{
			PropertyChanged?.Invoke(this, new PropertyChangedEventArgs(propertyName));
		}
	}
}