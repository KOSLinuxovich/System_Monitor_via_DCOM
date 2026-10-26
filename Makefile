SRC_DIR  = server\src
INC_DIR  = server\include
SQL_DIR  = server\third_party\sqlite
OBJ_DIR  = server\obj
BIN_DIR  = server\bin

DEF_FILE = $(SRC_DIR)\SystemMonitor.def
DLL_PATH = $(BIN_DIR)\SystemMonitor.dll
IMPLIB   = $(OBJ_DIR)\SystemMonitor.lib

CXXFLAGS = /nologo /c /EHsc /O2 /W3 /MT /I$(INC_DIR) /I$(SQL_DIR)
CFLAGS   = /nologo /c /O2 /w /MT /I$(SQL_DIR)
SYSLIBS  = ole32.lib oleaut32.lib advapi32.lib

CPP_OBJS = $(OBJ_DIR)\dllmain.obj \
           $(OBJ_DIR)\registration.obj \
           $(OBJ_DIR)\factory.obj \
           $(OBJ_DIR)\monitor.obj \
           $(OBJ_DIR)\monitor_v2.obj \
           $(OBJ_DIR)\monitor_v3.obj \
           $(OBJ_DIR)\database.obj \
           $(OBJ_DIR)\file_writer.obj \
           $(OBJ_DIR)\network.obj \
           $(OBJ_DIR)\process_info.obj \
           $(OBJ_DIR)\process_kill.obj \
           $(OBJ_DIR)\process_list.obj \
           $(OBJ_DIR)\report_builder.obj \
           $(OBJ_DIR)\system_info.obj \
           $(OBJ_DIR)\system_stats.obj \
           $(OBJ_DIR)\text_utils.obj \
           $(OBJ_DIR)\win_utils.obj

SQLITE_OBJ = $(OBJ_DIR)\sqlite3.obj

HEADERS = $(INC_DIR)\globals.h \
          $(INC_DIR)\guids.h \
          $(INC_DIR)\interface.h \
          $(INC_DIR)\text_utils.h \
          $(INC_DIR)\win_utils.h \
          $(INC_DIR)\system_stats.h \
          $(INC_DIR)\database.h \
          $(INC_DIR)\process.h \
          $(INC_DIR)\network.h \
          $(INC_DIR)\system_info.h \
          $(INC_DIR)\report.h \
          $(INC_DIR)\monitor.h \
          $(INC_DIR)\factory.h

all: nmake_server

{$(SRC_DIR)}.cpp{$(OBJ_DIR)}.obj:
    cl $(CXXFLAGS) /Fo$@ $<

{$(SQL_DIR)}.c{$(OBJ_DIR)}.obj:
    cl $(CFLAGS) /Fo$@ $<

nmake_server: dirs $(DLL_PATH)

dirs:
    @if not exist $(OBJ_DIR) mkdir $(OBJ_DIR)
    @if not exist $(BIN_DIR) mkdir $(BIN_DIR)

$(CPP_OBJS): $(HEADERS)

$(DLL_PATH): $(CPP_OBJS) $(SQLITE_OBJ) $(DEF_FILE)
    link /nologo /DLL /OUT:$@ /IMPLIB:$(IMPLIB) /DEF:$(DEF_FILE) $(CPP_OBJS) $(SQLITE_OBJ) $(SYSLIBS)

nmake_register: nmake_server
    @fltmc >nul 2>&1 || (echo ERROR: run this terminal as Administrator & exit /b 1)
    C:\Windows\System32\regsvr32.exe /s /u $(MAKEDIR)\$(DLL_PATH)
    C:\Windows\System32\regsvr32.exe /s $(MAKEDIR)\$(DLL_PATH)
    @echo Registered: $(MAKEDIR)\$(DLL_PATH)

nmake_unregister:
    @fltmc >nul 2>&1 || (echo ERROR: run this terminal as Administrator & exit /b 1)
    C:\Windows\System32\regsvr32.exe /s /u $(MAKEDIR)\$(DLL_PATH)

nmake_client_build:
    dotnet build client_cs -c Debug

nmake_client_lab1:
    cd client_c && bin\client_curses.exe

nmake_client_lab1_plain:
    cd client_c && bin\client.exe

nmake_client_lab2: nmake_client_lab3

nmake_client_lab3:
    dotnet run --project client_cs

nmake_rebuild:
    $(MAKE) /nologo nmake_clean
    $(MAKE) /nologo nmake_register

nmake_clean:
    -rmdir /s /q $(OBJ_DIR)
    -del /q $(BIN_DIR)\SystemMonitor.dll
    -del /q $(BIN_DIR)\SystemMonitor.exp
    -del /q $(BIN_DIR)\SystemMonitor.lib
    -del /q $(BIN_DIR)\*.obj
    -rmdir /s /q client_cs\bin
    -rmdir /s /q client_cs\obj