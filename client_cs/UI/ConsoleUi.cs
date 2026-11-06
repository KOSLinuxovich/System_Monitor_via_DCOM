using System;
using System.Runtime.InteropServices;

namespace client_cs
{
    internal static class ConsoleUi
    {
        public static string Describe(COMException ex)
        {
            return (uint)ex.HResult switch
            {
                0x80070490 => "process not found (never existed or already exited)",
                0x80070005 => "access denied (run as administrator or process is protected)",
                0x80070057 => "invalid argument",
                0x800705B4 => "operation timed out",
                _ => $"0x{ex.HResult:X8} {ex.Message}"
            };
        }

        public static int? ReadInt(string prompt)
        {
            Console.Write(prompt);
            return int.TryParse(Console.ReadLine(), out int value) && value > 0 ? value : null;
        }

        public static string Bar(double percent, int width = 20)
        {
            int filled = (int)Math.Round(Math.Clamp(percent, 0, 100) / 100.0 * width);
            return "[" + new string('#', filled) + new string('-', width - filled) + "]";
        }
    }
}