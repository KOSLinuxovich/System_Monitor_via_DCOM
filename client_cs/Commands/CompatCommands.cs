using System;

namespace client_cs
{
    internal static class CompatCommands
    {
        public static void Run(object comObject)
        {
            var v1 = (ISystemMonitor)comObject;
            v1.GetUptime(out string uptime);
            Console.WriteLine($"V1 (4 methods)  OK - uptime: {uptime}");

            var v2 = (ISystemMonitorV2)comObject;
            v2.GetHistory(1, out string last);
            Console.WriteLine($"V2 (6 methods)  OK - last record: {(string.IsNullOrEmpty(last) ? "(none)" : last)}");

            var v3 = (ISystemMonitorV3)comObject;
            v3.GetSystemInfo(out string info);
            Console.WriteLine($"V3 (12 methods) OK - {info.Split('\n')[0]}");
        }
    }
}