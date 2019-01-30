// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "RSkillCutscene.generated.h"



class ABasePlayerController;
/**
 * 
 */
UCLASS()
class PROJECTR_API URSkillCutscene : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = SkillCutsceneUI)
	void OnVisible(const FText& InSkillText, UTexture2D* InSkillPotrait);

	void PrepareSkillCutsceneInfo(FName InSkillName, UTexture2D* InSkillPotrait, ESkillCategoryTypeEnum InSkillControlType);

	UPROPERTY()
	FString SkillString = FString(TEXT("_Context_Name"));
};
