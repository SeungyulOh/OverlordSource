// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_HeroDetailStat.generated.h"

class UCharacterBattleInfo;
class UUC_HeroInfoStat;
/**
 * BP: UC_Hero_Stats
 * WBP_HeroDetail
 */
UCLASS()
class PROJECTR_API UUC_HeroDetailStat : public URBaseWidget
{
	GENERATED_BODY()
	
	
public:
	void NativeConstruct()	override;
	void NativeDestruct()	override;
	
	void InvalidateData() override;

	//UFUNCTION(BlueprintCallable, Category = UC_HeroDetailStat)
	UFUNCTION()
	void OnClick_Close();
	
	UFUNCTION(BlueprintCallable, Category = UC_HeroDetailStat)
	void SetPropertyData(UCharacterBattleInfo* InCharacterBattleInfo);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_HeroDetailStat|Ref")
	UButton*				Button_Close = nullptr;

private:
	UPROPERTY()
	UCharacterBattleInfo*	CharacterBattleInfo = nullptr;
	
	int32 UpdateCount = 0;
};
