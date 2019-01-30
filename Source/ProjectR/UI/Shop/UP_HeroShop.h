// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UI/RBaseWidget.h"
#include "UP_HeroShop.generated.h"

UCLASS()
class PROJECTR_API UUC_HeroSummonMenus : public URBaseWidget
{
	GENERATED_BODY()
public:
	void NativeConstruct() override;

public:
	UFUNCTION()
	void OnClick_Skip();
	UFUNCTION()
	void OnClick_SkipSequence();
	UFUNCTION()
	void OnClick_OpenAll_Premium();
	UFUNCTION()
	void OnClick_ReGacha();
	UFUNCTION()
	void OnClick_ReGacha_Additional();
	UFUNCTION()
	void OnClick_Exit();


	void Invalidate(FHERO& InHero);

public:
	

	UPROPERTY()
	UWidgetSwitcher* WidgetSwitcherStar;

	UPROPERTY()
	UTextBlock*	Text_HeroName;
	UPROPERTY()
	UTextBlock*	Text_HeroNick;
	
	UPROPERTY()
	UTextBlock*	Text_Top;
	UPROPERTY()
	UTextBlock*	Text_Bottom;
	UPROPERTY()
	UTextBlock*	Text_Right;
	UPROPERTY()
	UTextBlock*	Text_Left;
	UPROPERTY()
	UImage*		Image_Left;
	UPROPERTY()
	UImage*		Image_Right;

	UPROPERTY()
	TArray<UImage*>	HeroStarArray;

	UPROPERTY()
	UParticleSystem* NameEffect;

	/*CanvasPanel*/
	UPROPERTY()
	UCanvasPanel* HeroNameCanvas;
	UPROPERTY()
	UCanvasPanel* Btn_Skip;
	
	/*Buttons*/
	UPROPERTY()
	UButton*	Button_Skip;
	UPROPERTY()
	UButton*	ButtonExit;
	UPROPERTY()
	UButton*	SkipButton;
	
	UPROPERTY()
	UButton*	Button_Retry;
	UPROPERTY()
	UButton*	Button_Retry_Additional;

	UPROPERTY()
	UButton*	Button_OpenAll;

	/*WidgetSwitcher*/
	UPROPERTY()
	UWidgetSwitcher*	WidgetSwitcher_Gacha;
	
	
};

UCLASS()
class PROJECTR_API UUC_GachaClassSelectPopup : public URBaseWidget
{
	GENERATED_BODY()
public:
	enum eCLASSTYPE {TANKER, MELEE , RANGE , MAGIC , SUPPORT};
public:

	void NativeConstruct() override;

public:
	UFUNCTION()
	void OnClick_Cancel();
	UFUNCTION()
	void OnClick_OK();

	UFUNCTION()
	void OnClick_Tanker();
	UFUNCTION()
	void OnClick_Melee();
	UFUNCTION()
	void OnClick_Range();
	UFUNCTION()
	void OnClick_Magic();
	UFUNCTION()
	void OnClick_Support();


public:
	UPROPERTY()
	UButton*		Button_Cancel;
	UPROPERTY()
	UButton*		Button_OK;

	UPROPERTY()
	UButton*		Button_Tanker;
	UPROPERTY()
	UButton*		Button_Melee;
	UPROPERTY()
	UButton*		Button_Range;
	UPROPERTY()
	UButton*		Button_Magic;
	UPROPERTY()
	UButton*		Button_Supporter;



	UPROPERTY()
	TArray<UWidgetSwitcher*>	WidgetSwitcherArray;
};


UCLASS()
class PROJECTR_API UUC_GachaConfirmPopup : public URBaseWidget
{
	GENERATED_BODY()

public:

	void NativeConstruct() override;
	void Invalidate(int32 GachaKey);

	UFUNCTION()
	void OnClick_Cancel();
	UFUNCTION()
	void OnClick_OK();

public:
	UPROPERTY()
	UImage*			CostImage;
	UPROPERTY()
	UTextBlock*		CostAmount;
	UPROPERTY()
	UButton*		Button_Cancel;
	UPROPERTY()
	UButton*		Button_OK;

	UPROPERTY()
	UTextBlock*		Text_Top;
	UPROPERTY()
		UTextBlock*		Text_Bottom;
};

USTRUCT(BlueprintType)
struct PROJECTR_API FVariables_HeroShop
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UHorizontalBox*			ShopButtonUIBox = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	class UUP_BottomTip*	BottomTip = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UButton*				TicketSummonPack_Button = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UButton*				TicketSummonOnce_Button = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UButton*				NormalSummonPack_Button = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UButton*				NormalSummonOnce_Button = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UButton*				RareSummonPack_Button = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UButton*				RareSummonOnce_Button = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UButton*				PremiumSummonPack_Button = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	UButton*				PremiumSummonOnce_Button = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	ULocalizingTextBlock*	TicketSummonPack_Price_Text = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	ULocalizingTextBlock*	TicketSummonOnce_Price_Text = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	ULocalizingTextBlock*	NormalSummonPack_Price_Text = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	ULocalizingTextBlock*	NormalSummonOnce_Price_Text = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	ULocalizingTextBlock*	RareSummonPack_Price_Text = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	ULocalizingTextBlock*	RareSummonOnce_Price_Text = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	ULocalizingTextBlock*	PremiumSummonPack_Price_Text = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Variables")
	ULocalizingTextBlock*	PremiumSummonOnce_Price_Text = nullptr;
};



/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_HeroShop : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	void NativeConstruct() override;
	void NativeDestruct() override;
	
	void Invalidate();

	/*Buttons*/
	
	UFUNCTION()
	void OnClick_PointGacha();

	UFUNCTION()
	void OnClick_Premium_1Gacha();
	UFUNCTION()
	void OnClick_Premium_5Gacha();
	
	UFUNCTION()
	void OnClick_Rare_1Gacha_Ticket();
	UFUNCTION()
	void OnClick_Rare_1Gacha_Gem();
	UFUNCTION()
	void OnClick_Rare_10Gacha_Ticket();
	UFUNCTION()
	void OnClick_Rare_10Gacha_Gem();

	UFUNCTION()
	void OnClick_Normal_1Gacha_Ticket();
	UFUNCTION()
	void OnClick_Normal_1Gacha_Gold();
	UFUNCTION()
	void OnClick_Normal_10Gacha_Ticket();
	UFUNCTION()
	void OnClick_Normal_10Gacha_Gem();



	UFUNCTION()
	void OnSequenceEnd();
	UFUNCTION()
	void CallBack_SuccessSummon();
	UFUNCTION()
	void CallBack_Click(ETouchIndex::Type TouchIndex, FVector Location);

	void Execute_Gacha();
	void Pause_WatchLevelSequence(bool isPause);
	void SetVisibility_SummonMenus();
	void Binding_Delegates();

	FORCEINLINE ACameraActor* GetGachaViewTarget() { return GachaHeroShopCamActor.Get(); }
	FORCEINLINE void SetIsDoneSelecting(bool isDone) { isDoneSelecting = isDone; }
	FORCEINLINE void SetDealearHidden(bool InHidden) { GachaShopDealer->SetActorHiddenInGame(InHidden); }
	
public:

	UPROPERTY()
	UImage*		Line;
	UPROPERTY()
	UImage*		Image_BG_2;

	/*Buttons*/
	UPROPERTY()
	UButton*	Button_Gacha;
	UPROPERTY()
	UButton*	Button_Point;

	/*Custom Widget*/
	UPROPERTY()
	UUC_GachaConfirmPopup* Summon_Popup;
	UPROPERTY()
	UUC_GachaClassSelectPopup* PointSummonPopup;
	UPROPERTY()
	UUC_HeroSummonMenus*	HeroSummonMenus;

	UPROPERTY()
	UParticleSystem* PS;
	UPROPERTY()
	UParticleSystemComponent* PS_Component;

	/*Text*/
	UPROPERTY()
	UTextBlock* Text_CurrentMileage;

	/*ProgressBar*/
	UPROPERTY()
	UProgressBar* ProgressBar_Mileage;
	/*Canvas Panel*/
	UPROPERTY()
	UCanvasPanel* CanvasPanel_Head;

	int32 GachaKey = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UP_HeroShop")
	FVariables_HeroShop			variables;
private:
	UPROPERTY()
	FName SequenceKey = FName(TEXT("Gacha"));
	
	UPROPERTY()
	class ABaseSequenceActor* SequenceActor;
	UPROPERTY()
	TWeakObjectPtr<ACameraActor>	GachaHeroShopCamActor;
	UPROPERTY()
	ACharacter*	GachaShopDealer;


	EUIPageEnum PrevUIPage;
	bool isDoneSelecting = false;
};