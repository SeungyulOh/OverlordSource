 // Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "UI/UIDefs.h"
#include "CustomStructures/CharacterBattleData.h"
#include "Character/BattleBaseCharacter.h"
#include "Item/WeaponBase.h"
#include "CharacterTableInfo.generated.h"



/**
*
*/
USTRUCT(BlueprintType)
struct PROJECTR_API FCharacterTableInfo : public FCharacterBattleData
{
	GENERATED_USTRUCT_BODY()


public:
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FName		DisplayName;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FName		Nick;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FName		Summary;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FName		Desc;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	EClassTypeEnum		Class = EClassTypeEnum::VE_Tanker;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	float		ModelScale = 1.0f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	float		UnitHeight = 0.0f;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FName		ActiveSkill01;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FName		ActiveSkill02;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FName		ActiveSkill03;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FName		ActiveSkill04;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FName		PassiveSkill01;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FName		PassiveSkill02;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FName		SpiritSkill01;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FName		SpiritSkill02;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FName		SpiritSkill03;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FName		SpiritSkill04;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterTable)
	ECounterType					CounterType = ECounterType::END;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterTable)
	FName							AttributeSkillString;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterTable)
	FName							AttributeSynergyString;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterTable)
	float								WeaponScale = 1.0f;

	// Right weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterTable)
	TAssetSubclassOf< AWeaponBase >		RWeaponType = nullptr;

	// Left weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterTable)
	TAssetSubclassOf< AWeaponBase >		LWeaponType = nullptr;

	// projectile info
 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterTable)
 	FName							BulletID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FName							HitEffect;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	TAssetSubclassOf< ABattleBaseCharacter >		HeroClass = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	TAssetSubclassOf< ABattleBaseCharacter >		LobbyHeroClass = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	TAssetPtr< UMaterialInstance >		Material0 = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	TAssetPtr< UMaterialInstance >		Material1 = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	TAssetPtr< UMaterialInstance >		Material2 = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	TAssetPtr< UMaterialInstance >		Material3 = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FName							PortraitTextureKey;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FName							RoundPortraitTextureKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterTable)
	EContinentTypeEnum				Continent = EContinentTypeEnum::VE_None;

	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterTable)
	int32							Sorting_Order = 99;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterTable)
	EUIMenuIDEnum					AcquisitionCondition = EUIMenuIDEnum::VE_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterTable)
	FName							Currency1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterTable)
	int32							Currency1Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterTable)
	FName							Currency2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterTable)
	int32							Currency2Value;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterTable)
	FName							CampaignChapterKey;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterTable)
	bool							HeroEpisode = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterTable)
	bool							IsEnchantable = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterTable)
	bool							IsHero = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterTable)
	FName							GrowthTableID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterTable)
	FName							GrowthMultiplierID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterTable)
	FName							SpiritPassive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterTable)
	EItemEquipTypeEnum				EquipType = EItemEquipTypeEnum::VE_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CharacterTable)
	EItemEquipArmorTypeEnum			EquipArmorType = EItemEquipArmorTypeEnum::VE_EveryOne;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	bool					RenewalNormalAttack = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	float					NormalAttackTime = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	float					NormalAttackHitTime = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	float					NormalAttackDelayTime = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	int32					NormalAttackAnimCount = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	float					Attack1Time = 0.0f;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	float					Attack2Time = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	float					AttackEnd1AniDelay = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	float					AttackEnd2AniDelay = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	float					AttackEnd3AniDelay = 0.0f;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	TAssetPtr< USkeletalMesh >		DecoMesh;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	TAssetPtr< UParticleSystem >	DecoParticle = nullptr;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FName							DecoParticleSocket;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	TAssetPtr< UMaterialInstance >	MeshMaterial;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	TAssetPtr< UMaterialInstance >	MeshMaterial2;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	ENormalAttackTypeEnum			MaxNormalAttackType = ENormalAttackTypeEnum::VE_NormalAttack02;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	ESpawnTypeEnum					SpawnType = ESpawnTypeEnum::VE_SpawnNone;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FName							SpawnSequence;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	EAnimMontageEventDie			DeadType = EAnimMontageEventDie::VE_AnimMax;

	//////////////////////////////////////////////////////////////////////////

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FName							AiAsset;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FName							AIStrategy;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FName							TooltipType;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	TArray<FName>					Tooltip_Weak;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	TArray<FName>					Tooltip_Immune;
	
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	TArray<FName>					Tooltip;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FName							RuneCardImage;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FName							CrewSpot;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	int32							Grade = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	int32							MaxLevel = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	int32							Level = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FName							UpgradeID;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	int32							SkillLevelUpUnlock;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = CharacterTable)
	FName							SkillLevelUpUnlockString;

	float		GetFloatPropertyValue(FString PropertyName);

	UClass*		GetHeroBlueprintClass();
	UClass*		GetLobbyHeroBlueprintClass();
	UClass*		GetDefaultWeaponBlueprintClass( bool IsRWeapon = true);

	void		ResetLoadedAssetPtr();
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCharacterCostUpgrade : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostUpgrade)
	int32		HeroUpgradeMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostUpgrade)
	int32		Gold;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostUpgrade)
	EUpgradeCondition		CharacterCondition1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostUpgrade)
	FString					CharacterValue1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostUpgrade)
	int32					CharacterGrade1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostUpgrade)
	int32					CharacterNum1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostUpgrade)
	EUpgradeCondition		CharacterCondition2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostUpgrade)
	FString					CharacterValue2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostUpgrade)
	int32					CharacterGrade2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostUpgrade)
	int32					CharacterNum2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostUpgrade)
	EUpgradeCondition		CharacterCondition3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostUpgrade)
	FString					CharacterValue3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostUpgrade)
	int32					CharacterGrade3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostUpgrade)
	int32					CharacterNum3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostUpgrade)
	EUpgradeCondition		CharacterCondition4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostUpgrade)
	FString					CharacterValue4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostUpgrade)
	int32					CharacterGrade4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostUpgrade)
	int32					CharacterNum4;

	TArray<EUpgradeCondition>		CharacterCondition;
	TArray<FString>					CharacterValue;
	TArray<int32>					CharacterGrade;
	TArray<int32>					CharacterNum;

	void				DataSetting();
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCharacterCostGrowth : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostGrowth)
	int32		HeroLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostGrowth)
	int32		Cost_HeroRune;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostGrowth)
	int32		Cost_Gold;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCharacterGrind : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_FCharacterGrind)
	int32		HeroGrade;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_FCharacterGrind)
	int32		HeroRune_Base;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_FCharacterGrind)
	int32		HeroRune_Multiply;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_FCharacterGrind)
	int32		SPIRIT_STONE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_FCharacterGrind)
	FName		Wallet_Enum;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_FCharacterGrind)
	int32 Wallet_Amount;

	virtual void OnPostDataImport(const UDataTable* InDataTable, const FName InRowName, TArray<FString>& OutCollectedImportProblems);
};

// USTRUCT(BlueprintType)
// struct PROJECTR_API FHeroEnchant : public FTableRowBase
// {
// 	GENERATED_USTRUCT_BODY()
// 
// public:
// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FHeroEnchant)
// 	EClassTypeEnum		Class = EClassTypeEnum::VE_Tanker;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroEnchant)
// 	int32 HeroEnchantPointGrade;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroEnchant)
// 	int32 PhysicalAttackPower;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroEnchant)
// 	int32 MagicAttackPower;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroEnchant)
// 	int32 Armor;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroEnchant)
// 	int32 MagicResistance;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroEnchant)
// 	int32 MaxHP;
// };

USTRUCT(BlueprintType)
struct PROJECTR_API FHeroEnchant_V2 : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroEnchant)
		int32 PhysicalAttackPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroEnchant)
		int32 MagicAttackPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroEnchant)
		int32 Armor;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroEnchant)
		int32 MagicResistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FHeroEnchant)
		int32 MaxHP;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FCharacterCostEnchant : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostEnchant)
	int32 EnchantPoint_Min;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostEnchant)
	int32 EnchantPoint_Max;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostEnchant)
	int32 SpiritGrade1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostEnchant)
	int32 SpiritGrade2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostEnchant)
	int32 SpiritGrade3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostEnchant)
	int32 SpiritGrade4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostEnchant)
	int32 SpiritGrade5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostEnchant)
	int32 SpiritGrade6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostEnchant)
	FName Cost_WalletId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = DT_CharacterCostEnchant)
	int32 Cost_Value;
};