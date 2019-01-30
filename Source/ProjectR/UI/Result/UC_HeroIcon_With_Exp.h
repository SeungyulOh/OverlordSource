// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UI/UIDefs.h"
#include "UI/Control/RScrollItem.h"
#include "UI/Control/RHeroIconInterface.h"
#include "UC_HeroIcon_With_Exp.generated.h"


class UProgressBar;
class UImage;
class UTextBlock;
class URenewal_BaseHeroIcon;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_HeroIcon_With_Exp : public URScrollItem
{
	GENERATED_BODY()

public:
	void NativeConstruct() override;
	
	UFUNCTION(BlueprintCallable, Category = Herocon_With_Exp)
	void ShowAddExp(int32 InAddExp);

	UFUNCTION(BlueprintCallable, Category = ScrollItem)
	void SetScrollItemData(int32 Index, URScrollItemData* item, URScrollView* ScrollView) override;
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroIcon_With_Exp)
	URenewal_BaseHeroIcon*		HeroIconPtr = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroIcon_With_Exp)
	UProgressBar*	ProgressBar_Addition_Exp	= nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroIcon_With_Exp)
	UProgressBar*	ProgressBar_Base_Exp		= nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroIcon_With_Exp)
	UImage*			Image_LevelUp_Result		= nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = HeroIcon_With_Exp)
	UTextBlock*		Text_AddExp					= nullptr;
};
