using System.Collections.Generic;

namespace client_cs
{
    internal static class ProcessParser
    {
        public static List<ProcRow> Parse(string[] raw)
        {
            var rows = new List<ProcRow>(raw.Length);
            foreach (string line in raw)
            {
                string[] f = line.Split('|');
                if (f.Length < 5) continue;

                if (int.TryParse(f[0], out int pid) &&
                    int.TryParse(f[2], out int threads) &&
                    int.TryParse(f[3], out int ppid) &&
                    long.TryParse(f[4], out long wsKb))
                {
                    rows.Add(new ProcRow(pid, f[1], threads, ppid, wsKb));
                }
            }
            return rows;
        }
    }
}