using System;
using System.Runtime.InteropServices;

namespace client_cs
{
    internal static class ProcessCommands
    {
        public static void Info(ISystemMonitorV3 m)
        {
            int? pid = ConsoleUi.ReadInt("Enter PID: ");
            if (pid == null)
            {
                Console.WriteLine("Invalid PID.");
                return;
            }

            m.GetProcessInfo(pid.Value, out string info);
            Console.WriteLine(info);
        }

        public static void Kill(ISystemMonitorV3 m)
        {
            int? pid = ConsoleUi.ReadInt("Enter PID to kill: ");
            if (pid == null)
            {
                Console.WriteLine("Invalid PID.");
                return;
            }

            m.GetProcessInfo(pid.Value, out string info);
            Console.WriteLine(info);

            Console.Write("Type YES to terminate this process: ");
            if (Console.ReadLine()?.Trim() != "YES")
            {
                Console.WriteLine("Cancelled.");
                return;
            }

            m.KillProcess(pid.Value);
            Console.WriteLine("Process terminated.");
            Verify(m, pid.Value);
        }

        private static void Verify(ISystemMonitorV3 m, int pid)
        {
            try
            {
                m.GetProcessInfo(pid, out _);
                Console.WriteLine("Warning: process is still reported as running.");
            }
            catch (COMException ex) when ((uint)ex.HResult == 0x80070490)
            {
                Console.WriteLine("Verified: process no longer exists.");
            }
        }
    }
}