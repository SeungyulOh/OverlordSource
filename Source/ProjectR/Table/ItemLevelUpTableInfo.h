#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "Table/ItemTableInfo.h"
#include "Network/PacketDataStructures.h"
#include "ItemLevelUpTableInfo.generated.h"

/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FItemLevelUpTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemLevelUpTableInfo)
	EItemEquipTypeEnum		EquipType;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemLevelUpTableInfo)
	EItemGradeEnum			Grade;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemLevelUpTableInfo)
	int32					Level;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemLevelUpTableInfo)
	int32					NeedExp;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemLevelUpTableInfo)
	FName					NeedCurrency;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemLevelUpTableInfo)
	int32					NeedGold;

	// ?
	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemLevelUpTableInfo)
	//int32					MaxExp;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemLevelUpTableInfo)
	//int32					MaxGold;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemLevelUpTableInfo)
	//int32					PlusMaxHP;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemLevelUpTableInfo)
	int32					PlusPhysicalAttackPower;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemLevelUpTableInfo)
	int32					PlusMagicAttackPower;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemLevelUpTableInfo)
	int32					PlusArmor;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemLevelUpTableInfo)
	int32					PlusMagicResistance;

	// ?
	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemLevelUpTableInfo)
	//int32					AddLevelExp;


	static FItemLevelUpTableInfo* LookUp(EItemEquipTypeEnum EquipType, EItemGradeEnum Grade, int32 Level);

	static void ApplyItemLevel(const FItemTableInfo* ItemInfo, const FITEM* ItemData, TArray<int32>& BasicOptions);
};
