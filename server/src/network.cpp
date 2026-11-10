#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <windows.h>
#include "network.h"
#include "text_utils.h"
#include <cstdio>

#pragma comment(lib, "iphlpapi.lib")

namespace sm
{
    namespace
    {
        std::wstring MacText(const IP_ADAPTER_ADDRESSES* a)
        {
            if (a->PhysicalAddressLength == 0) return L"n/a";
            std::wstring mac;
            for (ULONG i = 0; i < a->PhysicalAddressLength; ++i)
            {
                wchar_t b[8];
                swprintf_s(b, L"%02X%ls", a->PhysicalAddress[i], (i + 1 < a->PhysicalAddressLength) ? L"-" : L"");
                mac += b;
            }
            return mac;
        }

        std::wstring Ipv4Text(const IP_ADAPTER_ADDRESSES* a)
        {
            for (PIP_ADAPTER_UNICAST_ADDRESS u = a->FirstUnicastAddress; u; u = u->Next)
            {
                if (u->Address.lpSockaddr && u->Address.lpSockaddr->sa_family == AF_INET)
                {
                    const sockaddr_in* sin = reinterpret_cast<const sockaddr_in*>(u->Address.lpSockaddr);
                    wchar_t b[32];
                    swprintf_s(b, L"%u.%u.%u.%u",
                        sin->sin_addr.S_un.S_un_b.s_b1, sin->sin_addr.S_un.S_un_b.s_b2,
                        sin->sin_addr.S_un.S_un_b.s_b3, sin->sin_addr.S_un.S_un_b.s_b4);
                    return b;
                }
            }
            return L"n/a";
        }

        std::wstring TrafficText(const IP_ADAPTER_ADDRESSES* a)
        {
            MIB_IF_ROW2 row = {};
            row.InterfaceIndex = a->IfIndex;
            if (GetIfEntry2(&row) != NO_ERROR) return L"";

            std::wstring t;
            if (row.TransmitLinkSpeed != 0 && row.TransmitLinkSpeed != (ULONG64)-1)
            {
                wchar_t b[48];
                swprintf_s(b, L" | Speed: %llu Mbps", row.TransmitLinkSpeed / 1000000ULL);
                t += b;
            }
            t += L" | RX: " + FormatBytes(row.InOctets) + L" | TX: " + FormatBytes(row.OutOctets);
            return t;
        }

        bool LoadAdapters(std::vector<BYTE>& buf)
        {
            ULONG size = 16 * 1024;
            ULONG rc = ERROR_BUFFER_OVERFLOW;
            for (int i = 0; i < 4 && rc == ERROR_BUFFER_OVERFLOW; ++i)
            {
                buf.resize(size);
                rc = GetAdaptersAddresses(AF_UNSPEC,
                    GAA_FLAG_SKIP_ANYCAST | GAA_FLAG_SKIP_MULTICAST | GAA_FLAG_SKIP_DNS_SERVER,
                    nullptr, reinterpret_cast<PIP_ADAPTER_ADDRESSES>(buf.data()), &size);
            }
            return rc == NO_ERROR;
        }
    }

    bool CollectNetwork(std::vector<std::wstring>& out)
    {
        std::vector<BYTE> buf;
        if (!LoadAdapters(buf)) return false;

        for (PIP_ADAPTER_ADDRESSES a = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(buf.data()); a; a = a->Next)
        {
            if (a->IfType == IF_TYPE_SOFTWARE_LOOPBACK) continue;

            std::wstring line = a->FriendlyName ? a->FriendlyName : L"?";
            line += (a->OperStatus == IfOperStatusUp) ? L" | Up" : L" | Down";
            line += L" | MAC: " + MacText(a);
            line += L" | IPv4: " + Ipv4Text(a);
            line += TrafficText(a);
            line += L" | " + std::wstring(a->Description ? a->Description : L"");
            out.push_back(line);
        }
        return true;
    }
}