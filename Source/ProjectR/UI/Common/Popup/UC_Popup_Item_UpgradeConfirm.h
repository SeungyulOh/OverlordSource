// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UI/ItemManagement/UC_ItemInventoryBase.h"
#include "UC_Popup_Item_UpgradeConfirm.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Popup_Item_UpgradeConfirm : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	FORCEINLINE void SetData(FString ItemUD, TArray<FString> materialList = {}) { m_ItemUD = ItemUD; m_MaterialList = materialList; }

	FString m_ItemUD;
	TArray<FString> m_MaterialList;
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

};
