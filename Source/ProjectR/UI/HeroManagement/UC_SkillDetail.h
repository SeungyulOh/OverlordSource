// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "Table/SkillResourceTableInfo.h"
#include "Table/SkillTableInfo_V2.h"
#include "Table/LevelAppliedTables/WorkedSkillTableInfo.h"
#include "Table/LevelAppliedTables/WorkedCharacterTableInfo.h"
#include "Table/SkillLevelUpCostTableInfo.h"
#include "Table/SpiritSkill.h"
#include "Network/PacketDataStructures.h"
#include "UC_SkillDetail.generated.h"

class UUC_HeroSkill_Definition;
class UCanvasPanel_RadioGroup;


/**
 * BP: UC_Hero_Skill_Detail
 * embedded in UC_SkillManagement
 */
UCLASS()
class PROJECTR_API UUC_SkillDetail : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void InvalidateSkillDetail(const FString& InHeroUD, const FHERO_SKILL& InHeroSkillInfo, const FSkillResourceTableInfo& InSkillResourceInfo, const FSkillTableInfo_V2& InSkillInfoV2);
	void InvalidateSpiritSkillDetail(const FString& InHeroUD, const FHERO_SKILL& InHeroSkillInfo, const FSkillResourceTableInfo& InSkillResourceInfo, const FSpiritSkillTableInfo& InSkillInfoV2);
	/************************************************************************/
	/* Button OnClick Event								                    */
	/************************************************************************/	
	UFUNCTION(BlueprintCallable, Category = UC_SkillDetail)
	void OnClick_SkillLevelTab( int32 InIndex );

	UFUNCTION(BlueprintCallable,Category = UC_SkillDetail)
	void OnClick_Close();

	UFUNCTION(BlueprintCallable, Category = UC_SkillDetail)
	void OnClick_SkillLevelUp();

	UFUNCTION()
	void Callback_NotEnoughGold();
public:
	
	// UI
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	UWidgetSwitcher*			WidgetSwitcher_CurrentSkillLevelIndicator;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	UWidgetSwitcher*			WidgetSwitcher_Active;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	UCanvasPanel_RadioGroup*	SkillLevel_Tab;

	UPROPERTY()
		UButton* Button_Close;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	//UUC_HeroSkill_Definition*	HeroSkill_Definition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	UWidgetSwitcher*			WidgetSwitcher_EnchantConfirm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	UButton*					SkillLevelUpBtn = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	UWidgetSwitcher*			SkillLevelUpSwitcher = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	ULocalizingTextBlock*		Text_SkillUnlockLevel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	ULocalizingTextBlock*		Text_Warning = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	UPanelWidget*				SkillCostInfoPanel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	UTextBlock*					Text_SkillLevelupCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	UTextBlock*					Text_LevelupInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	UHorizontalBox*				Hbox_RequireItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	UImage*						Image_NeedItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	UTextBlock*					Text_CurrentItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	UTextBlock*					Text_RequireItemCount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	ULocalizingTextBlock*		Text_RequireItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	UPanelWidget*				CostPanelWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	UTextBlock*					Text_SkillLevel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	UTextBlock*					Text_SkillName = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	UTextBlock*					Text_SkillType = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	UTextBlock*					Text_SkillDesc = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	UTextBlock*					Text_CoolTime = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	UTextBlock*					TextC = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	UTextBlock*					TextSecond = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	UTextBlock*					TextSkillRefreshTime = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	UImage*						Image_AttackRange = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	UTextBlock*					Text_AttackRange = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	UTextBlock*					Text_CastingRange = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	UTextBlock*					Text_CastingTime = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	UTextBlock*					Text_Lv = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	URRichTextBlock*			Text_CurrentLevelText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillDetail)
	URRichTextBlock*			Text_NextLevelText = nullptr;

	UPROPERTY()
	TArray<UTextBlock*>		ColorTextArray;
	UPROPERTY()
	TArray<FSlateColor>		ColorArray;
	UPROPERTY()
	TWeakObjectPtr<class UUC_SkillLvUpSuccessPopup> UC_SkillLvUpSuccessPopup;

private:
	void RefreshUI();
	void RefreshLevelUpEnable();
	void RefreshRequireItem();

private:
	FString					CurHeroUD;
	FName					CurHeroID;
	FHERO_SKILL				HeroSkillInfo;
	FSkillResourceTableInfo SkillResourceTableInfo;
	FSkillTableInfo_V2		SkillTableInfo_V2;
	FSpiritSkillTableInfo	SpiritSkillTableInfo;

	ESkillCategoryTypeEnum	SkillCategoryType;
	int32					CurrentSkillLevel;
	FName					CurHeroDisplayName;

	// RequireData
	int32					NeedSkillUpgradeGold;
	FName					NeedRequireItem;
	int32					NeedRequireItemCount;
	int32					CurrentRequireItemCount;

	bool					bHeroActive = false;
	bool					bSpiritSkill = false;
};