// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"

#include "UC_HUD_PVP_TotalHP.generated.h"

const int TEAM_COUNT = 2;
const float HP_CHANGE_TIME = 0.5f;
const int32 HP_CHANGE_SPEED_MIN = 1000;

UCLASS()
class PROJECTR_API UUC_CS_Topbar : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	UFUNCTION()
	void Callback_CSPointChanged(int32 MyPoint, int32 EnemyPoint);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_CS_Topbar")
	class UTextBlock* MyAvatarName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_CS_Topbar")
	class UTextBlock* EnemyAvatarName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_CS_Topbar")
	class UTextBlock* MyPointText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_CS_Topbar")
	class UTextBlock* EnemyPointText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_CS_Topbar")
	class UProgressBar* MyTargetProgressbar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_CS_Topbar")
	class UProgressBar* EnemyTargetProgressbar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_CS_Topbar")
	class UProgressBar* MyCurrentProgressbar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_CS_Topbar")
	class UProgressBar* EnemyCurrentProgressbar;

	float Duration = 0.5f;
	float MyElapsedTime = 0.f;
	float MyInitialPercent = 0.f;

	float EnemyElapsedTime = 0.f;
	float EnemyInitialPercent = 0.f;

};


UCLASS()
class PROJECTR_API UUC_HUD_PVP_TotalHP : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	// BlueprintImplementableEvent
	UFUNCTION()
	void BP_Init(bool bMyTeamLeft = true);

	void InitAvatarInfo(const FString& nick, const FString& guild, bool team_0);
	void InitHP(int32 curr_hp, int32 max_hp, bool team_0);
	void PrepareWidget();
	void UpdateHPBar();
	void SetTextBlockPoint(UTextBlock* curr_HP, UTextBlock* max_HP, UTextBlock* percent, bool team_0);

	void PointUpdate(int32 curr_hp, bool team_0);

	UFUNCTION(BlueprintCallable, Category = UUC_HUD_PVP_TotalHP)
	void SetAvatarInfo(UTextBlock* nick_name, UTextBlock* guild_name, bool team_0);

	UFUNCTION(BlueprintCallable, Category = UUC_HUD_PVP_TotalHP)
	void SetTextBlockHP(UTextBlock* curr_HP, UTextBlock* max_HP, UTextBlock* percent, bool team_0);

	UFUNCTION(BlueprintCallable, Category = UUC_HUD_PVP_TotalHP)
	void SetProgressBarHP(UProgressBar* back_bar, UProgressBar* real_bar, bool team_0);

	UFUNCTION(BlueprintCallable, Category = UUC_HUD_PVP_TotalHP)
	bool IsChangedHP(bool team_0) { if (team_0) { return HPData[0].ChangedHP; } else { return HPData[1].ChangedHP; } }

	void ChangeHP(int32 curr_hp, int32 max_hp, bool team_0);

private:
	bool SmoothlyChangeHP(int32& prev_hp, int32 curr_hp, float delta_time);

private:
	FString			NickName[TEAM_COUNT];
	FString			GuildName[TEAM_COUNT];

	FPvPTotalHPData	HPData[TEAM_COUNT];
public:
	UPROPERTY()
	bool			isMyTeamLeft;
};
