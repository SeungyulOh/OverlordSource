// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UI/ItemManagement/UC_ItemInventoryBase.h"
#include "UC_Popup_HeroInfo_Lock.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Popup_HeroInfo_Lock : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void SetDataList(TArray<FString> lockList, TArray<FString> unlockList, bool IsItem = false);

	UFUNCTION()
	void OnClickCancel();
	UFUNCTION()
	void OnClickOK();

public:
	UPROPERTY()
		UButton*					Button_Cancel = nullptr;
	UPROPERTY()
		UButton*					Button_Ok = nullptr;
	UPROPERTY()
		UButton*					Button_Close = nullptr;
	UPROPERTY()
		UTextBlock*  Text_LockCount = nullptr;
	UPROPERTY()
		UTextBlock*  Text_UnlockCount = nullptr;
	//
	FOnItemInventoryUpdated OnItemInventoryUpdated;

private:
	TArray<FString>					LockedHeros;
	TArray<FString>					UnLockedHeros;
	TArray<FString>					LockedItems;
	TArray<FString>					UnLockedItems;
	
};
