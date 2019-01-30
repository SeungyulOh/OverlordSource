#pragma once

#include "UI/RBaseWidget.h"
#include "UC_GenericPopupContent.h"
#include "Network/PacketDataStructures.h"
#include "Table/ItemTableInfo.h"
#include "UC_HeroItemInfo.h"
#include "UP_ItemInventoryPage.h"


#include "UP_ItemUpgrade.generated.h"

class UUC_GenericItemIcon;
class ULocalizingTextBlock;
class UUC_StatChange;
class UUC_EnchantMaterialButton;
class UUC_ItemEnchant_Button;
class URRichTextBlock;
class UUC_EquipmentIcon;
struct FItemGradeTableInfo;
class UUC_Popup_Item_UpgradeConfirm;
class UUP_ItemEnchantPopup;

/**
* 오버로드_장비시스템_승급과옵션재부여.docx
* UI: 아이템 UI_장비 승급_김현주.pdf
*/
UCLASS()
class PROJECTR_API UUP_ItemUpgrade : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	//// UUP_ItemInventoryPage_ContentBase interface

	void InvalidateData() override;

	void SetItemUD(FString InItemUD);

	void InvalidateBasicStats(const TArray<int32>& BasicOptions);
	void InvalidateSubstats(const TArray<int32>& BasicOptions);

	UFUNCTION()
	void OnClick_ShowEquipment();
	


	//// BP Configs

	/** "{grade} 등급의 장비 아이템을 획득할 수 있습니다." */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant")
	FOptionalUIStringKey GradeDescriptionText;

	//// BP Widgets

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UUC_GenericItemIcon* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UTextBlock* ItemText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	class UUC_MaterialIcon* UpgradeMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UImage*			EquipmentImage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UUC_GenericItemIcon* Icon_EquipmentMaterial1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UUC_GenericItemIcon* Icon_EquipmentMaterial2;

	/** "<span color="#7cbff9">에픽</> 등급의 장비 아이템을 획득할 수 있습니다." */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	URRichTextBlock* Text_ItemUpgradeDescription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UTextBlock* Text_GradeBefore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UTextBlock* Text_GradeAfter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UTextBlock* Text_ItemLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UTextBlock* Text_Enchant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UTextBlock* Text_CurAmountMaterial;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UTextBlock* Text_ReqAmountMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UTextBlock* Text_CurAmountEquipment;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UTextBlock* Text_ReqAmountEquipment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UButton*	Button_ShowEquipment;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	TArray<UUC_ItemInfo_ItemStat*> BasicStats;
	// 2 of them
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	TArray<UUC_ItemInfo_ItemStat*> Substats;


	


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UTextBlock* Text_SuccessRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UUC_ItemEnchant_Button* Button_Upgrade;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UUC_Popup_Item_UpgradeConfirm* UC_Popup_Item_UpgradeConfirm;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UUP_ItemEnchantPopup* UP_Popup_Item_Enchant_New;

	//// Delegates
	UFUNCTION()
	void OnButtonUpgradeClicked();
	UFUNCTION(BlueprintCallable)
	void OnClickClose();

	UFUNCTION()
	void OnInventoryItemUpgradeRp(bool bSuccess, TArray<FREWARD> Rewards, FString NewItemUD);


	//// temp: these are public for UC_ItemUpgradeResult...
	static void RefreshTextGrade(UTextBlock* TextBlockGrade, EItemGradeEnum ItemGrade);
	/** Get something like "<span color="#7cbff9">에픽</>" */
	static FText GetRichGradeText(EItemGradeEnum ItemGrade);

	FORCEINLINE FString GetItemUD() { return ItemUD; }

private:
	//// InvalidateData helpers

	// principal state
	FString ItemUD;

	// equipment material selection state
	/** Empty string means the slot is empty */
	// saved on InvalidateData
	bool HasSufficientGold = false;

	EItemGradeEnum CurrentGrade = EItemGradeEnum::Normal;
	int32 CurrentLevel = 0;
};

