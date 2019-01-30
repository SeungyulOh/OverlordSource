// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/Control/RHeroIconInterface.h"
#include "UI/Control/RScrollItem.h"
#include "HeroIconWithButton.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDelegate_ClickHeroIcon);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDelegate_ClickHeroIconOneParam, URBaseWidget*, clickWidget);

class URenewal_BaseHeroIcon;
/**
 * 
 */
UCLASS()
class PROJECTR_API UHeroIconWithButton : public URScrollItem
{
	GENERATED_BODY()
	
public:

	void NativeConstruct() override;
	void NativeDestruct() override;

	void Init();

	UFUNCTION(BlueprintCallable, Category = HeroIconWithButton)
	void OnClick_HeroIcon();

// 	UFUNCTION(BlueprintCallable, Category = HeroIconWithButton)
// 	void OnSetHeroIconInfo(FHERO_ICON_INFO InHeroInfo);

	UFUNCTION(BlueprintCallable, Category = HeroIconWithButton)
	bool IsValidHeroData();

	UFUNCTION()
	void BP_HeroIconHideCombat();

	bool GetHeroIconInfo(FHERO_ICON_INFO& HeroIconInfo);
public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HeroIconWithButton)
	FDelegate_ClickHeroIcon					OnClickHeroIcon;

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = HeroIconWithButton)
	FDelegate_ClickHeroIconOneParam			OnClickHeroIconOneParam;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HEroIconWithButton)
	URenewal_BaseHeroIcon*		UC_HeroIcon = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HEroIconWithButton)
	UButton*					Button_HeroIcon = nullptr;
};
