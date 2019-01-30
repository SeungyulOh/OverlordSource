// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "SharedConstants/GlobalConstants.h"
#include "Network/PacketDataEnum.h"
#include "Network/PacketDataStructures.h"
#include "Table/ItemTableInfo.h"
#include "UI/UIDefs.h"
#include "UI/RBaseWidget.h"
#include "UI/ItemManagement/ItemInvenScrollItemData.h"
#include "UP_ItemManufacture.generated.h"

class UUC_Item_Icons;
class URScrollView;
class UCanvasPanel_RadioGroup;
class UVerticalBox;
class UUC_HeroManagement_HeroList;
class UCheckBox_Radio;
class UItemInvenScrollItemData;
class URRichTextBlock;
class URBaseWidget;
UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EItemManufactureLeftTypeEnum : uint8
{
	VE_WeaponType			UMETA(DisplayName = "WeaponType"),
	VE_ArmorType			UMETA(DisplayName = "ArmorType"),
	VE_MagicArmorType		UMETA(DisplayName = "MagicArmorType"),
	VE_Accessories			UMETA(DisplayName = "Accessories"),
	VE_Expendables			UMETA(DisplayName = "Expendables"),
	VE_None					UMETA(DisplayName = "None"),
};

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EItemManufactureRightTypeEnum : uint8
{
	VE_RecipeType			UMETA(DisplayName = "RecipeType"),
	VE_InvenType			UMETA(DisplayName = "InvenType"),
	VE_None					UMETA(DisplayName = "None"),
};


UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class EItemManufactureCause : uint8
{
	VE_None					UMETA(DisplayName = "None"),
	VE_Material				UMETA(DisplayName = "Material"),
	VE_Gold					UMETA(DisplayName = "Gold"),
	VE_Level				UMETA(DisplayName = "Level"),
};

/**
 * BP: UP_Item_Craft
 * WBP_ItemManufacture
 */
UCLASS()
class PROJECTR_API UUP_ItemManufacture : public URBaseWidget
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void InvalidateData() override;

	UFUNCTION(BlueprintCallable, Category = UUP_ItemManufacture)
	void ChangeRight(EItemManufactureRightTypeEnum right);

	UFUNCTION(BlueprintCallable, Category = UUP_ItemManufacture)
	void ChangeRightItem(bool bAll);

	UFUNCTION(BlueprintCallable, Category = UUP_ItemManufacture)
	void ChangeLeft(EItemManufactureLeftTypeEnum left);

	UFUNCTION(BlueprintCallable, Category = UUP_ItemManufacture)
	void OnClick_ExcuteManufacture();

	UFUNCTION(BlueprintCallable, Category = UUP_ItemManufacture)
	void OnClick_Shop();

	//--------------------------------------------------------------------
	UFUNCTION()
	void OnInvalidateManufactItem();
	UFUNCTION(BlueprintImplementableEvent, Category = UUP_ItemManufacture)
	void OnInvalidateRightRadioText();
	//--------------------------------------------------------------------

	UFUNCTION()
	void ItemManufactureRp(ECRUD CRUD, FITEM ManufacturedItem);

	UFUNCTION()
	void InventoryScrollViewSelectItem(URScrollItemData* SelectItem);

	UFUNCTION()
	void RecipeScrollViewSelectItem(URScrollItemData* SelectItem);

	UFUNCTION()
	void ChangeHero(FString InHeroUD);

	UFUNCTION()
	void OnClick_Process();

	UFUNCTION()
	void OnClick_PopupCancel();

	UFUNCTION()
	void OnClick_PopupOK();

	UFUNCTION()
	void RadioStateChanged(bool isChecked);

	UFUNCTION()
	void SortRadioStateChanged(bool isChecked);

private:
	void InvalidateRight();
	void InvalidateManufactItem(FName name);
	bool InvalidateMaterial(FName name, int32 idx, int32 count);
	bool InvalidateRune(FName id, int32 count);

	void SelectItemUI(FName ItemID);
	void UpdateDetail();

	static int32 GetSortNumber(UItemInvenScrollItemData& InData);

	void UpdatePlusImage(int32 _count);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	TArray<UCheckBox_Radio*>	RadioList;

	// Right
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UCanvasPanel_RadioGroup*	InventoryGroup = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UCanvasPanel_RadioGroup*	Sort_RadioGroup = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UWidgetSwitcher*			InventorySwitcher = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	URScrollView*				InventoryScrollView = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	URScrollView*				BP_ScrollView_Manufacture = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemManufacture|Ref")
	URScrollView*				ScrollView_ItemInfo = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UTextBlock*					BagFrontText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UTextBlock*					BagBackText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	TArray<UImage*>				NotifyImageList;

	// Left
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UWidgetSwitcher*			WidgetSwitcher_Detail = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UWidgetSwitcher*			WidgetSwitcher_ItemType = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemManufacture|Ref")
	ULocalizingTextBlock*		LocalText_Prefix;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemManufacture|Ref")
	UImage*						Image_NameDot = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UTextBlock*					TextBlock_Detail = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UWidgetSwitcher*			ItemTypeSwitcher = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UTextBlock*					ItemWeaponTypeText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UTextBlock*					ItemArmorTypeText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UTextBlock*					ItemTypeText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UTextBlock*					TextBlock_ItemNameText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UVerticalBox*				ItemProperty = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UTextBlock*					ItemProperty1FrontText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UTextBlock*					ItemProperty1BackText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UTextBlock*					ItemProperty2FrontText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UTextBlock*					ItemProperty2BackText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UPanelWidget*				ItemProperty2Panel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UPanelWidget*				ItemOptionPanel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UTextBlock*					ItemOptionCountText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UPanelWidget*				ItemSkillPanel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UPanelWidget*				ItemHeroOptionPanel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UUC_Item_Icons*				UC_Item_Icons = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	TArray<UPanelWidget*>		ItemIconPanelList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	TArray<UUC_Item_Icons*>		ItemIconList;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UTextBlock*					BP_Text_NeedEnchantGold = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UButton*					ItemManufactureBtn = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemManufacture|Ref")
	UButton*					Button_ItemProcess = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemManufacture|Ref")
	UButton*					Button_OK = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemManufacture|Ref")
	UButton*					Button_Cancel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UPanelWidget*				BP_Craft_Disabled = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UTextBlock*					TextBlock_Level = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	EItemManufactureLeftTypeEnum		LeftType = EItemManufactureLeftTypeEnum::VE_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	EItemManufactureRightTypeEnum		RightType = EItemManufactureRightTypeEnum::VE_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	int32						MaterialCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UTextBlock*					InvenCountText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UTextBlock*					InvenCountMaxText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UPanelWidget*				PrivateWidget = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UTextBlock*					PrivateText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UPanelWidget*				EnchantInfoPanel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UTextBlock*					EnchantMaxText = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemManufacture|Ref")
	UPanelWidget*			Panel_PassiveSkill = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemManufacture|Ref")
	UPanelWidget*			Panel_SkillCoolTime = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemManufacture|Ref")
	UTextBlock*				Text_SkillCoolTime = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemManufacture|Ref")
	UTextBlock*				Text_PassiveSkillDesc = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	TArray<UTextBlock*>			RightRadioTextLIst;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	int32						WeaponManufactCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	int32						ArmorManufactCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	int32						MagicArmorManufactCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	int32						RingManufactCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	int32						MaterialManufactCount = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemManufacture|Temp")
	UPanelWidget*				Panel_CraftPopup = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemManufacture|Temp")
	UCanvasPanel_RadioGroup*	ItemMenu_btn = nullptr;



	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemManufacture|Ref")
	URRichTextBlock*			RText_PopupDesc = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemManufacture|Value")
	FColor						TextColor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_ItemManufacture|Value")
	FColor						TextColor2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UCheckBox_Radio*			CheckBoxRadio_Live;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_ItemManufacture)
	UCheckBox_Radio*			CheckBoxRadio_Dev;

	UPROPERTY()
	TArray<UImage*>				PlusImageArray;
	

private:
	UPROPERTY()
	class URGameInstance* RGameInstance = nullptr;

	UPROPERTY()
	FName						CurManufactureItemName;
	bool						bManufactureEnable = false;
	UPROPERTY()
	FName						PrevSelectedItemID;
	bool						bAllItemShow = true;

	EItemManufactureCause		FailCause = EItemManufactureCause::VE_None;
	int32						ShortageValue = 0;
	int32						ShortageLevel = 0;
};
