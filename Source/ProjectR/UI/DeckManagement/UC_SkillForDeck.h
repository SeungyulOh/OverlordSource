// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UC_SkillForDeck.generated.h"

class URGameInstance;
class UUC_SkillManagement;
/**
 * this class 
 */

UCLASS()
class PROJECTR_API UUC_SkillForDeck : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void PressSkill();

	UFUNCTION()
	void ChangeSkillIconImage(FString InHeroUD, int32 _index, bool _isRecursive = false);

	void SetSkillIcon(FString InHeroUD, bool isEliteMode);
	void SetSkillIconWithID(FName HeroID, bool isEliteMode);

	void InitSkillIcon();


public:
	UPROPERTY()
	UButton*		Button_Click = nullptr;
	UPROPERTY()
	UImage*			Image_SkillIcon = nullptr;
	UPROPERTY()
	UImage*			Image_Portrait = nullptr;
	UPROPERTY()
	UOverlay*		PanelUnlock = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite , Category = "UUC_SkillForDeck")
	EPartyRole		MyRole = EPartyRole::None;

	UPROPERTY()
	FString CurrentHeroUD;	
};
