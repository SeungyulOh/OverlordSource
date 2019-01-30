// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "GlobalConstants.h"
#include "UP_GenericScrollview.h"
#include "UC_Hero_SortingFilter.generated.h"

UENUM(BlueprintType)
enum CheckBoxOrder { Tanker, MeleeDealer, RangeDealer, MagicDealer, Supporter, GradeOne, GradeTwo, GradeThree, GradeFour, GradeFive, GradeSix, GradeSeven, GradeEight, GradeNine, GradeTen, Lux, Solona, Luna, Nox, Max };

USTRUCT(BlueprintType)
struct FFilterHelper
{
	GENERATED_USTRUCT_BODY()


	void Filtering(TArray<FString>& OutHeroUDs , EHeroListFilterEnum type , bool bDescending = true);
	void MultiFiltering(TArray<FString>& OutHeroUDs, EHeroInventoryType InvenType, TArray<CheckBoxOrder> Orders);


	
};


/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Hero_SortingFilter : public URBaseWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnClick_AttackPower();
	UFUNCTION()
	void OnClick_Level();
	UFUNCTION()
	void OnClick_Grade();

	void Render_Enable(class UUC_HeroFilterButton* Exception);
	void Filtering();

public:

	//	UMG Widget Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory_LightFilter)
	class UUC_HeroFilterButton*						Button_AttackPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory_LightFilter)
	class UUC_HeroFilterButton*						Button_Level;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory_LightFilter)
	class UUC_HeroFilterButton*						Button_Grade;

	EHeroListFilterEnum CurrentType = EHeroListFilterEnum::VE_Grade;
	bool				bDescending = true;
private:
	FFilterHelper FilterHelper;
	
	
};



UCLASS()
class PROJECTR_API UUC_Hero_MultiFilter : public URBaseWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void OnClicked_Button_Reset();
	UFUNCTION()
	void OnClicked_Button_Close();
	UFUNCTION()
	void MultiFiltering(bool isChecked);

	void Render(EScrollviewType eType);

private:
	EHeroInventoryType inven_Type = EHeroInventoryType::VE_InventoryTypeAll;

	FFilterHelper FilterHelper;

public:
	

	/*Variables*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
	UButton*						ResetButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
	UButton*						CloseButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory)
	TArray<UCheckBox*>				CheckboxArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory_LightFilter)
	USizeBox* ClassBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory_LightFilter)
	USizeBox* StarBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_HeroInventory_LightFilter)
	USizeBox* AreaBox;

};
