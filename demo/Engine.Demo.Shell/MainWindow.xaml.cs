using System.Text.RegularExpressions;
using System.Threading;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Controls.Primitives;
using System.Windows.Data;
using System.Windows.Input;
using Engine.Demo.Infrastructure;
using Engine.Demo.Shell.Models;

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

			InitList();
		}

		private void InitList()
		{
			for (var i = 0; i < WorldTickable.CargoVehiclesCount; i++)
			{
				StackPanelVehicles.Children.Add(AddVehicleInfo(i));
			}
		}

		private UIElement AddVehicleInfo(int number)
		{
			var vehicleInfoGrid = new Grid();
			vehicleInfoGrid.ColumnDefinitions.Add(new ColumnDefinition());
			vehicleInfoGrid.ColumnDefinitions.Add(new ColumnDefinition());
			vehicleInfoGrid.ColumnDefinitions.Add(new ColumnDefinition());
			vehicleInfoGrid.ColumnDefinitions.Add(new ColumnDefinition());
			vehicleInfoGrid.ColumnDefinitions.Add(new ColumnDefinition());
			vehicleInfoGrid.ColumnDefinitions.Add(new ColumnDefinition());
			vehicleInfoGrid.ColumnDefinitions.Add(new ColumnDefinition());
			vehicleInfoGrid.ColumnDefinitions.Add(new ColumnDefinition());
			vehicleInfoGrid.ColumnDefinitions.Add(new ColumnDefinition());
			vehicleInfoGrid.ColumnDefinitions.Add(new ColumnDefinition());
			vehicleInfoGrid.RowDefinitions.Add(new RowDefinition());
			vehicleInfoGrid.RowDefinitions.Add(new RowDefinition());

			var textBlockName = new TextBlock
			{
				Text = $"Вагон {number + 1}",
				VerticalAlignment = VerticalAlignment.Center,
				HorizontalAlignment = HorizontalAlignment.Center
			};
			textBlockName.SetValue(Grid.RowProperty, 0);
			textBlockName.SetValue(Grid.ColumnProperty, 0);
			textBlockName.SetValue(Grid.RowSpanProperty, 2);

			var textBlockTmName = new TextBlock
			{
				Text = "ТМ",
				HorizontalAlignment = HorizontalAlignment.Center
			};
			textBlockTmName.SetValue(Grid.RowProperty, 0);
			textBlockTmName.SetValue(Grid.ColumnProperty, 1);
			var textBlockTmValue = new TextBlock
			{
				HorizontalAlignment = HorizontalAlignment.Center
			};
			textBlockTmValue.SetValue(Grid.RowProperty, 0);
			textBlockTmValue.SetValue(Grid.ColumnProperty, 2);

			var bindingTm = new Binding($"MainTickable.CargoPhysicsOutputData[{number}].TM")
			{
				StringFormat = "0.####",
				UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged
			};
			textBlockTmValue.SetBinding(TextBlock.TextProperty, bindingTm);

			var textBlockTcName = new TextBlock
			{
				Text = "ТЦ",
				HorizontalAlignment = HorizontalAlignment.Center
			};
			textBlockTcName.SetValue(Grid.RowProperty, 0);
			textBlockTcName.SetValue(Grid.ColumnProperty, 3);
			var textBlockTcValue = new TextBlock
			{
				HorizontalAlignment = HorizontalAlignment.Center
			};
			textBlockTcValue.SetValue(Grid.RowProperty, 0);
			textBlockTcValue.SetValue(Grid.ColumnProperty, 4);
			var bindingTc = new Binding($"MainTickable.CargoPhysicsOutputData[{number}].TC")
			{
				StringFormat = "0.####",
				UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged
			};
			textBlockTcValue.SetBinding(TextBlock.TextProperty, bindingTc);

			var textBlockZrName = new TextBlock
			{
				Text = "ЗР",
				HorizontalAlignment = HorizontalAlignment.Center
			};
			textBlockZrName.SetValue(Grid.RowProperty, 0);
			textBlockZrName.SetValue(Grid.ColumnProperty, 5);
			var textBlockZrValue = new TextBlock
			{
				HorizontalAlignment = HorizontalAlignment.Center
			};
			textBlockZrValue.SetValue(Grid.RowProperty, 0);
			textBlockZrValue.SetValue(Grid.ColumnProperty, 6);
			var bindingZr = new Binding($"MainTickable.CargoPhysicsOutputData[{number}].ZR")
			{
				StringFormat = "0.####",
				UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged
			};
			textBlockZrValue.SetBinding(TextBlock.TextProperty, bindingZr);

			var progressBarTm = new ProgressBar
			{
				Margin = new Thickness(5), 
				Maximum = 10,
				Height = 15
			};
			progressBarTm.SetValue(Grid.RowProperty, 1);
			progressBarTm.SetValue(Grid.ColumnProperty, 1);
			progressBarTm.SetValue(Grid.ColumnSpanProperty, 2);
			progressBarTm.SetBinding(RangeBase.ValueProperty, bindingTm);

			var progressBarTc = new ProgressBar
			{
				Margin = new Thickness(5),
				Maximum = 4,
				Height = 15
			};
			progressBarTc.SetValue(Grid.RowProperty, 1);
			progressBarTc.SetValue(Grid.ColumnProperty, 3);
			progressBarTc.SetValue(Grid.ColumnSpanProperty, 2);
			progressBarTc.SetBinding(RangeBase.ValueProperty, bindingTc);

			var progressBarZr = new ProgressBar
			{
				Margin = new Thickness(5),
				Maximum = 10,
				Height = 15
			};
			progressBarZr.SetValue(Grid.RowProperty, 1);
			progressBarZr.SetValue(Grid.ColumnProperty, 5);
			progressBarZr.SetValue(Grid.ColumnSpanProperty, 2);
			progressBarZr.SetBinding(RangeBase.ValueProperty, bindingZr);

			var grid = new Grid();

			grid.ColumnDefinitions.Add(new ColumnDefinition());
			grid.ColumnDefinitions.Add(new ColumnDefinition());
			grid.ColumnDefinitions.Add(new ColumnDefinition());
			grid.RowDefinitions.Add(new RowDefinition());
			grid.RowDefinitions.Add(new RowDefinition());

			grid.SetValue(Grid.RowProperty, 0);
			grid.SetValue(Grid.ColumnProperty, 7);
			grid.SetValue(Grid.RowSpanProperty, 2);

			var bindingConnected = new Binding($"MainTickable.VehiclesInputData[{number + 1}].IsConnected")
			{
				UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged
			};
			var bindingFront = new Binding($"MainTickable.VehiclesInputData[{number + 1}].FrontValve")
			{
				UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged
			};
			var bindingBack = new Binding($"MainTickable.VehiclesInputData[{number + 1}].BackValve")
			{
				UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged
			};
			var bindingVr = new Binding($"MainTickable.VehiclesInputData[{number + 1}].VrValve")
			{
				UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged
			};
			var checkBoxIsConnected = new CheckBox
			{
				HorizontalAlignment = HorizontalAlignment.Center,
				VerticalAlignment = VerticalAlignment.Center
			};
			checkBoxIsConnected.SetValue(Grid.RowProperty, 0);
			checkBoxIsConnected.SetValue(Grid.ColumnProperty, 0);
			checkBoxIsConnected.SetValue(Grid.RowSpanProperty, 2);
			checkBoxIsConnected.SetBinding(ToggleButton.IsCheckedProperty, bindingConnected);

			var checkBoxFront = new CheckBox
			{
				HorizontalAlignment = HorizontalAlignment.Center, 
				VerticalAlignment = VerticalAlignment.Center
			};
			checkBoxFront.SetValue(Grid.RowProperty, 0);
			checkBoxFront.SetValue(Grid.ColumnProperty, 1);
			checkBoxFront.SetBinding(ToggleButton.IsCheckedProperty, bindingFront);

			var checkBoxBack = new CheckBox
			{
				HorizontalAlignment = HorizontalAlignment.Center,
				VerticalAlignment = VerticalAlignment.Center
			};
			checkBoxBack.SetValue(Grid.RowProperty, 1);
			checkBoxBack.SetValue(Grid.ColumnProperty, 1);
			checkBoxBack.SetBinding(ToggleButton.IsCheckedProperty, bindingBack);

			var checkBoxVr = new CheckBox
			{
				HorizontalAlignment = HorizontalAlignment.Center,
				VerticalAlignment = VerticalAlignment.Center
			};
			checkBoxVr.SetValue(Grid.RowProperty, 0);
			checkBoxVr.SetValue(Grid.ColumnProperty, 2);
			checkBoxVr.SetValue(Grid.RowSpanProperty, 2);
			checkBoxVr.SetBinding(ToggleButton.IsCheckedProperty, bindingVr);

			grid.Children.Add(checkBoxIsConnected);
			grid.Children.Add(checkBoxFront);
			grid.Children.Add(checkBoxBack);
			grid.Children.Add(checkBoxVr);

			var textBlockVelocityName = new TextBlock
			{
				Text = "Скорость",
				HorizontalAlignment = HorizontalAlignment.Center,
				VerticalAlignment = VerticalAlignment.Center
			};
			textBlockVelocityName.SetValue(Grid.RowProperty, 0);
			textBlockVelocityName.SetValue(Grid.ColumnProperty, 8);

			var textBlockDistanceName = new TextBlock
			{
				Text = "Путь",
				HorizontalAlignment = HorizontalAlignment.Center,
				VerticalAlignment = VerticalAlignment.Center
			};
			textBlockDistanceName.SetValue(Grid.RowProperty, 0);
			textBlockDistanceName.SetValue(Grid.ColumnProperty, 9);

			var bindingVelocity = new Binding($"MainTickable.CargoPhysicsOutputData[{number}].Velocity")
			{
				StringFormat = "0.##",
				UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged
			};
			var textBlockVelocityValue = new TextBlock
			{
				HorizontalAlignment = HorizontalAlignment.Center,
				VerticalAlignment = VerticalAlignment.Center
			};
			textBlockVelocityValue.SetValue(Grid.RowProperty, 1);
			textBlockVelocityValue.SetValue(Grid.ColumnProperty, 8);
			textBlockVelocityValue.SetBinding(TextBlock.TextProperty, bindingVelocity);

			var bindingDistance = new Binding($"MainTickable.CargoPhysicsOutputData[{number}].Distance")
			{
				StringFormat = "0.##",
				UpdateSourceTrigger = UpdateSourceTrigger.PropertyChanged
			};
			var textBlockDistanceValue = new TextBlock
			{
				HorizontalAlignment = HorizontalAlignment.Center,
				VerticalAlignment = VerticalAlignment.Center
			};
			textBlockDistanceValue.SetValue(Grid.RowProperty, 1);
			textBlockDistanceValue.SetValue(Grid.ColumnProperty, 9);
			textBlockDistanceValue.SetBinding(TextBlock.TextProperty, bindingDistance);

			vehicleInfoGrid.Children.Add(textBlockName);
			vehicleInfoGrid.Children.Add(textBlockTmName);
			vehicleInfoGrid.Children.Add(textBlockTmValue);
			vehicleInfoGrid.Children.Add(textBlockTcName);
			vehicleInfoGrid.Children.Add(textBlockTcValue);
			vehicleInfoGrid.Children.Add(textBlockZrName);
			vehicleInfoGrid.Children.Add(textBlockZrValue);

			vehicleInfoGrid.Children.Add(textBlockDistanceName);
			vehicleInfoGrid.Children.Add(textBlockDistanceValue);
			vehicleInfoGrid.Children.Add(textBlockVelocityName);
			vehicleInfoGrid.Children.Add(textBlockVelocityValue);

			vehicleInfoGrid.Children.Add(progressBarTm);
			vehicleInfoGrid.Children.Add(progressBarTc);
			vehicleInfoGrid.Children.Add(progressBarZr);

			vehicleInfoGrid.Children.Add(grid);

			return vehicleInfoGrid;
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
