using System;
using System.Collections.Generic;
using System.Threading;
using System.Threading.Tasks;
using Engine.Demo.Infrastructure.Abstractions;
using Microsoft.Extensions.Hosting;

namespace Engine.Demo.Infrastructure
{
    internal class ConsoleConstantLabel : BackgroundService
    {
        private readonly IEnumerable<ILifeCycle> cycles;

        public ConsoleConstantLabel(IEnumerable<ILifeCycle> cycles)
        {
            this.cycles = cycles;
        }


        protected override async Task ExecuteAsync(CancellationToken stoppingToken)
        {
            while (!stoppingToken.IsCancellationRequested)
            {
                Console.Clear();
                foreach (var cycle in cycles)
                {
                    var outputStringFps = $"FPS - {cycle.State.Fps:F1}";
                    var outputStringCycleSpeed = $"CycleSpeed - {cycle.State.Speed:F1}";
                    var outputStringTime = $"CycleTime - {cycle.State.CycleTime:hh:mm:ss:fff}";
                    var outputStringTimeNow = $"TimeNow   - {DateTime.Now:hh:mm:ss:fff}";
                    await WriteOut(cycle.ToString());
                    await WriteOut(outputStringFps);
                    await WriteOut(outputStringTime);
                    await WriteOut(outputStringCycleSpeed);
                    await WriteOut(outputStringTimeNow);
                }
                await Task.Delay(200, stoppingToken);
            }
        }

        private static Task WriteOut(string str) => Console.Out.WriteLineAsync(str);
    }
}