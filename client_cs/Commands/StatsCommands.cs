using System;

namespace client_cs
{
    internal static class StatsCommands
    {
        public static void Show(ISystemMonitorV3 m)
        {
            m.GetCpuUsage(out double cpu);
            m.GetMemoryInfo(out string mem);
            m.GetDiskFree(out double disk);
            m.GetUptime(out string uptime);

            Console.WriteLine($"CPU:    {cpu:F2} % {ConsoleUi.Bar(cpu)}");
            Console.WriteLine($"Memory: {mem}");
            Console.WriteLine($"Disk C: {disk:F2} GB free");
            Console.WriteLine($"Uptime: {uptime}");
        }

        public static void SaveSnapshot(ISystemMonitorV3 m)
        {
            m.SaveSnapshot();
            Console.WriteLine("Snapshot saved.");
        }

        public static void ShowHistory(ISystemMonitorV3 m)
        {
            int count = ConsoleUi.ReadInt("How many records [5]: ") ?? 5;
            m.GetHistory(count, out string history);
            Console.WriteLine(string.IsNullOrEmpty(history) ? "(history is empty - save a snapshot first)" : history);
        }

        public static void Live(ISystemMonitorV3 m)
        {
            Console.WriteLine("Live monitor - press any key to stop.");
            try
            {
                while (!Console.KeyAvailable)
                {
                    m.GetCpuUsage(out double cpu);
                    m.GetMemoryInfo(out string mem);
                    Console.WriteLine($"{DateTime.Now:HH:mm:ss}  CPU {cpu,6:F2}% {ConsoleUi.Bar(cpu)}  {mem}");
                }
                Console.ReadKey(true);
            }
            catch (InvalidOperationException)
            {
                Console.WriteLine("(console input is redirected)");
            }
        }
    }
}