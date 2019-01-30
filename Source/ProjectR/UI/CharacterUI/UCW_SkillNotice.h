// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/CharacterUI/CharacterUserWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "UCW_SkillNotice.generated.h"


class ABasePlayerController;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUCW_SkillNotice : public UCharacterUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintImplementableEvent, Category = SkillNotice)
	void OnAutoSkillNotice(FName InSkillID);
	
	UFUNCTION(BlueprintCallable, Category = SkillNotice)
	int32 GetSkillTextIndex(FName InSkillID);

	void InitSkillData();

public:
	UPROPERTY(EditAnywhere, BLueprintReadWrite, Category = SkillNotice)
	TArray<FText>			SkillTexts;

private:
	UPROPERTY()
	TArray<FName>			SkillIDs;

};
