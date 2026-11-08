using System;
using System.Collections.Generic;
using System.Linq;

namespace client_cs
{
    internal static class ProcessBrowser
    {
        private static readonly string[] SortNames = { "memory", "name", "PID", "threads" };
        private const int PageSize = 20;

        public static void Run(ISystemMonitorV3 monitor)
        {
            List<ProcRow> rows = Load(monitor);
            int sort = 0;
            int page = 0;
            string filter = "";

            while (true)
            {
                List<ProcRow> list = Query(rows, filter, sort);
                int pages = Math.Max(1, (list.Count + PageSize - 1) / PageSize);
                page = Math.Clamp(page, 0, pages - 1);

                Render(list, page, pages, sort, filter);

                Console.Write("[N]ext [P]rev [S]ort [F]ilter [R]efresh [Q]uit: ");
                ConsoleKey key = Console.ReadKey(true).Key;
                Console.WriteLine();

                switch (key)
                {
                    case ConsoleKey.N: page++; break;
                    case ConsoleKey.P: page--; break;
                    case ConsoleKey.S: sort = (sort + 1) % SortNames.Length; page = 0; break;
                    case ConsoleKey.F: filter = AskFilter(); page = 0; break;
                    case ConsoleKey.R: rows = Load(monitor); break;
                    case ConsoleKey.Q: return;
                }
            }
        }

        private static List<ProcRow> Load(ISystemMonitorV3 monitor)
        {
            monitor.GetProcessList(out string[] raw);
            return ProcessParser.Parse(raw);
        }

        private static List<ProcRow> Query(List<ProcRow> rows, string filter, int sort)
        {
            IEnumerable<ProcRow> q = rows;
            if (filter.Length > 0)
                q = q.Where(r => r.Name.Contains(filter, StringComparison.OrdinalIgnoreCase));

            q = sort switch
            {
                0 => q.OrderByDescending(r => r.WsKb),
                1 => q.OrderBy(r => r.Name, StringComparer.OrdinalIgnoreCase),
                2 => q.OrderBy(r => r.Pid),
                _ => q.OrderByDescending(r => r.Threads)
            };
            return q.ToList();
        }

        private static string AskFilter()
        {
            Console.Write("Name contains (empty = all): ");
            return Console.ReadLine()?.Trim() ?? "";
        }

        private static void Render(List<ProcRow> list, int page, int pages, int sort, string filter)
        {
            Console.WriteLine();
            Console.WriteLine($"Processes: {list.Count} | sort: {SortNames[sort]} | filter: '{filter}' | page {page + 1}/{pages}");
            Console.WriteLine($"{"PID",7}  {"Name",-34} {"Thr",4} {"PPID",7} {"Memory",10}");

            foreach (ProcRow r in list.Skip(page * PageSize).Take(PageSize))
                Console.WriteLine($"{r.Pid,7}  {r.Name,-34} {r.Threads,4} {r.Ppid,7} {r.WsKb / 1024.0,7:F1} MB");
        }
    }
}