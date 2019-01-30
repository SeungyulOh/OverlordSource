#pragma once

#include "UI/RBaseWidget.h"
#include "UC_GenericPopup.generated.h"

class UButton;
class ULocalizingTextBlock;
class UUC_GenericPopupContent;

/**
* BP: popup with modal backdrop covering the screen. with popup animation.
*/
UCLASS()
class PROJECTR_API UUC_GenericPopup : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativePreConstruct() override;
	void NativeConstruct() override;
	void NativeDestruct() override;

	//// Interface

	void InvalidateTitleBar();

	//// BP Interface

	// Set this from BP, per content widget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GenericPopup)
	TSubclassOf<UUserWidget> WidgetClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GenericPopup)
	bool bHideTitleBar = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GenericPopup)
	FName TitleLocalizingKey = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GenericPopup)
	EStringTableType TitleStringTableType = EStringTableType::VE_UI;
	
	// Overriding if not empty
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GenericPopup)
	FText TitleOverrideText;

	TMap<FString, UUserWidget*> CachedWidgetMap;
	
	//// Interface

	UUserWidget* GetContentWidget();

	// overridable
	virtual void Close();

	void InvalidateData() override;

	UFUNCTION()
	void OnClick_Back();

	//// BP Widgets
	
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_GenericPopup|Ref")
	//UWidget* Panel_ModalBackdrop;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_GenericPopup)
	UButton* Button_Back;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_GenericPopup|Ref")
	UButton* Button_Close;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_GenericPopup|Ref")
	ULocalizingTextBlock* Text_PopupTitle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_GenericPopup|Ref")
	UPanelWidget* Panel_Header;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_GenericPopup|Ref")
	UPanelWidget* Panel_Body;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_GenericPopup|Ref")
	//UWidget* Placeholder;

private:

	UPROPERTY()
	UUserWidget* Widget_Content = nullptr;

};