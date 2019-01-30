// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UI/Avatar/Item/UC_AvatarSkill.h"
#include "UC_AvatarSkillInfo.generated.h"

class URGameInstance;
class URRichTextBlock;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_AvatarSkillInfo : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void SetInfo(UUC_AvatarSkill* InSelectSkill);
	void SetEmpty();
	

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarSkillInfo|Ref")
	UTextBlock*				Text_ButtonMessage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarSkillInfo|Ref")
	UTextBlock*				Text_APCurrent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarSkillInfo|Ref")
	UTextBlock*				Text_APMax = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarSkillInfo|Ref")
	UTextBlock*				Text_APUse = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarSkillInfo|Ref")
	UTextBlock*				Text_LevelCurrent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarSkillInfo|Ref")
	UTextBlock*				Text_LevelNext = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarSkillInfo|Ref")
	URRichTextBlock*		Text_DescCurrent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarSkillInfo|Ref")
	URRichTextBlock*		Text_DescNext = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarSkillInfo|Ref")
	UImage*					Image_SPIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarSkillInfo|Ref")
	UTextBlock*				Text_SPValue = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarSkillInfo|Ref")
	UButton*				Button_Up = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarSkillInfo|Ref")
	UButton*				Button_Reset = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarSkillInfo|Ref")
	UWidgetSwitcher*		Switcher_Info = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarSkillInfo|Ref")
	UTextBlock*				Text_Empty = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarSkillInfo|Ref")
	UOverlay*				Overlay_Use = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarSkillInfo|Ref")
	UTextBlock*				Text_Property1 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarSkillInfo|Ref")
	UTextBlock*				Text_Property2 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarSkillInfo|Ref")
	UTextBlock*				Text_Property3 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarSkillInfo|Ref")
	UTextBlock*				Text_Property4 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarSkillInfo|Ref")
	UTextBlock*				Text_Property5 = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_AvatarSkillInfo|Ref")
	UTextBlock*				Text_Property6 = nullptr;

private:
	UFUNCTION()
	void ClickUpButton();

	UFUNCTION()
	void ShowResetPopup();

	UFUNCTION()
	void ClickResetButton();

	UFUNCTION()
	void Refresh();

	FText GetPropertyName(int32 InIndex);
	void ShowSkillUpSplashPopup();

private:
	UPROPERTY()
	URGameInstance* RGameInstance = nullptr;
	
	UPROPERTY()
	TArray<UTextBlock*> PropertyArray;

	UPROPERTY()
	EAvatarAbilityType AbilityType;

	int32	Index = -1;
	int32	UsePoint = -1;
	int32	MaxPoint = -1;
	int32	APCurrentValue = -1;
	int32	UseGold = -1;
	int32	NextLevel = 0;
//	int32   ResetGems = -1;
	int32	PrevValue = -1;
	int32	NextValue = -1;

	bool	IsSplashPopup = false;
};
