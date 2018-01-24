#include <roa_lib/debug.h>
#include <windows.h>
#include <DbgHelp.h>


#pragma comment(lib, "Dbghelp.lib")


void
roa_debug_symbol_name(void *symbol, char *buffer2, unsigned buf_size)
{
  #ifdef _WIN32
  DWORD  error;
  HANDLE hProcess;

  hProcess = GetCurrentProcess();

  SymSetOptions(SYMOPT_UNDNAME | SYMOPT_DEFERRED_LOADS);


  if (!SymInitialize(hProcess, "C:/Users/SimStim/Developer/mono/output/development/", TRUE))
  {
    // SymInitialize failed
    error = GetLastError();
    printf("SymInitialize returned error : %d\n", error);
  }


  DWORD64  dwDisplacement = 0;
  DWORD64  dwAddress = symbol;
  IMAGEHLP_LINE64 line;

  char buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
  PSYMBOL_INFO pSymbol = (PSYMBOL_INFO)buffer;
  memset(pSymbol, 0, sizeof(buffer));

  pSymbol->SizeOfStruct = sizeof(SYMBOL_INFO);
  pSymbol->MaxNameLen = MAX_SYM_NAME;

  if (SymFromAddr(hProcess, dwAddress, &dwDisplacement, pSymbol))
  {
    // SymFromAddr returned success
    int i = 0;
  }
  else
  {
    // SymFromAddr failed
    DWORD error = GetLastError();
    printf("SymFromAddr returned error : %d\n", error);
  }
  #endif
}