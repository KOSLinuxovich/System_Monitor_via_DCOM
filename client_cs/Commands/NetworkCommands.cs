using System;
using System.Linq;

namespace client_cs
{
    internal static class NetworkCommands
    {
        public static void Show(ISystemMonitorV3 m)
        {
            m.GetNetworkStats(out string[] items);
            Console.WriteLine($"Network adapters: {items.Length}");

            for (int i = 0; i < items.Length; i++)
            {
                string[] parts = items[i].Split(" | ");
                Console.WriteLine();
                Console.WriteLine($"[{i + 1}] {parts[0]}  ({parts.LastOrDefault()})");
                for (int p = 1; p < parts.Length - 1; p++)
                    Console.WriteLine("    " + parts[p]);
            }
        }
    }
}