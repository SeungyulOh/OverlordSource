// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "FlatBuffersHelper.h"


#define BUILDER_INITIAL_SIZE 1024

bool FFlatBuffersHelper::UStructToFlatbuffer(const UStruct* StructDefinition, const void* Struct, TArray<uint8>& OutBuf, int64 CheckFlags /*= 0*/, int64 SkipFlags /*= 0*/)
{
	OutBuf.Empty();

	flatbuffers::FlatBufferBuilder Builder(BUILDER_INITIAL_SIZE);

	// Need this because I don't know how to get the init value of a property of a UStruct
	Builder.ForceDefaults(true);

	flatbuffers::uoffset_t Offset;

	if (!SerializeUStruct(Builder, Offset, StructDefinition, Struct, CheckFlags, SkipFlags)) {
		UE_LOG(LogFlatBuffersHelper, Log, TEXT("Failed to serialize ustruct '%s'"), *StructDefinition->GetName());
		return false;
	}

	Builder.Finish<void>(Offset);

	// copy, inefficient
	OutBuf.Append(Builder.GetBufferPointer(), Builder.GetSize());
	
	return true;
}

bool FFlatBuffersHelper::SerializeNonEmbeddedProperty(flatbuffers::FlatBufferBuilder& Builder, flatbuffers::uoffset_t &OutOffset, UProperty* Property, const void* Value, int64 CheckFlags /*= 0*/, int64 SkipFlags /*= 0*/)
{

	if (UStrProperty *StringProperty = Cast<UStrProperty>(Property))
	{
		// string case
		auto o = Builder.CreateString((const char *)TCHAR_TO_UTF8(*StringProperty->GetPropertyValue(Value))).o;
		OutOffset = o;
	}
	else if (UTextProperty *TextProperty = Cast<UTextProperty>(Property))
	{
		// string case
		auto o = Builder.CreateString((const char *)TCHAR_TO_UTF8(*TextProperty->GetPropertyValue(Value).ToString())).o;
		OutOffset = o;
	}
	else if (UNameProperty *NameProperty = Cast<UNameProperty>(Property))
	{
		// string case
		auto o = Builder.CreateString((const char *)TCHAR_TO_UTF8(*NameProperty->GetPropertyValue(Value).ToString())).o;
		OutOffset = o;
	}
	else if (UArrayProperty *ArrayProperty = Cast<UArrayProperty>(Property))
	{
		// vector case
		flatbuffers::uoffset_t ArrayUoffset;
		if (!SerializeArray(Builder, ArrayUoffset, ArrayProperty, static_cast<const FScriptArray *>(Value), CheckFlags, SkipFlags))
		{
			return false;
		}
		OutOffset = ArrayUoffset;
	}
	else if (UStructProperty *StructProperty = Cast<UStructProperty>(Property))
	{
		// table case
		flatbuffers::uoffset_t TableUoffset;
		if (!SerializeUStruct(Builder, TableUoffset, StructProperty->Struct, Value, CheckFlags, SkipFlags))
		{
			return false;
		}
		OutOffset = TableUoffset;
	}
	else {
		OutOffset = 0;
	}
	return true;
}

bool FFlatBuffersHelper::SerializeUStruct(flatbuffers::FlatBufferBuilder& Builder, flatbuffers::uoffset_t &OutOffset, const UStruct* StructDefinition, const void* Struct, int64 CheckFlags /*= 0*/, int64 SkipFlags /*= 0*/)
{
	TArray<flatbuffers::uoffset_t> FieldUoffsets;
	int NumFields;

	class FPropertySerializer : public FPropertyVisitor
	{
	public:
		flatbuffers::FlatBufferBuilder &Builder;
		int NumFields;
		TArray<flatbuffers::uoffset_t> &FieldUoffsets;

		FPropertySerializer(flatbuffers::FlatBufferBuilder &InBuilder, TArray<flatbuffers::uoffset_t> &InFieldUoffsets) : Builder(InBuilder), NumFields(0), FieldUoffsets(InFieldUoffsets)
		{
		}

		virtual bool Visit(int Index, UProperty *Property, FString VariableName, const void* Value, int64 InCheckFlags, int64 InSkipFlags)
		{
			NumFields++;
			UClass* PropClass = Property->GetClass();

			// What does ArrayDim mean for non-Arrays?
			if (Property->ArrayDim != 1)
			{
				UE_LOG(LogFlatBuffersHelper, Log, TEXT("ArrayDim is not 1: Unhandled property type '%s': %s"), *PropClass->GetName(), *Property->GetPathName());
				return false;
			}

			flatbuffers::uoffset_t o;

			if (!SerializeNonEmbeddedProperty(Builder, o, Property, Value, InCheckFlags, InSkipFlags))
			{
				return false;
			}

			FieldUoffsets.Push(o);

			return true;

		}
	};

	FPropertySerializer PropertySerializer(Builder, FieldUoffsets);


	// Phase 1: Serialize objects not embeddable in the parent object, populate the offsets in FieldUoffsets.
	if (!IterateUProperties(StructDefinition, Struct, CheckFlags, SkipFlags, &PropertySerializer)) {
		return false;
	}

	NumFields = PropertySerializer.NumFields;


	// Phase 2: Serialize FieldUoffsets and scalar fields

	auto TableStart = Builder.StartTable();

	class FPropertyAdder : public FPropertyVisitor
	{
	public:
		flatbuffers::FlatBufferBuilder &Builder;
		TArray<flatbuffers::uoffset_t> &FieldUoffsets;

		FPropertyAdder(flatbuffers::FlatBufferBuilder &InBuilder, TArray<flatbuffers::uoffset_t> &InFieldUoffsets) : Builder(InBuilder), FieldUoffsets(InFieldUoffsets)
		{
		}

		virtual bool Visit(int Index, UProperty *Property, FString VariableName, const void* Value, int64 InCheckFlags, int64 InSkipFlags)
		{
			int FieldIndex = (Index + 2) * 2;
			UClass* PropClass = Property->GetClass();

			if (UByteProperty *ByteProperty = Cast<UByteProperty>(Property))
			{
				// Enum case
				Builder.AddElement<uint8_t>(FieldIndex, static_cast<uint8_t>(ByteProperty->GetUnsignedIntPropertyValue(Value)), 0);
			}
			else if (UInt64Property *Int64Property = Cast <UInt64Property>(Property))
			{
				// long case
				Builder.AddElement<int64_t>(FieldIndex, static_cast<int64_t>(Int64Property->GetSignedIntPropertyValue(Value)), 0);
			}
			else if (UIntProperty *IntProperty = Cast <UIntProperty>(Property))
			{
				// int32 case
				Builder.AddElement<int32_t>(FieldIndex, static_cast<int32_t>(IntProperty->GetSignedIntPropertyValue(Value)), 0);
			}
			else if (UUInt32Property *UInt32Property = Cast <UUInt32Property>(Property))
			{
				// uint case
				Builder.AddElement<uint32_t>(FieldIndex, static_cast<uint32_t>(UInt32Property->GetUnsignedIntPropertyValue(Value)), 0);
			}
			else if (UBoolProperty *BoolProperty = Cast<UBoolProperty>(Property))
			{
				// bool case
				Builder.AddElement<uint8_t>(FieldIndex, static_cast<uint8_t>(BoolProperty->GetPropertyValue(Value)), 0);
			}
			else if (UStrProperty *StringProperty = Cast<UStrProperty>(Property))
			{
				// string case
				Builder.AddOffset<void>(FieldIndex, FieldUoffsets[Index]);
			}
			else if (UTextProperty *TextProperty = Cast<UTextProperty>(Property))
			{
				// string case
				Builder.AddOffset<void>(FieldIndex, FieldUoffsets[Index]);
			}
			else if (UNameProperty *NameProperty = Cast<UNameProperty>(Property))
			{
				// string case
				Builder.AddOffset<void>(FieldIndex, FieldUoffsets[Index]);
			}
			else if (UArrayProperty *ArrayProperty = Cast<UArrayProperty>(Property))
			{
				// vector case
				Builder.AddOffset<void>(FieldIndex, FieldUoffsets[Index]);
			}
			else if (UStructProperty *StructProperty = Cast<UStructProperty>(Property))
			{
				// table case
				Builder.AddOffset<void>(FieldIndex, FieldUoffsets[Index]);
			}
			else {
				// unhandled case
				UE_LOG(LogFlatBuffersHelper, Log, TEXT("Unhandled property type '%s': %s"), *PropClass->GetName(), *Property->GetPathName());
				return false;
			}

			return true;

		}
	};

	FPropertyAdder PropertyAdder(Builder, FieldUoffsets);

	if (!IterateUProperties(StructDefinition, Struct, CheckFlags, SkipFlags, &PropertyAdder))
	{
		return false;
	}
	
	// Wrap up
	OutOffset = Builder.EndTable(TableStart, NumFields);

	return true;
}

bool FFlatBuffersHelper::SerializeArray(flatbuffers::FlatBufferBuilder& Builder, flatbuffers::uoffset_t &OutOffset, const UArrayProperty* ArrayProperty, const FScriptArray* Array, int64 CheckFlags /*= 0*/, int64 SkipFlags /*= 0*/)
{

	TArray<flatbuffers::uoffset_t> FieldUoffsets;
	int NumElems = Array->Num();

	class FPropertySerializer : public FPropertyVisitor
	{
	public:
		flatbuffers::FlatBufferBuilder &Builder;
		TArray<flatbuffers::uoffset_t> &FieldUoffsets;

		FPropertySerializer(flatbuffers::FlatBufferBuilder &InBuilder, TArray<flatbuffers::uoffset_t> &InFieldUoffsets) : Builder(InBuilder), FieldUoffsets(InFieldUoffsets)
		{
		}

		virtual bool Visit(int Index, UProperty *Property, FString VariableName, const void* Value, int64 InCheckFlags, int64 InSkipFlags)
		{
			UClass* PropClass = Property->GetClass();

			// What does ArrayDim mean for non-Arrays?
			if (Property->ArrayDim != 1)
			{
				UE_LOG(LogFlatBuffersHelper, Log, TEXT("ArrayDim is not 1: Unhandled property type '%s': %s"), *PropClass->GetName(), *Property->GetPathName());
				return false;
			}

			flatbuffers::uoffset_t o;

			if (!SerializeNonEmbeddedProperty(Builder, o, Property, Value, InCheckFlags, InSkipFlags))
			{
				return false;
			}

			FieldUoffsets.Push(o);

			return true;

		}
	};

	FPropertySerializer PropertySerializer(Builder, FieldUoffsets);


	// Phase 1: Serialize objects not embeddable in the parent object, populate the offsets in FieldUoffsets.
	if (!IterateArrayElements(ArrayProperty, Array, CheckFlags, SkipFlags, &PropertySerializer)) {
		return false;
	}

	// Phase 2: Serialize FieldUoffsets and scalar fields


	class FPropertyPusher : public FPropertyVisitor
	{
	public:
		flatbuffers::FlatBufferBuilder &Builder;
		TArray<flatbuffers::uoffset_t> &FieldUoffsets;

		FPropertyPusher(flatbuffers::FlatBufferBuilder &InBuilder, TArray<flatbuffers::uoffset_t> &InFieldUoffsets) : Builder(InBuilder), FieldUoffsets(InFieldUoffsets)
		{
		}

		virtual bool Visit(int Index, UProperty *Property, FString VariableName, const void* Value, int64 InCheckFlags, int64 InSkipFlags)
		{
			if (UByteProperty *ByteProperty = Cast<UByteProperty>(Property))
			{
				// Enum case
				Builder.PushElement<uint8_t>(static_cast<uint8_t>(ByteProperty->GetUnsignedIntPropertyValue(Value)));
			}
			else if (UInt64Property *Int64Property = Cast <UInt64Property>(Property))
			{
				// long case
				Builder.PushElement<int64_t>(static_cast<int64_t>(Int64Property->GetSignedIntPropertyValue(Value)));
			}
			else if (UIntProperty *IntProperty = Cast <UIntProperty>(Property))
			{
				// int32 case
				Builder.PushElement<int32_t>(static_cast<int32_t>(IntProperty->GetSignedIntPropertyValue(Value)));
			}
			else if (UUInt32Property *UInt32Property = Cast <UUInt32Property>(Property))
			{
				// int32 case
				Builder.PushElement<uint32_t>(static_cast<uint32_t>(UInt32Property->GetUnsignedIntPropertyValue(Value)));
			}
			else if (UBoolProperty *BoolProperty = Cast<UBoolProperty>(Property))
			{
				// bool case
				Builder.PushElement<uint8_t>(static_cast<uint8_t>(BoolProperty->GetPropertyValue(Value)));
			}
			else if (UStrProperty *StringProperty = Cast<UStrProperty>(Property))
			{
				// string case
				Builder.PushElement(static_cast<flatbuffers::Offset<void>>(FieldUoffsets[Index]));
			}
			else if (UTextProperty *TextProperty = Cast<UTextProperty>(Property))
			{
				// string case
				Builder.PushElement(static_cast<flatbuffers::Offset<void>>(FieldUoffsets[Index]));
			}
			else if (UNameProperty *NameProperty = Cast<UNameProperty>(Property))
			{
				// string case
				Builder.PushElement(static_cast<flatbuffers::Offset<void>>(FieldUoffsets[Index]));
			}
			else if (UArrayProperty *InArrayProperty = Cast<UArrayProperty>(Property))
			{
				// vector case
				Builder.PushElement(static_cast<flatbuffers::Offset<void>>(FieldUoffsets[Index]));
			}
			else if (UStructProperty *StructProperty = Cast<UStructProperty>(Property))
			{
				// table case
				Builder.PushElement(static_cast<flatbuffers::Offset<void>>(FieldUoffsets[Index]));
			}
			else {
				// unhandled case
				UClass* PropClass = Property->GetClass();
				UE_LOG(LogFlatBuffersHelper, Log, TEXT("Unhandled property type '%s': %s"), *PropClass->GetName(), *Property->GetPathName());
				return false;
			}

			return true;

		}
	};

	FPropertyPusher PropertyPusher(Builder, FieldUoffsets);

	Builder.StartVector(NumElems, ElemSize(ArrayProperty->Inner));

	if (!IterateArrayElements(ArrayProperty, Array, CheckFlags, SkipFlags, &PropertyPusher))
	{
		return false;
	}

	// Wrap up
	OutOffset = Builder.EndVector(NumElems);

	return true;
}

bool FFlatBuffersHelper::IterateUProperties(const UStruct* StructDefinition, const void* Struct, int64 CheckFlags /*= 0*/, int64 SkipFlags /*= 0*/, FPropertyVisitor *Visitor)
{
	int Index = 0;
	for (TFieldIterator<UProperty> It(StructDefinition); It; ++It)
	{
		UProperty *Property = *It;

		if (CheckFlags != 0 && !Property->HasAnyPropertyFlags(CheckFlags))
		{
			continue;
		}
		if (Property->HasAnyPropertyFlags(SkipFlags))
		{
			continue;
		}

		FString VariableName = Property->GetName();
		const void *Value = Property->ContainerPtrToValuePtr<uint8>(Struct);

		if (!Visitor->Visit(Index, Property, VariableName, Value, CheckFlags, SkipFlags))
		{
			return false;
		}

		Index++;
	}
	return true;

}

bool FFlatBuffersHelper::IterateArrayElements(const UArrayProperty* ArrayProperty, const void* Array, int64 CheckFlags /*= 0*/, int64 SkipFlags /*= 0*/, FPropertyVisitor *Visitor)
{
	FScriptArrayHelper Helper(ArrayProperty, Array);

	FString VariableName = ArrayProperty->GetName();
	
	for (int Index = Helper.Num() - 1; Index >= 0; --Index)
	{
		UProperty *Property = ArrayProperty->Inner;

		if (CheckFlags != 0 && !Property->HasAnyPropertyFlags(CheckFlags))
		{
			continue;
		}
		if (Property->HasAnyPropertyFlags(SkipFlags))
		{
			continue;
		}

		const void *Value = Helper.GetRawPtr(Index);

		if (!Visitor->Visit(Index, Property, VariableName, Value, CheckFlags, SkipFlags))
		{
			return false;
		}
	}
	return true;

}

size_t FFlatBuffersHelper::ElemSize(const UProperty *Property)
{
	size_t Result = 0;
	if (const UByteProperty *ByteProperty = Cast<UByteProperty>(Property))
	{
		Result = 1;
	}
	else if (const UInt64Property *Int64Property = Cast <UInt64Property>(Property))
	{
		Result = 8;
	}
	else if (const UIntProperty *IntProperty = Cast <UIntProperty>(Property))
	{
		Result = 4;
	}
	else if (const UUInt32Property *UInt32Property = Cast <UUInt32Property>(Property))
	{
		Result = 4;
	}
	else if (const UBoolProperty *BoolProperty = Cast<UBoolProperty>(Property))
	{
		Result = 1;
	}
	else if (const UStrProperty *StringProperty = Cast<UStrProperty>(Property))
	{
		Result = 4;
	}
	else if (const UTextProperty *TextProperty = Cast<UTextProperty>(Property))
	{
		Result = 4;
	}
	else if (const UNameProperty *NameProperty = Cast<UNameProperty>(Property))
	{
		Result = 4;
	}
	else if (const UArrayProperty *ArrayProperty = Cast<UArrayProperty>(Property))
	{
		Result = 4;
	}
	else if (const UStructProperty *StructProperty = Cast<UStructProperty>(Property))
	{
		Result = 4;
	}
	return Result;
}

bool FFlatBuffersHelper::FlatBufferToUStruct(const uint8 *Buf, size_t BufSize, const UStruct *StructDefinition, void *OutStruct, int64 CheckFlags, int64 SkipFlags)
{
	// Need a way to make sure flatbuffers doesn't touch the memory out of bound of Buf.

	const flatbuffers::Table *Table = flatbuffers::GetRoot<flatbuffers::Table>(Buf);

	if (!DeserializeUStruct(Table, StructDefinition, OutStruct, CheckFlags, SkipFlags)) {
		return false;
	}
	return true;
}

bool FFlatBuffersHelper::DeserializeUStruct(const flatbuffers::Table *Table, const UStruct *StructDefinition, void *OutStruct, int64 CheckFlags, int64 SkipFlags)
{
	//UE_LOG(LogFlatBuffersHelper, Log, TEXT("ENTER DeserializeUStruct"));
	int Index = 0;

	for (TFieldIterator<UProperty> PropIt(StructDefinition); PropIt; ++PropIt, ++Index)
	{
		UProperty* Property = *PropIt;
		FString PropertyName = Property->GetName();

		//UE_LOG(LogFlatBuffersHelper, Log, TEXT("DeserializeUStruct -> %s"), *PropertyName);

		UClass* PropClass = Property->GetClass();

		// Check to see if we should ignore this property
		if (CheckFlags != 0 && !Property->HasAnyPropertyFlags(CheckFlags))
		{
			continue;
		}
		if (Property->HasAnyPropertyFlags(SkipFlags))
		{
			continue;
		}
		void* Value = Property->ContainerPtrToValuePtr<uint8>(OutStruct);

		// What does ArrayDim mean for non-Arrays?
		if (Property->ArrayDim != 1)
		{
			UE_LOG(LogFlatBuffersHelper, Log, TEXT("ArrayDim is not 1: Unhandled property type '%s': %s"), *PropClass->GetName(), *Property->GetPathName());
			return false;
		}

		flatbuffers::voffset_t vo = Table->GetOptionalFieldOffset((Index + 2) * 2);
		if (vo == 0)
		{
			// There are two cases for this:
			// (1) The field has the default value for the field. (NULL for object type)
			// (2) This data is from an old version of the protocol which doesn't have the field.

			// We don't know the protocol default value so we assume zero here.
			// Case (2) shouldn't happen for our use case.

			Property->ClearValue(Value);
			continue;
		}

		if (!DeserializeProperty(reinterpret_cast<const uint8_t *>(Table) + vo, Property, Value, CheckFlags, SkipFlags))
		{
			return false;
		}
	}

	return true;
}

// In case of nonembeddable field type, Buf points to uoffset_t of the field not the field itself.
bool FFlatBuffersHelper::DeserializeProperty(const uint8 *Buf, const UProperty *Property, void *OutValue, int64 CheckFlags, int64 SkipFlags)
{
	if (const UByteProperty *ByteProperty = Cast<UByteProperty>(Property))
	{
		ByteProperty->SetIntPropertyValue(OutValue, static_cast<uint64>(flatbuffers::ReadScalar<uint8>(Buf)));
	}
	else if (const UInt64Property *Int64Property = Cast <UInt64Property>(Property))
	{
		Int64Property->SetIntPropertyValue(OutValue, static_cast<int64>(flatbuffers::ReadScalar<int64>(Buf)));
	}
	else if (const UIntProperty *IntProperty = Cast <UIntProperty>(Property))
	{
		IntProperty->SetIntPropertyValue(OutValue, static_cast<int64>(flatbuffers::ReadScalar<int32>(Buf)));
	}
	else if (const UUInt32Property *UInt32Property = Cast <UUInt32Property>(Property))
	{
		UInt32Property->SetIntPropertyValue(OutValue, static_cast<uint64>(flatbuffers::ReadScalar<uint32>(Buf)));
	}
	else if (const UBoolProperty *BoolProperty = Cast<UBoolProperty>(Property))
	{
		BoolProperty->SetPropertyValue(OutValue, flatbuffers::ReadScalar<uint8_t>(Buf) != 0);
	}
	else if (const UStrProperty *StringProperty = Cast<UStrProperty>(Property))
	{
		auto Str = reinterpret_cast<const flatbuffers::String *>(Buf + flatbuffers::ReadScalar<flatbuffers::uoffset_t>(Buf));
		StringProperty->SetPropertyValue(OutValue, UTF8_TO_TCHAR(Str->c_str()));
	}
	else if (const UTextProperty *TextProperty = Cast<UTextProperty>(Property))
	{
		auto Str = reinterpret_cast<const flatbuffers::String *>(Buf + flatbuffers::ReadScalar<flatbuffers::uoffset_t>(Buf));
		TextProperty->SetPropertyValue(OutValue, FText::FromString(UTF8_TO_TCHAR(Str->c_str())));
	}
	else if (const UNameProperty *NameProperty = Cast<UNameProperty>(Property))
	{
		auto Str = reinterpret_cast<const flatbuffers::String *>(Buf + flatbuffers::ReadScalar<flatbuffers::uoffset_t>(Buf));
		NameProperty->SetPropertyValue(OutValue, FName(UTF8_TO_TCHAR(Str->c_str())));
	}
	else if (const UArrayProperty *ArrayProperty = Cast<UArrayProperty>(Property))
	{
		if (!DeserializeArray(Buf + flatbuffers::ReadScalar<flatbuffers::uoffset_t>(Buf), ArrayProperty, OutValue, CheckFlags, SkipFlags))
		{
			return false;
		}
	}
	else if (const UStructProperty *StructProperty = Cast<UStructProperty>(Property))
	{
		const flatbuffers::Table *Table = reinterpret_cast<const flatbuffers::Table *>(Buf + flatbuffers::ReadScalar<flatbuffers::uoffset_t>(Buf));

		if (!DeserializeUStruct(Table, StructProperty->Struct, OutValue, CheckFlags, SkipFlags))
		{
			return false;
		}
	}
	else
	{
		// unhandled case
		UClass* PropClass = Property->GetClass();
		UE_LOG(LogFlatBuffersHelper, Log, TEXT("DeserializeProperty: Unhandled property type '%s': %s"), *PropClass->GetName(), *Property->GetPathName());
		return false;
	}

	return true;
}

bool FFlatBuffersHelper::DeserializeArray(const uint8 *Buf, const UArrayProperty *ArrayProperty, void *OutValue, int64 CheckFlags, int64 SkipFlags)
{
	int Size = flatbuffers::ReadScalar<flatbuffers::uoffset_t>(Buf);

	const UProperty *Property = ArrayProperty->Inner;
	flatbuffers::uoffset_t Stride = ElemSize(Property);
	
	FScriptArrayHelper Helper(ArrayProperty, OutValue);
	Helper.Resize(Size);

	const uint8 *b = Buf + sizeof(flatbuffers::uoffset_t);
	for (int Index = 0; Index < Size; ++Index, b += Stride)
	{
		if (!DeserializeProperty(b, Property, Helper.GetRawPtr(Index), CheckFlags, SkipFlags))
		{
			return false;
		}
	}

	return true;
}