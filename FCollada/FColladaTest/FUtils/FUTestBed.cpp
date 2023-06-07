/*
	Copyright (C) 2005-2007 Feeling Software Inc.
	Portions of the code are:
	Copyright (C) 2005-2007 Sony Computer Entertainment America

	MIT License: http://www.opensource.org/licenses/mit-license.php
*/

#include "StdAfx.h"
#include "FUTestBed.h"

bool FUTestBed_skipAsserts = false;

//
// FUTestbed
//

FUTestBed::FUTestBed(const fchar* _filename, bool _isVerbose)
:	testPassed(0), testFailed(0)
,	fileOut(_filename), filename(_filename), isVerbose(_isVerbose)
{
}

bool FUTestBed::RunTestbed(FUTestSuite* headTestSuite)
{
	if (headTestSuite == NULL) return false;

	testPassed = testFailed = 0;

	RunTestSuite(headTestSuite);

	fileOut.WriteLine("---------------------------------");
	fileOut.WriteLine("Tests passed: %zu.", testPassed);
	fileOut.WriteLine("Tests failed: %zu.", testFailed);
	fileOut.WriteLine("");
	fileOut.WriteLine("Testbed score: [%zu/%zu]", testPassed, testFailed + testPassed);
	fileOut.WriteLine("---------------------------------");

	return true;
}

void FUTestBed::RunTestSuite(FUTestSuite* testSuite)
{
	if (testSuite == NULL) return;

	bool testSuiteDone = false;
	testSuite->RunTest(*this, fileOut, testSuiteDone, (size_t) ~0);
	for (size_t j = 0; !testSuiteDone; ++j)
	{
		bool passed = testSuite->RunTest(*this, fileOut, testSuiteDone, j);
		if (testSuiteDone) break;
		if (passed) testPassed++;
		else testFailed++;
	}
}
