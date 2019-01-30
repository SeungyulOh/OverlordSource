// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Object.h"
#include "SharedConstants/GlobalConstants.h"
#include "UI/UIDefs.h"
#include "XmlNode.h"
#include "Network/PacketDataStructures.h"
#include "Network/ClientProtocols.h"
#include "ForceTutorialManager.generated.h"


//class URGameInstance;
class FXmlNode;
class UXmlString;
// class UUC_QuestArrow;
class URScrollItemData;
class UUP_ControlBlock;
struct FQuestTableInfo;

#define UNLOCKTUTOINDEX_SINGLERAID				4
#define UNLOCKTUTOINDEX_STAGEHARD				5
#define UNLOCKTUTOINDEX_SINGLERAIDDECKSETTING	6
#define UNLOCKTUTOINDEX_ARENA					7
#define UNLOCKTUTOINDEX_COLOSSEUM				8
/**
 * 
 */

UCLASS(Config=Game)
class PROJECTR_API UForceTutorialManager : public UObject, public FTickableGameObject
{
	GENERATED_BODY()

public:
	void		Initialize(/*URGameInstance* GameInstance*/);
	void		Tick(float DeltaTime) override;
	bool		IsTickable() const override;
	TStatId		GetStatId() const override;

	void		ConsoleCommand(const FString& Command);

//	void		SetGameInstance(URGameInstance* GameInstance){ RGameInstance = GameInstance; }
	void		SettingCurQuest(int32 InTutoIndex = -1);
	void		SetForceTutorial(FName InTutorial);
	void		SetDetailForceTutorial();
	void		SetAvatar(FAVATAR& avatar);
	void		ChangeAvatar(FAVATAR& avatar);

	void		UnlockTutorialTriggerSetting(EFirstCheckContentsType InContents);
	void		OnStartUnlockTutorial();

	bool		IsForceTutorialing() { return bForceTutorialing; }
	bool		IsCheckLockUI(EWidgetBluePrintEnum InUI);
	//void		SetCheckShowUI(EWidgetBluePrintEnum InUI);
	//
	void CheckTutoProgress(EUIPageEnum page);
	void CheckUnlockTutorial(EFirstCheckContentsType InType);

	void SetCheckHaveTuto(FName currStageID);
	void SetCheckCurrentStage(FName currStageID);
	void SetCheckFirstMosterSpawn();
	void		SetCheckOpenUIPage(EUIPageEnum InPage);
	void		SetCheckInGame();
	void		SetCheckDecks(TArray<FString>& DeckList);
	void		SetCheckUIVisible();
	void		SetCheckGachaWait();
	void		SetPlayStateEnter();
	void		SetPlayStateLeave();

	void		SetCheckWaitEventOpenUIPage(EUIPageEnum InPage);

// 	void		SetROA_STATUS_RP(FROA_ADVENTURE& Indata);
// 	void		SetQuestData(FROA_ADVENTURE& Indata);
// 	FROA_ADVENTURE* GetQuestData() { return &CurQuestData; }
// 	FROA_QUEST*	GetQuest(FName InQuestId);
// 	FROA_QUEST*	GetQuestID(int32 InQuestId);
// 	FROA_QUEST* GetCurExeQuest() { return CurExeQuest; }
// 	FQuestTableInfo* GetQuestTable(int32 id);

// 	void		QuestGuideGoContent(FQuestTableInfo* InTableInfo);

// 	float		GetQuestPercent();

// 	bool		GetNoCompleteQuest(FROA_QUEST& OutQuest);

	void		TestForceTutorial(FString InName);
	void		HideBlockUI();

	UFUNCTION()
	void		OnSetLobbyUIStart();

	UFUNCTION()
	void		OnExeNextNode();

	UFUNCTION()
	void		OnExeNodeDelay();
	
	void		SkipUnlockTutorial();
	void		ClearForceTutorial();

private:	
	void		SetConditionsXml(FXmlNode* node);
	void		SetLocksXml(FXmlNode* node);
	void		SetBlanksXml(FXmlNode* node);
	void		SetButtonsXml(FXmlNode* node);
	void		SetEventsXml(FXmlNode* node);
	void		SetExecuteXml(FXmlNode* node);

	bool		IsCheckConditionNode(FXmlNode* InNode);
	void		ExeConditionCheck();
	void		ExeShowForceTutorialUI();
	void		ExeEvents(FXmlNode* node);
	void ShowUIPage(FXmlNode* InNode);
	void		SetBlankNode(FXmlNode* InNode, UUP_ControlBlock* InControlBlock);
	void		SetButtonNode(FXmlNode* InNode, UUP_ControlBlock* InControlBlock);

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UForceTutorialManager)
	UXmlString*					CurXmlString = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UForceTutorialManager)
	FName						CurQuest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UForceTutorialManager)
	FName						CurSubQuest;

// 	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = UForceTutorialManager)
// 	TWeakObjectPtr<UUC_QuestArrow>				UIArrow;


private:
// 	UPROPERTY()
// 	URGameInstance*				RGameInstance = nullptr;
	FAVATAR						CurAvatar;
// 	FROA_ADVENTURE				CurQuestData;

// 	FROA_QUEST*					CurExeQuest = nullptr;

	bool						bFirstLobby = true;
	TArray<FXmlNode*>			CurXmlNodeList;
	FTimerHandle				TimeHandler_ExeNextNode;
	FTimerHandle				TimeHandler_ExeNodeDelay;

	float						fCurrentCoolTime = 0.0f;
	bool						bExeCoolTime = false;

	bool						bForceTutorialing = false;
	bool						bNextFrameExe = false;
	bool						bPlayState = false;
	TArray<FName>				SubQuestList;
	TMap<FString, FXmlNode*>	CurXmlNodeMap;

	TMap<EWidgetBluePrintEnum, EWidgetBluePrintEnum>	LockUIMap;
	EWidgetBluePrintEnum		CheckShowUI = EWidgetBluePrintEnum::WBP_None;
	EUIPageEnum					CheckOpenUIPage = EUIPageEnum::UIPage_None;
	TArray<FXmlNode*>			ConditionsNodeList; //del?
	FXmlNode*					ConditionsNode = nullptr;
	FXmlNode*					GachaWaitNode = nullptr;

	FXmlNode*					OpenUIPageNode = nullptr;
	FXmlNode*					ShowUINode = nullptr;
	FXmlNode*					InGameNode = nullptr;
	FXmlNode*					UIVisibleNode = nullptr;
	FXmlNode*					UIEnableNode = nullptr;

	FXmlNode*					BlanksNode = nullptr;
	FXmlNode*					ButtonsNode = nullptr;
	FXmlNode*					EventsNode = nullptr;
	FXmlNode*					NewCreatesNode = nullptr;	
	FXmlNode*					DecksNode = nullptr;	
	FXmlNode*					WaitEventNode = nullptr;
	FXmlNode*					DlgNode = nullptr;	
	FXmlNode*					DlgAnimationStartNode = nullptr;
	FXmlNode*					SendContentsUnlockNode = nullptr;
	FXmlNode*					InGameDlgStartNode = nullptr;
	FXmlNode*					InGameDlgEndNode = nullptr;
	FXmlNode*					InGameDlg = nullptr;
	FXmlNode* FirstSpawn = nullptr;
	FXmlNode* checkCurrentStage = nullptr;

	UPROPERTY()
	UButton* currEventBtn = nullptr;
public:
	bool						bUnlockTutorialTrigger = false;
	bool						bSkipable = false;
	bool						bForcePause = false;
	int32						UnlockTutorialIndex = -1;
	FName						StageID;
	int32						nCurrentTutorialStep;
	bool HaveTuto = false;
};