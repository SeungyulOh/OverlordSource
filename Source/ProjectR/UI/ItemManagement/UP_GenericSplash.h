#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UP_GenericSplash.generated.h"


/**
* self closes
*/
UCLASS()
class PROJECTR_API UUP_GenericSplash : public URBaseWidget
{
	GENERATED_BODY()

public:
	void NativePreConstruct() override;
	
	// life time is controlled by BP

	void InvalidateData() override;
	FORCEINLINE void SetCaptionTextStringKey(FName key) { CaptionText.UIStringKey = key; }
	//// Interface

	UUserWidget* GetContentWidget();


	//// BP Config
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_GenericSplash)
	TSubclassOf<UUserWidget> ContentWidgetClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_GenericSplash)
	float PositionYOffset = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_GenericSplash)
	bool bShowIconHeader = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_GenericSplash)
	bool bHideBottomLabel = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_GenericSplash)
	FOptionalUIStringKey CaptionText;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_GenericSplash)
	bool DisappearAfterDisplayTime = false;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UC_GenericSplash)
	float DisplayTimeInSecond = 1.5;

	//// BP Widgets
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_GenericSplash|Ref")
	UPanelWidget* Panel_Splash;

	/** 0: caption header, 1: icon header */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_GenericSplash|Ref")
	UWidgetSwitcher* Switcher_Header;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_GenericSplash|Ref")
	class ULocalizingTextBlock* Text_Caption;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_GenericSplash|Ref")
	UImage* Image_Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_GenericSplash|Ref")
	UPanelWidget* Panel_Content;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_GenericSplash|Ref")
	//UWidget* ContentPlaceholder;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_GenericSplash|Ref")
	class ULocalizingTextBlock* Text_BottomLabel;


private:
	UPROPERTY()
	UUserWidget* ContentWidget = nullptr;
};