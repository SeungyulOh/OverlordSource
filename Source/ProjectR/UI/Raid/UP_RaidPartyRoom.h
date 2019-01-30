// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "UI/UIDefs.h"
#include "UI/RBaseWidget.h"
#include "Network/PacketDataStructures.h"
#include "UI/Control/RBaseButton.h"
#include "UI/Common/HeroBaseScrollItem.h"
#include "UP_RaidPartyRoom.generated.h"

class URScrollView;
class URenewal_BaseHeroIcon;
class UUC_HeroManagement_HeroList;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_OnClickedHeroSlot, EPartyRole, InPartyRole);

UCLASS()
class PROJECTR_API UUC_RaidPartyDeckBtn : public URBaseButton
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void Init(EPartyRole InPartyRole);
	void ReadyHeroSlot();
	void FrezeeHeroSlot();
	void ReceiveHero(FString HeroID, FString HeroUD, int16 level);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = UUC_RaidPartyDeckBtn)
	FDelegate_OnClickedHeroSlot		OnClickedHeroSlot;

	UFUNCTION()
	void OnClickedThis();

	EPartyRole mPartyRole = EPartyRole::None;

	UPROPERTY()
	FString					mHeroID;
	UPROPERTY()
	FString					mHeroUD;
	UPROPERTY()
	int16					mHeroLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RaidPartyDeckBtn)
	bool					m_bMyHero = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RaidPartyDeckBtn)
	bool					m_bShowSelectedMark = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RaidPartyDeckBtn)
	bool					m_bEmptyDeck = true;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RaidPartyDeckBtn)
	FHERO_ICON_INFO			m_HeroIconInfo;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RaidPartyDeckBtn)
	int32					m_HeroPower = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RaidPartyDeckBtn)
	UButton*				Select_DeckBtn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RaidPartyDeckBtn)
	URenewal_BaseHeroIcon*	DeckHeroIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RaidPartyDeckBtn)
	UWidgetSwitcher*		ButtonSwitcher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RaidPartyDeckBtn)
	UImage*					Icon_Clear;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RaidPartyDeckBtn)
	UScaleBox*				Button_ScaleBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RaidPartyDeckBtn)
	UImage*					NotiImage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RaidPartyDeckBtn)
	UOverlay*				Overlay_EquipHero;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_RaidPartyDeckBtn)
	UButton*				ButtonInput;
};

UCLASS()
class PROJECTR_API UUC_RaidPartyDeck : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void InitPlayer(FRTS_PLAYER InPlayer, bool bIsMyDeck = false);
	void ReadyHeroSlot();
	void FrezeeHeroSlot();
	void HideNotiImage();
	void OnReceiveHero(FDeckSelected InReceiveHero);
	void SetReady(bool InReady);
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = UUC_RaidPartyDeckBtn)
	FDelegate_OnClickedHeroSlot		OnClickedHeroSlot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RaidPartyRoom)
	UWidgetSwitcher*				DeckSwitcher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RaidPartyRoom)
	ULocalizingTextBlock*			nickname;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RaidPartyRoom)
	UUC_RaidPartyDeckBtn*			UC_Hero_Slot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RaidPartyRoom)
	UUC_RaidPartyDeckBtn*			UC_Soul_Slot1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RaidPartyRoom)
	UUC_RaidPartyDeckBtn*			UC_Soul_Slot2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RaidPartyRoom)
	UUC_RaidPartyDeckBtn*			UC_Soul_Slot3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RaidPartyRoom)
	UUC_RaidPartyDeckBtn*			UC_Soul_Slot4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_RaidPartyRoom)
	ULocalizingTextBlock*			LocalTextBlockReady;
	UPROPERTY()
	FRTS_PLAYER						mPlayer;
	bool							mIsMyDeck;
	bool							mReady;
	int32							mKID = -1;
};

