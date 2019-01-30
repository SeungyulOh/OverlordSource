// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Table/ItemTableInfo.h"
#include "UI/RBaseWidget.h"

#include "UC_HeroItems_Icon.generated.h"

class UUC_GenericItemIcon;
class UImage;

/*
* Embedded in BP UC_HeroItems
* has UC_ItemIcon and some additional functionality
* * selection
* * locked state?
*/
UCLASS()
class PROJECTR_API UUC_HeroItems_Icon : public URBaseWidget
{
	GENERATED_BODY()
public:
	
	void SetItemUD(FString InItemUD) { ItemUD = InItemUD; }
	FString GetItemUD() { return ItemUD; }

	void SetEquipPosition(EItemEquipPositionEnum InEquipPosition);

	void InvalidateData() override;

	//// BP Widgets

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItems_Icon|Ref")
	UUC_GenericItemIcon* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItems_Icon|Ref")
	UImage* Image_EmptyIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_HeroItems_Icon|Ref")
	UImage* Image_Selecting;

private:
	// Empty string means unequipped at the equip position
	FString ItemUD;

	EItemEquipPositionEnum EquipPosition = EItemEquipPositionEnum::VE_Weapon;
};
