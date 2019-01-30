// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UP_AvatarAbility.generated.h"

class UUC_AvatarSkill;
class UUC_AvatarSkillInfo;
class UUC_CurrencyDisplay;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_AvatarAbility : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void SetActiveSubPage() override;

	void SelectIndex(int32 InIndex);
	void RefreshPopupInfo(int32 InIndex);

	UFUNCTION()
	void ShowResetPopup();

	UFUNCTION()
	void ClickResetButton();

public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarAbility|Ref")
	//URScrollView*			Scroll_Ability = nullptr;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarAbility|Ref")
	//UWidgetSwitcher*		Switcher_Info = nullptr;

// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarAbility|Ref")
// 	UUC_AvatarSkillInfo*	SkillInfo = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarAbility|Ref")
	UUC_AvatarSkill*		Avatar_Skill1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarAbility|Ref")
	UUC_AvatarSkill*		Avatar_Skill2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarAbility|Ref")
	UUC_AvatarSkill*		Avatar_Skill3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarAbility|Ref")
	UUC_AvatarSkill*		Avatar_Skill4;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarAbility|Ref")
	UUC_AvatarSkill*		Avatar_Skill5;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarAbility|Ref")
	UUC_AvatarSkill*		Avatar_Skill6;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarAbility|Ref")
	UUC_AvatarSkill*		Avatar_Skill7;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarAbility|Ref")
	UUC_AvatarSkill*		Avatar_Skill8;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarAbility|Ref")
	UUC_AvatarSkill*		Avatar_Skill9;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarAbility|Ref")
	UUC_AvatarSkill*		Avatar_Skill10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarAbility|Ref")
	UUC_AvatarSkill*		Avatar_Skill11;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarAbility|Ref")
	UUC_AvatarSkill*		Avatar_Skill12;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarAbility|Ref")
	UUC_AvatarSkill*		Avatar_Skill13;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarAbility|Ref")
	UUC_AvatarSkill*		Avatar_Skill14;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarAbility|Ref")
	UUC_AvatarSkill*		Avatar_Skill15;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarAbility|Ref")
	UUC_AvatarSkill*		Avatar_Skill16;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarAbility|Ref")
	UUC_AvatarSkill*		Avatar_Skill17;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarAbility|Ref")
	UUC_AvatarSkill*		Avatar_Skill18;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarAbility|Ref")
	TArray<UUC_AvatarSkill*> IconArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarAbility|Ref")
	UButton*				Button_Reset = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarAbility|Ref")
	ULocalizingTextBlock*	Text_APCurrent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarAbility|Ref")
	UUC_CurrencyDisplay*	ResetCurrency = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_AvatarAbility|Ref")
	UHorizontalBox*			AbilityResetBox = nullptr;
private:
	UFUNCTION()
	void Refresh();

	void Reset();
	void SetUITopBarString();

	UFUNCTION()
	void OnRequestReceive();

	FText GetPropertyName(EAvatarAbilityType InType);
	void ShowSkillUpSplashPopup(int32 InIndex);

	void SetAbilityPointUI();
private:
	UPROPERTY()
	class URGameInstance*	RGameInstance = nullptr;

	int32					CurrentIndex = -1;
};
