#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "Table/ItemTableInfo.h"
#include "Table/LevelAppliedTables/WorkedSkillTableInfo.h"
#include "WorkedItemTableInfo.generated.h"

/**
* legacy. NewDesign is FWorkedItemStats
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FWorkedItemTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FItemTableInfo		Data;
	
private:

	int32				BasePhysicalAttackPower;
	int32				BaseMagicAttackPower;
	int32				BaseArmor;
	int32				BaseMagicResistance;
	int32				BaseMaxHP;

	int32				EnchantPhysicalAttackPowerNotAddOption;
	int32				EnchantMagicAttackPowerNotAddOption;
	int32				EnchantArmorNotAddOption;
	int32				EnchantMagicResistanceNotAddOption;
	int32				EnchantMaxHPNotAddOption;

public:
	void	SetDataFromItemInfo(FItemTableInfo* itemInfo, int32 EnchantLevel, int32 InItemLevel = 1);

	void	ApplyEnchantValue(int32 EnchantLevel);

	// TODO: redo
	//void	ApplyItemOption(const FWorkedCharacterTableInfo& EquippedCharacterWorkedTableInfo, FName ItemID, TArray<int32>& Options);

	// legacy. See [서버] 영웅성장 - 영웅 레벨업 / 아이템 랜덤옵션 관련 공유
	//void	ApplyGemItemOption(const FWorkedCharacterTableInfo& EquippedCharacterWorkedTableInfo, const FITEM& Item);
	//void	SetItemGenerateFixedOption(EItemGenerateOptionType InOptionType, float InValue, int32 InEnchantValue, int32 InEnchantLevel);
	//void	SetItemGenerateOption(EItemGenerateOptionType InOptionType, float InValue);

	FORCEINLINE int32	GetBasePhysicalAttackPower() { return BasePhysicalAttackPower; }
	FORCEINLINE int32	GetBaseMagicAttackPower() { return BaseMagicAttackPower; }
	FORCEINLINE int32	GetBasePhysicalArmor() { return BaseArmor; }
	FORCEINLINE int32	GetBaseMagicResistance() { return BaseMagicResistance; }

	FORCEINLINE int32	GetEnchantPhysicalAttackPowerNotAddOption() { return EnchantPhysicalAttackPowerNotAddOption; }
	FORCEINLINE int32	GetEnchantMagicAttackPowerNotAddOption() { return EnchantMagicAttackPowerNotAddOption; }
	FORCEINLINE int32	GetEnchantArmorNotAddOption() { return EnchantArmorNotAddOption; }
	FORCEINLINE int32	GetEnchantMagicResistanceNotAddOption() { return EnchantMagicResistanceNotAddOption; }
	FORCEINLINE int32	GetEnchantMaxHPNotAddOption() { return EnchantMaxHPNotAddOption; }

	//Option Value
	int32	MoveSpeed = 0;
	int32	HealBoost = 0;
	int32	ItemLevel = 0;
private:
	//void	SetItemOptions(FItemTableInfo* itemInfo, TArray<int32>& Options);
	int32	SetEnchantValue(int32 OriginalValue, int32 EnchantLevel);
	
	void SetItemLevel();
};
