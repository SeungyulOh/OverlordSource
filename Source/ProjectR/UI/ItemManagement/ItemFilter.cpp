// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ItemFilter.h"

#include "UtilFunctionIntegrated.h"

bool UEquipmentFilter::Predicate(const FItemTableInfo* ItemInfo, const FITEM* ItemData, const FCharacterTableInfo* CurrentCharacterInfo)
{
	for (const FItemFilterFactor& CheckedFactor : CheckedFactors)
	{
		int32 CategoryInt = UUtilFunctionLibrary::StringToEnum(TEXT("EEquipmentFilterCategory"), CheckedFactor.Category.ToString());
		if (CategoryInt == INDEX_NONE)
		{
			continue; // error
		}
		EEquipmentFilterCategory Category = (EEquipmentFilterCategory)CategoryInt;

		bool bPass = false;
		switch (Category)
		{
		case EEquipmentFilterCategory::Grade:
			bPass = Predicate_Grade(CheckedFactor.Factor, ItemInfo->ItemGrade);
			break;
		case EEquipmentFilterCategory::Class:
			bPass = Predicate_Class(CheckedFactor.Factor, ItemInfo->EquipType);
			break;
		case EEquipmentFilterCategory::Type:
			bPass = Predicate_Type(CheckedFactor.Factor, ItemInfo->EquipType);
			break;
		case EEquipmentFilterCategory::Enchant:
			bPass = Predicate_Enchant(CheckedFactor.Factor, ItemInfo, ItemData);
			break;
		case EEquipmentFilterCategory::Etc:
			bPass = Predicate_Etc(CheckedFactor.Factor, ItemInfo, ItemData);
			break;
		default:
			break;
		}

		if (bPass)
		{
			return true;
		}
	}

	return false;
}

void UItemFilterBase::LoadCheckedFactors(const TArray<FString>& InConfig)
{
	CheckedFactors.Empty();

	for (const FString& ConfigFactor : InConfig)
	{
		// read the form "<category>+<factor>"

		FString Category;
		FString Factor;
		if (!ConfigFactor.Split(TEXT("+"), &Category, &Factor))
		{
			UE_LOG(LogGamePlay, Log, TEXT("UEquipmentFilter::LoadUncheckedFactors: failed to read ConfigFactor: %s"), *ConfigFactor);
			continue;
		}

		FItemFilterFactor FilterFactor;
		FilterFactor.Category = FName(*Category);
		FilterFactor.Factor = Factor;

		CheckedFactors.Emplace(FilterFactor);
	}
}

void UItemFilterBase::StoreCheckedFactors(TArray<FString>& OutConfig)
{
	OutConfig.Empty();

	for (const FItemFilterFactor& FilterFactor : CheckedFactors)
	{
		OutConfig.Emplace(FString::Printf(TEXT("%s+%s"), *FilterFactor.Category.ToString(), *FilterFactor.Factor));
	}
}

bool UItemFilterBase::Predicate(const FItemTableInfo* ItemInfo, const FITEM* ItemData, const FCharacterTableInfo* CurrentCharacterInfo)
{
	return true;
}

bool UItemFilterBase::Predicate(const FWalletTableInfo* WalletInfo)
{
	return true;
}

bool UEquipmentFilter::Predicate_Grade(FString Factor, EItemGradeEnum ItemGrade)
{
	int32 IntValue = UUtilFunctionLibrary::StringToEnum(TEXT("EItemGradeEnum"), Factor);
	return (int32)ItemGrade == IntValue;
}

bool UEquipmentFilter::Predicate_Class(FString Factor, EItemEquipTypeEnum EquipType)
{
	int32 IntValue = UUtilFunctionLibrary::StringToEnum(TEXT("EClassTypeEnum"), Factor);
	if (IntValue == INDEX_NONE)
		return true;

	return FItemTableInfo::CanEquip((EClassTypeEnum)IntValue, EquipType);
}

bool UEquipmentFilter::Predicate_Type(FString Factor, EItemEquipTypeEnum EquipType)
{
	int32 IntValue = UUtilFunctionLibrary::StringToEnum(TEXT("EItemEquipTypeEnum"), Factor);
	return (int32)EquipType == IntValue;
}

bool UEquipmentFilter::Predicate_Enchant(FString Factor, const FItemTableInfo* ItemInfo, const FITEM* ItemData)
{
	if (Factor == TEXT("EnchantLocked"))
	{
		return ItemData->enchantLocked;
	}
	else if (Factor.StartsWith(TEXT("(")))
	{
		// handle "(m, n)"
		FString ParensShedded = Factor.Mid(1, Factor.Len() - 2);
		FString RangeStartString;
		FString RangeEndString;
		if (!ParensShedded.Split(TEXT(","), &RangeStartString, &RangeEndString))
		{
			UE_LOG(LogGamePlay, Log, TEXT("UEquipmentFilter::Predicate_Enchant: no ,"));
			return true;
		}

		int32 RangeStart = FCString::Atoi(*RangeStartString);
		int32 RangeEnd = FCString::Atoi(*RangeEndString);

		return RangeStart <= ItemData->enchant && ItemData->enchant <= RangeEnd;
	}
	else
	{
		UE_LOG(LogGamePlay, Log, TEXT("UEquipmentFilter::Predicate_Enchant: unexpected Factor string: %s"), *Factor);
		return true;
	}
}

bool UEquipmentFilter::Predicate_Etc(FString Factor, const FItemTableInfo* ItemInfo, const FITEM* ItemData)
{
	if (Factor == TEXT("HasRunes"))
	{
		return ItemData->runes.Num() != 0;
	}
	else if (Factor == TEXT("Locked"))
	{
		return ItemData->locked;
	}
	else if (Factor == TEXT("All"))
	{
		// TODO
		return true;
	}
	else
	{
		UE_LOG(LogGamePlay, Log, TEXT("UEquipmentFilter::Predicate_Etc: unexpected Factor string: %s"), *Factor);
		return true;
	}
}

bool UHeroPieceFilter::Predicate(const FWalletTableInfo* WalletInfo)
{
	for (const FItemFilterFactor& CheckedFactor : CheckedFactors)
	{
		int32 CategoryInt = UUtilFunctionLibrary::StringToEnum(TEXT("EHeroPieceFilterCategory"), CheckedFactor.Category.ToString());
		if (CategoryInt == INDEX_NONE)
		{
			continue; // error
		}
		EHeroPieceFilterCategory Category = (EHeroPieceFilterCategory)CategoryInt;

		bool bPass = false;
		switch (Category)
		{
		case EHeroPieceFilterCategory::Grade:
			// TODO: fix grade
			bPass = Predicate_Grade(CheckedFactor.Factor, (EItemGradeEnum)(WalletInfo->Grade - 1));
			break;
		case EHeroPieceFilterCategory::Class:
			bPass = false; // TODO: need HeroPiece table or something
			break;
		case EHeroPieceFilterCategory::Continent:
			bPass = false; // TODO: need HeroPiece table or something
			break;
		default:
			break;
		}

		if (bPass)
		{
			return true;
		}
	}

	return false;
}

bool UHeroPieceFilter::Predicate_Grade(FString Factor, EItemGradeEnum ItemGrade)
{
	int32 IntValue = UUtilFunctionLibrary::StringToEnum(TEXT("EItemGradeEnum"), Factor);
	return (int32)ItemGrade == IntValue;
}
