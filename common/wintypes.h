
/*defined windows data types*/
typedef void * HINSTANCE;
typedef void * HMODULE;
typedef void * FARPROC;
typedef const char* LPCSTR;

typedef unsigned char byte;
typedef int BOOL;

typedef unsigned long DWORD;
typedef unsigned short WORD;

typedef char CHAR;
typedef CHAR *LPSTR;
typedef unsigned char BYTE;
typedef long LONG;

typedef void *LPVOID;
typedef void *PVOID;
typedef PVOID HANDLE;
typedef HANDLE HWND;

typedef struct tagPOINT {
  int x;
  int y;
} POINT, *PPOINT;

typedef unsigned int UINT;
typedef unsigned int UINT_PTR;
typedef long LONG_PTR;
typedef UINT_PTR WPARAM;
typedef LONG_PTR LPARAM;

#define TRUE 1
#define FALSE 0

#define BI_RGB 0 //

#define _stdcall
#define __cdecl
#define _cdecl
#define _inline static inline
#define CL_DLLEXPORT

#define MAX_PATHLEN 256

#define MAKEWORD(a,b)   ((WORD)(((BYTE)(a))|(((WORD)((BYTE)(b)))<<8)))

//warnings disable :)

// It's a bad practice.
//  Somewhen it must go to the Tartar
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Winvalid-offsetof"
#pragma GCC diagnostic ignored "-Wconversion-null"
#pragma GCC diagnostic ignored "-Wswitch"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wsign-compare"
#pragma GCC diagnostic ignored "-Wint-to-pointer-cast"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wuninitialized"
#pragma GCC diagnostic ignored "-Wparentheses"
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
#pragma GCC diagnostic ignored "-Wformat-extra-args"
#pragma GCC diagnostic ignored "-Wformat"
#pragma GCC diagnostic ignored "-Waddress"
#pragma GCC diagnostic ignored "-Wunused-value"
#pragma GCC diagnostic ignored "-Wtype-limits"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wunused-function"


















