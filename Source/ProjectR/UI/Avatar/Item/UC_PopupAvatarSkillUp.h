// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_PopupAvatarSkillUp.generated.h"

class UUC_AvatarSkill;
class UUC_CurrencyDisplay;
/**
*
*/
UCLASS()
class PROJECTR_API UUC_PopupAvatarSkillUp : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void SetInfo(UUC_AvatarSkill* InSelectSkill);
	void SetEmpty();

public:
	UFUNCTION(BlueprintCallable, Category = "UC_PopupAvatarSkillUp|Ref")
	void OnClickClose();
	
	UFUNCTION(BlueprintCallable, Category = "UC_PopupAvatarSkillUp|Ref")
	void OnClickOK();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_PopupAvatarSkillUp|Ref")
	UUC_AvatarSkill*		BeforeSkill = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_PopupAvatarSkillUp|Ref")
	UUC_AvatarSkill*		AfterSkill = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_PopupAvatarSkillUp|Ref")
	ULocalizingTextBlock*	BeforeSkillText = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_PopupAvatarSkillUp|Ref")
	ULocalizingTextBlock*	AfterSkillText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_PopupAvatarSkillUp|Ref")
	UTextBlock*				Text_NeedPoint = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_PopupAvatarSkillUp|Ref")
	UUC_CurrencyDisplay*	NeedCurrency;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_PopupAvatarSkillUp|Ref")
	UButton*				Button_Close = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_PopupAvatarSkillUp|Ref")
	UButton*				Button_Cancel = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_PopupAvatarSkillUp|Ref")
	UButton*				Button_OK = nullptr;

private:
	bool		bShortageMoney = false;
	int32 Index = -1;
	int32 NextLevel = 0;
};