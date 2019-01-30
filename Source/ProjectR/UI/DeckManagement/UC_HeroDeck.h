// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "RBaseDeckWidget.h"
#include "SharedConstants/GlobalConstants.h"
#include "UI/UIDefs.h"
#include "UI/Control/RScrollItemData.h"
#include "UC_HeroDeck.generated.h"

class UTextBlock;
class UWidgetSwitcher;
/**
 * 
 */
UCLASS()
class PROJECTR_API UUC_HeroDeck : public URBaseDeckWidget
{
	GENERATED_BODY()
	
public:
	virtual bool	Initialize() override;
	virtual void	NativePreConstruct() override;
	virtual void	NativeConstruct()	override;

	virtual void	SetHero(FString heroUD) override;
	void			SetHeroWithID(FString heroID);

	virtual void			SetAvailable(bool inAvailable) override;
	virtual void			SetSelecting(bool bSelecting) override;
	
	FORCEINLINE bool	isAvailableSelect() { return NotiImage->GetVisibility() == ESlateVisibility::HitTestInvisible ? true : false; }

	

	UFUNCTION(BlueprintCallable, Category = UUC_HeroDeck)
	void			OnClick_X();

	UFUNCTION()
	void			ReqDeckSelect();

private:
	void			RefreshHeroDeck();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroDeck)
	UButton*				Button_HeroCard = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroDeck)
	UButton*				Button_X = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroDeck)
	UWidgetSwitcher*		WidgetSwitcher_Select = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroDeck)
	UImage*					Image_HeroCard = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroDeck)
	UImage*					Image_HeroJob = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroDeck)
	UImage*					Image_Continent = nullptr;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroDeck)
	UImage*					Image_Selecting = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
	UImage*				Grade_BG = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
	UImage*				Grade_10 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
	UImage*				Grade_soul = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
	UImage*				Image_Back = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
	UWidgetSwitcher*		WidgetSwitcherStar = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroInventory_ScrollItem)
	TArray<UImage*>			HeroStarArray;
	


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroDeck)
	UTextBlock*				TextBlock_HeroLevel = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroDeck)
	UTextBlock*				TextBlock_HeroName = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroDeck)
	UTextBlock*				Text_Order = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroDeck)
	UPanelWidget*			CrusaderHP = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroDeck)
	UProgressBar*			Hp_Progressbar = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroDeck)
	UTextBlock*				Hp_TextBlock = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroDeck)
	UWidgetSwitcher*		Lock_Switcher = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroDeck)
	UImage*					NotiImage = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroDeck)
	UWidgetAnimation*		Deck_SelectingAnim;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroDeck)
	UVerticalBox*			HeroLevelStarInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUC_HeroDeck)
	UTextBlock*				Text_Strong;
	

private:
	
	FName					CurHeroID;
	int32					CurHeroLevel;
	bool					isSelecting = false;

};
