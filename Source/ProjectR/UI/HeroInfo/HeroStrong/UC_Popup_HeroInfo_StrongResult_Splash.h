// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"

#include "UI/Common/Renewal_BaseHeroIcon.h"

#include "UC_Popup_HeroInfo_StrongResult_Splash.generated.h"

USTRUCT(BlueprintType)
struct PROJECTR_API FVariables_StrongResultPopup
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_StrongResultPopup)
		TArray<UWidgetSwitcher*>				WidgetSwitcher_Icon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_StrongResultPopup)
		TArray<URenewal_BaseHeroIcon*>			SuccessIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_StrongResultPopup)
		TArray<URenewal_BaseHeroIcon*>			FailIcon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_StrongResultPopup)
		TArray<UTextBlock*>						Text_EnchantPer;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = FVariables_StrongResultPopup)
		ULocalizingTextBlock*					Text_Result;
};

UCLASS()
class PROJECTR_API UUC_Popup_HeroInfo_StrongResult_Splash : public URBaseWidget
{
	GENERATED_BODY()
//	¸É¹ö ÇÔ¼ö
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	void SetEnchantResult(TArray<bool>& InResult, TArray<FName>& InHeroUDs, TArray<int32>& InPercent, int32 InGetPoint);

	virtual FReply NativeOnTouchEnded(const FGeometry& InGeometry, const FPointerEvent& InGestureEvent) override;


//	¸É¹ö º¯¼ö
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_Popup_HeroInfo_StrongResult_Splash)
		FVariables_StrongResultPopup variables;

};