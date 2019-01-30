#pragma once

#include "UI/RBaseWidget.h"
#include "Network/PacketDataStructures.h"
#include "UC_ItemLevelUpResult.generated.h"

class ULocalizingTextBlock;
class UUC_StatDiffs;
class UTextBlock;
class UButton;
class UWidget;
class UUC_GenericItemIconWithFlare;

/**
* display for the result of Item Level Up
* 오버로드_장비아이템_레벨업.docx
* UI: 아이템 UI_장비 레벨업_김현주.pdf
* 
* GenericSplash content for BP UP_Splash_ItemLevelUpResult
* Splash removes itself when done
*/
UCLASS()
class PROJECTR_API UUC_ItemLevelUpResult : public URBaseWidget
{
	GENERATED_BODY()
public:
	//// Interface

	void InvalidateData() override;

	bool IsLucky = false;

	FString ItemUD;
	FITEM ItemDataBefore;

	//// BP Widgets
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ItemLevelUpResult)
	UWidget* Text_Lucky;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ItemLevelUpResult)
	UUC_GenericItemIconWithFlare* ItemIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ItemLevelUpResult)
	UTextBlock* Text_ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ItemLevelUpResult)
	UTextBlock* Text_LvBefore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ItemLevelUpResult)
	UTextBlock* Text_LvAfter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_ItemLevelUpResult)
	UUC_StatDiffs* StatDiffs;

private:
};
