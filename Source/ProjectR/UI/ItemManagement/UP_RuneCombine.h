#pragma once

#include "EngineMinimal.h"
#include "UI/RBaseWidget.h"
#include "UC_GenericPopupContent.h"
#include "Network/PacketDataStructures.h"
#include "Table/ItemTableInfo.h"
#include "UC_HeroItemInfo.h"
#include "UP_ItemInventoryPage.h"
#include "UP_RuneCombine.generated.h"


class UUC_GenericItemIcon;
class ULocalizingTextBlock;
class UUC_StatChange;
class UUC_EnchantMaterialButton;
class UUC_EquipmentIcon;
class UUC_ExpBar;
class UUC_ItemEnchant_Button;
class UUC_MaterialIcon;
class UWalletInventoryScrollData;
class UUC_AmountSelectSlider;


/**
*
*/
UCLASS()
class PROJECTR_API UUP_RuneCombine : public UUP_ItemInventoryPage_ContentBase
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	//// Constants
	static const int32 OneCombineAmount = 3;

	//// UUP_ItemInventoryPage_ContentBase interface

	void SetWalletKey(FName InWalletKey) override;

	void InvalidateData() override;

	void OnInventoryWalletItemSelected(FName SelectedWalletKey) override;

	void UpdateScrollItemData_Material(UInventoryScrollDataBase* ScrollItemData) override;
	
	//// BP Widgets
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_ItemLevelUp)
	UUC_MaterialIcon* InputRuneIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_ItemLevelUp)
	UTextBlock* Text_InputRuneName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_ItemLevelUp)
	UUC_MaterialIcon* OutputRuneIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_ItemLevelUp)
	UTextBlock* Text_OutputRuneName;

	// num output runes produced
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_ItemLevelUp)
	UTextBlock* Text_CurrentCountCombine;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_ItemLevelUp)
	UTextBlock* Text_MaxCountCombine;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_ItemLevelUp)
	//UTextBlock* Text_ItemLevelBefore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_ItemLevelUp)
	UUC_AmountSelectSlider* AmountSelectSlider;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_PopupContent_ItemLevelUp)
	UUC_ItemEnchant_Button* Button_Combine;

	//// Delegate functions

	UFUNCTION()
	void OnButtonCombineClicked();

	UFUNCTION()
	void OnInventoryRuneMakeRp(bool bSuccess, int32 Count, int32 OutputRuneGrade, int32 PrevOutputRuneCount);

	UFUNCTION()
	void OnSliderCountChanged(int32 NewCount);

private:
	// invalidate helpers
	void InvalidateOnCurrentCountChanged();

	// num output runes produced
	int32 CurrentCount = 0;

	// saved
	int32 GoldCostRate = 0;

	FName WalletKey = NAME_None;
};
