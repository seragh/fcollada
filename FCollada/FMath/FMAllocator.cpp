/*
	Copyright (C) 2005-2007 Feeling Software Inc.
	Portions of the code are:
	Copyright (C) 2005-2007 Sony Computer Entertainment America

	MIT License: http://www.opensource.org/licenses/mit-license.php
*/

#include "StdAfx.h"
#include "FMAllocator.h"

#include <cstdlib>

namespace fm
{
	AllocateFunc af;
	FreeFunc ff;

	void SetAllocationFunctions(AllocateFunc a, FreeFunc f)
	{
		af = a;
		ff = f;
	}

	// These two are simple enough, but have the advantage of
	// always allocating/releasing memory from the same heap.
	void* Allocate(size_t byteCount)
	{
		if (!af) af = std::malloc;
		return (*af)(byteCount);
	}

	void Release(void* buffer)
	{
		if (!ff) ff = std::free;
		(*ff)(buffer);
	}
}
