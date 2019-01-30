#pragma once

#include "EngineMinimal.h"
#include "UI/RBaseWidget.h"
#include "UC_GenericPopupContent.h"
#include "Network/PacketDataStructures.h"
#include "Table/ItemTableInfo.h"
#include "UC_HeroItemInfo.h"
#include "UP_ItemInventoryPage.h"
#include "UC_ItemUpgradeResult.h"
#include "UP_RuneManagement.generated.h"


class UUC_GenericItemIcon;
class ULocalizingTextBlock;
class UUC_StatDiff;
class UUC_EnchantMaterialButton;
class UUC_EquipmentIcon;
class UUC_ExpBar;
class UUC_ItemEnchant_Button;
class UUC_MaterialIcon;
class UWalletInventoryScrollData;
class UUC_ItemIconCommon;

UCLASS()
class PROJECTR_API UUC_RuneSocket : public URBaseWidget
{
	GENERATED_BODY()
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	//// Interface

	bool IsDisabled = false;

	FName IconKey = NAME_None;
	bool IsEmpty = false;
	bool IsSelected = false;
	bool IsStaged = false;

	void InvalidateData() override;

	DECLARE_DELEGATE(FOnClicked);
	FOnClicked OnClicked;
	
	//// BP Configs

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_MaterialIcon|Ref")
	float IconRenderOpacityWhenStaged = 0.5;

	//// BP Widgets

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_MaterialIcon|Ref")
	UUC_ItemIconCommon* ItemIconCommon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_MaterialIcon|Ref")
	UWidget* Panel_Selected;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_MaterialIcon|Ref")
	UWidget* Panel_Item;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_MaterialIcon|Ref")
	UButton* Button_Socket;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_MaterialIcon|Ref")
	UWidget* Panel_Disabled;

	//// Delegate functions
	UFUNCTION()
	void OnButtonSocketClicked();
};


UENUM(BlueprintType)
enum class ERuneManagementState : uint8
{
	Normal = 0,
	Equipping,
	Unequipping,
};

UCLASS()
class PROJECTR_API UUC_RuneWordRuneIcon : public URBaseWidget
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_RuneManagement|Ref")
	UUC_ItemIconCommon* ItemIcon;
};

/**
* content of BP UP_Splash_RuneEquipResult
* shown when equipping runes results in a rune word
*/
UCLASS()
class PROJECTR_API UUC_RuneEquipResult : public URBaseWidget
{
	GENERATED_BODY()
public:

	void Refresh(FName RuneWordRowName);

	//// BP Widgets

	/** 4 of them */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_RuneManagement|Ref")
	TArray<UUC_RuneWordRuneIcon*> RuneIcons;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_RuneManagement|Ref")
	UTextBlock* Text_RuneWordName;
};



/**
* UC_StatDiff with coloring to indicate that this stat is from a staged rune
*/
UCLASS()
class PROJECTR_API UUC_RuneManagement_StatDiff : public UUC_StatDiff
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;

	//// Interface
	/** staged StatDiff has blue tint in text per (design) */
	void RefreshWithStaged(EItemStatOption ItemStatOption, int32 DiffValue, bool IsStaged);

	//// BP Config

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_ItemIcon")
	FSlateColor Color_StagedRuneStatName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_ItemIcon")
	FSlateColor Color_StagedRuneStatValue;

private:
	// saved
	FSlateColor Color_NormalStatName;
	FSlateColor Color_NormalStatValue;
};

/**
*
*/
UCLASS()
class PROJECTR_API UUP_RuneManagement : public UUP_ItemInventoryPage_ContentBase
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	
	//// UUP_ItemInventoryPage_ContentBase interface

	void SetItemUD(FString InItemUD) override;

	void InvalidateData() override;

	void OnInventoryWalletItemSelected(FName SelectedWalletKey) override;

	void UpdateScrollItemData_Material(UInventoryScrollDataBase* ScrollItemData) override;
	
	//// BP Widgets
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_RuneManagement|Ref")
	UUC_GenericItemIcon* ItemIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_RuneManagement|Ref")
	UTextBlock* Text_ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_RuneManagement|Ref")
	TArray<UUC_RuneSocket*> RuneSockets;
	
	/** 0: when no socketed runes, 1: stats */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_RuneManagement|Ref")
	UWidgetSwitcher* Switcher_Description;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_RuneManagement|Ref")
	TArray<UUC_RuneManagement_StatDiff*> StatDiffs;
	
	/** 0: Equip button enabled, 1: Equip button disabled, 2: Unequip button */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_RuneManagement|Ref")
	UWidgetSwitcher* Switcher_BottomButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_RuneManagement|Ref")
	UButton* Button_Equip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_RuneManagement|Ref")
	UButton* Button_Unequip;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_RuneManagement|Ref")
	UTextBlock* Text_UnequipGemAmount;

	//// Delegate functions

	UFUNCTION()
	void OnButtonEquipClicked();
	UFUNCTION()
	void OnButtonUnequipClicked();
	UFUNCTION()
	void OnRuneSocketClicked(int32 Index);

	UFUNCTION()
	void OnInventoryRuneEquipRp(bool bSuccess);
	UFUNCTION()
	void OnInventoryRuneUnequipRp(bool bSuccess);

private:
	void InvalidateRuneSocketsAndStats(const FItemTableInfo* ItemInfo, const FITEM* ItemData);

	// ItemData passed to save lookup
	void InvalidateStatChanges(const FITEM* ItemData);

	ERuneManagementState State = ERuneManagementState::Normal;

	/** into RuneSockets */
	int32 CurrentSelectedSlot = 0;

	/**
	* ItemData->runes saved on InvalidateData and padded to MaxNumSockets
	*/
	TArray<FName> Runes;
	/**
	 * Staging state
	 * staged runes. Make sure not to overlap with Runes
	 */
	TArray<FName> StagedRunes;

	/** helper */
	bool HasStagedRunes() const;

	// Principal state
	FString ItemUD;
};
