// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_GuildRaid_StageButton.generated.h"



/**
*
*/
UCLASS()
class PROJECTR_API UUC_GuildRaid_StageButton : public URBaseWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = GuildRaid_StageButton)
	void	SetStageInfo(int32 InStageNO, bool InLocked);
	
	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = GuildRaid_StageButton)
	void	SetButtonLockState(bool InLocked);

	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = GuildRaid_StageButton)
	void	SelectStage(bool bSelected);

	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = GuildRaid_StageButton)
	void	OnClickStage();

	UFUNCTION(BlueprintCallable, BlueprintCallable, Category = GuildRaid_StageButton)
	void	SetStageBaseBrushFromTexture(UTexture2D* Texture, bool bMatchSize = false);

	bool	IsLocked() { return bLocked; }
// 	void	SetGradeState(EPLAY_CLEAR_GRADE InClearGrade);

	void	InitButton(int32 InDifficulty);

public:
	bool						bLocked = false;
	int32						StageNumber = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildRaid_StageButton)
	UImage*						Image_SelectedStage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildRaid_StageButton)
	UImage*						Image_StageBaseIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildRaid_StageButton)
	UImage*						Image_StageLock = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildRaid_StageButton)
	UOverlay*					Overlay_LockPanel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GuildRaid_StageButton)
	UTextBlock*					Text_StageLevel = nullptr;

};
