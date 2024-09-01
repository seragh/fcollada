/*
	Copyright (C) 2005-2007 Feeling Software Inc.
	Portions of the code are:
	Copyright (C) 2005-2007 Sony Computer Entertainment America

	MIT License: http://www.opensource.org/licenses/mit-license.php
*/

#include "FCollada.h"
#include "FAXColladaParser.h"
#include "FAXColladaWriter.h"
#include "FUtils/FUXmlDocument.h"
#include "FUtils/FUFileManager.h"
#include "FUtils/FUUniqueStringMap.h"
using namespace FUDaeParser;
using namespace FUDaeWriter;
#include "FAXStructures.h"
#include "FArchiveXML.h"
#include "FCDocument/FCDExtra.h"
#include "FCDocument/FCDForceTyped.h"
#include "FCDocument/FCDForceDeflector.h"
#include "FCDocument/FCDForceDrag.h"
#include "FCDocument/FCDForceGravity.h"
#include "FCDocument/FCDForcePBomb.h"
#include "FCDocument/FCDForceWind.h"
#include "FCDocument/FCDForceField.h"


xmlNode* FArchiveXML::WriteForceField(FCDObject* object, xmlNode* parentNode)
{
	FCDForceField* forceField = (FCDForceField*)object;

	xmlNode* forceFieldNode = FArchiveXML::WriteToEntityXMLFCDEntity(forceField, parentNode, DAE_FORCE_FIELD_ELEMENT);


	if (forceField->GetInformation() != nullptr)
	{
		FArchiveXML::WriteTechniquesFCDExtra(forceField->GetInformation(), forceFieldNode);
	}

	FArchiveXML::WriteEntityExtra(forceField, forceFieldNode);
	return forceFieldNode;
}
