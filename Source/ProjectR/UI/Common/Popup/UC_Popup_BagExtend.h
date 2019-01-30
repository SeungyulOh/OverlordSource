// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "UI/ItemManagement/UC_GenericPopupContent.h"
#include "UI/RBaseWidget.h"
#include "UI/ItemManagement/UC_ItemInventoryBase.h"
#include "UC_Popup_BagExtend.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_Popup_BagExtend : public UUC_GenericPopupContent
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	//// Interface

	void InvalidateData() override;

	FOnItemInventoryUpdated OnItemInventoryUpdated;

	//// BP Config

	/** "��� ���� {bluestone}���� �Ҹ��Ͽ� ���� ĭ {slot}�� Ȯ���� �� �ֽ��ϴ�." */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Bag)
		FOptionalUIStringKey DescriptionText;

	//// BP Widgets

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_BagExtend)
		UTextBlock* Text_Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_BagExtend)
		UTextBlock* Text_BagCapacityBefore = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_BagExtend)
		UTextBlock* Text_BagCapacityAfter = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_BagExtend)
		UTextBlock* Text_BluestoneCost = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_BagExtend)
		UButton* Button_Cancel = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_BagExtend)
		UButton* Button_OK = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_BagExtend)
		UButton* Button_Close = nullptr;

	//// Delegates
	UFUNCTION()
		void OnButtonCancelClicked();
	UFUNCTION()
		void OnButtonOKClicked();

	void OnInventoryUpgradeRp(bool bSuccess);

private:
	bool HasEnoughBluestones();
};
