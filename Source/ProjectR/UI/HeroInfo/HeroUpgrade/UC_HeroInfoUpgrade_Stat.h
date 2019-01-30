// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"

#include "SharedConstants/GlobalConstants.h"

#include "Table/CharacterTableInfo.h"

#include "UC_HeroInfoUpgrade_Stat.generated.h"

UCLASS()
class PROJECTR_API UUC_HeroInfoUpgrade_Stat : public URBaseWidget
{
	GENERATED_BODY()
	
private:
	void SetValue(int32& InPrevValue, int32& IntNextValue, FCharacterTableInfo* InPrevTable, FCharacterTableInfo* InNextTable);
	
public:
	void NativeConstruct()	override;
	void NativeDestruct()	override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroInfoUpgrade_Stat")
	ECharacterPropertyTypeEnum CharacterPropertyType;

	void Refresh(FCharacterTableInfo* InPrevTable, FCharacterTableInfo* InNextTable);

	//// BP Widgets

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroInfoUpgrade_Stat|Ref")
	ULocalizingTextBlock* Text_Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroInfoUpgrade_Stat|Ref")
	UTextBlock* Text_PrevValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroInfoUpgrade_Stat|Ref")
	UTextBlock* Text_NextValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroInfoUpgrade_Stat|Ref")
	UTextBlock* Text_AddValue;
};
