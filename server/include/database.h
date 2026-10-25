#ifndef DATABASE_H
#define DATABASE_H

#include <string>

namespace sm
{
    const int kMaxHistory = 1000;

    bool InsertSnapshot(double cpu, const std::wstring& mem);
    bool QueryHistory(int count, std::string& out);
}

#endif