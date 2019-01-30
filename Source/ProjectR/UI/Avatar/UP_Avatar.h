// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UP_Avatar.generated.h"

class UCheckBox_Radio;
class UUC_PopupAvatarSkillUp;
class UUC_AvatarSkill;
class URGameInstance;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_Avatar : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	
	UFUNCTION(BlueprintCallable, Category = UP_Avatar)
	void SelectMenu(/*int InIndex, */bool InChecked);

	UFUNCTION()
	void OnClick_SkillIcon(UUC_AvatarSkill* InSelectSkill);
	UFUNCTION()
	void OnRefreshSkillInfo(UUC_AvatarSkill* InSelectSkill);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_Avatar|Ref")
	UCheckBox_Radio*		Radio_Profile = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_Avatar|Ref")
	UCheckBox_Radio*		Radio_Ability = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_Avatar|Ref")
	UCheckBox_Radio*		Radio_Icon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_Avatar|Ref")
	UWidgetSwitcher*		PageSwitcher = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_Avatar|Ref")
	UButton*				Button_Setting = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_Avatar|Ref")
	UUC_PopupAvatarSkillUp*	Avatar_Popup_LevelUP = nullptr;

private:
	UFUNCTION()
	void OnClick_Setting();

private:
	UPROPERTY()
	URGameInstance* RGameInstance = nullptr;

	UPROPERTY()
	TArray<UCheckBox_Radio*>	MenuArray;

	UPROPERTY()
	int32			SelectMenuNumber;
};
