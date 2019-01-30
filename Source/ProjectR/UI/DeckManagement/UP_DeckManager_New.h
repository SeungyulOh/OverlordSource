// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UI/Common/ScrollView/UP_HeroScrollView_Bottom.h"
#include <list>
#include "UP_DeckManager_New.generated.h"


/*			FUI_Variable			*/
/* this structure is only for variables in UMG blueprint */
/* Reason 1. if UI designer is trying to change variable that is using in C++ , they would need to double-check if the name of variable matches in Widget BP*/
/* Reason 2. Easier to handle and manage*/
/*									*/				

USTRUCT(BlueprintType)
struct PROJECTR_API FUI_Variables
{
	GENERATED_USTRUCT_BODY()
public:

	/************** UMG Variables **************/

	//TextBlock
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager)
	UTextBlock*				TextBlock_AttackPower = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager)
	UTextBlock*				TextBlock_PartyAttackPower = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager)
	UTextBlock*				CrewSpirit_Text = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager)
		UTextBlock*			Text_NeedStamina = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager)
	UTextBlock*				StageNumber = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager)
	UTextBlock*				StageTitle = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager)
	UTextBlock*				ClearTitle = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager)
	UImage*					ChangeImage1 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager)
	UImage*					ChangeImage2 = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager)
	UImage*					SpiritCircleImage = nullptr;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager)
	UHorizontalBox*			HorizontalPrev = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager)
	UHorizontalBox*			HorizontalAfter = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager)
	UTextBlock*				TextPrev = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager)
	UTextBlock*				TextAfter = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager)
	UButton*				ButtonPrev = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager)
	UButton*				ButtonAfter = nullptr;

	//Buttons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager)
	UButton*				Button_Start = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager)
	UButton*				Button_AutoSetting = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager)
	UButton*				Button_Clear = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager)
	class UUC_PartyEffect*	UC_PartyEffect = nullptr;

	//Arrays
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager)
	TArray<class UUC_HeroDeck*>	HeroDeckList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager)
	TArray<class UUC_SkillForDeck*> HeroSkillList;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager)
	TArray<class UUC_Item_Icons*> RewardIcons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager)
	TArray<class USizeBox*> RewardSizeBoxes;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager)
	TArray<class UBaseMonsterIcon*> MonsterIcons;

// 	//ScrollView
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager)
// 	class URScrollView*			ScrollView_Monster = nullptr;

	enum class EDeckUIType : uint8
	{
		CAMPAIGN_NORMAL,
		CAMPAIGN_HARD,
		SINGLERAID,
		Max
	};

	/*Current UI Page is for what?*/
	EDeckUIType DeckUIType = EDeckUIType::Max;

	/*********** UMG Variables End ****************/
};




USTRUCT(BlueprintType)
struct PROJECTR_API FUI_Renderer
{
	GENERATED_USTRUCT_BODY()

	void Prepare();
	void Render();


	FDeckManager_Blackboard* blackboard;
	FUI_StateController*	 StateController;
	FUI_Variables*			 variables;
};






/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_DeckManager_New : public URBaseWidget
{
	GENERATED_BODY()

/*overrided funtion*/
public:
	void NativeConstruct() override;
	void NativeDestruct() override;


/*custom function*/
public:

	//when blackboard state changed, this function will be called.
	UFUNCTION()
	void Update_FromBlackboard();

	
	
	
	//when user touch gamestart button
	UFUNCTION()
	void OnClick_GameStart();
	UFUNCTION()
	void GameStart();
	UFUNCTION()
	void AutoDeckSetting();
	UFUNCTION()
	void ClearDeckSetting();
	UFUNCTION()
	void onACK_DeckList(FDeckSelected DeckSelected);

	UFUNCTION()
	void OnClick_ButtonPrev();
	UFUNCTION()
	void OnClick_ButtonAfter();



/*UProperty variables*/
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUP_DeckManager")
	FUI_Variables					Variables;
	/*Custom Modules*/
	UPROPERTY()
	FUI_Renderer					Renderer;
	
	FDeckManager_Blackboard*		Blackboard;
	FUI_StateController*			StateController;

	/*PlayHeroInfo For GameStart*/
	UPROPERTY()
	FPlay_Heroes_Info PlayHeroInfo;

};







