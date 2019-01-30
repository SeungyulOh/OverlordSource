#pragma once

#include "UI/RBaseWidget.h"
#include "Network/PacketDataStructures.h"
#include "UP_ItemEnchantResult_NewDesign.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnItemEnchantResultComplete);

class UUC_GenericItemIcon;
class ULocalizingTextBlock;
class UUC_StatChanges;
class UTextBlock;
class UButton;
class UWidget;

/**
* display for the result of item enchant
* 오버로드_장비아이템_강화.docx
*
*/
UCLASS()
class PROJECTR_API UUP_ItemEnchantResult_NewDesign : public URBaseWidget
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	//// Interface
	
	void InvalidateData() override;

	FString ItemUD;
	FITEM ItemDataBefore;

	FOnItemEnchantResultComplete OnComplete;

	//// BP Widgets
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GenericPopupContent)
	ULocalizingTextBlock* Text_Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GenericPopupContent)
	UUC_GenericItemIcon* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GenericPopupContent)
	ULocalizingTextBlock* Text_NamePrefix;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GenericPopupContent)
	UTextBlock* Text_EnchantLvBefore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GenericPopupContent)
	UTextBlock* Text_EnchantLvAfter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GenericPopupContent)
	UButton* Button_OK;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GenericPopupContent)
	//UUC_StatChanges* StatChanges;

	//// BP set up at construct
	// tempy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GenericPopupContent)
	TArray<UWidget*> FlareWidgets;

	//// Delegates

	UFUNCTION()
	void OnButtonOKClicked();
};
