#include "database.h"
#include "text_utils.h"
#include "win_utils.h"
#include "sqlite3.h"
#include <cstdio>

namespace sm
{
    namespace
    {
        SRWLOCK g_dbLock = SRWLOCK_INIT;

        struct SrwGuard
        {
            SRWLOCK* l;
            explicit SrwGuard(SRWLOCK* lock) : l(lock) { AcquireSRWLockExclusive(l); }
            ~SrwGuard() { ReleaseSRWLockExclusive(l); }
        };

        struct Db
        {
            sqlite3* h = nullptr;

            ~Db()
            {
                if (h) sqlite3_close(h);
            }

            bool Open()
            {
                std::string path = WideToUtf8(ModuleDirectory() + L"monitor.db");
                if (sqlite3_open_v2(path.c_str(), &h,
                    SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE | SQLITE_OPEN_FULLMUTEX, nullptr) != SQLITE_OK)
                    return false;
                sqlite3_busy_timeout(h, 3000);
                return sqlite3_exec(h,
                    "CREATE TABLE IF NOT EXISTS history ("
                    "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                    "timestamp TEXT NOT NULL, cpu REAL NOT NULL, mem TEXT NOT NULL);",
                    nullptr, nullptr, nullptr) == SQLITE_OK;
            }
        };

        struct Stmt
        {
            sqlite3_stmt* s = nullptr;

            ~Stmt()
            {
                if (s) sqlite3_finalize(s);
            }
        };
    }

    bool InsertSnapshot(double cpu, const std::wstring& mem)
    {
        SrwGuard lock(&g_dbLock);

        Db db;
        if (!db.Open()) return false;

        Stmt st;
        if (sqlite3_prepare_v2(db.h,
            "INSERT INTO history (timestamp, cpu, mem) VALUES (datetime('now','localtime'), ?, ?);",
            -1, &st.s, nullptr) != SQLITE_OK)
            return false;

        std::string memUtf8 = WideToUtf8(mem);
        sqlite3_bind_double(st.s, 1, cpu);
        sqlite3_bind_text(st.s, 2, memUtf8.c_str(), (int)memUtf8.size(), SQLITE_TRANSIENT);
        return sqlite3_step(st.s) == SQLITE_DONE;
    }

    bool QueryHistory(int count, std::string& out)
    {
        out.clear();
        SrwGuard lock(&g_dbLock);

        Db db;
        if (!db.Open()) return false;

        Stmt st;
        if (sqlite3_prepare_v2(db.h,
            "SELECT timestamp, cpu, mem FROM history ORDER BY id DESC LIMIT ?;",
            -1, &st.s, nullptr) != SQLITE_OK)
            return false;

        sqlite3_bind_int(st.s, 1, count);

        int rc;
        while ((rc = sqlite3_step(st.s)) == SQLITE_ROW)
        {
            const char* ts  = (const char*)sqlite3_column_text(st.s, 0);
            double      cpu = sqlite3_column_double(st.s, 1);
            const char* mem = (const char*)sqlite3_column_text(st.s, 2);

            char cpuBuf[32];
            snprintf(cpuBuf, sizeof(cpuBuf), "%.2f", cpu);

            if (!out.empty()) out += "\n";
            out += ts ? ts : "?";
            out += " | CPU: ";
            out += cpuBuf;
            out += "% | ";
            out += mem ? mem : "";
        }
        return rc == SQLITE_DONE;
    }
}