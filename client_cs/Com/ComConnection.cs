using System;
using System.Runtime.InteropServices;

namespace client_cs
{
    internal static class ComConnection
    {
        private static readonly Guid ClsidV3 = new Guid("80ED9BC2-480D-4C98-A5DC-0240E9BB2B7A");

        public static bool TryConnect(out object? comObject, out ISystemMonitorV3? monitor)
        {
            comObject = null;
            monitor = null;

            try
            {
                Type? type = Type.GetTypeFromCLSID(ClsidV3, false);
                if (type == null)
                {
                    Console.WriteLine("CLSID V3 is not registered. Run regsvr32 as administrator.");
                    return false;
                }

                comObject = Activator.CreateInstance(type);
                if (comObject == null)
                {
                    Console.WriteLine("Failed to create the COM object.");
                    return false;
                }

                monitor = (ISystemMonitorV3)comObject;
                return true;
            }
            catch (InvalidCastException ex)
            {
                Console.WriteLine("E_NOINTERFACE: " + ex.Message);
                return false;
            }
            catch (COMException ex)
            {
                Console.WriteLine($"COM error: 0x{ex.HResult:X8} {ex.Message}");
                return false;
            }
        }
    }
}