#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "CustomStructures/CharacterBattleData.h"
#include "Network/PacketDataStructures.h"
#include "ItemTableInfo.generated.h"

class AWeaponBase;

/**
* 오버로드_장비아이템_변경안.docs.3.(6)
* ## Battle stats relevant to items
* 
* ### Basic Stats
* PhysicalAttackPower, MagicAttackPower
* Armor, MagicResistance
*
* ## Substats
* MaxHP
* AttackRange
* MoveSpeed
* AttackSpeed
* DodgeValue
* CriticalValue
* CriticalDamageRate_Scale
* PierceValue
* AccuracyValue
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FItemTableInfo : public FCharacterBattleData
{
	GENERATED_USTRUCT_BODY()


public:
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	FName	ItemName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	int32	ItemLevel;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	EItemTypeEnum ItemType = EItemTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	EItemEquipTypeEnum	EquipType = EItemEquipTypeEnum::VE_None; // TODO: there is no None ItemEquipType

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	EItemEquipPositionEnum	EquipPosition = EItemEquipPositionEnum::VE_Weapon;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	EItemGradeEnum	ItemGrade;

	// Nullable keys to DT_ItemRandomOption
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	FName AddOption0;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	FName AddOption1;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	FName AddOption2;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	FName AddOption3;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	int32 ItemRuneSlotCount = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	EEquipItemPropertyType EquipItemPropertyType;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	FName TowerOption;
	

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	FName	ItemIcon;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "CharacterItem|None")
	bool	Grind = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	FName	RewardGrindMaterial;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	int32	MaxLevel = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "CharacterItem|None")
	bool	LevelUpPossible = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	int32	LevelUpMaterialExp = 0;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	bool	isEnchantable = false;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	int32	MaxEnchantLevel = 0;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	FName	Description; 

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	int32	SellPrice = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterItem)
	FName	TooltipType;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	FName	Tooltip;


	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	EAttackTypeEnum				EquipLimit;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	FName	UpgradeIndex;


	// Legacy item mesh stuff

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	//TAssetSubclassOf< AWeaponBase >		Main_RMeshRef = nullptr;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	//TAssetSubclassOf< AWeaponBase >		Main_LMeshRef = nullptr;
	
	//// Custom target mesh

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	//FName								Mesh_CustomTarget01;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	//TAssetSubclassOf< AWeaponBase >		CustomTarget01_RMeshRef = nullptr;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	//TAssetSubclassOf< AWeaponBase >		CustomTarget01_LMeshRef = nullptr;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	//FName								Mesh_CustomTarget02;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	//TAssetSubclassOf< AWeaponBase >		CustomTarget02_RMeshRef = nullptr;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	//TAssetSubclassOf< AWeaponBase >		CustomTarget02_LMeshRef = nullptr;
	//UClass*		GetWeaponBlueprintClass(bool IsRWeapon = true, FName InCustomTargetCharacterID = FName() );


	// 오버로드_장비아이템_변경안.docs.1.(2)
	// AttackType is DT_Character.AttackType
	//static bool CanEquip(EAttackTypeEnum AttackType, EItemEquipTypeEnum EquipType);
	static bool CanEquip(EClassTypeEnum ClassType, EItemEquipTypeEnum EquipType);

	void GetBasicOptions(TArray<int32>& OutArray) const;

	/** "전투력" */
	static int32 GetCombatPower(const FITEM* ItemData);
	
	// either additive increment or multiplicative increment with factor given as per mil
	static bool IsItemStatPerMil(EItemStatOption ItemStatOption);

	void GetAdditionalOptions(const FITEM* ItemData, TArray<int32>& OutAdditionalOptions) const;
	
	/** return of false means no additional option at this Index */
	bool GetOneAdditionalOption(const FITEM* ItemData, int32 Index, EItemStatOption& OutItemStatOption, int32& OutValue) const;

	// (design)
	static bool IsAccessory(EItemEquipPositionEnum EquipPosition);

	static int32 NumItemEquipPositions();

};

/*
* 오버로드_장비아이템_강화.docx.2 관련 테이블
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FItemEnchantTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()


public:
	//// BEGIN row index

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	EItemEquipPositionEnum EquipPosition;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	EItemGradeEnum ItemGrade;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	int32	EnchantLevel;

	//// END row index

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	//int32	AddEnchantExp;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	int32	SuccessProbability = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	int32	EnchantGoldCost = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	int32 EnchantEffect = 0;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	//int32	AddGoldCost = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	int32	SpecialEnchantPossible = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	int32	SpecialEnchant1UpRate = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	int32	SpecialEnchant2UpRate = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	int32	SpecialEnchant3UpRate = 0;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	//int32	EnchantValueMin = 0;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	//int32	EnchantValueMax = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	FName	CommonMaterial;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	int32	CommonMaterialCount = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	FName	SpecialMaterial;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	int32	SpecialMaterialCount = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = "CharacterItem|Server")
	FName	ProtectionMaterial;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterItem)
	int32	ProtectionMaterialCount = 0;



	static FItemEnchantTableInfo* LookUp(EItemEquipPositionEnum EquipPosition, EItemGradeEnum Grade, int32 EnchantLevel);

	static void ApplyEnchant(const FItemTableInfo* ItemInfo, const FITEM* ItemData, TArray<int32>& BasicOptions);
};

// item manufacturing is legacy
USTRUCT(BlueprintType)
struct PROJECTR_API FItemManufatureTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemManufatureTable)
// 	FName ItemID;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemManufatureTable|None")
	//int32 isRand = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemManufatureTable)
	FName displayedItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemManufatureTable)
	FName targetHero;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemManufatureTable)
	int32 craftableHeroLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemManufatureTable)
	int32 craftableAccountLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemManufatureTable)
	int32 IsShow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemManufatureTable)
	int32 runeCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemManufatureTable)
	int32 GoldCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemManufatureTable)
	int32 GemCost;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemManufatureTable|None")
	//int32 SpecialItemProbability;
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemManufatureTable|None")
	//int32 BonusSpecialItemProbability;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemManufatureTable)
	FName SpecialItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemManufatureTable)
	FName material1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemManufatureTable)
	int32 material1Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemManufatureTable)
	FName material2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemManufatureTable)
	int32 material2Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemManufatureTable)
	FName material3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemManufatureTable)
	int32 material3Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemManufatureTable)
	FName material4;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemManufatureTable)
	int32 material4Count;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemManufatureTable)
	FName material5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemManufatureTable)
	int32 material5Count;
};

// DT_ItemRandomOption_reformatted
USTRUCT(BlueprintType)
struct PROJECTR_API FItemRandomOptionTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemRandomOptionTableInfo)
	TArray<EItemStatOption> Options;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemRandomOptionTableInfo)
	TArray<FString> OptionStrings;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemRandomOptionTableInfo)
	TArray<int32> OptionValueBases;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemRandomOptionTableInfo)
	TArray<float> OptionValueAdds;

	static void ApplyItemRandomOptions(const FItemTableInfo* ItemInfo, const FITEM* ItemData, TArray<int32>& InOutItemStatOptions);

	bool GetOneRandomOption(int32 OptionIndex, float Factor, EItemStatOption& OutItemStatOption, int32& OutValue) const;

	static void GetOneAdditionalOption(FName RandomOptionKey, int32 OptionIndex, float Factor, TArray<int32>& OutAdditionalOptions);
};

USTRUCT(BlueprintType)
struct PROJECTR_API FRuneTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	// Rune 'Grade'
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneTable)
	int32				ID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneTable)
	FName				RuneOption_WeaponID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneTable)
	FName				RuneOption_ArmorID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneTable)
	int32				RuneReleaseCashCost;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneTable)
	int32				RuneMakeGoldCost;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneTable)
	FName				RuneInventoryImage;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneTable)
	FName				RuneMountingImage;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneTable)
	FName				RuneMaterial;

	EItemStatOption		GetAdditionalOptionStat(EItemEquipPositionEnum InEquipPosition);
	int32				GetAdditionalOptionValue(EItemEquipPositionEnum InEquipPosition);
	FText				GetAdditionalOptionFText(EItemEquipPositionEnum InEquipPosition);

	static void ApplyRunes(const FITEM* ItemData, TArray<int32>& BasicOptions);

	static int32 WalletKeyToRuneGrade(FName WalletKey);

	static FRuneTable* LookUp(int32 RuneGrade);

	// (design)
	static const int32 MaxNumSockets = 4;
	static int32 EquipPositionToNumSockets(EItemEquipPositionEnum EquipPosition);
	static int32 EquipPositionToNumSocketsByTable(const FItemTableInfo* InTableInfo);

};

USTRUCT(BlueprintType)
struct PROJECTR_API FRuneWordTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordTable)
	FName			Runeword_Name;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordTable)
	FName			Slot01;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordTable)
	FName			Slot02;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordTable)
	FName			Slot03;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordTable)
	FName			Slot04;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordTable)
	FName			RunewordSkillID;

	TArray<FName> GetRowRunes();
	int32 GetRowNumSockets();

	/**
	* returns NAME_None if not rune word
	*/
	static FName CheckForRuneWord(TArray<FName> Runes, int32 NumSockets);
};

USTRUCT(BlueprintType)
struct PROJECTR_API FRuneWordSkillTable : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	FName		SkillName;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	FName		Description;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	FName		Summary;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	FName		SkillResourceID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	ESkillAniTypeEnum		SkillAniType = ESkillAniTypeEnum::VE_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneWordSkill)
	ESkillControlTypeEnum	SkillControlType = ESkillControlTypeEnum::VE_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneWordSkill)
	ESkillAttributeEnum		SkillAttribute = ESkillAttributeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	ESkillCastingTypeEnum	CastingType = ESkillCastingTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	float					CastingTime = 0.3f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	float					CoolTime = 1.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	float					CoolTimeAdd1 = 1.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	float					CoolTimeAdd2 = 1.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	float					CrewCoolTime = 1.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	float					Interval = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	int32					CastingRange = 100;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	EAbnormalApplyTypeEnum	CasterAbnormalType = EAbnormalApplyTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	EAbnormalGroupTypeEnum	CasterGroupType = EAbnormalGroupTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	EAbnormalSlotTypeEnum	CasterSlotType = EAbnormalSlotTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	int32					CasterSlotLevel = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	float					CasterDuration = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	ESkillApplierTypeEnum		CasterEffect1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	float		CasterPercent1 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	int32		CasterValue1 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	float		CasterMovingDuration = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	ESkillApplierTypeEnum		CasterEffect2;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	float		CasterPercent2 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	int32		CasterValue2 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	ESkillApplierTypeEnum		CasterEffect3;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	float		CasterPercent3 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	int32		CasterValue3 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	ESkillApplierTypeEnum		CasterEffect4;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	float		CasterPercent4 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	int32		CasterValue4 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	ESkillApplierTypeEnum		CasterEffect5;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	float		CasterPercent5 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	int32		CasterValue5 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	ESkillApplierTypeEnum		CasterEffect6;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	float		CasterPercent6 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	int32		CasterValue6 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	EAbnormalApplyTypeEnum	TargetAbnormalType = EAbnormalApplyTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	EAbnormalGroupTypeEnum	TargetGroupType = EAbnormalGroupTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	EAbnormalSlotTypeEnum	TargetSlotType = EAbnormalSlotTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	int32					TargetSlotLevel = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	float					TargetDuration = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	ESkillApplierTypeEnum		TargetEffect1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	float		TargetPercent1 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	int32		TargetValue1 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	float		TargetMovingDuration = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	ESkillApplierTypeEnum		TargetEffect2;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	float		TargetPercent2 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	int32		TargetValue2 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	ESkillApplierTypeEnum		TargetEffect3;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	float		TargetPercent3 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	int32		TargetValue3 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	ESkillApplierTypeEnum		TargetEffect4;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	float		TargetPercent4 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	int32		TargetValue4 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	ESkillApplierTypeEnum		TargetEffect5;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	float		TargetPercent5 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	int32		TargetValue5 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	ESkillApplierTypeEnum		TargetEffect6;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	float		TargetPercent6 = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	int32		TargetValue6 = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	int32		ApplyProcedure = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	ESkillTargetTypeEnum		TargetType = ESkillTargetTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	EFriendshipTypeEnum	TargetApply = EFriendshipTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	EPriorityTypeEnum		AISearchTargetType;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	ESearchShapeTypeEnum		EffectShape = ESearchShapeTypeEnum::VE_None;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	TArray<float>				ShapeScope;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	float						SummonTime;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	FName AICondition1;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	FName AICondition2;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	FName AICondition3;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	float AIForceUseOverTime;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	float StartCoolTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneWordSkill)
	ESkillShapedIconType	UISkillShape = ESkillShapedIconType::VE_Me;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneWordSkill)
	int32					UISkillRange = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneWordSkill)
	bool					VisibleDecal = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneWordSkill)
	bool					VisibleDPAD = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = RuneWordSkill)
	bool					VisibleNotifyIcon = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	float CombineInputRange;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	float JustInputStart;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = RuneWordSkill)
	float JustInputEnd;
};


USTRUCT(BlueprintType)
struct PROJECTR_API FItemUpgradeCostInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	// BEGIN Index
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemUpgradeCostInfo)
	EItemGradeEnum ItemGrade;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemUpgradeCostInfo)
	int32 ItemLevel;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemUpgradeCostInfo)
	EItemEquipTypeEnum EquipType;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemUpgradeCostInfo)
	int32 Material_EquipAmount = 2;
	
	// END
	
	/** => DT_Wallet */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemUpgradeCostInfo)
	FName Material_PropertyIndex = NAME_None;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemUpgradeCostInfo)
	int32 Material_PropertyAmount = 0;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemUpgradeCostInfo)
	int32 Gold = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemUpgradeCostInfo)
	int32 ItemUpgradeProbability = 0;

	/** (design) Num of required equipment materials for ItemUpgrade and ItemOptionReset is fixed to 2. */
	

	static FItemUpgradeCostInfo* LookUp(EItemGradeEnum InItemGrade, EItemEquipTypeEnum InEquipType, int32 InItemLevel);

	// (design)
	static bool CanUpgrade(EItemGradeEnum InCurrentGrade);
	static bool CanResetOption(EItemGradeEnum InCurrentGrade);
};

/**
 * DT_TowerOption.xlsm
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FTowerOptionInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemUpgradeCostInfo)
	int32 ActivateFloor = 0; // ActivatingFloor

	//// Not certain the meaning of the below

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemUpgradeCostInfo)
	FName TowerSkillIndex01 = NAME_None;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemUpgradeCostInfo)
	FName TowerSkillIndex02 = NAME_None;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemUpgradeCostInfo)
	FName TowerSkillIndex03 = NAME_None;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemUpgradeCostInfo)
	FName TowerSkillIndex04 = NAME_None;
};

/**
 * DT_HeroPiece.xlsm
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FHeroPieceInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemUpgradeCostInfo)
	int32 HeroPiece_UseAmount = 0;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemUpgradeCostInfo)
	int32 Grade = 0; // seems to be redundant, what with DT_Wallet.Grade
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemUpgradeCostInfo)
	FName TowerSkillIndex01 = NAME_None;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemUpgradeCostInfo)
	FName TowerSkillIndex02 = NAME_None;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemUpgradeCostInfo)
	FName TowerSkillIndex03 = NAME_None;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = ItemUpgradeCostInfo)
	FName TowerSkillIndex04 = NAME_None;
};
