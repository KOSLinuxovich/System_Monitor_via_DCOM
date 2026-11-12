# DCOM System Monitor

Учебный проект по компонентным системам. 
Это клиент-серверное приложение, которое использует технологию COM/DCOM для удаленного вызова методов. Сервер написан на C++, а клиенты: на C (с интерфейсом ncurses) и C#.

## Содержание

- [Что умеет проект](#что-умеет-проект)
- [Как это устроено](#как-это-устроено)
- [Структура проекта](#структура-проекта)
- [Как собрать](#как-собрать)
- [Зависимости](#зависимости)

## Что умеет проект

Программа собирает данные о системе. Сервер отдает загрузку процессора, информацию о памяти, свободном месте на диске и время работы. Клиенты могут сохранять эти данные в базу SQLite и просматривать историю. Все данные передаются через COM-интерфейсы.

В третьей версии функционал расширен. Теперь сервер умеет показывать список всех запущенных процессов с их PID, количеством потоков и потреблением памяти. Есть возможность сортировать этот список по разным парамертам и фильтровать по имени. Также можно посмотреть подробную информацию о конкретном процессе и даже завершить его, если хватит прав.

Кроме того, сервер собирает данные о сетевых адаптерах. Вы увидите MAC-адреса, IP, скорость соединения и объем переданного трафика. 

Есть функция экспорта отчета в текстовый файл и режим мониторинга в реальном времени, который обновляет данные каждую секунду.

## Как это устроено

Проект демонстрирует обратную совместимость интерфейсов. В сервере реализованы три версии контракта: `ISystemMonitor`, `ISystemMonitorV2` и `ISystemMonitorV3`. Каждая новая версия наследует предыдущую. Это значт, что старые клиенты продолжают работать без изменений.

Например, клиент на C из первой лабораторной использует только базовые методы (4 метода). Клиент на C# из второй лабы умеет работать с базой данных (6 методов). А финальный C# клиент использует все двенадцать методов третьей версии. При этом все они обращаются к одной и той же DLL.

Регистрация сервера происходит через реестр Windows. Для каждого класса прописан уникальный CLSID.

## Структура проекта

```text
dcom/
│   .gitignore
│   Makefile
│   README.md
│
├───client_c
│   ├───bin
│   │       client.exe
│   │       client_curses.exe
│   │       pdcurses.dll
│   │
│   └───src
│           client.c
│           client_curses.c
│
├───client_cs
│   │   client_cs.csproj
│   │   Program.cs
│   │
│   ├───Com
│   │       ComConnection.cs
│   │       ComInterfaces.cs
│   │
│   ├───Commands
│   │       CompatCommands.cs
│   │       NetworkCommands.cs
│   │       ProcessCommands.cs
│   │       StatsCommands.cs
│   │       SystemCommands.cs
│   │
│   ├───Processes
│   │       ProcessBrowser.cs
│   │       ProcessParser.cs
│   │       ProcRow.cs
│   │
│   └───UI
│           ConsoleUi.cs
│           Menu.cs
│
└───server
    ├───bin
    │       monitor.db
    │
    ├───include
    │       database.h
    │       factory.h
    │       globals.h
    │       guids.h
    │       interface.h
    │       monitor.h
    │       network.h
    │       process.h
    │       report.h
    │       system_info.h
    │       system_stats.h
    │       text_utils.h
    │       win_utils.h
    │
    ├───src
    │       database.cpp
    │       dllmain.cpp
    │       factory.cpp
    │       file_writer.cpp
    │       monitor.cpp
    │       monitor_v2.cpp
    │       monitor_v3.cpp
    │       network.cpp
    │       process_info.cpp
    │       process_kill.cpp
    │       process_list.cpp
    │       registration.cpp
    │       report_builder.cpp
    │       SystemMonitor.def
    │       system_info.cpp
    │       system_stats.cpp
    │       text_utils.cpp
    │       win_utils.cpp
    │
    └───third_party
        └───sqlite
                sqlite3.c
                sqlite3.h
``` 

## Как собрать

Все команды выполняются из корневой папки проекта в консоли x64 Native Tools Command Prompt for VS.

```cmd
nmake nmake_server          # Собрать сервер (SystemMonitor.dll)
nmake nmake_register        # Зарегистрировать сервер в реестре
nmake nmake_client_lab1     # Запустить клиент на C (Лаба 1, ncurses)
nmake nmake_client_lab2     # Запустить клиент на C# (Лаба 2, базовая версия)
nmake nmake_client_lab3     # Запустить клиент на C# (Лаба 3, полная версия)
nmake nmake_clean           # Очстить проект от скомпилированных файлов
```

## Зависимости

Для работы сервера нужен SQLite. Его исходники уже лежат в папке server/third_party/sqlite, поэтому отдельная установка не требуется. 
Для клиента на C требуется библиотека PDCurses, она уже лежит в папке client_c/bin.

Все необходимые библиотеки линкуются статически. 
Готовый файл SystemMonitor.dll не требует установки дополнительных компонентов на целевой машине. 
Для работы клиента на C# нужен установленный .NET SDK версии 8.0 или выше.