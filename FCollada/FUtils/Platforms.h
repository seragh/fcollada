/*
	Copyright (C) 2005-2007 Feeling Software Inc.
	Portions of the code are:
	Copyright (C) 2005-2007 Sony Computer Entertainment America

	MIT License: http://www.opensource.org/licenses/mit-license.php
*/
/*
	Based on the FS Import classes:
	Copyright (C) 2005-2006 Feeling Software Inc
	Copyright (C) 2005-2006 Autodesk Media Entertainment
	MIT License: http://www.opensource.org/licenses/mit-license.php
*/

#ifndef _FU_PLATFORMS_H_
#define _FU_PLATFORMS_H_

#include "fcollada_export.h"

#ifdef _MSC_VER
// Disable the "private member not available for export" warning,
// because I don't feel like writing interfaces
#pragma warning(disable:4251)

#pragma warning(disable:4702)
#endif // _MSC_VER

#ifdef __PPU__
#define UNICODE
#endif // __PPU__

// Ensure that both UNICODE and _UNICODE are set.
#ifdef UNICODE
#ifndef _UNICODE
#define _UNICODE
#endif
#else
#ifdef _UNICODE
#define UNICODE
#endif
#endif

#include <cmath>

#ifdef _WIN32
#ifndef _WIN32_WINNT		// Allow use of features specific to Windows XP or later.
#define _WIN32_WINNT 0x0501	// Change this to the appropriate value to target other versions of Windows.
#endif

#include <windows.h>
#include <stdio.h>
#include <wchar.h>

#elif __APPLE__

#include <ctype.h>
#include <wctype.h>
#include <unistd.h>
#include <string.h>
#include <wchar.h>
#include <stdint.h>

#else

#include <ctype.h>
#include <wctype.h>
#include <unistd.h>
#include <string.h>
#include <wchar.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#endif // _WIN32 __APPLE__

// Cross-platform type definitions
#ifdef _WIN32

typedef signed char int8;
typedef short int16;
typedef long int32;
typedef __int64 int64;
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
typedef unsigned __int64 uint64;

#else

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;
typedef uint8_t byte;

#ifndef _CLIMITS_
#include <climits>
#endif // _CLIMITS_

#endif // PLATFORMS

// Important functions that some OSes have missing!
#if defined(__PPU__)
#define glClearDepth glClearDepthf
#elif !defined(_WIN32)
inline char* strlower(char* str) { char* it = str; while (*it != 0) { *it = tolower(*it); ++it; } return str; }
inline wchar_t* wcslwr(wchar_t* str) { wchar_t* it = str; while (*it != 0) { *it = towlower(*it); ++it; } return str; }
inline int wcsicmp(const wchar_t* s1, const wchar_t* s2) { wchar_t c1 = *s1, c2 = *s2; while (c1 != 0 && c2 != 0) { if (c1 >= 'a' && c1 <= 'z') c1 -= 'a' + 'A'; if (c2 >= 'a' && c2 <= 'z') c2 -= 'a' + 'A'; if (c2 < c1) return -1; else if (c2 > c1) return 1; c1 = *(++s1); c2 = *(++s2); } return 0; }
#define _stricmp strcasecmp
#define _getcwd getcwd
#endif

// Cross-platform needed functions
#ifdef _WIN32

#if _MSC_VER < 1900
	#define vsnprintf _vsnprintf
	#define snprintf _snprintf
#endif

#define vsnwprintf _vsnwprintf

#if _MSC_VER >= 1400 //vc8.0 use new secure
	#define snwprintf _snwprintf_s
#else
	#define snwprintf _snwprintf
#endif // _MSC_VER

#define strlower _strlwr

#else // _WIN32

#define vsnwprintf vswprintf
#define snwprintf swprintf

#endif // _WIN32

// For Doxygen purposes, we stopped using the "using namespace std;" statement and use shortcuts instead.

// fstring and character definition
#ifdef UNICODE

	#define fchar wchar_t
	#define FC(a) L ## a

	#define fstrlen wcslen
	#define fstrcmp wcscmp
	#define fstrncpy wcsncpy
	#define fstrrchr wcsrchr
	#define fstrchr wcschr
	#define fsnprintf snwprintf
	#define fvsnprintf vsnwprintf
	#define fstrup _wcsupr

	#ifdef __PPU__
		#define fstricmp wcscmp		// [claforte] TODO: Implement __PPU__ version of wcsicmp
	#elif defined(_WIN32)
		#define fstricmp _wcsicmp
	#else
		#define fstricmp wcsicmp
	#endif // !__PPU__

	#ifdef _WIN32
		#define fstrlower _wcslwr
	#else
		#define fstrlower wcslwr
	#endif // _WIN32

	#ifdef _WIN32
		#define fchdir _tchdir
	#else // _WIN32
		#define fchdir(a) chdir(FUStringConversion::ToString(a).c_str())
	#endif // !_WIN32

#else // UNICODE

	typedef char fchar;
	#define FC(a) a

	#define fstrlen strlen
	#define fstrcmp strcmp
	#define fstricmp _stricmp
	#define fstrncpy strncpy
	#define fstrrchr strrchr
	#define fstrchr strchr
	#define fstrlower strlower
	#define fsnprintf snprintf
	#define fvsnprintf vsnprintf
	#define fstrup _strupr

	#define fchdir chdir

#endif // UNICODE

#ifndef _WIN32
#define MAX_PATH 1024
#endif // !_WIN32

#ifdef _WIN32
//#pragma warning(disable:4324) // Don't bother me about forcing the padding of aligned structure.
/** Alignment macro for classes and structures.
	Only supported in MSVS 2005 for now.
	@param byteCount The number of bytes to align to.*/
//#define ALIGN_STRUCT(byteCount) __declspec(align(byteCount))
#define ALIGN_STRUCT(byteCount)
#else // !_WIN32
#define ALIGN_STRUCT(byteCount)
#endif // _WIN32

#if defined(_WIN32) && _MSC_VER >= 1400
#define DEPRECATED(versionNumber, alternative) __declspec(deprecated("[" #versionNumber "] This function is now deprecated. Please use '" #alternative "' instead."))
#else
/** Deprecated macro for functions.
	Only supported in MSVS 2005 for now.
	@param versionNumber The version of FCollada that officially deprecated this function.
	@param alternative The function or class to use instead. */
#define DEPRECATED(versionNumber, alternative)
#endif // _WIN32 && MSVS2005

#endif // _FU_PLATFORMS_H_
