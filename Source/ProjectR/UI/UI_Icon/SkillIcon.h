// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "Table/SkillResourceTableInfo.h"
#include "Table/LevelAppliedTables/WorkedSkillTableInfo.h"
#include "Network/PacketDataStructures.h"
#include "SkillIcon.generated.h"

class UUC_Notify;

/**
 * 
 */
UCLASS()
class PROJECTR_API USkillIcon : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	
	void InvalidateData_SkillInfo(FString& InHeroUD, FHERO_SKILL& InHeroSkillInfo, FSkillResourceTableInfo& InSkillResourceInfo, FWorkedSkillTableInfo& InWorkedSkillTableInfo);

	void Refresh();

	UFUNCTION(BlueprintCallable, Category = USkillIcon)
	void OnClick_SkillDetail();

	UFUNCTION(BlueprintCallable, Category = USkillIcon)
	void SetNotifyState(ENotifyStateFlagEnum InNotifyState);

	FName GetSkillLevelUpKey(FString heroUD, int32 InLevel);
public:
	// UI
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = USkillIcon)
	UImage*							Image_SkillIcon;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = USkillIcon)
	UImage*							Image_SkillCircle;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = USkillIcon)
	UPanelWidget*					PanelUnlock;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = USkillIcon)
	UTextBlock*						Text_Unlocked_SkillLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = USkillIcon)
	UTextBlock*						Text_SkillLevel;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = USkillIcon)
	UTextBlock*						Text_SkillName;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = USkillIcon)
	UWidgetSwitcher*				Switcher_SkillType;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = USkillIcon)
	UPanelWidget*					PanelNeedCost_1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = USkillIcon)
	UPanelWidget*					PanelNeedCost_2;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = USkillIcon)
	UImage*							Image_NeedItem;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = USkillIcon)
	UTextBlock*						Text_NeedItemCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = USkillIcon)
	UUC_Notify*						Notify_SkillLevelUp;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = USkillIcon)
	UButton*						LevelUpBtn = nullptr;

public:
	// Data
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = USkillIcon)
	FString							OwnerHeroUD;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = USkillIcon)
	FHERO_SKILL						HeroSkillInfo;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = USkillIcon)
	FSkillResourceTableInfo			SkillResourceTable;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = USkillIcon)
	FWorkedSkillTableInfo			WorkedSkillTableInfo;

private:
	bool							bDisplay = false;
};