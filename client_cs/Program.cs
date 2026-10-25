using System;
using System.Runtime.InteropServices;
using System.Text;

namespace client_cs
{
    internal static class Program
    {
        [STAThread]
        private static int Main()
        {
            Console.OutputEncoding = Encoding.UTF8;
            Console.WriteLine("=== System Monitor (COM / DCOM Lab 3) ===");

            if (!ComConnection.TryConnect(out object? comObject, out ISystemMonitorV3? monitor) ||
                comObject == null || monitor == null)
                return 1;

            try
            {
                Menu.Run(monitor, comObject);
            }
            finally
            {
                Marshal.FinalReleaseComObject(comObject);
            }
            return 0;
        }
    }
}