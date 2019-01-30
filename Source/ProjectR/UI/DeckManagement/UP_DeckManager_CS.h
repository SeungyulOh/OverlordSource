// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UI/Common/ScrollView/UP_HeroScrollView_Bottom.h"
#include "UP_DeckManager_CS.generated.h"

USTRUCT(BlueprintType)
struct PROJECTR_API FVariables_CSDeckManager
{
	GENERATED_USTRUCT_BODY()
public:

	/************** UMG Variables **************/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Colosseum_DeckSetting)
	UTextBlock*							TextBlock_PickTimer = nullptr;
	

	// Left
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Colosseum_DeckSetting)
	UTextBlock*							LAvatarNameText = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Colosseum_DeckSetting)
	UTextBlock*							LAvatarLevel = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Colosseum_DeckSetting)
	UTextBlock*							LGuildName = nullptr;

	// Right
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Colosseum_DeckSetting)
	UTextBlock*							RAvatarNameText = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Colosseum_DeckSetting)
	UTextBlock*							RAvatarLevel = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Colosseum_DeckSetting)
	UTextBlock*							RGuildName = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Colosseum_DeckSetting)
	UTextBlock*							Text_Ready = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Colosseum_DeckSetting)
	UButton*							Button_SelectHero = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Colosseum_DeckSetting)
	UButton*							Button_QuickStart = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Colosseum_DeckSetting)
	UButton*							Button_DeselectAll = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Colosseum_DeckSetting)
	UButton*							Button_Back = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Colosseum_DeckSetting)
	class UUC_Colosseum_GroupDeck* MyGroupDeck;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_Colosseum_DeckSetting)
	class UUC_Colosseum_GroupDeck* EnemyGroupDeck;

	//Arrays

	/*********** UMG Variables End ****************/
};

USTRUCT(BlueprintType)
struct PROJECTR_API FRenderer_CSDeckManager
{
	GENERATED_USTRUCT_BODY()

	void Prepare();
	void Render();


	FDeckManager_Blackboard*  blackboard;
	FUI_StateController*	  StateController;
	FVariables_CSDeckManager* variables;
};

UCLASS()
class PROJECTR_API UUC_Colosseum_Deck_New : public URBaseDeckWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void SetAvailable(bool inAvailable) override;
	virtual void SetSelecting(bool bSelecting) override;
	virtual void SetHero(FString heroUD) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_Colosseum_Deck_New")
	UWidgetSwitcher* SelectingSwitcher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_Colosseum_Deck_New")
	UButton* InputButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_Colosseum_Deck_New")
	UImage*					Image_Selecting = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_Colosseum_Deck_New")
	class URenewal_BaseHeroIcon* HeroIcon = nullptr;


};

UCLASS()
class PROJECTR_API UUC_Colosseum_GroupDeck : public URBaseWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_Colosseum_GroupDeck")
	UWidgetSwitcher*				TeamSwitcher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_Colosseum_GroupDeck")
	TArray<URBaseDeckWidget*> Decks;

};


/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_DeckManager_CS : public URBaseWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	
	//when blackboard state changed, this function will be called.
	UFUNCTION()
	void Update_FromBlackboard();

	UFUNCTION()
	void Render_First(int32 UserKID);
	UFUNCTION()
	void Render_EnemyDeck(FCSDeckSelected InDeckSelected);
	UFUNCTION()
	void Render_Time(int32 Time);

	UFUNCTION()
	void OnClick_Start();
	UFUNCTION()
	void OnStepGo(int32 Sequence);
	UFUNCTION()
	void OnReceiveReady(int32 InKid, bool InReady);

	UFUNCTION()
	void AutoSettings();
	UFUNCTION()
	void DeselectAll();
	UFUNCTION()
	void Onclick_BackButton();

public:


	/**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUP_DeckManager_CS")
	FVariables_CSDeckManager Variables;
	UPROPERTY()
	FRenderer_CSDeckManager Renderer;

	FDeckManager_Blackboard*		Blackboard;
	FUI_StateController*			StateController;

private:
	bool							IsReady = false;
	
};
