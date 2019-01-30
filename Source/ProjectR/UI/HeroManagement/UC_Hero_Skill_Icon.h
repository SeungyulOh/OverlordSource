// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "UC_Hero_Skill_Icon.generated.h"

/**
 * Stars
 * Hero Detail Info Skill Icon
 */
UCLASS()
class PROJECTR_API UUC_Hero_Skill_Icon : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	//// Interface

	//void Refresh(int32 Grade);
	void SetSkillName(const FString& InHeroUD, FName InSkillID, uint32 InLevel = 1, ESkillCategoryTypeEnum eSkillCategoryTypeEnum = ESkillCategoryTypeEnum::VE_None);
	void SetSkillInfo(const FString& InHeroUD, FHERO_SKILL InSkillInfo, uint32 InLevel = 1, ESkillCategoryTypeEnum eSkillCategoryTypeEnum = ESkillCategoryTypeEnum::VE_None);

	void SetSpiritSkill(FName InSpiritSkillName);
	

	void SetLock(bool bLock);
	void SetControlType(bool isActive);//temp parameter

	void SetDisable(bool bDisable) override;
	UFUNCTION()
	void OnClick_Hero_Skill_Icon();
	//// BP Widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Hero_Skill_Icon)
	UButton*						Button_Click;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Hero_Skill_Icon)
	UImage*							Image_SkillIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Hero_Skill_Icon)
	UImage*							Image_Synergy;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Hero_Skill_Icon)
	UOverlay* PanelUnlock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Hero_Skill_Icon)
	UImage*							Image_Lock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Hero_Skill_Icon)
	UImage*							Image_SkillLock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Hero_Skill_Icon)
	UImage*							Image_Star;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Hero_Skill_Icon)
	ULocalizingTextBlock*			LocalizingTextBlock_Skill_Passive;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Hero_Skill_Icon)
	ULocalizingTextBlock*			LocalizingTextBlock_Skill_Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Hero_Skill_Icon)
	UWidgetSwitcher* ControlTypeSwitcher = nullptr;
	UPROPERTY()
	FString			HeroUD;
	UPROPERTY()
	FName			SkillID;
	UPROPERTY()
	FHERO_SKILL		SkillInfo;
	
	

	ESkillCategoryTypeEnum eSkillCategoryTypeEnum = ESkillCategoryTypeEnum::VE_None;

	
};
