/*
	Copyright (C) 2005-2007 Feeling Software Inc.
	Portions of the code are:
	Copyright (C) 2005-2007 Sony Computer Entertainment America

	MIT License: http://www.opensource.org/licenses/mit-license.php
*/

#include "StdAfx.h"
#include "FUAssert.h"

#include <fmt/format.h>

#ifndef WIN32
#  include <signal.h>
#endif
#include <string>


static FUAssertion::FUAssertCallback* curAssertCallback = NULL;

void FUAssertion::SetAssertionFailedCallback(FUAssertCallback* assertionCallback)
{
	curAssertCallback = assertionCallback;
}

bool FUAssertion::OnAssertionFailed(const char* file, uint32 line)
{
	std::string message = fmt::format("[{}@{}] Assertion failed.\n"
			"Abort: Enter debugger.\n"
			"Retry: Continue execution.\n"
			"Ignore: Do not assert at this line for the duration of the application.",
			file,
			line);

	if (curAssertCallback != NULL)
		return (*curAssertCallback)(message.c_str());
#ifdef _DEBUG
	else
	{
#  ifdef WIN32
		int32 buttonPressed = MessageBoxA(NULL,
				message.c_str(),
				"Assertion failed.",
				MB_ABORTRETRYIGNORE | MB_ICONWARNING);
		if (buttonPressed == IDABORT)
		{
			__debugbreak();
		}
		else if (buttonPressed == IDRETRY)
		{
		}
		else if (buttonPressed == IDIGNORE)
		{
			return true;
		}
#  else  // WIN32
		raise(SIGTRAP);
#  endif // WIN32
		return false;
	}
#else  // _DEBUG
	return false;
#endif // _DEBUG
}
