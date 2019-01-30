// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "UI/RBaseWidget.h"
#include "UP_PVP_ReadyToStart.generated.h"


class URGameInstance;
class UHeroBaseScrollItem;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_PVP_ReadyToStart : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	void PrepareWidget();

	void RTSColosseumSet(TArray<UHeroBaseScrollItem*>& HeroScrollArrayTemp);
	void ArenaAndMirrorSet(TArray<UHeroBaseScrollItem*>& HeroScrollArrayTemp);

	// Delegate Funtion
	UFUNCTION()
	void OnReadyToStartEvent();

	// BlueprintImplementableEvent
	UFUNCTION()
	void BP_ReadyToStartEvent();

	UFUNCTION()
	void EndAnimationAction();

	UFUNCTION(BlueprintCallable)
	void PauseAnim();

	UFUNCTION(BlueprintCallable, Category = UUP_PVP_ReadyToStart)
	void SetUIPvPSummary(UTextBlock* wdg_level, UTextBlock* wdg_nick, UTextBlock* wdg_power, bool my_summary);

// 	UFUNCTION(BlueprintCallable, Category = UUP_PVP_ReadyToStart)
// 	void GetPvPHeroIconInfo(TArray<FHERO_ICON_INFO>& hero_icon_infos, bool my_heroes);

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UUP_PVP_ReadyToStart)
	TWeakObjectPtr<URGameInstance> RGameInstance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_PVP_ReadyToStart)
	TArray<UHeroBaseScrollItem*> AllyHeroItemArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_PVP_ReadyToStart)
	TArray<UHeroBaseScrollItem*> EnemyHeroItemArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_PVP_ReadyToStart)
	UWidgetAnimation*			Start_VersusAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_PVP_ReadyToStart)
	UTextBlock*					TextBlock_PickTimer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_PVP_ReadyToStart)
	UTextBlock*					Text_AttackPowerLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_PVP_ReadyToStart)
	UTextBlock*					Text_AttackPowerRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_PVP_ReadyToStart)
	UWidgetSwitcher*			WidgetSwitcher_Count;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UUP_PVP_ReadyToStart)
	EGamePlayModeTypeEnum	CheckGameMode = EGamePlayModeTypeEnum::VE_None;

public:
	/*Deleting CountText*/
	float Duration = 0.f;
	/**/
};
