using System;
using System.Linq;
using System.Threading;
using System.Threading.Tasks;
using Engine.Demo.Infrastructure.Abstractions;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Hosting;

namespace Engine.Demo.Infrastructure
{
    class MainService : BackgroundService
    {
        private readonly IServiceProvider provider;

        public MainService(IServiceProvider provider)
        {
            this.provider = provider;
        }

        /// <inheritdoc />
        protected override async Task ExecuteAsync(CancellationToken stoppingToken)
        {
            using var scope = provider.CreateScope();
            var owners = scope.ServiceProvider.GetServices<ILifeCycleOwner>();
            var label =  scope.ServiceProvider.GetRequiredService<ConsoleConstantLabel>();
            await label.StartAsync(stoppingToken);
            await Task.WhenAll(owners.Select(o => o.Run(stoppingToken)));
            await label.StopAsync(CancellationToken.None);
        }
    }
}