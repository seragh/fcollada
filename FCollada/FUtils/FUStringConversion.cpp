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

#include "StdAfx.h"

#include "FUStringConversion.h"
#include "FUStringConversion.hpp"

//
// FUStringConversion
//

// Convert a UTF-8 string to a fstring
#ifdef UNICODE
	fstring FUStringConversion::ToFString(const char* value)
	{
		FUStringBuilder builder;
		if (value != NULL)
		{
			uint32 length = (uint32) strlen(value);
			builder.reserve(length + 1);
			for (uint32 i = 0; i < length; ++i)
			{
				builder.append((fchar) value[i]);
			}
		}
		return builder.ToString();
	}
#else // UNICODE
	fstring FUStringConversion::ToFString(const char* value)
	{
		return fstring(value);
	}
#endif // UNICODE

// Convert a fstring string to a UTF-8 string
#ifdef UNICODE
	fm::string FUStringConversion::ToString(const fchar* value)
	{
		FUSStringBuilder builder;
		if (value != NULL)
		{
			uint32 length = (uint32) fstrlen(value);
			builder.reserve(length + 1);
			for (uint32 i = 0; i < length; ++i)
			{
				if (value[i] < 0xFF || (value[i] & (~0xFF)) >= 32) builder.append((char)value[i]);
				else builder.append('_'); // some generic enough character
			}
		}
		return builder.ToString();
	}
#else // UNICODE
	fm::string FUStringConversion::ToString(const fchar* value)
	{
		return fm::string(value);
	}
#endif // UNICODE

fm::string FUStringConversion::ToString(const FMMatrix44& m)
{
	FUSStringBuilder builder;
	ToString(builder, m);
	return builder.ToString();
}

fm::string FUStringConversion::ToString(const FUDateTime& dateTime)
{
	char sz[21];
	snprintf(sz, 21, "%04u-%02u-%02uT%02u:%02u:%02uZ", (unsigned int) dateTime.GetYear(), (unsigned int) dateTime.GetMonth(), (unsigned int) dateTime.GetDay(), (unsigned int) dateTime.GetHour(), (unsigned int) dateTime.GetMinutes(), (unsigned) dateTime.GetSeconds());
	sz[20] = 0;
    return fm::string(sz);
}

fstring FUStringConversion::ToFString(const FMMatrix44& m)
{
    FUStringBuilder builder;
	ToString(builder, m);
	return builder.ToString();
}

fstring FUStringConversion::ToFString(const FUDateTime& dateTime)
{
	fchar sz[21];
	fsnprintf(sz, 21, FC("%04u-%02u-%02uT%02u:%02u:%02uZ"), (unsigned int) dateTime.GetYear(), (unsigned int) dateTime.GetMonth(), (unsigned int) dateTime.GetDay(), (unsigned int) dateTime.GetHour(), (unsigned int) dateTime.GetMinutes(), (unsigned int) dateTime.GetSeconds());
	sz[20] = 0;
	return fstring(sz);
}

#ifdef HAS_VECTORTYPES

// Split a fstring into multiple substrings
void FUStringConversion::ToFStringList(const fstring& value, FStringList& array)
{
	const fchar* s = value.c_str();

	// Skip beginning white spaces
	fchar c;
	while ((c = *s) != 0 && (c == ' ' || c == '\t' || c == '\r' || c == '\n')) { ++s; }

	size_t index = 0;
	while (*s != 0)
	{
		const fchar* word = s;

		// Find next white space
		while ((c = *s) != 0 && c != ' ' && c != '\t' && c != '\r' && c != '\n') { ++s; }

		if (index < array.size()) array[index++].append(word, s - word);
		else { array.push_back(fstring(word, s - word)); ++index; }

		// Skip all white spaces
		while ((c = *s) != 0 && (c == ' ' || c == '\t' || c == '\r' || c == '\n')) { ++s; }
	}
	array.resize(index);
}

void FUStringConversion::ToStringList(const char* s, StringList& array)
{
	// Skip beginning white spaces
	char c;
	while ((c = *s) != 0 && (c == ' ' || c == '\t' || c == '\r' || c == '\n')) { ++s; }

	size_t index = 0;
	while (*s != 0)
	{
		const char* word = s;

		// Find next white space
		while ((c = *s) != 0 && c != ' ' && c != '\t' && c != '\r' && c != '\n') { ++s; }

		if (index < array.size()) array[index++].append(word, s - word);
		else { array.push_back(fm::string(word, s - word)); ++index; }

		// Skip all white spaces
		while ((c = *s) != 0 && (c == ' ' || c == '\t' || c == '\r' || c == '\n')) { ++s; }
	}
	array.resize(index);
}

#ifdef UNICODE
void FUStringConversion::ToStringList(const fchar* value, StringList& array)
{
	// Performance could be improved...
	ToStringList(ToString(value), array);
}
#endif // UNICODE

#endif // HAS_VECTORTYPES

// Convert a 2D point to a string
fm::string FUStringConversion::ToString(const FMVector2& p)
{
	FUSStringBuilder builder;
	ToString(builder, p);
	return builder.ToString();
}

fstring FUStringConversion::ToFString(const FMVector2& p)
{
	FUStringBuilder builder;
	ToString(builder, p);
	return builder.ToString();
}

// Convert a point to a string
fm::string FUStringConversion::ToString(const FMVector3& p)
{
	FUSStringBuilder builder;
	ToString(builder, p);
	return builder.ToString();
}

// Convert a vector4 to a string
fm::string FUStringConversion::ToString(const FMVector4& p)
{
	FUSStringBuilder builder;
	ToString(builder, p);
	return builder.ToString();
}

fstring FUStringConversion::ToFString(const FMVector4& p)
{
	FUStringBuilder builder;
	ToString(builder, p);
	return builder.ToString();
}


// Convert a point to a fstring
fstring FUStringConversion::ToFString(const FMVector3& p)
{
	FUStringBuilder builder;
	ToString(builder, p);
	return builder.ToString();
}

// Split the target string into its pointer and its qualifier(s)
void FUStringConversion::SplitTarget(const fm::string& target, fm::string& pointer, fm::string& qualifier)
{
	size_t splitIndex = target.find_first_of("([.");
	if (splitIndex != fm::string::npos)
	{
		pointer = target.substr(0, splitIndex);
		qualifier = target.substr(splitIndex);
	}
	else
	{
		pointer = target;
		qualifier.clear();
	}
}

int32 FUStringConversion::ParseQualifier(const char* qualifier)
{
	int32 returnValue = -1;
	const char* c = qualifier;
	while (*c == '(' || *c == '[')
	{
		const char* number = ++c;
		while (*c >= '0' && *c <= '9') ++c;
		if (*c == ')' || *c == ']')
		{
			returnValue = FUStringConversion::ToInt32(number);
//			qualifier.erase(0, c + 1 - qualifier.c_str());
			break;
		}
	}
	return returnValue;
}

template bool FUStringConversion::ToBoolean<char>(const char*);
template int32 FUStringConversion::ToInt32<char>(const char**);
template float FUStringConversion::ToFloat<char>(const char**);
template uint32 FUStringConversion::ToUInt32<char>(const char**);
template uint32 FUStringConversion::HexToUInt32<char>(const char**, uint32);
template void FUStringConversion::ToMatrix<char>(const char**, FMMatrix44&);
template void FUStringConversion::ToDateTime<char>(const char*, FUDateTime&);
template FMVector2 FUStringConversion::ToVector2<char>(const char**);
template FMVector3 FUStringConversion::ToVector3<char>(const char**);
template FMVector4 FUStringConversion::ToVector4<char>(const char**);
template void FUStringConversion::ToString<char>(FUStringBuilderT<char>&, const FMMatrix44&);
template void FUStringConversion::ToString<char>(FUStringBuilderT<char>&, const FMVector2&);
template void FUStringConversion::ToString<char>(FUStringBuilderT<char>&, const FMVector3&);
template void FUStringConversion::ToString<char>(FUStringBuilderT<char>&, const FMVector4&);
template size_t FUStringConversion::CountValues<char>(const char*);
template void FUStringConversion::ToBooleanList<char>(const char*, BooleanList&);
template void FUStringConversion::ToInt32List<char>(const char*, Int32List&);
template void FUStringConversion::ToUInt32List<char>(const char*, UInt32List&);
template void FUStringConversion::ToFloatList<char>(const char*, FloatList&);
template void FUStringConversion::ToInterleavedFloatList<char>(const char*, fm::pvector<FloatList>&);
template void FUStringConversion::ToInterleavedUInt32List<char>(const char*, fm::pvector<UInt32List>&);
template void FUStringConversion::ToMatrixList<char>(const char*, FMMatrix44List&);
template void FUStringConversion::ToVector2List<char>(const char*, FMVector2List&);
template void FUStringConversion::ToVector3List<char>(const char*, FMVector3List&);
template void FUStringConversion::ToVector4List<char>(const char*, FMVector4List&);
template void FUStringConversion::ToString<char>(FUStringBuilderT<char>&, const FloatList&);
template void FUStringConversion::ToString<char>(FUStringBuilderT<char>&, const Int32List&);
template void FUStringConversion::ToString<char>(FUStringBuilderT<char>&, const uint32*, size_t);

#ifdef UNICODE
template bool FUStringConversion::ToBoolean<fchar>(const fchar*);
template int32 FUStringConversion::ToInt32<fchar>(const fchar**);
template float FUStringConversion::ToFloat<fchar>(const fchar**);
template uint32 FUStringConversion::ToUInt32<fchar>(const fchar**);
template uint32 FUStringConversion::HexToUInt32<fchar>(const fchar**, uint32);
template void FUStringConversion::ToMatrix<fchar>(const fchar**, FMMatrix44&);
template void FUStringConversion::ToDateTime<fchar>(const fchar*, FUDateTime&);
template FMVector2 FUStringConversion::ToVector2<fchar>(const fchar**);
template FMVector3 FUStringConversion::ToVector3<fchar>(const fchar**);
template FMVector4 FUStringConversion::ToVector4<fchar>(const fchar**);
template void FUStringConversion::ToString<fchar>(FUStringBuilderT<fchar>&, const FMMatrix44&);
template void FUStringConversion::ToString<fchar>(FUStringBuilderT<fchar>&, const FMVector2&);
template void FUStringConversion::ToString<fchar>(FUStringBuilderT<fchar>&, const FMVector3&);
template void FUStringConversion::ToString<fchar>(FUStringBuilderT<fchar>&, const FMVector4&);
template size_t FUStringConversion::CountValues<fchar>(const fchar*);
template void FUStringConversion::ToBooleanList<fchar>(const fchar*, BooleanList&);
template void FUStringConversion::ToInt32List<fchar>(const fchar*, Int32List&);
template void FUStringConversion::ToUInt32List<fchar>(const fchar*, UInt32List&);
template void FUStringConversion::ToFloatList<fchar>(const fchar*, FloatList&);
template void FUStringConversion::ToInterleavedFloatList<fchar>(const fchar*, fm::pvector<FloatList>&);
template void FUStringConversion::ToInterleavedUInt32List<fchar>(const fchar*, fm::pvector<UInt32List>&);
template void FUStringConversion::ToMatrixList<fchar>(const fchar*, FMMatrix44List&);
template void FUStringConversion::ToVector2List<fchar>(const fchar*, FMVector2List&);
template void FUStringConversion::ToVector3List<fchar>(const fchar*, FMVector3List&);
template void FUStringConversion::ToVector4List<fchar>(const fchar*, FMVector4List&);
template void FUStringConversion::ToString<fchar>(FUStringBuilderT<fchar>&, const FloatList&);
template void FUStringConversion::ToString<fchar>(FUStringBuilderT<fchar>&, const Int32List&);
template void FUStringConversion::ToString<fchar>(FUStringBuilderT<fchar>&, const uint32*, size_t);
#endif // UNICODE
