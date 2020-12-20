using Engine.Demo.Infrastructure.Abstractions;
using Microsoft.Extensions.DependencyInjection;

namespace Engine.Demo.Infrastructure
{
    public static class ServiceCollectionExtensions
    {
        public static IServiceCollection AddGameEngine(this IServiceCollection services)
        {
            services.AddTransient<IFrameCounter, FrameCounter>();
            services.AddTransient<IDeltaTimeCounter, DeltaTimeCounter>();
            services.AddTransient<ITimestampProvider, DefaultTimestampProvider>();
            return services;
        }

        public static IServiceCollection AddLifeCycle<TCycle, TRunner, TTickable>(this IServiceCollection services)
            where TCycle : class, ILifeCycle
            where TTickable : class, ITickable
            where TRunner : class, ILifeCycleOwner<TCycle>
        {
            services.AddScoped<TTickable>();
            services.AddScoped<ITickable>(p => p.GetRequiredService<TTickable>());
            services.AddScoped<TCycle>();
            services.AddScoped<ILifeCycle>(p => p.GetRequiredService<TCycle>());
            services.AddScoped<TRunner>();
            services.AddScoped<ILifeCycleOwner>(p => p.GetRequiredService<TRunner>());
            services.AddScoped<ILifeCycleOwner<TCycle>>(p => p.GetRequiredService<TRunner>());
            return services;
        }
    }
}