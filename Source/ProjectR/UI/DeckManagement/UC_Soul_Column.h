// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UC_Soul_Column.generated.h"

class URenewal_BaseHeroIcon;
class UUC_SkillForDeck;
/**
*
*/
UCLASS()
class PROJECTR_API UUC_Soul_Column : public URBaseWidget
{
	GENERATED_BODY()

public:
	virtual void	NativeConstruct()	override;

public:
	void SetSoulInfo(FString InHeroUD);

	void SetEmpty();
	void InitHeroPortrait();
	void InitSkillIcon();
	void InitText();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Soul_Column)
	URenewal_BaseHeroIcon*		SoulIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Soul_Column)
	UUC_SkillForDeck*			SoulSkill;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Soul_Column)
	ULocalizingTextBlock*		SoulDescription;


	FName SoulUD;
};