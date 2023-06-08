/*
	Copyright (C) 2005-2007 Feeling Software Inc.
	Portions of the code are:
	Copyright (C) 2005-2007 Sony Computer Entertainment America

	MIT License: http://www.opensource.org/licenses/mit-license.php
*/

#include "StdAfx.h"

#include "FCDEffectParameter.h"
#include "FCDEffectParameter.hpp"

#include "FCDEffectParameterFactory.h"
#include "FCDEffectPass.h"
#include "FCDEffectProfile.h"
#include "FCDEffectTechnique.h"
#include "FCDImage.h"
#include "FCDocument.h"

//
// FCDEffectParameter
//

ImplementObjectType(FCDEffectParameter)
ImplementParameterObjectNoArg(FCDEffectParameter, FCDEffectParameterAnnotation, annotations)

FCDEffectParameter::FCDEffectParameter(FCDocument* document)
:	FCDObject(document)
,	InitializeParameter(paramType, FCDEffectParameter::GENERATOR)
,	InitializeParameterNoArg(reference)
,	InitializeParameterNoArg(semantic)
,	InitializeParameterNoArg(annotations)
{
}

FCDEffectParameter::~FCDEffectParameter()
{
}

void FCDEffectParameter::SetReference(const char* _reference)
{
	reference = FCDObjectWithId::CleanSubId(_reference);
	SetDirtyFlag();
}

FCDEffectParameterAnnotation* FCDEffectParameter::AddAnnotation()
{
	FCDEffectParameterAnnotation* annotation = new FCDEffectParameterAnnotation();
	annotations.push_back(annotation);
	SetNewChildFlag();
	return annotation;
}

void FCDEffectParameter::AddAnnotation(const fchar* name, FCDEffectParameter::Type type, const fchar* value)
{
	FCDEffectParameterAnnotation* annotation = AddAnnotation();
	annotation->name = name;
	annotation->type = type;
	annotation->value = value;
	SetNewChildFlag();
}

bool FCDEffectParameter::IsValueEqual(FCDEffectParameter* parameter)
{
	return (parameter != NULL && this->GetType() == parameter->GetType());
}

// Clones the base parameter values
FCDEffectParameter* FCDEffectParameter::Clone(FCDEffectParameter* clone) const
{
	if (clone == NULL)
	{
		// Recursively call the cloning function in an attempt to clone the up-class parameters
		clone = FCDEffectParameterFactory::Create(const_cast<FCDocument*>(GetDocument()), GetType());
		return clone != NULL ? Clone(clone) : NULL;
	}
	else
	{
		clone->reference = reference;
		clone->semantic = semantic;
		clone->paramType = paramType;
		clone->annotations.reserve(annotations.size());
		for (const FCDEffectParameterAnnotation** itA = annotations.begin(); itA != annotations.end(); ++itA)
		{
			clone->AddAnnotation(*(*itA)->name, (FCDEffectParameter::Type) *(*itA)->type, *(*itA)->value);
		}
		return clone;
	}
}

// Flattening: overwrite the target parameter with this parameter
void FCDEffectParameter::Overwrite(FCDEffectParameter* UNUSED(target))
{
	// Do nothing on the base class, only values and animations should be overwritten
}

//
// FCDEffectParameterAnnotation
//

ImplementObjectType(FCDEffectParameterAnnotation)

FCDEffectParameterAnnotation::FCDEffectParameterAnnotation()
:	FUParameterizable()
,	InitializeParameterNoArg(name)
,	InitializeParameter(type, FCDEffectParameter::STRING)
,	InitializeParameterNoArg(value)
{
}

FCDEffectParameterAnnotation::~FCDEffectParameterAnnotation()
{
}

//
// FCDEffectParameterT
//

ImplementObjectTypeT(FCDEffectParameterInt)
ImplementObjectTypeT(FCDEffectParameterString)
ImplementObjectTypeT(FCDEffectParameterBool)

template <> FCDEffectParameter::Type FCDEffectParameterInt::GetType() const { return FCDEffectParameter::INTEGER; }
template <> FCDEffectParameter::Type FCDEffectParameterString::GetType() const { return FCDEffectParameter::STRING; }
template <> FCDEffectParameter::Type FCDEffectParameterBool::GetType() const { return FCDEffectParameter::BOOLEAN; }

template <> FCDEffectParameterInt::FCDEffectParameterT(FCDocument* document)
:	FCDEffectParameter(document), InitializeParameter(value, 0) {}
template <> FCDEffectParameterString::FCDEffectParameterT(FCDocument* document)
:	FCDEffectParameter(document), InitializeParameterNoArg(value) {}
template <> FCDEffectParameterBool::FCDEffectParameterT(FCDocument* document)
:	FCDEffectParameter(document), InitializeParameter(value, false) {}

//
// FCDEffectParameterAnimatableT
//

ImplementObjectTypeT(FCDEffectParameterFloat)
ImplementObjectTypeT(FCDEffectParameterFloat2)
ImplementObjectTypeT(FCDEffectParameterFloat3)
ImplementObjectTypeT(FCDEffectParameterColor3)
ImplementObjectTypeT(FCDEffectParameterVector)
ImplementObjectTypeT(FCDEffectParameterColor4)
ImplementObjectTypeT(FCDEffectParameterMatrix)

template <> FCDEffectParameter::Type FCDEffectParameterFloat::GetType() const { return FCDEffectParameter::FLOAT; }
template <> FCDEffectParameter::Type FCDEffectParameterFloat2::GetType() const { return FCDEffectParameter::FLOAT2; }
template <> FCDEffectParameter::Type FCDEffectParameterFloat3::GetType() const { return FCDEffectParameter::FLOAT3; }
template <> FCDEffectParameter::Type FCDEffectParameterColor3::GetType() const { return FCDEffectParameter::FLOAT3; }
template <> FCDEffectParameter::Type FCDEffectParameterVector::GetType() const { return FCDEffectParameter::VECTOR; }
template <> FCDEffectParameter::Type FCDEffectParameterColor4::GetType() const { return FCDEffectParameter::VECTOR; }
template <> FCDEffectParameter::Type FCDEffectParameterMatrix::GetType() const { return FCDEffectParameter::MATRIX; }

template <> FCDEffectParameterFloat::FCDEffectParameterAnimatableT(FCDocument* document)
:	FCDEffectParameter(document), floatType(FLOAT),	InitializeParameterAnimatable(value, 0.0f) {}
template <> FCDEffectParameterFloat2::FCDEffectParameterAnimatableT(FCDocument* document)
:	FCDEffectParameter(document), floatType(FLOAT),	InitializeParameterAnimatable(value, FMVector2::Zero) {}
template <> FCDEffectParameterFloat3::FCDEffectParameterAnimatableT(FCDocument* document)
:	FCDEffectParameter(document), floatType(FLOAT),	InitializeParameterAnimatable(value, FMVector3::Zero) {}
template <> FCDEffectParameterColor3::FCDEffectParameterAnimatableT(FCDocument* document)
:	FCDEffectParameter(document), floatType(FLOAT),	InitializeParameterAnimatable(value, FMVector3::One) {}
template <> FCDEffectParameterVector::FCDEffectParameterAnimatableT(FCDocument* document)
:	FCDEffectParameter(document), floatType(FLOAT),	InitializeParameterAnimatable(value, FMVector4::Zero) {}
template <> FCDEffectParameterColor4::FCDEffectParameterAnimatableT(FCDocument* document)
:	FCDEffectParameter(document), floatType(FLOAT),	InitializeParameterAnimatable(value, FMVector4::One) {}
template <> FCDEffectParameterMatrix::FCDEffectParameterAnimatableT(FCDocument* document)
:	FCDEffectParameter(document), floatType(FLOAT),	InitializeParameterAnimatable(value, FMMatrix44::Identity) {}


//
// Explicit instanciation
//
template class FCDEffectParameterT<int32>;
template class FCDEffectParameterT<bool>;
template class FCDEffectParameterT<fm::string>;

template class FCDEffectParameterAnimatableT<float, FUParameterQualifiers::SIMPLE>;
template class FCDEffectParameterAnimatableT<FMVector2, FUParameterQualifiers::SIMPLE>;
template class FCDEffectParameterAnimatableT<FMVector3, FUParameterQualifiers::VECTOR>;
template class FCDEffectParameterAnimatableT<FMVector3, FUParameterQualifiers::COLOR>;
template class FCDEffectParameterAnimatableT<FMMatrix44, FUParameterQualifiers::SIMPLE>;
