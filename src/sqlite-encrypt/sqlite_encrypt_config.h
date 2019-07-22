
#ifndef ___HEADFILE_E8BDD496_685E_49CB_A8ED_91263CF6E7D6_
#define ___HEADFILE_E8BDD496_685E_49CB_A8ED_91263CF6E7D6_

#include <nut/platform/platform.h>


#if defined(BUILDING_SQLITE_ENCRYPT)
#   define SQLITE_API DLL_EXPORT
#else
#   define SQLITE_API DLL_IMPORT
#endif

// 开启 sqlite3 加密接口
#define SQLITE_HAS_CODEC 1

#endif
