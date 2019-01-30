// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "UI/ItemManagement/UC_GenericPopupContent.h"
#include "Table/ItemTableInfo.h"
#include "UC_ItemInventoryBase.h"
#include "UC_WalletItemInfo.generated.h"

/*
* 아이템 UI_가방화면_김현주.pdf.Ⅲ. 기타 상세 정보, 영웅 조각 상세 정보, 룬 상세 정보
*
* The UI lumped them all in one Widget Blueprint
*
* after the user uses the item, this popup is caused to be closed.

*/

class UUP_ItemInventoryPage;
class UUP_ItemInventoryPage_ContentBase;

UCLASS()
class PROJECTR_API UUC_WalletItemInfo : public UUC_GenericPopupContent
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	//// Constants

	static const int32 LastRuneGrade = 20;

	//// Interface

	FName WalletKey = NAME_None;

	void InvalidateData() override;

	FOnItemInventoryUpdated OnItemInventoryUpdatedDelegate;


	//// BP Widgets

	/** The master switch. 0: Material, 1: HeroPiece, 2: Rune */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_WalletItemInfo|Ref")
	UWidgetSwitcher* Switcher_Item;

	
	// Shared among item types
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_WalletItemInfo|Ref")
	UUC_GenericItemIcon* ItemIcon;

	// Shared BP Interface
	UFUNCTION(BlueprintCallable, Category = "UC_WalletItemInfo")
	void OnButtonCloseClicked();

	// Material
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_WalletItemInfo|Ref")
	UTextBlock* Text_Material_Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_WalletItemInfo|Ref")
	UTextBlock* Text_Material_Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_WalletItemInfo|Ref")
	UTextBlock* Text_Material_Usage;

	UPROPERTY()
	UButton*  Button_Close;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_WalletItemInfo|Ref")
	class UUC_ItemInfo_Button* Button_Material_Use;

	// HeroPiece

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_WalletItemInfo|Ref")
	UTextBlock* Text_HeroPiece_Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_WalletItemInfo|Ref")
	UTextBlock* Text_HeroPiece_Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_WalletItemInfo|Ref")
	UTextBlock* Text_HeroPiece_Usage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_WalletItemInfo|Ref")
	class UUC_ItemInfo_Button* Button_HeroPiece_Use;

	// Rune
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_WalletItemInfo|Ref")
	UTextBlock* Text_Rune_Name;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_WalletItemInfo|Ref")
	UTextBlock* Text_Rune_WeaponStatName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_WalletItemInfo|Ref")
	UTextBlock* Text_Rune_WeaponStatValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_WalletItemInfo|Ref")
	UTextBlock* Text_Rune_DefenseStatName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_WalletItemInfo|Ref")
	UTextBlock* Text_Rune_DefenseStatValue;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_WalletItemInfo|Ref")
	UTextBlock* Text_Rune_Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_WalletItemInfo|Ref")
	UButton* Button_Rune_Combine;

	// Delegate functions

	UFUNCTION()
	void OnButtonRuneCombineClicked();

private:
	// Invalidate helpers
	void InvalidateMaterial(const FWalletTableInfo* WalletInfo);
	void InvalidateHeroPiece(const FWalletTableInfo* WalletInfo);
	void InvalidateRune(const FWalletTableInfo* WalletInfo);
};
