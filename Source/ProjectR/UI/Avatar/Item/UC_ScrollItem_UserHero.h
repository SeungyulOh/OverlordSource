// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UI/Control/RScrollItem.h"
#include "UC_ScrollItem_UserHero.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_ScrollItem_UserHero : public URScrollItem
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;

	UFUNCTION(BlueprintCallable, Category = UC_ScrollItem_UserHero)
	void SetUseHeroScrollItemData();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_ScrollItem_UserHero|Ref")
	UImage*					Image_Hero = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_ScrollItem_UserHero|Ref")
	UTextBlock*				Text_UserName = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_ScrollItem_UserHero|Ref")
	UTextBlock*				Text_Use = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_ScrollItem_UserHero|Ref")
	UProgressBar*			Bar_Use = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_ScrollItem_UserHero|Ref")
	ULocalizingTextBlock*	Text_Description = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UC_ScrollItem_UserHero|Ref")
	ULocalizingTextBlock*	Text_Colon = nullptr;
private:
	int32					CurrentValue = 0;

};
