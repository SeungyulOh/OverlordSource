// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"

#include "UI/Raid/UP_RaidPartyRoom.h"

#include "UC_Colosseum_Deck.generated.h"

UCLASS()
class PROJECTR_API UUC_Colosseum_Deck : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION()
	void InitPlayer(FRTS_PLAYER InPlayer, bool bIsMyDeck = false);
	UFUNCTION()
	void ReadyHeroSlot();
	UFUNCTION()
	void OnReceiveHero(FCSDeckSelected InReceiveHero);
	UFUNCTION()
	void FrezeeHeroSlot();
	UFUNCTION()
		void HideNotiImage();
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Colosseum_Deck)
	UWidgetSwitcher*					Team_Switcher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Colosseum_Deck)
	TArray<UUC_RaidPartyDeckBtn*>		BlueDeck;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Colosseum_Deck)
	TArray<UUC_RaidPartyDeckBtn*>		RedDeck;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = UUC_Colosseum_Deck)
	FDelegate_OnClickedHeroSlot		OnClickedHeroSlot;
	UPROPERTY()
	FRTS_PLAYER						mPlayer;
	bool							mIsMyDeck;
	bool							mReady;
	int32							mKID = -1;
};
