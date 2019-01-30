#pragma once

#include "UI/RBaseWidget.h"
#include "UC_GenericPopupContent.h"
#include "Network/PacketDataStructures.h"
#include "Table/ItemTableInfo.h"
#include "UC_HeroItemInfo.h"
#include "UP_ItemInventoryPage.h"
#include "UP_ItemOptionReset.generated.h"

class UUC_GenericItemIcon;
class ULocalizingTextBlock;
class UUC_StatChange;
class UUC_EnchantMaterialButton;
class UUC_ItemEnchant_Button;
class URRichTextBlock;
class UUC_EquipmentIcon;
struct FItemGradeTableInfo;
class UUC_GenericItemIconWithFlare;
class UUC_ItemOptionReset_AdditionalStats;

/**
* 오버로드_장비시스템_승급과옵션재부여.docx
* UI: 아이템 UI_장비 옵션 재부여_김현주.pdf
*
* currently class definition copied from UUP_ItemUpgrade with modifications
*/
UCLASS()
class PROJECTR_API UUP_ItemOptionReset : public UUP_ItemInventoryPage_ContentBase
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	//// UUP_ItemInventoryPage_ContentBase interface

	void InvalidateData() override;

	void SetItemUD(FString InItemUD) override;

	bool IsInventoryIncludingEquipmentItem(const FItemTableInfo* ItemInfo, const FITEM* ItemData) override;

	void OnInventoryEquipmentItemSelected(FString SelectedItemUD) override;

	void UpdateScrollItemData_EquipmentItem(UInventoryScrollDataBase* ScrollItemData) override;
	
	//// BP Widgets

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UUC_GenericItemIcon* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UTextBlock* Text_ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UUC_EnchantMaterialButton* OptionResetMaterial;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UUC_GenericItemIcon* Icon_EquipmentMaterial1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UUC_GenericItemIcon* Icon_EquipmentMaterial2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UUC_ItemOptionReset_AdditionalStats* AdditionalStats;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UUC_ItemEnchant_Button* Button_Upgrade;
	
	//// Delegates

	UFUNCTION()
	void OnButtonUpgradeClicked();

	UFUNCTION()
	void OnInventoryItemUpgradeRp(bool bSuccess, FITEM ItemDataBefore);

	/** Please call this from BP. WhichOne is either 0 or 1 */
	UFUNCTION(BlueprintCallable, Category = "UUC_ItemEnchant|Ref")
	void OnButtonEquipmentMaterialClicked(int32 WhichOne);

private:
	//// InvalidateData helpers

	void InvalidateOnEquipmentMaterialSelected();

	bool CanUpgrade();

	// principal state
	FString ItemUD;

	// equipment material selection state
	/** Empty string means the slot is empty */
	FString EquipmentMaterial1_ItemUD;
	FString EquipmentMaterial2_ItemUD;

	// saved on InvalidateData
	bool HasSufficientGold = false;
	
	EItemGradeEnum CurrentGrade = EItemGradeEnum::Normal;
};


UCLASS()
class PROJECTR_API UUC_ItemOptionResetResult : public URBaseWidget
{
	GENERATED_BODY()
public:
	//// Interface

	void InvalidateData() override;

	FString ItemUD;
	FITEM ItemDataBefore; // item id different from what it is now (ItemUD)

	//// BP Widgets

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UUC_GenericItemIconWithFlare* ItemIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UTextBlock* Text_ItemName;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UUC_ItemOptionReset_AdditionalStats* AdditionalStatsBefore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UUC_ItemOptionReset_AdditionalStats* AdditionalStatsAfter;
};

/** a stack of additional options for FITEM */
UCLASS()
class PROJECTR_API UUC_ItemOptionReset_AdditionalStats : public URBaseWidget
{
	GENERATED_BODY()
public:
	//// Interface

	void Refresh(const FITEM* ItemData);

	//// BP Widgets
	/** 4 of them expected */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ItemEnchantResult)
	TArray<UUC_ItemInfo_ItemStat*> AdditionalStats;
};