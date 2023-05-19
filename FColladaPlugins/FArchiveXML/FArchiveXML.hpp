/*
	Copyright (C) 2005-2007 Feeling Software Inc.
	Portions of the code are:
	Copyright (C) 2005-2007 Sony Computer Entertainment America

	MIT License: http://www.opensource.org/licenses/mit-license.php
*/

template <class TYPE, int QUAL>
FCOLLADA_EXPORT xmlNode* FArchiveXML::AddPhysicsParameter(xmlNode* parentNode, const char* name, FCDParameterAnimatableT<TYPE,QUAL>& value)
{
	xmlNode* paramNode = AddChild(parentNode, name);
	AddContent(paramNode, FUStringConversion::ToString((TYPE&) value));
	if (value.IsAnimated())
	{
		const FCDAnimated* animated = value.GetAnimated();
		FArchiveXML::WriteAnimatedValue(animated, paramNode, name);
	}
	return paramNode;
}
