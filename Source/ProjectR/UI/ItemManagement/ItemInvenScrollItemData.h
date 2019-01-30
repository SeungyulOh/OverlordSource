// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/Common/ItemBaseScrollItemData.h"
#include "UI/UIDefs.h"
#include "ItemInvenScrollItemData.generated.h"

class URGameInstance;
/**
 * used by UC_InventoryScrollView
 * rendered by BP UC_InventoryScrollItem
 */
UCLASS()
class PROJECTR_API UItemInvenScrollItemData : public UItemBaseScrollItemData
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UItemInvenScrollItemData)
	bool		bRecipeItem = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UItemInvenScrollItemData)
	FName		RecipeId;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UItemInvenScrollItemData)
	EItemGradeEnum	ItemGrade = EItemGradeEnum::Normal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UItemInvenScrollItemData)
	bool		bCompareEnable = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UItemInvenScrollItemData)
	FString		CompareItemUD;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UItemInvenScrollItemData)
	bool		bCurHeroEnable = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UItemInvenScrollItemData)
	FString		CurHeroUD;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UItemInvenScrollItemData)
	URGameInstance*		RGameInstance = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UItemInvenScrollItemData)
	bool		bNotify = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UItemInvenScrollItemData)
	int32		CraftableAccountLevel = 0;

public:
	UFUNCTION(BlueprintCallable, Category = UItemInvenScrollItemData)
	bool IsNotify();

	UFUNCTION(BlueprintCallable, Category = UItemInvenScrollItemData)
	void SetCalData();

	UFUNCTION(BlueprintCallable, Category = UItemInvenScrollItemData)
	bool IsCraftEnable();

};
