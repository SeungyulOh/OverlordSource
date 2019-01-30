// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UC_SkillManagement.generated.h"


class URScrollView;
class UUC_SkillDetail;
class USkillInfoScrollItemData;
class UUC_SkillInfoScrollItem;
class UUC_HeroManagement_HeroList;

UCLASS()
class PROJECTR_API UUC_SkillLvUpSuccessPopup : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct()  override;
	void NativeDestruct()	override;

	UFUNCTION()
	void OnClick_Close();

public:
	UPROPERTY()
	UImage*			Image_SkillIcon;

	UPROPERTY()
	UTextBlock*		Text_SkillLevel;

	UPROPERTY()
	UTextBlock*		Text_SkillName;

	UPROPERTY()
	UWidgetAnimation*	AppearAnim;

	UPROPERTY()
	UButton*			Button_Close;

};



/**
 * BP: UP_HeroSkill
 * WBP_SkillManagement
 */
UCLASS()
class PROJECTR_API UUC_SkillManagement : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct()  override;
	void NativeDestruct()	override;
	void InvalidateByHeroUD(const FString& InHeroUD);
	void RefreshScrollItemData();
	UFUNCTION()
	void			OnSelectScrollItem(URScrollItemData* selectItem);

private:
	void SetSkillData(USkillInfoScrollItemData* InItemData, FName InSkillName, int32 InLevel, EHERO_SKILL_SLOT InSkillSlot);
	
	void SetSpiritSkillData(USkillInfoScrollItemData* InItemData, FName InSpiritSkillName, int32 InLevel);


	UFUNCTION()
	void			OnSkillLvUpSuccess();

	UFUNCTION()
	void			OnClick_BackButton();


	UFUNCTION()
	void OnClick_HeroIcon(FString InHeroUD);
public:
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillManagement)
// 	URScrollView*			SkillScrollView;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_SkillManagement)
	UUC_SkillDetail*		SkillDetail;

	UPROPERTY()
	UUC_SkillLvUpSuccessPopup* SkillLevelUp_Splash;

	UPROPERTY()
	UButton*				Button_Back;

	UPROPERTY()
		UButton* Button_Close;
private:
	UPROPERTY()
	FString						CurHeroString;
	UPROPERTY()
	USkillInfoScrollItemData*	CurSelectScrollItemData = nullptr;
	uint8 CurSelectIdx;
	UPROPERTY() TArray<USkillInfoScrollItemData*> SkillDataArray;
	UPROPERTY() TArray<UUC_SkillInfoScrollItem*> SkillItemArray;
};
