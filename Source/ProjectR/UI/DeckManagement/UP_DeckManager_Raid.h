// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/RBaseWidget.h"
#include "UI/Common/ScrollView/UP_HeroScrollView_Bottom.h"
#include "UP_DeckManager_Raid.generated.h"

class UUC_HeroDeck;
class UUC_SkillForDeck;
class UUC_Colosseum_Deck_New;
struct FRaidStageTableInfo;
class UBaseStageInfo;
struct FFloorStageDetail;

USTRUCT(BlueprintType)
struct PROJECTR_API FVariables_MultiRaid
{
	GENERATED_USTRUCT_BODY()
public:

	/************** UMG Variables **************/
	///////////////top bar
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		ULocalizingTextBlock* Text_Title;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		UButton* Button_Close;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		UWidgetSwitcher* WidgetSwitcherDifficulty;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		ULocalizingTextBlock* CombatTitleTextblock;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		UTextBlock* TextBlock_AttackPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		ULocalizingTextBlock* PartyCombatTitle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		UTextBlock* TextBlock_PartyAttackPower;
	/////////////////Deck button
	//my deck button
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		ULocalizingTextBlock* Text_MyNick;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		ULocalizingTextBlock* Text_MyPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		UUC_HeroDeck* UC_DeckManager_Hero_01;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		UUC_SkillForDeck* UC_DeckManager_Hero_Skill01;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		ULocalizingTextBlock* Text_MyLeaderEffectTitle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		ULocalizingTextBlock* Text_MyLeaderEffectDesc;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
	 	TArray<class UUC_Colosseum_Deck_New*> UC_Party_Icons;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		ULocalizingTextBlock* Text_MyCrewEffectTitle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		TArray<UTextBlock*>		Text_PartyEffectFirstArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		TArray<UTextBlock*>		Text_PartyEffectSecondArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		UOverlay* MyReadyOverlay;
	//party deck button
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		TArray<ULocalizingTextBlock*> Text_PartyNicks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		TArray<ULocalizingTextBlock*> Text_PartyPowers;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		TArray<UUC_HeroDeck*> UC_DeckManager_Heros;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		TArray<UUC_SkillForDeck*> UC_Skill_ForDecks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		TArray<ULocalizingTextBlock*> Text_PartyEffect_Titles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		TArray<ULocalizingTextBlock*> Text_PartyEffect_Descs;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		TArray<UOverlay*> PartyReadyOverlays;
	//footers
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		UButton* Button_On;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		ULocalizingTextBlock* LocalTextBlockPartyID;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		UButton* Button_Invite;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		UWidgetSwitcher* WidgetSwitcher_IsReady;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		UButton* Button_Ready;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		UButton* Button_Cancel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		ULocalizingTextBlock* Text_NeedStamina;
	//counter
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
		ULocalizingTextBlock* TextBlock_PickTimer;

	TArray<int32> PartyCombatStat;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
// 	UButton*						ButtonPartyPublic;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
// 	UWidgetSwitcher*				SwitcherPartyPublic;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
// 	UButton*						ButtonPartyPrivate;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
// 	UWidgetSwitcher*				SwitcherPartyPrivate;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
// 	UWidgetSwitcher*				ControlSwither;
// 
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
// 	UButton*						ReadyButton;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
// 	UButton*						CancelButton;
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
// 	UButton*						BattleButton;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
// 	UTextBlock*			TextBlock_AttackPower = nullptr;
// 
// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UUP_DeckManager_Raid)
// 	TArray<class UUC_Raid_Deck_MR*> PlayerDecks;

	int32 MySlot = INDEX_NONE;
	bool bRoomLeader = false;

	/*********** UMG Variables End ****************/
};

USTRUCT(BlueprintType)
struct PROJECTR_API FRenderer_MultiRaid
{
	GENERATED_USTRUCT_BODY()

	void Prepare();
	void Render();

	void SetPartyCombatStat();

	FDeckManager_Blackboard*  blackboard;
	FUI_StateController*	  StateController;
	FVariables_MultiRaid*	variables;

	UBaseStageInfo*	BaseStageInfo;
	FRaidStageTableInfo* FloorStageTable;
};


UCLASS()
class PROJECTR_API UUC_Raid_Deck_MR : public URBaseWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void Init(FString nickname);
	void SetReady(bool InReady);
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_Raid_Deck_MR")
	TArray<URBaseDeckWidget*> Decks;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_Raid_Deck_MR")
	UWidgetSwitcher* DeckSwitcher;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_Raid_Deck_MR")
	UTextBlock* NickName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUC_Raid_Deck_MR")
	UTextBlock* ReadyText;

	bool bReady = false;
};

/**
 * 
 */
UCLASS()
class PROJECTR_API UUP_DeckManager_Raid : public URBaseWidget
{
	GENERATED_BODY()
	
	
public:
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	
	//when blackboard state changed, this function will be called.
	UFUNCTION()
	void Update_FromBlackboard();

	UFUNCTION()
	void PartyUpdatePlayer(int32 kid);
	UFUNCTION()
	void RenewPartyPublic();
	UFUNCTION()
	void OnReceiveDeckSelect(FDeckSelected InDeckSelected);
	UFUNCTION()
	void OnPartyPlayerExit(int32 InSlotNumber);
	UFUNCTION()
	void OnLeaderSlot(int32 InSlotNumber, bool IsLeader);
	
	UFUNCTION()
	void OnDeckReady(int32 InSlotNumber, bool InReady);
	
	UFUNCTION()
		void OnClickedPublicButton();

	UFUNCTION()
	void OnClickedBattle();
	UFUNCTION()
	void OnClickedCancel();
	UFUNCTION()
	void OnClickedReady();


	void SetPartyOverlay(int32 state, int32 index);
public:
	/**/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UUP_DeckManager_Raid")
	FVariables_MultiRaid Variables;
	UPROPERTY()
	FRenderer_MultiRaid Renderer;

	FDeckManager_Blackboard*		Blackboard;
	FUI_StateController*			StateController;

	UBaseStageInfo*	BaseStageInfo;
	FRaidStageTableInfo* FloorStageTable;
};
