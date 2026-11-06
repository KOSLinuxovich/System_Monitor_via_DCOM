using System.Runtime.InteropServices;

namespace client_cs
{
    [ComImport, Guid("78A5FD98-4D87-48F0-8D31-423C5DFF95CB"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    public interface ISystemMonitor
    {
        void GetCpuUsage(out double pCpu);
        void GetMemoryInfo([MarshalAs(UnmanagedType.BStr)] out string pMemory);
        void GetDiskFree(out double pDisk);
        void GetUptime([MarshalAs(UnmanagedType.BStr)] out string pUptime);
    }

    [ComImport, Guid("6005E7FB-A0EC-4804-A169-E675EFD7CDE3"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    public interface ISystemMonitorV2
    {
        void GetCpuUsage(out double pCpu);
        void GetMemoryInfo([MarshalAs(UnmanagedType.BStr)] out string pMemory);
        void GetDiskFree(out double pDisk);
        void GetUptime([MarshalAs(UnmanagedType.BStr)] out string pUptime);
        void SaveSnapshot();
        void GetHistory(int count, [MarshalAs(UnmanagedType.BStr)] out string pHistory);
    }

    [ComImport, Guid("2A438A26-83DC-42EF-913F-3D9543DEC4FA"), InterfaceType(ComInterfaceType.InterfaceIsIUnknown)]
    public interface ISystemMonitorV3
    {
        void GetCpuUsage(out double pCpu);
        void GetMemoryInfo([MarshalAs(UnmanagedType.BStr)] out string pMemory);
        void GetDiskFree(out double pDisk);
        void GetUptime([MarshalAs(UnmanagedType.BStr)] out string pUptime);
        void SaveSnapshot();
        void GetHistory(int count, [MarshalAs(UnmanagedType.BStr)] out string pHistory);
        void GetProcessList([MarshalAs(UnmanagedType.SafeArray, SafeArraySubType = VarEnum.VT_BSTR)] out string[] pProcesses);
        void GetProcessInfo(int pid, [MarshalAs(UnmanagedType.BStr)] out string pInfo);
        void GetNetworkStats([MarshalAs(UnmanagedType.SafeArray, SafeArraySubType = VarEnum.VT_BSTR)] out string[] pNetworks);
        void GetSystemInfo([MarshalAs(UnmanagedType.BStr)] out string pInfo);
        void KillProcess(int pid);
        void ExportReport([MarshalAs(UnmanagedType.BStr)] string filePath);
    }
}