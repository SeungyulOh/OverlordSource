#pragma once

#include "UI/RBaseWidget.h"
#include "Network/PacketDataStructures.h"
#include "UC_ItemUpgradeResult.generated.h"

class ULocalizingTextBlock;
class UUC_StatChanges;
class UTextBlock;
class UButton;
class UWidget;
class UUC_GenericItemIconWithFlare;
class UUC_StatDiffs;

/**
* display for the result of item upgrade
* 
* GenericSplash content for BP UP_Splash_ItemUpgradeResult
* Splash removes itself when done
*/
UCLASS()
class PROJECTR_API UUC_ItemUpgradeResult : public URBaseWidget
{
	GENERATED_BODY()
public:
	//// Interface

	void InvalidateData() override;

	FString ItemUD;
	FITEM ItemDataBefore; // item id different from what it is now (ItemUD)

	//// BP Widgets

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ItemEnchantResult)
	UUC_GenericItemIconWithFlare* ItemIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ItemEnchantResult)
	UTextBlock* Text_ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ItemEnchantResult)
		USizeBox* SizeBox_1;
	/** "에픽" */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ItemEnchantResult)
	UTextBlock* Text_Before;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ItemEnchantResult)
	UTextBlock* Text_After;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ItemEnchantResult)
	UUC_StatDiffs* StatDiffs;
};

UCLASS()
class PROJECTR_API UUC_StatDiff : public URBaseWidget
{
	GENERATED_BODY()

public:

	//// Interface
	void Refresh(EItemStatOption ItemStatOption, int32 DiffValue);

	//// BP Widgets
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	ULocalizingTextBlock* Text_Name;
	
	/** "+15" */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UTextBlock* Text_Value;
};

UCLASS()
class PROJECTR_API UUC_StatDiffs : public URBaseWidget
{
	GENERATED_BODY()

public:

	//// Interface

	void Refresh(const FITEM* ItemDataBefore, const FITEM* ItemDataAfter);

	//// BP Widgets

	/** 전투력. Widget nullable */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	UTextBlock* Text_PowerValue;

	/** 2 of them */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	TArray<UUC_StatDiff*> BasicStatDiffs;
	/** 0~2 of them */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_ItemEnchant|Ref")
	TArray<UUC_StatDiff*> SubstatDiffs;
};
