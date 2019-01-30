// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "Network/PacketDataStructures.h"
#include "UP_MirrorDungeonLobby.generated.h"

class UUC_MirrorDeckSlot;
class URenewal_BaseHeroIcon;
enum { eFreeIndex = 0, ePaidIndex = 1 };

#define PAID_REFRESH_PRICE 20

UCLASS()
class PROJECTR_API UUP_MirrorDungeonLobby : public URBaseWidget
{
	GENERATED_BODY()

public:

	void NativeConstruct() override;
// 	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void InvalidateData() override;


public:
	// button delegate
	UFUNCTION()
	void OnClickFreeRefreshButton();
	UFUNCTION()
	void OnClickPaidRefreshButton();
	UFUNCTION()
	void OnClickEnterButton();

	// network delegate
	UFUNCTION()
	void OnMirrorDungeunStatusRp();

	void SetMirrorSlotData(int32 InIndex, FMIRROR& InMirror);
	
	UFUNCTION()
	void OnPaidRefreshConfirmOK();
	UFUNCTION()
	void OnPaidRefreshRequest();

	UFUNCTION()
	void OnFreeRefreshConfirmOK();
	UFUNCTION()
	void OnFreeRefreshRequest();

	UFUNCTION()
	void CheckEnoughCurrencyDelay();

	UFUNCTION()
	void ShowRejectRequestPopupDelay();

	FORCEINLINE bool IsSelectSlot() { return bSelectSlot; }
private:
	void SetTime(int32 InRemainTime);

	void SetButtonText();
	void SetRefreshButtonState();
	void SetEnterButtonState();

	void InitializeMirrorLobby();
	void SetDummyHeros();

	void ResetTimeTick(float InDeltaTime);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MirrorDungeonLobby)
	TArray<UUC_MirrorDeckSlot*>				MirrorSlotList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MirrorDungeonLobby)
	TArray<URenewal_BaseHeroIcon*>			HeroList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MirrorDungeonLobby)
	ULocalizingTextBlock*					RemainTimeText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MirrorDungeonLobby)
	ULocalizingTextBlock*					SelectedDeckNickNameText = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MirrorDungeonLobby)
	ULocalizingTextBlock*					SelectedDeckPowerText = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MirrorDungeonLobby)
	UWidgetSwitcher*						RefreshButtonSwitcher = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MirrorDungeonLobby)
	UButton*								FreeRefreshButton = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MirrorDungeonLobby)
	UTextBlock*								FreeRefreshText = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MirrorDungeonLobby)
	UButton*								PaidRefreshButton = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MirrorDungeonLobby)
	UTextBlock*								PaidRefreshText = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MirrorDungeonLobby)
	UTextBlock*								NeedGemText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MirrorDungeonLobby)
	UButton*								EnterButton = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UP_MirrorDungeonLobby)
	ULocalizingTextBlock*					EnterButtonText = nullptr;


private:
	bool				bResetTimeDecreaseTick = false;
	int64				nCurrentCheckTime = 0;
	bool				bSelectSlot = false;
	bool				bCheckbeforeRequest = false;
	FTimerHandle		DelayTimer;

};