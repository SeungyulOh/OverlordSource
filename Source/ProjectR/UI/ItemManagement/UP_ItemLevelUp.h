#pragma once

#include "UI/RBaseWidget.h"
#include "UC_GenericPopupContent.h"
#include "Network/PacketDataStructures.h"
#include "Table/ItemTableInfo.h"
#include "UC_HeroItemInfo.h"
#include "UP_ItemInventoryPage.h"
#include "UI/Common/UC_StatHeroLevelUp.h"
#include "UP_ItemLevelUP.generated.h"


class UUC_GenericItemIcon;
class ULocalizingTextBlock;
class UUC_StatChange;
class UUC_EnchantMaterialButton;
class UUC_EquipmentIcon;
class UUC_ExpBar;
class UUC_ItemEnchant_Button;
class UUC_MaterialIcon;

enum EBasicStatType
{
	MaxHP,
	PhysicalAttackPower,
	MagicAttackPower,
	Armor,
	MagicResistance
};

USTRUCT(BlueprintType)
struct PROJECTR_API FVariables_ItemLevelUp
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_ItemLevelUp)
		UUC_GenericItemIcon*				ItemIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_ItemLevelUp)
		ULocalizingTextBlock*				Text_ItemName = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_ItemLevelUp)
		UWidgetSwitcher*					Switcher_Lock = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_ItemLevelUp)
		ULocalizingTextBlock*				Text_ItemLevel = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_ItemLevelUp)
		ULocalizingTextBlock*				Text_ItemEnchant = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_ItemLevelUp)
		UUC_ItemInfo_ItemStat*				BasicStat1 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_ItemLevelUp)
		UUC_ItemInfo_ItemStat*				BasicStat2 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_ItemLevelUp)
		UUC_ItemInfo_ItemStat*				Substat1 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_ItemLevelUp)
		UUC_ItemInfo_ItemStat*				Substat2 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_ItemLevelUp)
		ULocalizingTextBlock*				Text_CurrentLevel = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_ItemLevelUp)
		ULocalizingTextBlock*				Text_CurrentLevelWithMax = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_ItemLevelUp)
		ULocalizingTextBlock*				Text_MaxLevel = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_ItemLevelUp)
		TArray<UUC_StatHeroLevelUp*>		LevelupStatArr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_ItemLevelUp)
		ULocalizingTextBlock*				Text_MaterialName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_ItemLevelUp)
		UUC_MaterialIcon*					MaterialIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_ItemLevelUp)
		UButton*							Button_LevelupMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_ItemLevelUp)
		UButton*							Button_Slider_Minus;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_ItemLevelUp)
		USlider*							Slider_SelectedAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_ItemLevelUp)
		UImage*								Image_BackgroundBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_ItemLevelUp)
		ULocalizingTextBlock*				Text_CurrecyInProgressBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_ItemLevelUp)
		UButton*							Button_Slider_Plus;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_ItemLevelUp)
// 		UUC_ItemEnchant_Button*				Button_LevelUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_ItemLevelUp)
		ULocalizingTextBlock*				Text_GoldEnabled;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_ItemLevelUp)
		UButton*							Button_LevelUp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_ItemLevelUp)
		UButton*							Button_Cancel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_ItemLevelUp)
		UButton*							Button_Exit;
};

/**
* 
* UI: 아이템 UI_장비 레벨업_김현주.pdf
*/
UCLASS()
class PROJECTR_API UUP_ItemLevelUp : public UUP_ItemInventoryPage_ContentBase
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	//// UUP_ItemInventoryPage_ContentBase interface

	void InvalidateData() override;
	void RefreshUI();

	void SetItemUD(FString InItemUD) override;
	void SetCurrentStats(TArray<int32> options);
	void RefreshLevelUpChangeText();
	void GetBasicStats(TArray<int32>& BasicStatArr);
	void RefreshStatChanges(TArray<int32> OptionArr, int32 LevelGap);
	void RefreshSliderWidget(FItemTableInfo* itemInfo);
	void RefreshLevelupButton();
	bool IsInventoryIncludingEquipmentItem(const FItemTableInfo* Info, const FITEM* ItemData) override;

	FName GetStatName(EBasicStatType type);

	void OnInventoryEquipmentItemSelected(FString SelectedItemUD) override;

	//// BP Widgets
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemLevelUp)
		FVariables_ItemLevelUp variables;

	//// Delegate functions

	UFUNCTION()
	void OnButtonLevelUpClicked();

	UFUNCTION()
	void OnSliderValueChanged(float Value);

	UFUNCTION()
	void OnInventoryItemLevelUpRp(bool bSuccess, bool bIsLucky, FITEM ItemDataBefore);

	UFUNCTION()
		void OnClick_Button_Close();

	// these are silly
	UFUNCTION()
	void OnButtonSliderMinusClicked();
	UFUNCTION()
	void OnButtonSliderPlusClicked();
	UFUNCTION()
	void OnButtonMaxClicked();

private:
	// invalidate helper
	void InvalidateOnCurrentMaterialCountChanged();
	
	void SetLevelupWidgetEnable(bool bEnable);
	// state variables for InvalidateOnCurrentMaterialCountChanged
	int32 BaseLevel = 0;
	int32 LevelAfter = 0;

	int32 MaxGold = 0;
	int32 GoldCostRate = 0;

	int32 HaveCurrency = 0;
	int32 NeedCurrency = 0;
	int32 NeedTotalGold = 0;
// 	int32 MaterialCount = 0;
// 	int32 MaxMaterialCount = 0; // clamped by the amount gold owned
// 	int32 CurrentMaterialCount = 0;

	FString ItemUD;
	FITEM* ItemData;
	FItemTableInfo* ItemInfo;
};
