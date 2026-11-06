using System;
using System.Runtime.InteropServices;

namespace client_cs
{
    internal static class Menu
    {
        public static void Run(ISystemMonitorV3 monitor, object comObject)
        {
            while (true)
            {
                Print();
                string? choice = Console.ReadLine()?.Trim();
                if (choice == null || choice == "0") return;

                try
                {
                    Dispatch(choice, monitor, comObject);
                }
                catch (COMException ex)
                {
                    Console.WriteLine("Error: " + ConsoleUi.Describe(ex));
                }
                catch (InvalidCastException ex)
                {
                    Console.WriteLine("Interface error: " + ex.Message);
                }
            }
        }

        private static void Print()
        {
            Console.WriteLine();
            Console.WriteLine(" 1. Current stats          7. Export report");
            Console.WriteLine(" 2. Save snapshot          8. Process info (by PID)");
            Console.WriteLine(" 3. Show history           9. Kill process (by PID)");
            Console.WriteLine(" 4. Process list          10. Live monitor");
            Console.WriteLine(" 5. Network adapters      11. Compatibility test (V1/V2/V3)");
            Console.WriteLine(" 6. System info            0. Exit");
            Console.Write("Choice: ");
        }

        private static void Dispatch(string choice, ISystemMonitorV3 m, object comObject)
        {
            switch (choice)
            {
                case "1": StatsCommands.Show(m); break;
                case "2": StatsCommands.SaveSnapshot(m); break;
                case "3": StatsCommands.ShowHistory(m); break;
                case "4": ProcessBrowser.Run(m); break;
                case "5": NetworkCommands.Show(m); break;
                case "6": SystemCommands.ShowInfo(m); break;
                case "7": SystemCommands.Export(m); break;
                case "8": ProcessCommands.Info(m); break;
                case "9": ProcessCommands.Kill(m); break;
                case "10": StatsCommands.Live(m); break;
                case "11": CompatCommands.Run(comObject); break;
                default: Console.WriteLine("Unknown choice."); break;
            }
        }
    }
}