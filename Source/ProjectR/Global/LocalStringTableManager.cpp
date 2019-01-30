// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "Table/StringTableInfo.h"

//#include "GlobalIntegrated.h"

//#include "GameOptionManager.h"
#include "LocalStringTableManager.h"


static const FString ContextLocalString(TEXT("GENERAL"));

ULocalStringTableManager* ULocalStringTableManager::InstancePtr = nullptr;

ULocalStringTableManager::ULocalStringTableManager()
{

}

ULocalStringTableManager*	ULocalStringTableManager::GetInstancePtr()
{
	if (InstancePtr == nullptr)
	{
		InstancePtr = NewObject<ULocalStringTableManager>((UObject*)GetTransientPackage(), ULocalStringTableManager::StaticClass());
		InstancePtr->Initialize();
		InstancePtr->AddToRoot();
	}

	return InstancePtr;
}

void ULocalStringTableManager::ReleaseSingleton()
{
	if (InstancePtr)
	{
		InstancePtr->UnloadTableAll();
		InstancePtr->RemoveFromRoot();
	}
	InstancePtr = nullptr;
}

void ULocalStringTableManager::Initialize()
{
	StringTableDefaultPath.Empty(); 
	StringDataTableMap.Empty();
	LanguageMap.Empty();

	EStringTableTypePtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EStringTableType"), true);
	auto StringTableTypeMaxCount = EStringTableTypePtr->GetMaxEnumValue();
	
	StringTableDefaultPath.Reserve(StringTableTypeMaxCount);
	StringDataTableMap.Reserve(StringTableTypeMaxCount);
	for (int32 EnumIndex = 0; EnumIndex <= StringTableTypeMaxCount; ++EnumIndex)
	{
		auto StringTableTypeEnum	= static_cast<EStringTableType>(EnumIndex);
		StringDataTableMap.Emplace(StringTableTypeEnum, nullptr);

		auto EnumString				= EStringTableTypePtr->GetNameStringByIndex(EnumIndex);
		EnumString.RemoveFromStart("VE_");
		EnumString = "DT_" + EnumString + "String";

		//DT_"Name"String
		StringTableDefaultPath.Emplace(StringTableTypeEnum, EnumString);
	}

	
	ELanguageTypePtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("ELanguageType"), true);
	auto LanguageTypeCount = ELanguageTypePtr->GetMaxEnumValue();

	LanguageMap.Reserve(LanguageTypeCount);
	for (int32 EnumIndex = 0; EnumIndex <= LanguageTypeCount; ++EnumIndex)
	{
		auto	LanguageTypeEnum	= static_cast<ELanguageType>(EnumIndex);
		auto	EnumString			= ELanguageTypePtr->GetNameStringByIndex(EnumIndex);

		EnumString.RemoveFromStart("VE");
		LanguageMap.Emplace(LanguageTypeEnum, EnumString);
	}

	DefaultStringTablePath = "DataTable'/Game/Data/StringTables/StringTable_Local/";
	LanguageType = UGameOptionManager::GetLanguageType();
	LoadTable(LanguageType);
}

void ULocalStringTableManager::LoadTable(const ELanguageType InLanguageType)
{
	LanguageType = InLanguageType;
	UnloadTableAll();

	auto LanguageString = LanguageMap[InLanguageType];

	auto StringTableTypeMaxCount = EStringTableTypePtr->GetMaxEnumValue();
	for (int32 EnumIndex = 0; EnumIndex < StringTableTypeMaxCount; ++EnumIndex)
	{
		auto StringTableTypeEnum = static_cast<EStringTableType>(EnumIndex);

		auto CurrentTableString = StringTableDefaultPath[StringTableTypeEnum] + LanguageString;
		auto CurrentTableStringByLanguageType = DefaultStringTablePath + CurrentTableString + "." + CurrentTableString + "'";
		StringDataTableMap[StringTableTypeEnum] = UTableManager::GetInstancePtr()->LoadTableFromPath(*CurrentTableStringByLanguageType);
	}
}

void ULocalStringTableManager::UnloadTableAll()
{
	for (auto& MapElem : StringDataTableMap)
	{
		if (MapElem.Value != nullptr)
		{
			//MapElem.Value->ConditionalBeginDestroy();
			MapElem.Value = nullptr;
		}
	}
}

bool ULocalStringTableManager::GetLocalizedText(EStringTableType TableType, FName StringID, FText& StringValue)
{
	if (StringDataTableMap.Contains(TableType) == false)
	{
		return false;
	}

	auto StringTable = StringDataTableMap[TableType];
	if (StringTable == nullptr)
	{
		return false;
	}

	auto TableRow = StringTable->FindRow<FStringTableInfo>(StringID, ContextLocalString);
	if (TableRow == nullptr)
	{
		return false;
	}
	switch (LanguageType)
	{
	case ELanguageType::VE_KOR:
		StringValue = TableRow->KOR;
		break;
	case ELanguageType::VE_ENG:
		StringValue = TableRow->ENG;
		break;
	case ELanguageType::VE_CHN:
		StringValue = TableRow->CHN;
		break;
	case ELanguageType::VE_TWN:
		StringValue = TableRow->TWN;
		break;
	}

	return true;
}

FStringTableInfo* ULocalStringTableManager::GetStringTable(EStringTableType TableType, FName StringID)
{
	if (StringDataTableMap.Contains(TableType) == false)
	{
		return nullptr;
	}

	auto StringTable = StringDataTableMap[TableType];
	if (StringTable == nullptr)
	{
		return nullptr;
	}

	auto TableRow = StringTable->FindRow<FStringTableInfo>(StringID, ContextLocalString);
	if (TableRow == nullptr)
	{
		return nullptr;
	}
	return TableRow;
}


TArray< FName > ULocalStringTableManager::GetStringKeyNames(EStringTableType TableType)
{
	TArray< FName > Result;
	if (StringDataTableMap.Contains(TableType))
	{
		UDataTable* StringTable = StringDataTableMap[TableType];
		if (StringTable)
		{
			return StringTable->GetRowNames();
		}
	}
	
	return Result;
}