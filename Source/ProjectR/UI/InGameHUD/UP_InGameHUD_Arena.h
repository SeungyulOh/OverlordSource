// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "PVP/UC_HUD_Header_Areana.h"
#include "UP_InGameHUD_Arena.generated.h"


class URGameInstance;
class UPlayStateInfo;
class UUC_HUD_Icon_Hero_Arena;
class UUC_HUD_PVP_TotalHP;
class ABaseCameraActor;
class URCameraIcon;
class URToggleCameraIcon;
class URBattleOptionMenu;
class UUC_HUD_KillNotify;
struct FBATTLE_USER;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_InGameHUD_Arena : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;

	void Init();

	UFUNCTION(BlueprintCallable, Category = UUP_InGameHUD_Arena)
	void InitMyHeroIcons();

	UFUNCTION(BlueprintCallable, Category = UUP_InGameHUD_Arena)
	void InitTargetHeroIcons();

	UFUNCTION()
	void PrepareWidget();

	void UpdateMyTotalHP(int32 curr_hp, int32 max_hp, float percent);
	void UpdateEnemyTotalHP(int32 curr_hp, int32 max_hp, float percent);

	UFUNCTION(BlueprintCallable, Category = UUP_InGameHUD_Arena)
	void SetToggleCameraIcon(int32 Index);

	UFUNCTION()
	void OnClickCameraMode(int32 Index);

	void ResetCameraMode();
	void SetArenaCamera();

	void IsPrevProcess();


	TArray<AActor*>& GetUDList();
	TArray<AActor*>& GetEnemyUDList();
	int32 GetMaximumTotalHP();
	int32 GetTargetMaximumTotalHP();
	void GetMatchTargetInfo(FBATTLE_USER& target_info);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_InGameHUD_Arena)
	URGameInstance*			RGameInstance = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_InGameHUD_Arena)
	UPlayStateInfo*			PlayStateInfo = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_InGameHUD_Arena)
	FString					AvataName_FriendValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_InGameHUD_Arena)
	FString					AvataName_EnemyValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_InGameHUD_Arena)
	FString					AvataLv_FriendValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_InGameHUD_Arena)
	FString					AvataLv_EnemyValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_InGameHUD_Arena)
	URToggleCameraIcon*		UC_HUD_ToggleCamera;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_InGameHUD_Arena)
	URBattleOptionMenu*		UC_HUD_Option = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_InGameHUD_Arena)
	TArray<UUC_HUD_Icon_Hero_Arena*>	MyIconHeroArenaArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_InGameHUD_Arena)
	TArray<UUC_HUD_Icon_Hero_Arena*>	EnemyIconHeroArenaArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_InGameHUD_Arena)
		UUC_HUD_Header_Areana*	UC_TotalHP;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_InGameHUD_Arena)
	UUC_HUD_KillNotify*	UC_HUD_PVP_Kill;

public:
	bool	PlaySequence(FName SequenceID);
	void	StopSequence();
	void	ChangeDefaultCamera();

private:
	EArenaCameraMode					CameraMode;

	UPROPERTY()
	TArray<FName>						SequenceCamNames;


	EGamePlayModeTypeEnum				CurrentGamePlayMode = EGamePlayModeTypeEnum::VE_None;
	

};
