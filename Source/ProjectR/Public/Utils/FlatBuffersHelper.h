// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "flatbuffers/flatbuffers.h"
#include "ProjectR.h"

/**
 * 
 */
class PROJECTR_API FFlatBuffersHelper
{
public:
	// Serialize
	static bool UStructToFlatbuffer(const UStruct* StructDefinition, const void* Struct, TArray<uint8>& OutBuf, int64 CheckFlags = 0, int64 SkipFlags = 0);

	// Deserialize
	static bool FlatBufferToUStruct(const uint8 *Buf, size_t BufSize, const UStruct *StructDefinition, void *OutStruct, int64 CheckFlags = 0, int64 SkipFlags = 0);


private:
	class FPropertyVisitor
	{
	public:
		virtual bool Visit(int Index, UProperty *Property, FString VariableName, const void* Value, int64 CheckFlags, int64 SkipFlags) = 0;
	};

	static bool IterateUProperties(const UStruct* StructDefinition, const void* Struct, int64 CheckFlags /*= 0*/, int64 SkipFlags /*= 0*/, FPropertyVisitor *Visitor);
	static bool IterateArrayElements(const UArrayProperty* ArrayProperty, const void* Array, int64 CheckFlags /*= 0*/, int64 SkipFlags /*= 0*/, FPropertyVisitor *Visitor);
	static size_t ElemSize(const UProperty *Property);

	static bool SerializeUStruct(flatbuffers::FlatBufferBuilder& Builder, flatbuffers::uoffset_t &OutOffset, const UStruct* StructDefinition, const void* Struct, int64 CheckFlags = 0, int64 SkipFlags = 0);

	static bool SerializeArray(flatbuffers::FlatBufferBuilder& Builder, flatbuffers::uoffset_t &OutOffset, const UArrayProperty* ArrayProperty, const FScriptArray* Array, int64 CheckFlags /*= 0*/, int64 SkipFlags /*= 0*/);

	static bool SerializeNonEmbeddedProperty(flatbuffers::FlatBufferBuilder& Builder, flatbuffers::uoffset_t &OutOffset, UProperty* Property, const void* Value, int64 CheckFlags /*= 0*/, int64 SkipFlags /*= 0*/);

	static bool DeserializeProperty(const uint8 *Buf, const UProperty *Property, void *OutValue, int64 CheckFlags, int64 SkipFlags);
	static bool DeserializeUStruct(const flatbuffers::Table *Table, const UStruct *StructDefinition, void *OutStruct, int64 CheckFlags, int64 SkipFlags);
	static bool DeserializeArray(const uint8 *Buf, const UArrayProperty *ArrayProperty, void *OutValue, int64 CheckFlags, int64 SkipFlags);
};
