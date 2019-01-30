// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/Control/RScrollItem.h"
#include "UC_SkillInfoScrollItem.generated.h"

class UUC_Notify;
class USkillInfoScrollItemData;
class UUC_SkillManagement;
/**
 * BP: UC_Hero_Skill_Icon_ScrollItem
 */
UCLASS()
class PROJECTR_API UUC_SkillInfoScrollItem : public URScrollItem
{
	GENERATED_BODY()
	
public:
	void NativeConstruct()	override;
	void NativeDestruct() override;

	void SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;

	void SetSkillInfo(USkillInfoScrollItemData* SkillInfoItemData);
	void SetSpiritSkillInfo(USkillInfoScrollItemData* SkillInfoItemData);

	//delegate
	UFUNCTION()
		virtual void SetSelected(bool bSelected) override;
	UFUNCTION()
		void SelectDelegate();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_SkillInfoScrollItem)
	UTextBlock*					Text_SkillLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_SkillInfoScrollItem)
	UTextBlock*					Text_SkillName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_SkillInfoScrollItem)
	UImage*						SkillImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_SkillInfoScrollItem)
	UPanelWidget*				LockPanelWidget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_SkillInfoScrollItem)
	UTextBlock*					Text_UnlockLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_SkillInfoScrollItem)
	UTextBlock*					Text_SkillCategory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_SkillInfoScrollItem)
	UTextBlock*					Text_SkillType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_SkillInfoScrollItem)
	UUC_Notify*					Notify;

	UPROPERTY() UImage*	Image_Select;
	UPROPERTY()	UButton* Button_Select;
	
public:
	UUC_SkillManagement* ManagerUI;
};
