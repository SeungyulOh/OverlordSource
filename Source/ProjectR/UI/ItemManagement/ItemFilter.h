// Fill out your copyright notice in the Description page of Project Settings.


#pragma once

#include "EngineMinimal.h"
#include "UC_ItemInventoryBase.h"
#include "Table/ItemTableInfo.h"
#include "Table/CharacterTableInfo.h"
#include "Network/PacketDataStructures.h"
#include "Table/CurrencyPropertyTableInfo.h"
#include "ItemFilter.generated.h"


UENUM(BlueprintType)
enum class EEquipmentFilterCategory : uint8
{
	Grade,
	Class,
	Type,
	Enchant,
	Etc,
};

UENUM(BlueprintType)
enum class ERuneFilterCategory : uint8
{
	Etc,
};

UENUM(BlueprintType)
enum class EHeroPieceFilterCategory : uint8
{
	Grade,
	Class,
	Continent,
};

UENUM(BlueprintType)
enum class EMaterialFilterCategory : uint8
{
	Etc,
};


USTRUCT(BlueprintType)
struct PROJECTR_API FItemFilterRow
{
	GENERATED_BODY()

	FName Category = NAME_None;
	FName Category_UIStringKey = NAME_None;
	TArray<FString> Factors;
	TArray<FName> FactorUIStrings;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FItemFilterFactor
{
	GENERATED_BODY()

	FName Category = NAME_None;
	FString Factor;
};

/**
* Represents item filter categories and factors for one item inventory (Weapons, Defense, ...)
* can be rendered into filter dialogue UI
*/
UCLASS(BlueprintType)
class PROJECTR_API UItemFilterBase : public UObject
{
	GENERATED_BODY()
public:
	/** UI rows */
	TArray<FItemFilterRow> Rows;

	/** factors that are *checked* that is used for filtering. client saved. */
	TArray<FItemFilterFactor> CheckedFactors;

	// for loading/storing in GameUserSettings config. See URGameUserSetting.ItemInventoryFilter_Weapons_UncheckedFactors
	void LoadCheckedFactors(const TArray<FString>& InConfig);
	void StoreCheckedFactors(TArray<FString>& OutConfig);
	virtual bool Predicate(const FItemTableInfo* ItemInfo, const FITEM* ItemData, const FCharacterTableInfo* CurrentCharacterInfo);
	virtual bool Predicate(const FWalletTableInfo* WalletInfo);
};

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTR_API UEquipmentFilter : public UItemFilterBase
{
	GENERATED_BODY()
public:
	// returns false if filtered out
	bool Predicate(const FItemTableInfo* ItemInfo, const FITEM* ItemData, const FCharacterTableInfo* CurrentCharacterInfo);

private:
	bool Predicate_Grade(FString Factor, EItemGradeEnum ItemGrade);
	bool Predicate_Class(FString Factor, EItemEquipTypeEnum EquipType);
	bool Predicate_Type(FString Factor, EItemEquipTypeEnum EquipType);
	bool Predicate_Enchant(FString Factor, const FItemTableInfo* ItemInfo, const FITEM* ItemData);
	bool Predicate_Etc(FString Factor, const FItemTableInfo* ItemInfo, const FITEM* ItemData);
};

/**
*
*/
UCLASS(BlueprintType)
class PROJECTR_API UHeroPieceFilter : public UItemFilterBase
{
	GENERATED_BODY()
public:
	// returns false if filtered out
	bool Predicate(const FWalletTableInfo* WalletInfo);

private:
	bool Predicate_Grade(FString Factor, EItemGradeEnum ItemGrade);
};