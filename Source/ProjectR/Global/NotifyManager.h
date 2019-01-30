// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "Network/PacketDataStructures.h"
#include "NotifyManager.generated.h"

/**
 * 

 !!!!!!!! Check calling order!!!!!
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FDelegate_OnNotify, ENotifyStateFlagEnum, StateFlag, bool, bState);


UCLASS(BlueprintType)
class PROJECTR_API UNotifyManager : public UObject
{
	GENERATED_BODY()
		
public:
	void BeginDestroy() override;

	UFUNCTION(BlueprintCallable, Category = NotifyManager)
	bool	IsNotify(ENotifyStateFlagEnum notifyState);

	UFUNCTION(BlueprintCallable, Category = NotifyManager)
	void	SetNotify(ENotifyStateFlagEnum notifyState, bool bState);

	void	Initialize(/*URGameInstance* InRGameInstance*/);

	//void	CallEveryNotify();

public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = NotifyManager)
	FDelegate_OnNotify	OnNotifyDelegate;

	void	Init();
	void	CheckLobby();
	bool	CheckHeroNew();
	bool	CheckHeroSkillUpgradeState();
	bool	CheckHeroUpgradeItem();
	bool	CheckHeroLevelUp();
	void	CheckHeroSkillUpgradeEach(FString heroUD);
	void	CheckHeroUpgradeItemEach(FString heroUD);
	void	CheckHeroLevelEach(FString heroUD);
	bool	CheckHeroNotify(FString InHeroUD);
	bool	CheckItemManufacture();
	bool	CheckAchievement();
	UFUNCTION()
	bool	CheckDailyMission();
	bool	CheckAdventure();

	bool	IsCheckHeroSkillUpgrade(FString heroUD);
	bool	IsCheckSkillUpgrade(FHERO* InHero, int32 idx);
	bool	IsCheckHeroLevelUp(FHERO* Inhero, bool bAllLevelUp);
	

	//// Used for updating notify widgets in Item Management

	// Returns true if there is an item in the inventory that the hero can equip anew or upgrade from any previously equipped item
	bool	IsCheckHeroUpgradeItem(FString heroUD);
	// find an item in the inventory that can be equipped in the same position as the given item and has higher ItemValue
	bool	IsCheckHeroItemEach(FString sourceUD, FITEM& outItem);
	// Return true if the player has item that the hero can equip in the equip position
	bool	IsCheckHeroEquipHaveItem(FName InHeroID, int32 InEquipPosition);
	// find an item in the inventory that can be equipped in the same position as the given item and has higher ItemValue, return true if found
	// Not sure how this differs from IsCheckHeroItemEach...
	bool	GetUpAbilityItem(FString InItemUD, FITEM& outItem);
	// Find the item with the highest ItemValue that the hero can equip in the equip position
	bool	GetUpAbilityItemHave(FName InHeroID, int32 InEquipPosition, FITEM& outItem);
	float	GetItemValue(FITEM& sourceItem);

private:
	UNotifyManager();

	void	SetNotifyExe(ENotifyStateFlagEnum notifyState, bool bState);

private:
// 	UPROPERTY()
// 	URGameInstance*			RGameInstance = nullptr;

	UPROPERTY()
	TArray<bool>			NotifyList;

	bool					bFirstCheck = true;
};
