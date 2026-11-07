using System;
using System.IO;

namespace client_cs
{
    internal static class SystemCommands
    {
        public static void ShowInfo(ISystemMonitorV3 m)
        {
            m.GetSystemInfo(out string info);
            Console.WriteLine(info);
        }

        public static void Export(ISystemMonitorV3 m)
        {
            string fallback = Path.Combine(AppContext.BaseDirectory, $"report_{DateTime.Now:yyyyMMdd_HHmmss}.txt");
            Console.Write($"File path [{fallback}]: ");
            string? input = Console.ReadLine()?.Trim().Trim('"');
            string path = Path.GetFullPath(string.IsNullOrEmpty(input) ? fallback : input);

            m.ExportReport(path);
            Console.WriteLine("Report saved: " + path);
        }
    }
}