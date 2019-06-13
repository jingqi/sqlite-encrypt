
#ifndef ___HEADFILE_C12C0134_E5B6_443E_A214_29C920C5A616_
#define ___HEADFILE_C12C0134_E5B6_443E_A214_29C920C5A616_

#include <nut/platform/platform.h>


#if defined(BUILDING_SQLITECPP)
#   define SQLITE_API    DLL_EXPORT
#   define SQLITECPP_API DLL_EXPORT
#else
#   define SQLITE_API    DLL_IMPORT
#   define SQLITECPP_API DLL_IMPORT
#endif

// 开启 sqlite3 加密接口
#define SQLITE_HAS_CODEC 1

#endif
