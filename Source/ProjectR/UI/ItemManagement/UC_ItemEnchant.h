#pragma once

#include "UI/RBaseWidget.h"
#include "UC_GenericPopupContent.h"
#include "Network/PacketDataStructures.h"
#include "Table/ItemTableInfo.h"
#include "UC_HeroItemInfo.h"
#include "UP_ItemInventoryPage.h"
#include "UC_ItemEnchant.generated.h"


DECLARE_MULTICAST_DELEGATE(FOnItemEnchantComplete);

class UUC_GenericItemIcon;
class ULocalizingTextBlock;
class UUC_StatChange;
class UUC_EnchantMaterialButton;

UENUM(BlueprintType)
enum class EItemButtonType: uint8
{
	ItemEnchantType,
	ItemLevelUpType,
	ItemUpgradeType,
};

UCLASS()
class PROJECTR_API UUC_ItemEnchant_Button : public UUC_ItemInfo_Button
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
public:
	UFUNCTION(BlueprintImplementableEvent, Category = UC_ItemEnchant_Button)
	void SetTextGoldAmount(const FText& AmountText);
	UFUNCTION(BlueprintImplementableEvent, Category = UC_ItemEnchant_Button)
		void SetTextRed();
	UFUNCTION(BlueprintImplementableEvent, Category = UC_ItemEnchant_Button)
		void SetTextWhite();
	void SetGoldAmount(int32 Amount);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_ItemEnchant_Button")
	ULocalizingTextBlock*	Text_ButtonType_Enable = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_ItemEnchant_Button")
		ULocalizingTextBlock*	Text_ButtonType_Disable = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_ItemEnchant_Button")
	EItemButtonType ButtonType = EItemButtonType::ItemEnchantType;
};

/**
* 오버로드_장비아이템_강화.docx.3
* UI: 아이템 UI_장비 강화_김현주.pdf
*/
UCLASS()
class PROJECTR_API UUC_ItemEnchant : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	//// UUP_ItemInventoryPage_ContentBase interface

	void InvalidateData() override;

	void InvalidateBasicStats(const TArray<int32>& BasicOptions);
	void InvalidateSubstats(const TArray<int32>& BasicOptions);

	void SetItemUD(FString InItemUD);

	//// BP Widgets

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UUC_GenericItemIcon* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UTextBlock* Text_ItemName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UTextBlock* Text_ItemLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UTextBlock* Text_Enchant;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UTextBlock* Text_CurrentEnchant;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UTextBlock* Text_NextEnchant;

	// stat changes
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	TArray<UUC_ItemInfo_ItemStat*> BasicStats;
	// 2 of them
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	TArray<UUC_ItemInfo_ItemStat*> Substats;

	// 2 of them
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	TArray<class UUC_StatHeroLevelUp*> BasicStatChanges;
	// 2 of them
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	TArray<class UUC_StatHeroLevelUp*> SubstatChanges;
	
	// configure use of material for regular/special enchant
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UVerticalBox* NoramlVerticalBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UUC_EnchantMaterialButton* NormalEnchantMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UTextBlock*	Text_CurrentAmount1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UTextBlock*	Text_ReqAmount1;
	
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UVerticalBox* ProtectVerticalBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UUC_EnchantMaterialButton* ProtectionEnchantMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UTextBlock*	Text_CurrentAmount2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UTextBlock*	Text_ReqAmount2;

	/** "90%" */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UTextBlock* Text_EnchantSuccessProbability;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UUC_ItemEnchant_Button* Button_Enchant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UWidgetSwitcher*	EquipmentSwitcher;

	UFUNCTION()
	void OnButtonEnchantClicked();

	UFUNCTION()
	void OnInventoryItemEnchantRp(bool bSuccess, int32 affectedEnchant, FITEM ItemDataBefore);

	UFUNCTION()
	void OnEnchantMaterialToggled(bool IsOn, bool IsSpecial);

	UFUNCTION(BlueprintCallable)
	void OnClickClose();

	void RefreshStatChanges(TArray<UUC_StatHeroLevelUp*>& InBasicStatChanges, TArray<UUC_StatHeroLevelUp*>& InSubstatChanges, TArray<int32>& BasicOptionsBefore, TArray<int32>& BasicOptionsAfter, bool bMaxLevel = false);
	// helper function temporarily here - should move to UIFunctionLibrary or

private:
	//// InvalidateData helpers

	void InvalidateStatChanges(const FItemTableInfo* ItemInfo, const FITEM* ItemData);

	void InvalidateButtonEnchant();
	
	bool CanEnchant();

	// saved value
	int32 EnchantGoldCost = 0;

	// principal state
	FString ItemUD;
};
