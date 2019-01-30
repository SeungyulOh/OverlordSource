// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "UI/ItemManagement/UC_ItemInventoryBase.h"
#include "UI/ItemManagement/ItemFilter.h"
#include "InventoryFilterTableInfo.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct PROJECTR_API FInventoryFilterTableInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FInventoryFilterTableInfo)
	EItemInventoryEnum Inventory = EItemInventoryEnum::Weapons;

	//UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FInventoryFilterTableInfo)
	//int32 Row = 0;
	
	/** EEquipmentFilterCategory, ... */
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FInventoryFilterTableInfo)
	FName Category;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FInventoryFilterTableInfo)
	FName Category_UIString;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FInventoryFilterTableInfo)
	FString Factor01;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FInventoryFilterTableInfo)
	FName Factor01_UIString;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FInventoryFilterTableInfo)
	FString Factor02;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FInventoryFilterTableInfo)
	FName Factor02_UIString;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FInventoryFilterTableInfo)
	FString Factor03;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FInventoryFilterTableInfo)
	FName Factor03_UIString;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FInventoryFilterTableInfo)
	FString Factor04;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FInventoryFilterTableInfo)
	FName Factor04_UIString;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FInventoryFilterTableInfo)
	FString Factor05;
	UPROPERTY(EditAnyWhere, BlueprintReadWrite, Category = FInventoryFilterTableInfo)
	FName Factor05_UIString;

	static bool LookUp(TArray<FItemFilterRow>& OutRows, EItemInventoryEnum ItemInventoryEnum);
};
