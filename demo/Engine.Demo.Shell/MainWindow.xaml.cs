using System.Text.RegularExpressions;
using System.Threading;
using System.Windows;
using System.Windows.Input;
using Engine.Demo.Infrastructure;
using Engine.Demo.Shell.Models;
using Engine.Demo.Shell.Properties;

namespace Engine.Demo.Shell
{
    /// <summary>
    /// Логика взаимодействия для MainWindow.xaml
    /// </summary>
    public partial class MainWindow
	{
		private bool isRun;
		private readonly WorldCycle lifeCycle;
		public WorldTickable MainTickable { get; set; }
		private readonly DefaultOwner<WorldCycle> owner;
		private readonly CancellationTokenSource cancellationTokenSource = new CancellationTokenSource();

		public MainWindow()
		{
			InitializeComponent();
			DataContext = this;
			MainTickable = new WorldTickable();
			var frameCounter = new FrameCounter(new DefaultTimestampProvider());
			var deltaTimeCounter = new DeltaTimeCounter(new DefaultTimestampProvider());
			lifeCycle = new WorldCycle(
				MainTickable,
				deltaTimeCounter, 
				frameCounter);
			owner = new DefaultOwner<WorldCycle>(lifeCycle);

			MainTickable.Tap395Position = 2;
			MainTickable.Tap254Position = 2;
			MainTickable.EpkSignal = true;
			MainTickable.EpkKey = true;
			MainTickable.Reducer = 5.0;

			MainTickable.DynamicSettings.K = Settings.Default.K;
			MainTickable.DynamicSettings.R = Settings.Default.R;
			MainTickable.DynamicSettings.MaxTensionCoupling = Settings.Default.MaxTensionCoupling;
			MainTickable.DynamicSettings.MaxCompressionCoupling = Settings.Default.MaxCompressionCoupling;
			MainTickable.DynamicSettings.FreeWheelAmount = Settings.Default.FreeWheelAmount;

			InitList();
		}

		private void Start_Button_Click(object sender, RoutedEventArgs e)
		{
			if (!isRun)
			{
				owner?.Run(cancellationTokenSource.Token);
				isRun = true;
			}
			lifeCycle.Play();
		}

		private void Stop_Button_Click(object sender, RoutedEventArgs e)
		{
			lifeCycle.Pause();
		}

		private void NumberValidationTextBox(object sender, TextCompositionEventArgs e)
		{
			Regex regex = new Regex("[^0-9.]+");
			e.Handled = regex.IsMatch(e.Text);
		}
	}
}
