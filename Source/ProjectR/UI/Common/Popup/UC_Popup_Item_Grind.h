// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UI/ItemManagement/UC_ItemInventoryBase.h"
#include "UC_Popup_Item_Grind.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Popup_Item_Grind : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void SetData(TArray<FString> selectedItemList, TArray<FName> selectedWalletKeyList = {}, TArray<int32> selectedWalletAmountList = {});

	TArray<FString> SelectedItemUDs;
	TArray<FName> SelectedWalletKeys;
	TArray<int32> SelectedWalletAmounts;
	FOnItemInventoryUpdated OnItemInventoryUpdated;

	////
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Grind)
		UButton* Button_Cancel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Grind)
		UButton* Button_OK;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_Item_Grind)
		UButton* Button_Close;

	UFUNCTION()
		void OnButtonCancelClicked();
	UFUNCTION()
		void OnButtonOKClicked();
	UFUNCTION()
		void OnInventoryGrindRp(bool bSuccess, TArray<FREWARD> Rewards);
};
