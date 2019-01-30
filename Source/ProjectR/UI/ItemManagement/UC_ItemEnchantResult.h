#pragma once

#include "UI/RBaseWidget.h"
#include "Network/PacketDataStructures.h"
#include "UC_ItemEnchantResult.generated.h"

class ULocalizingTextBlock;
class UUC_StatDiffs;
class UTextBlock;
class UButton;
class UWidget;
class UUC_GenericItemIconWithFlare;
class URRichTextBlock;

/**
* display for the result of item enchant
* 오버로드_장비아이템_강화.docx
* UI: 아이템 UI_장비 강화_김현주.pdf
* 
* GenericSplash content for BP UP_Splash_ItemEnchantResult
* Splash removes itself when done
*/
UCLASS()
class PROJECTR_API UUC_ItemEnchantResult : public URBaseWidget
{
	GENERATED_BODY()
public:
	//// Interface
	void NativeConstruct() override;

	void InvalidateData() override;

	FString ItemUD;
	FITEM ItemDataBefore;
	
	//// BP Config
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_ItemEnchantResult")
	FSlateColor Color_EnchantLevelUnenchantable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_ItemEnchantResult")
	FOptionalUIStringKey DescriptionSpecialEnchant;

	//// BP Widgets
	
	/** 0: normal enchant, 1: special enchant, 2: enchant fail */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ItemEnchantResult)
	UWidgetSwitcher* Switcher_Result;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ItemEnchantResult)
	URRichTextBlock* Text_DescriptionSpecialEnchant;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ItemEnchantResult)
	UUC_GenericItemIconWithFlare* ItemIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ItemEnchantResult)
	UTextBlock* Text_ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ItemEnchantResult)
	UTextBlock* Text_EnchantLvBefore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ItemEnchantResult)
	UTextBlock* Text_EnchantLvAfter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ItemEnchantResult)
	UUC_StatDiffs* StatDiffs;

private:
	// saved
	FSlateColor Color_EnchantLvAfter;
};
