// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "Network/PacketDataStructures.h"
#include "SharedConstants/GlobalConstants.h"
#include "UserInfo.generated.h"

//class URGameInstance;
class UBattleEntityComponent;
class USkillEntityComponent;

/**
 * 
 */
UCLASS()
class PROJECTR_API UUserInfo : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnyWhere, BlueprintReadOnly, Category = UserInfo)
	int32					kId = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UserInfo)
	FAVATAR					Avatar;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UserInfo)
	int32					StaminaMax = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UserInfo)
	int32					StaminaPVPMax = 0;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UserInfo)
	int32					AvatarExpMax = 0;

// 	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = UserInfo)
// 	FAVATAR					PrevAvatar;

	UPROPERTY()
	TArray<FACCOUNT_RECORD>	AccountRecord;

	UPROPERTY()
	TArray<FHERO_USAGE_TAB> HeroUsageTab;

	UPROPERTY()
	TArray<int32>			AvatarIconList;

// 	CAMPAIGN = 1,
// 	HERO_EPISODE = 2,
// 	RAID = 3,
// 	CRUSADER = 4,
// 	CHALLENGE = 5,
// 	COLOSSEUM = 6,

	UPROPERTY()
	TMap<FString, FHERO_CONFIG>		HeroCampaignConfigMap;
// 	UPROPERTY()
// 	TMap<FString, FHERO_CONFIG>		HeroHeroEpisodeConfigMap;
	UPROPERTY()
	TMap<FString, FHERO_CONFIG>		HeroRaidConfigMap;
	UPROPERTY()
	TMap<FString, FHERO_CONFIG>		HeroCrusaderConfigMap;
	UPROPERTY()
	TMap<FString, FHERO_CONFIG>		HeroWeekDungeonConfigMap;
	UPROPERTY()
	TMap<FString, FHERO_CONFIG>		HeroColosseumConfigMap;

public:
	void	Initialize(/*URGameInstance* InRGameInstance*/);
	void	SetAvatarData(FAVATAR& avatar);
	void	SetAvatarProfile(TArray<FACCOUNT_RECORD>& InRecord, TArray<FHERO_USAGE_TAB>& InHeroUsageTab);
	void	SetAvatarIconList(TArray<int32>& InIconList);
	void	CleanAvatarAccountInfo();
	bool	IsEnoughPVPStamina(FName TableKey);
		
	bool	GetCurrentPlayHeroesUDsByDeckUsingType(TArray<FString>& InPlayHeroUDs);
	bool	GetPlayHeroesUDsByDeckUsingType(EDeckUsingTypeEnum TargetType, TArray<FString>& InPlayHeroUDs);

//	int32	GetUserCurrency(EREWARD_TYPE InCurrencyType);
	int32	GetQuestState(int32 sequence);

// 	UFUNCTION(BlueprintCallable, Category = UserInfo)
// 	void	UpdateCharacterStone(FName CharacterName, int32 Value);
// 	
// 	UFUNCTION(BlueprintCallable, Category = UserInfo)
// 	int32	GetCharacterStone(FName CharacterName);

	void	UpdateHeroConfig(TArray<FHERO_CONFIG> InHeroConfigList);
	bool	GetHeroConfig(ECONTENTS_TYPE InContents, FString InHeroUD, FHERO_CONFIG& outHeroConfig);
	void	ApplyHeroConfig(EGamePlayModeTypeEnum ContentType, FString HeroUD, UBattleEntityComponent* BattleComp, USkillEntityComponent* SkillComp);


	//Setter & Getter
	FORCEINLINE void						SetCurrentDeckUsingType(EDeckUsingTypeEnum InType)	{ CurrentDeckUsingType = InType; }
	FORCEINLINE EDeckUsingTypeEnum			GetCurrentDeckUsingType()							{ return CurrentDeckUsingType; }
	FORCEINLINE FAVATAR&					GetAvatarData()										{ return Avatar; }
	FORCEINLINE TArray<FACCOUNT_RECORD>&	GetAccountRecord()									{ return AccountRecord; }
	FORCEINLINE TArray<FHERO_USAGE_TAB>&	GetHeroUsage()										{ return HeroUsageTab; }
	FORCEINLINE TArray<int32>&				GetIconList()										{ return AvatarIconList; }
	FORCEINLINE int32						GetkId()											{ return kId; }
	FORCEINLINE void						SetkId(int32 InValue)								{ kId = InValue; }
	
	FORCEINLINE bool						GetIsTreasureMap()									{ return IsTreasureMap; }
	FORCEINLINE void						SetIsTreasureMap(bool InMap)						{ IsTreasureMap = InMap; }
	FORCEINLINE bool						GetIsTreasureKey()									{ return IsTreasureKey; }
	FORCEINLINE void						SetIsTreasureKey(bool InKey)						{ IsTreasureKey = InKey; }
	FORCEINLINE int32						GetTreasureTime()									{ return TreasureTime; }
	FORCEINLINE void						SetTreasureTime(int32 InValue)						{ TreasureTime = InValue; }
	FORCEINLINE FString&					GetTreasureID()										{ return TreasureID; }
	FORCEINLINE void						SetTreasureID(FString InId)							{ TreasureID = InId; }

public:
	UPROPERTY()
	EDeckUsingTypeEnum CurrentDeckUsingType = EDeckUsingTypeEnum::VE_Max;

// 	UPROPERTY()
// 	URGameInstance* RGameInstance = nullptr;

// 	UPROPERTY()
// 	TMap<FName, int32>	CharacterStoneMap;

// 	UPROPERTY()
// 	TMap<EDeckUsingTypeEnum, TArray<FString>> Decks;

	int32		nPageIndex = -1;
private:
	bool IsLevelUpAvatar(int32 BeforLevel, int32 AfterLevel);

private:
//	bool		bFirstInit = true;
	bool		IsTreasureMap = false;
	bool		IsTreasureKey = false;
	int32		TreasureTime = 0;
	FString		TreasureID;
};
 