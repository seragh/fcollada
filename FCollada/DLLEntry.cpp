/*
	Copyright (C) 2005-2007 Feeling Software Inc.
	Portions of the code are:
	Copyright (C) 2005-2007 Sony Computer Entertainment America

	MIT License: http://www.opensource.org/licenses/mit-license.php
*/

#include "StdAfx.h"

#ifdef FCOLLADA_DLL
#ifdef WIN32
HINSTANCE hInstance = NULL;

BOOL WINAPI DllMain(HINSTANCE _hInstance, ULONG fdwReason, LPVOID UNUSED(lpvReserved))
{
	static int initCount = 0;
	switch (fdwReason)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
		// Initialize and Release are ref-counted.
		if (initCount == 0)
		{
			//FCollada::Initialize();
		}
		initCount++;
		break;

	case DLL_PROCESS_DETACH:
	case DLL_THREAD_DETACH:
		// Initialize and Release are ref-counted.
		--initCount;
		if (initCount == 0)
		{
			//FCollada::Release();
		}
		break;

	case DLL_PROCESS_VERIFIER:
	default:
		break;
	}
	hInstance = _hInstance;
	return TRUE;
}
#elif defined(__APPLE__) || defined(LINUX)
void __attribute((constructor)) DllEntry(void)
{
}

void __attribute((destructor)) DllTerminate(void)
{
}
#endif // WIN32

#endif // FCOLLADA_DLL
