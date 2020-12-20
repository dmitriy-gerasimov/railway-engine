using Engine.Demo.Infrastructure;
using Engine.Demo.Infrastructure.Abstractions;

namespace Engine.Demo.Shell.Models
{
	public class WorldCycle : LifeCycle
	{
		public WorldCycle(ITickable tickableObj, IDeltaTimeCounter deltaTimeCounter,
			IFrameCounter frameCounter) : base(tickableObj, deltaTimeCounter, frameCounter)
		{
		}
	}
}