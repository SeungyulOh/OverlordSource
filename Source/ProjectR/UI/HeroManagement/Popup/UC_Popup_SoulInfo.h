// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UI/Common/Renewal_BaseHeroIcon.h"
#include "../UC_Hero_Skill_Icon.h"



#include "UC_Popup_SoulInfo.generated.h"
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Popup_SoulInfo : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	//
	void BindData(FString spiritUD);

	UFUNCTION()
		void CloseWidget();

	//PROPERTY
	UPROPERTY()
		UButton* Button_Close;

	UPROPERTY()
		UButton* Button_OK;

	UPROPERTY()
		URenewal_BaseHeroIcon* UC_Icon_Soul;

	UPROPERTY()
		ULocalizingTextBlock* Text_SoulName;

	UPROPERTY()
		ULocalizingTextBlock* Text_Species;

	UPROPERTY()
		ULocalizingTextBlock* Text_Explain;

	UPROPERTY()
		UUC_Hero_Skill_Icon* UC_Hero_SKill;

	UPROPERTY()
		ULocalizingTextBlock* Text_SkillLevel;
	UPROPERTY()
		ULocalizingTextBlock* Text_SkillName;

	UPROPERTY()
		ULocalizingTextBlock* Text_SkillCaption;

	UPROPERTY()
		TArray<ULocalizingTextBlock*> Text_SkillDescs;
};
