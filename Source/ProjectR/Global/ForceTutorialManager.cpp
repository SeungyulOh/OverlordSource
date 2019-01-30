// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ForceTutorialManager.h"

#include "SharedConstants/GlobalConstants.h"
#include "GlobalIntegrated.h"
#include "Global/XmlManager.h"

#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Network/PacketFunctionLibrary.h"
#include "Network/HttpRequestClient.h"

#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"

#include "GamePlayBluePrintFunction.h"

#include "GameInfo/UserInfo.h"
#include "GameInfo/RInventory.h"
#include "GameInfo/PlayStateInfo.h"

#include "Utils/CharacterHelpFunctionLibrary.h"
#include "UI/RWidgetManager.h"
#include "UI/RBaseWidget.h"
#include "UI/Common/UP_CommonPopupSplash.h"
#include "UI/Common/UP_ControlBlock.h"
#include "UI/Control/RScrollView.h"
#include "UI/Control/RScrollItem.h"
#include "UI/Control/RScrollItemData.h"
#include "UI/Component/CheckBox_Radio.h"
#include "UI/Control/RBaseButton.h"
#include "UI/UI_Campaign/UP_CampaignTower_Scrollview.h"
#include "UI/UI_Campaign/RTowerScrollView.h"
#include "UI/UI_Campaign/UP_Campaign_Tower.h"
#include "UI/Campaign/UC_StageButton.h"
#include "UI/DeckManagement/UP_DeckManager_New.h"
#include "UI/DeckManagement/UC_HeroDeck.h"

#include "Utils/UIFunctionLibrary.h"
#include "Utils/UtilFunctionLibrary.h"

#include "UI/Common/ScrollView/UP_HeroScrollView_Bottom.h"

#include "Public/XmlString.h"
#include "XmlFile.h"
#include "UP_GenericScrollview.h"

#include "Engine/UserInterfaceSettings.h"
#include "ContentGameMode/Controllers/LobbyPlayerController.h"
#include "Lobby/TowerLobbyLevelScriptActor.h"
#include "CustomActors/RHeroGacha.h"


void UForceTutorialManager::Initialize(/*URGameInstance* GameInstance*/)
{	
//	SetGameInstance(GameInstance);
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance))
	{
		if (IsValid(RGameInstance->EventManager))
		{
			RGameInstance->EventManager->OnLobbyEnterUIEvent.RemoveDynamic(this, &UForceTutorialManager::OnSetLobbyUIStart);
			RGameInstance->EventManager->OnLobbyEnterUIEvent.AddDynamic(this, &UForceTutorialManager::OnSetLobbyUIStart);
		}
	}
}

void UForceTutorialManager::Tick(float DeltaTime)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (bExeCoolTime)
	{
		fCurrentCoolTime += DeltaTime;
		if(0.5f - fCurrentCoolTime <= 0)
		{
			fCurrentCoolTime = 0.0f;
			bExeCoolTime = false;
		}
	}

	if (bNextFrameExe)
	{		
		bNextFrameExe = false;
		ExeShowForceTutorialUI();
	}

	SetCheckUIVisible();
	SetCheckGachaWait();

	if (bForceTutorialing)
	{
		if (bPlayState)
		{
			if (IsValid(RGameInstance) && IsValid(RGameInstance->PlayStateInfo))
			{
				int32 spawnCount = RGameInstance->PlayStateInfo->GetHeroSpawnCount();
				if (spawnCount > 0)
				{
					for (int32 i = 0; i < spawnCount; ++i)
					{
						AActor* actor = RGameInstance->PlayStateInfo->GetOwnerCharacter(i);
						if (IsValid(actor))
						{
							UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(actor);
							if (IsValid(BattleEntityComponent))
							{
								float maxHp = BattleEntityComponent->GetMaxHP();
								float curHp = BattleEntityComponent->GetHP();
								float percent = curHp / maxHp * 100.0f;
								if (percent < 30.0f)
								{
									BattleEntityComponent->AddHP(maxHp, nullptr);
								}
							}
						}
					}
				}
			}
		}
	}
}

bool UForceTutorialManager::IsTickable() const
{
	return true;
}

TStatId UForceTutorialManager::GetStatId() const
{
	return Super::GetStatID();
}

void UForceTutorialManager::ConsoleCommand(const FString& Command)
{
#ifdef WITH_EDITOR
	const TCHAR* Delims[] = { TEXT(" ") };
	FString strLower = Command.ToLower();
	TArray<FString> Parsed;
	strLower.ParseIntoArray(Parsed, Delims, 1);

	if (Parsed.Num() > 0)
	{
		FString strComp = Parsed[0];
	}
#endif
}

void UForceTutorialManager::ClearForceTutorial()
{
	//clear Member variable
}

void UForceTutorialManager::OnSetLobbyUIStart()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (bFirstLobby)
	{
		if (IsValid(RGameInstance) && RGameInstance->GameEnvironmentVar.ForceTutorialEnable)
			SettingCurQuest();

		//SettingCurQuest();

		bFirstLobby = false;
	}
	else
	{

	}
}

void UForceTutorialManager::SkipUnlockTutorial()
{
	UE_LOG(LogUI, Log, TEXT("Tutorial Skip!!!!"));
	CurQuest = "";
	CurSubQuest = "";
	bForceTutorialing = false;
	bSkipable = false;
	if(bForcePause)
	{
		bForcePause = false;
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		UGameplayStatics::SetGlobalTimeDilation(RGameInstance, 1.0f);
	}

	HideBlockUI();
}

void UForceTutorialManager::OnStartUnlockTutorial()
{
	if (!bUnlockTutorialTrigger)
		return;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance) && RGameInstance->GameEnvironmentVar.ForceTutorialEnable)
	{
		SettingCurQuest(UnlockTutorialIndex);

		bUnlockTutorialTrigger = false;
		UnlockTutorialIndex = -1;
	}
}

void UForceTutorialManager::UnlockTutorialTriggerSetting(EFirstCheckContentsType InContents)
{
// #define UNLOCKTUTOINDEX_SINGLERAID				4
// #define UNLOCKTUTOINDEX_STAGEHARD				5
// #define UNLOCKTUTOINDEX_SINGLERAIDDECKSETTING	6
// #define UNLOCKTUTOINDEX_ARENA					7
// #define UNLOCKTUTOINDEX_COLOSSEUM				8

#define UNLOCKTUTOINDEX_HEROMANAGEMENT				5
	int32 nTut = -1;
 	switch (InContents)
 	{
	case EFirstCheckContentsType::FirstHeroManagement: nTut = UNLOCKTUTOINDEX_HEROMANAGEMENT; break;
//  		case EFirstCheckContentsType::FirstSingleRaid:					nTut = UNLOCKTUTOINDEX_SINGLERAID;				break;
//  		case EFirstCheckContentsType::FirstStageHard1_1:				nTut = UNLOCKTUTOINDEX_STAGEHARD;				break;
//  		case EFirstCheckContentsType::FirstArena:						nTut = UNLOCKTUTOINDEX_ARENA;					break;
//  		case EFirstCheckContentsType::FirstColosseum:					nTut = UNLOCKTUTOINDEX_COLOSSEUM;				break;
//  		case EFirstCheckContentsType::FirstSingleRaid_DeckSetting:		nTut = UNLOCKTUTOINDEX_SINGLERAIDDECKSETTING;	break;
	default: break;
 	}

	if (nTut == -1)
		return;

	bUnlockTutorialTrigger = true;
	bSkipable = true;
	UnlockTutorialIndex = nTut;
}

void UForceTutorialManager::TestForceTutorial(FString InName)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (!IsValid(CurXmlString))
	{
		CurXmlString = UXmlManager::GetInstancePtr()->GetEtcString(TEXT("ForceTutorial"));
		if (IsValid(CurXmlString))
		{
			//CurXmlNodeList.Empty();
			CurXmlNodeMap.Empty();

			FXmlNode* rootNode = CurXmlString->GetRootNode();
			if (rootNode)
			{
				//int32 NumChildrenNode = rootNode->GetChildrenNodes().Num();
				//CurXmlNodeList.Reset(NumChildrenNode);

				FString strTag;
				for (FXmlNode* child : rootNode->GetChildrenNodes())
				{
					//CurXmlNodeList.Emplace(child);

					FString name = child->GetAttribute(TEXT("name"));
					CurXmlNodeMap.Emplace(name, child);
				}
			}
		}
	}

	if (CurXmlNodeMap.Contains(InName))
	{
		FXmlNode* node = CurXmlNodeMap[InName];
		if (node)
		{
			if (IsValid(RGameInstance) && IsValid(RGameInstance->RWidgetManager))
			{
				UUserWidget* userWidget = RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_ControlBlock);
				if (userWidget == nullptr)
				{
					userWidget = RGameInstance->RWidgetManager->CreateUserWidget(EWidgetBluePrintEnum::WBP_ControlBlock);
				}

				RGameInstance->RWidgetManager->ShowUserWidget(EWidgetBluePrintEnum::WBP_ControlBlock, true, FORCETUTORIAL_BLOCK_ZORDER);
				UUP_ControlBlock* BlockUI = Cast<UUP_ControlBlock>(RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_ControlBlock));
				if (IsValid(BlockUI))
				{
					FString strTag;
					for (FXmlNode* child : node->GetChildrenNodes())
					{
						strTag = child->GetTag();
						if (strTag == TEXT("blanks"))
						{
							SetBlankNode(child, BlockUI);
						}
						else if (strTag == TEXT("buttons"))
						{
							SetButtonNode(child, BlockUI);
						}
					}
				}
			}
		}
	}
}

void UForceTutorialManager::HideBlockUI()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance) && IsValid(RGameInstance->RWidgetManager))
	{
		RGameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_ControlBlock);
		RGameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_BlockScreen);
		RGameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_InGame_Tuto);
	}
}

void UForceTutorialManager::SettingCurQuest(int32 InTutoIndex)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (IsValid(RGameInstance) && IsValid(RGameInstance->DialogueManager))
// 	{
// 		RGameInstance->DialogueManager->LoadXml(TEXT("DLG_ForceTutorial"));
// 	}

	CurXmlString = UXmlManager::GetInstancePtr()->GetEtcString(TEXT("ForceTutorial"));
	if (IsValid(CurXmlString))
	{
		//CurXmlNodeList.Empty();
		CurXmlNodeMap.Empty();

		FXmlNode* rootNode = CurXmlString->GetRootNode();
		if (rootNode)
		{
			//int32 NumChildrenNode = rootNode->GetChildrenNodes().Num();
			//CurXmlNodeList.Reset(NumChildrenNode);

			FString strTag;
			for (FXmlNode* child : rootNode->GetChildrenNodes())
			{
				//CurXmlNodeList.Emplace(child);

				FString name = child->GetAttribute(TEXT("name"));
				CurXmlNodeMap.Emplace(name, child);
			}
		}		

		FName tutorialName;
		if (InTutoIndex == -1)
		{
			if (nCurrentTutorialStep == 0)
			{
				nCurrentTutorialStep = 1;

				if (IsValid(RGameInstance->HttpClient))
				{
					UPacketFunctionLibrary::AVATAR_SELECTIVE_TUTORIAL_UPDATE_RQ((int32)EFirstCheckContentsType::FirstForceTutorial, 1);
					//UPacketFunctionLibrary::ROA_TUTORIAL_PROGRESS_RQ(1);
				}
			}
			tutorialName = FName(*FString::FromInt(nCurrentTutorialStep));
		}
		else
		{
			tutorialName = FName(*FString::FromInt(InTutoIndex));
		}

		

		FForceTutorialTableInfo* ForceTutorialTableInfo = UTableManager::GetInstancePtr()->GetForceTutorialTableRow(tutorialName);
		if (ForceTutorialTableInfo)
		{
			if (!ForceTutorialTableInfo->PrevEvent.IsNone())
			{
				tutorialName = ForceTutorialTableInfo->PrevEvent;
			}
		}

		SetForceTutorial(tutorialName);
//*/

		//SetForceTutorial(TEXT("5"));
	}
}

void UForceTutorialManager::SetForceTutorial(FName InTutorial)
{
	CurQuest = "";
	CurSubQuest = "";
	bForceTutorialing = false;

	FForceTutorialTableInfo* ForceTutorialTableInfo = UTableManager::GetInstancePtr()->GetForceTutorialTableRow(InTutorial);
	if (ForceTutorialTableInfo)
	{
		CurQuest = InTutorial;

		int32 NumDetails = ForceTutorialTableInfo->Details.Num();
		SubQuestList.Empty(NumDetails);
		for (int32 i = 0; i < NumDetails; ++i)
		{
			FForceTutorialDetailTableInfo* DetailTableInfo = UTableManager::GetInstancePtr()->GetForceTutorialDetailTableRow(ForceTutorialTableInfo->Details[i]);
			if (DetailTableInfo)
			{
				SubQuestList.Emplace(ForceTutorialTableInfo->Details[i]);
			}
		}

		if (SubQuestList.Num() > 0)
		{
			CurSubQuest = SubQuestList[0];
		}
	}

	if (!CurQuest.IsNone() && !CurSubQuest.IsNone())
	{
		UE_LOG(LogBattle, Log, TEXT("UForceTutorialManager::SetForceTutorial %s %s"), *CurQuest.ToString(), *CurSubQuest.ToString());

		bForceTutorialing = true;

		SetDetailForceTutorial();
	}
	else
	{
		HideBlockUI();
		bForceTutorialing = false;
	}
}

void UForceTutorialManager::SetDetailForceTutorial()
{
	if (bExeCoolTime)
	{
		UE_LOG(LogBattle, Log, TEXT("tutorial Cooltime.. try again"));
		return;
	}
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	bExeCoolTime = true;

	if (bForceTutorialing && !CurSubQuest.IsNone())
	{
		FForceTutorialDetailTableInfo* DetailTableInfo = UTableManager::GetInstancePtr()->GetForceTutorialDetailTableRow(CurSubQuest);
		if (DetailTableInfo)
		{
			UE_LOG(LogBattle, Log, TEXT("UForceTutorialManager::SetDetailForceTutorial %s %s"), *CurSubQuest.ToString(), *DetailTableInfo->Xml);

			if (CurXmlNodeMap.Contains(DetailTableInfo->Xml))
			{
				FXmlNode* node = CurXmlNodeMap[DetailTableInfo->Xml];
				if (node)
				{
					LockUIMap.Empty();
					CheckShowUI = EWidgetBluePrintEnum::WBP_None;
					CheckOpenUIPage = EUIPageEnum::UIPage_None;
					ConditionsNodeList.Empty();
					ConditionsNode = nullptr;
					BlanksNode = nullptr;
					ButtonsNode = nullptr;
					EventsNode = nullptr;
					NewCreatesNode = nullptr;
					InGameNode = nullptr;
					DecksNode = nullptr;
					UIVisibleNode = nullptr;
					UIEnableNode = nullptr;
					WaitEventNode = nullptr;
					GachaWaitNode = nullptr;
					DlgNode = nullptr;
					DlgAnimationStartNode = nullptr;
					SendContentsUnlockNode = nullptr;
					InGameDlgStartNode = nullptr;
					InGameDlg = nullptr;
					FirstSpawn = nullptr;
					ShowUINode = nullptr;
					StageID = TEXT("");

					FString tag;
					for (FXmlNode* child : node->GetChildrenNodes())
					{
						tag = child->GetTag();

						if (tag == TEXT("conditions"))
						{
							SetConditionsXml(child);
						}
						else if (tag == TEXT("locks"))
						{
							SetLocksXml(child);
						}
						else if (tag == TEXT("blanks"))
						{
							SetBlanksXml(child);
						}
						else if (tag == TEXT("buttons"))
						{
							SetButtonsXml(child);
						}
						else if (tag == TEXT("events"))
						{
							SetEventsXml(child);
						}
						else if (tag == TEXT("execute"))
						{
							SetExecuteXml(child);
						}
						else if (tag == TEXT("newcreate"))
						{
							NewCreatesNode = child;
						}
						else if (tag == TEXT("play"))
						{
							UGameplayStatics::SetGlobalTimeDilation(RGameInstance, 1.0f);
							bForcePause = false;
						}
						else if (tag == TEXT("hideblock"))
						{
							HideBlockUI();
						}
						else if (tag == TEXT("decksettings"))
						{
							DecksNode = child;
						}
						else if (tag == TEXT("dlg"))
						{
							DlgNode = child;
						}
						else if (tag == TEXT("dlganimationstart"))
						{
							DlgAnimationStartNode = child;
						}
						else if (tag == TEXT("sendcontentsunlock"))
						{
							SendContentsUnlockNode = child;
						}
						else if (tag == TEXT("ingamedlgstart"))
						{
							InGameDlgStartNode = child;
						}
						else if (tag == TEXT("ingamedlgend"))
						{
							UUP_ControlBlock* BlockUI = Cast<UUP_ControlBlock>(RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_ControlBlock));
							if (BlockUI)
							{
								BlockUI->HideIndicator();
							}
						}
						else if (tag == TEXT("ingamedlg"))
						{
							InGameDlg = child;
						}
						else if (tag == TEXT("showui"))
						{
							ShowUINode = child;
						}
					}

					if (ConditionsNode)
					{
						if (ConditionsNode->GetChildrenNodes().Num() == 0)
						{
							ExeShowForceTutorialUI();
						}
					}
				}
			}
		}
	}
}

void UForceTutorialManager::SetConditionsXml(FXmlNode* node)
{
	if (node)
	{
		ConditionsNode = node;

		ConditionsNodeList.Empty();

		FString tag;
		for (FXmlNode* child : node->GetChildrenNodes())
		{
			tag = child->GetTag();
			if (tag == TEXT("openuipage"))
			{
				ConditionsNodeList.Emplace(child);
				OpenUIPageNode = child;

				EUIPageEnum pageid = EUIPageEnum::UIPage_None;
				UEnum* PageEnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EUIPageEnum"), true);
				if (IsValid(PageEnumPtr))
				{
					pageid = (EUIPageEnum)(PageEnumPtr->GetValueByName(FName(*child->GetAttribute("enum"))));
				}

				if (pageid != EUIPageEnum::UIPage_None)
				{
					CheckOpenUIPage = pageid;
				}
			}
			else if (tag == TEXT("ingame"))
			{
				ConditionsNodeList.Emplace(child);
				InGameNode = child;

				StageID = FName(*child->GetAttribute("stageid"));
			}
			else if (tag == TEXT("uivisible"))
			{
				ConditionsNodeList.Emplace(child);
				UIVisibleNode = child;
			}
			else if (tag == TEXT("uienable"))
			{
				ConditionsNodeList.Emplace(child);
				UIEnableNode = child;
			}
			else if (tag == TEXT("firstspawn"))
			{
				ConditionsNodeList.Emplace(child);
				FirstSpawn = child;
			}
			else if (tag == TEXT("stagecheck"))
			{
				StageID = FName(*child->GetAttribute(TEXT("stageid")));
				ConditionsNodeList.Emplace(child);
				checkCurrentStage = child;
			}
			else if (tag == TEXT("gachawait"))
			{
				ConditionsNodeList.Emplace(child);
				GachaWaitNode = child;
			}
		}
	}
}

void UForceTutorialManager::SetLocksXml(FXmlNode* node)
{
	FString tag;
	for (FXmlNode* child : node->GetChildrenNodes())
	{
		tag = child->GetTag();
		if (tag == TEXT("lockui"))
		{
			EWidgetBluePrintEnum widgetid = EWidgetBluePrintEnum::WBP_None;
			UEnum* WidgetTypeEnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWidgetBluePrintEnum"), true);
			if (IsValid(WidgetTypeEnumPtr))
			{
				widgetid = (EWidgetBluePrintEnum)(WidgetTypeEnumPtr->GetValueByName(FName(*child->GetAttribute("enum"))));
			}

			if (widgetid != EWidgetBluePrintEnum::WBP_None)
			{
				LockUIMap.Emplace(widgetid, widgetid);
			}
		}
	}
}

void UForceTutorialManager::SetBlanksXml(FXmlNode* node)
{
	BlanksNode = node;
}

void UForceTutorialManager::SetButtonsXml(FXmlNode* node)
{
	ButtonsNode = node;
}

void UForceTutorialManager::SetEventsXml(FXmlNode* node)
{
	EventsNode = node;
	WaitEventNode = nullptr;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	FString tag;
	for (FXmlNode* child : node->GetChildrenNodes())
	{
		tag = child->GetTag();
		if (tag == TEXT("waitevent"))
		{
			WaitEventNode = child;
		}
		else if (tag == TEXT("timeevent"))
		{
			if (IsValid(RGameInstance))
			{
				float delayTime = FCString::Atof(*child->GetAttribute(TEXT("delay")));
				RGameInstance->GetWorld()->GetTimerManager().ClearTimer(TimeHandler_ExeNodeDelay);
				RGameInstance->GetWorld()->GetTimerManager().SetTimer(TimeHandler_ExeNodeDelay, this, &UForceTutorialManager::OnExeNodeDelay, delayTime, false);
			}
		}
	}	
}

void UForceTutorialManager::SetExecuteXml(FXmlNode* node)
{
	FString tag;
	for (FXmlNode* child : node->GetChildrenNodes())
	{
		tag = child->GetTag();
		if (tag == TEXT("ingameautoskillicon"))
		{
			int32 value = FCString::Atoi(*child->GetAttribute("value"));

			URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
			if (IsValid(RGameUserSetting))
			{
				RGameUserSetting->AutoSkillMode = (EAIControlModeEnum)value;
				RGameUserSetting->ApplySettings(true);
			}
		}
	}
}

bool UForceTutorialManager::IsCheckLockUI(EWidgetBluePrintEnum InUI)
{
	if (bForceTutorialing)
	{
		if (LockUIMap.Contains(InUI))
		{
			return true;
		}
	}

	return false;
}

// void UForceTutorialManager::SetCheckShowUI(EWidgetBluePrintEnum InUI)
// {
// 	if (bForceTutorialing)
// 	{
// 		if (CheckShowUI != EWidgetBluePrintEnum::WBP_None)
// 		{
// 			if (CheckShowUI == InUI)
// 			{
// 				if (IsCheckConditionNode(ShowUINode))
// 				{					
// 					ExeConditionCheck();
// 				}
// 			}
// 		}
// 	}
// }

void UForceTutorialManager::CheckTutoProgress(EUIPageEnum page)
{
	if (bForceTutorialing)
	{
		UBaseStageInfo *BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
		FName stageid = BaseStageInfo->StageID;
		//in result page
		SetCheckHaveTuto(stageid);
		//in decksetting
		SetCheckCurrentStage(stageid);
		//in everypage
		SetCheckOpenUIPage(page);
	}
}

void UForceTutorialManager::CheckUnlockTutorial(EFirstCheckContentsType InType)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (!RGameInstance->ContentsUnLockManager->IsFirstEnter(InType))
		return;

	switch (InType)
	{
	case EFirstCheckContentsType::FirstForceTutorial:
		break;

	case EFirstCheckContentsType::FirstHeroManagement:
	case EFirstCheckContentsType::FirstArena:
	case EFirstCheckContentsType::FirstColosseum:
	case EFirstCheckContentsType::FirstSingleRaid:
	{
		if (RGameInstance->ContentsUnLockManager)
		{
			RGameInstance->ContentsUnLockManager->StartAndSaveUnlockTutorial(InType);
		}
	}
	break;

	case EFirstCheckContentsType::FirstStageHard1_1:
	{
		if (RGameInstance->ContentsUnLockManager)
		{
			UBaseStageInfo * BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
			FName stageid = BaseStageInfo->StageID;
			if (stageid.Compare(TEXT("H_1_6")) != 0)
				break;

			RGameInstance->ContentsUnLockManager->StartAndSaveUnlockTutorial(InType);
		}
	}
	break;

	case EFirstCheckContentsType::FirstSingleRaid_DeckSetting:
	{
		if (RGameInstance->ContentsUnLockManager)
		{
			UBaseStageInfo * BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
			FName stageid = BaseStageInfo->StageID;

			// 싱글레이드인지 체크
			if (!stageid.ToString().Contains(TEXT("S")))
				break;

			// 마지막 강제 튜토리얼 인덱스(4번까지 완료한 이후에 체크한다.)
			const int32 nLastForceTutorialIndex = 4;
			if (RGameInstance->ContentsUnLockManager->GetSavedForcetutorialStep() <= nLastForceTutorialIndex)
				break;
		
			RGameInstance->ContentsUnLockManager->StartAndSaveUnlockTutorial(EFirstCheckContentsType::FirstSingleRaid_DeckSetting);
		}
	}
	break;

	default:
	break;

	}
}

void UForceTutorialManager::SetCheckHaveTuto(FName currStageID)
{
	if (nCurrentTutorialStep == 3)
	{
		if (currStageID.Compare(TEXT("N_1_1")) == 0)
			HaveTuto = true;
	}
}

void UForceTutorialManager::SetCheckCurrentStage(FName currStageID)
{
	if (StageID != NAME_None && StageID == currStageID)
	{
		if (IsCheckConditionNode(checkCurrentStage))
			ExeConditionCheck();
	}
}

void UForceTutorialManager::SetCheckFirstMosterSpawn()
{
	if (IsCheckConditionNode(InGameNode))
	{
		if (IsCheckConditionNode(FirstSpawn))
			ExeConditionCheck();
	}
}

void UForceTutorialManager::SetCheckWaitEventOpenUIPage(EUIPageEnum InPage)
{
	if (bForceTutorialing)
	{
		if (WaitEventNode)
		{
			FString tag;
			for (FXmlNode* child : WaitEventNode->GetChildrenNodes())
			{
				tag = child->GetTag();
				if (tag == TEXT("openuipage"))
				{
					EUIPageEnum pageid = EUIPageEnum::UIPage_None;
					UEnum* PageEnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EUIPageEnum"), true);
					if (IsValid(PageEnumPtr))
					{
						pageid = (EUIPageEnum)(PageEnumPtr->GetValueByName(FName(*child->GetAttribute("enum"))));
					}

					if (pageid != EUIPageEnum::UIPage_None)
					{
						if (pageid == InPage)
						{
							WaitEventNode = nullptr;
							OnExeNextNode();
						}
					}
				}
			}
		}
	}
}

void UForceTutorialManager::SetCheckOpenUIPage(EUIPageEnum InPage)
{
	if (CheckOpenUIPage != EUIPageEnum::UIPage_None)
	{
		if (CheckOpenUIPage == InPage)
		{
			if (IsCheckConditionNode(OpenUIPageNode))
			{					
				ExeConditionCheck();
			}
		}
	}
}

void UForceTutorialManager::SetCheckInGame()
{
	if (bForceTutorialing)
	{
		if (InGameNode)
		{
			UBaseStageInfo *BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
			if (StageID == BaseStageInfo->StageID)
			{
				if (IsCheckConditionNode(InGameNode))
				{				
					ExeConditionCheck();
				}
			}
		}
	}
}

void UForceTutorialManager::SetCheckDecks(TArray<FString>& DeckList)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (bForceTutorialing)
	{
		if (DecksNode)
		{
			if (IsValid(RGameInstance) && IsValid(RGameInstance->RInventory))
			{
				DeckList.Empty();

				FString tag;
				for (FXmlNode* child : DecksNode->GetChildrenNodes())
				{
					tag = child->GetTag();
					if (tag == TEXT("hero"))
					{
						FHERO* outHero = RGameInstance->RInventory->GetHeroWithID(FName(*child->GetAttribute(TEXT("id"))));
						if (outHero != nullptr)
						{
							DeckList.Emplace(outHero->heroUD);
						}
					}
				}
			}
		}
	}
}

void UForceTutorialManager::SetPlayStateEnter()
{
	bPlayState = true;
}

void UForceTutorialManager::SetPlayStateLeave()
{
	bPlayState = false;
}

void UForceTutorialManager::SetCheckUIVisible()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (bForceTutorialing)
	{
		if (UIVisibleNode)
		{
			if (IsValid(RGameInstance) && IsValid(RGameInstance->RWidgetManager))
			{
				EWidgetBluePrintEnum widgetid = EWidgetBluePrintEnum::WBP_None;
				UEnum* WidgetTypeEnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWidgetBluePrintEnum"), true);
				if (IsValid(WidgetTypeEnumPtr))
				{
					widgetid = (EWidgetBluePrintEnum)(WidgetTypeEnumPtr->GetValueByName(FName(*UIVisibleNode->GetAttribute("enum"))));
				}

				if (widgetid != EWidgetBluePrintEnum::WBP_None)
				{
					URBaseWidget* BaseWidget = Cast<URBaseWidget>(RGameInstance->RWidgetManager->GetUserWidget(widgetid));
					if (IsValid(BaseWidget))
					{
						UWidget* widget = BaseWidget->FindChildWidgetWithCorrectName<UWidget>(BaseWidget, UIVisibleNode->GetAttribute(TEXT("name")));
						if (IsValid(widget))
						{
							switch (widget->GetVisibility())
							{
							case ESlateVisibility::HitTestInvisible:
							case ESlateVisibility::SelfHitTestInvisible:
							case ESlateVisibility::Visible:
							{
								if (IsCheckConditionNode(UIVisibleNode))
								{
									UIVisibleNode = nullptr;
									ExeConditionCheck();
								}
							}
							break;
							}
						}
					}
				}
			}
		}

		if (UIEnableNode)
		{
			if (IsValid(RGameInstance) && IsValid(RGameInstance->RWidgetManager))
			{
				EWidgetBluePrintEnum widgetid = EWidgetBluePrintEnum::WBP_None;
				UEnum* WidgetTypeEnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWidgetBluePrintEnum"), true);
				if (IsValid(WidgetTypeEnumPtr))
				{
					widgetid = (EWidgetBluePrintEnum)(WidgetTypeEnumPtr->GetValueByName(FName(*UIEnableNode->GetAttribute("enum"))));
				}

				if (widgetid != EWidgetBluePrintEnum::WBP_None)
				{
					URBaseWidget* BaseWidget = Cast<URBaseWidget>(RGameInstance->RWidgetManager->GetUserWidget(widgetid));
					if (IsValid(BaseWidget))
					{
						UWidget* widget = BaseWidget->FindChildWidgetWithCorrectName<UWidget>(BaseWidget, UIEnableNode->GetAttribute(TEXT("name")));
						if (IsValid(widget))
						{
							if (UIEnableNode->GetAttribute(TEXT("enable")) == TEXT("false"))
							{
								if (!widget->GetIsEnabled())
								{
									if (IsCheckConditionNode(UIEnableNode))
									{
										UIEnableNode = nullptr;
										ExeConditionCheck();
									}
								}
							}
							else
							{
								if (widget->GetIsEnabled())
								{
									if (IsCheckConditionNode(UIEnableNode))
									{
										UIEnableNode = nullptr;
										ExeConditionCheck();
									}
								}
							}
						}
					}
				}
			}
		}
	}
}


void UForceTutorialManager::SetCheckGachaWait()
{
	if (GachaWaitNode)
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(this);
		UWorld* GameWorld = RGameInstance->GetWorld();

		ATowerLobbyLevelScriptActor* TowerLobbyLSA = Cast<ATowerLobbyLevelScriptActor>(GameWorld->GetLevelScriptActor());

		if (TowerLobbyLSA->HeroGachaActor)
		{
			if (IsCheckConditionNode(GachaWaitNode))
			{
				GachaWaitNode = nullptr;
				ExeConditionCheck();
			}
		}
	}
}

void UForceTutorialManager::SetBlankNode(FXmlNode* InNode, UUP_ControlBlock* InControlBlock)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (InNode && IsValid(InControlBlock) && IsValid(RGameInstance) && IsValid(RGameInstance->RWidgetManager))
	{
		float offsetx = 0.0f;
		float offsety = 0.0f;
		float offsetxwidth = 0.0f;
		float offsetyheight = 0.0f;
		bool bRefresh = false;
		bool bCardRefresh = false;
		bool bBlank = false;
		TArray<UWidget*> widgetList;

		int32 x = 0;
		int32 y = 0;
		int32 width = 0;
		int32 height = 0;

		offsetx = FCString::Atof(*InNode->GetAttribute(TEXT("xoffset")));
		offsety = FCString::Atof(*InNode->GetAttribute(TEXT("yoffset")));
		offsetxwidth = FCString::Atof(*InNode->GetAttribute(TEXT("widthoffset")));
		offsetyheight = FCString::Atof(*InNode->GetAttribute(TEXT("heightoffset")));

		FString tag;
		for (FXmlNode* child : InNode->GetChildrenNodes())
		{
			tag = child->GetTag();
			if (tag == TEXT("blank"))
			{
				bBlank = true;
				x = FCString::Atoi(*child->GetAttribute(TEXT("x")));
				y = FCString::Atoi(*child->GetAttribute(TEXT("y")));
				width = FCString::Atoi(*child->GetAttribute(TEXT("width")));
				height = FCString::Atoi(*child->GetAttribute(TEXT("height")));
			}
			else if (tag == TEXT("blankgachacard"))
			{
				UWorld* GameWorld = RGameInstance->GetWorld();

				ATowerLobbyLevelScriptActor* TowerLobbyLSA = Cast<ATowerLobbyLevelScriptActor>(GameWorld->GetLevelScriptActor());

				ARHeroGachaCard* pCard = nullptr;

				if (TowerLobbyLSA->HeroGachaActor->HeroCardArray.IsValidIndex(0))
					pCard = TowerLobbyLSA->HeroGachaActor->HeroCardArray[0];

				ALobbyPlayerController * PlayerController = UUtilFunctionLibrary::GetLobbyPlayerController(GameWorld);;
				FVector2D TempLoca(0, 0);

				if (PlayerController && pCard && pCard->IsValidLowLevel())
					PlayerController->ProjectWorldLocationToScreen(pCard->GetTargetLocation(), TempLoca);

				const FVector2D viewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
				const float ViewportScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(viewportSize.X, viewportSize.Y));

				TempLoca /= ViewportScale;

				bBlank = true;
				bCardRefresh = true;

				width = FCString::Atoi(*child->GetAttribute(TEXT("width")));
				height = FCString::Atoi(*child->GetAttribute(TEXT("height")));

				x = TempLoca.X - (width / 2);
				y = TempLoca.Y - (height / 2);
			}
			else if (tag == TEXT("widgetblanks"))
			{
				EWidgetBluePrintEnum widgetid = EWidgetBluePrintEnum::WBP_None;
				UEnum* WidgetTypeEnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWidgetBluePrintEnum"), true);
				if (IsValid(WidgetTypeEnumPtr))
				{
					widgetid = (EWidgetBluePrintEnum)(WidgetTypeEnumPtr->GetValueByName(FName(*child->GetAttribute("enum"))));
				}

				if (widgetid != EWidgetBluePrintEnum::WBP_None)
				{
					if (child->GetAttribute(TEXT("refresh")) == TEXT("true"))
					{
						bRefresh = true;
					}

					URBaseWidget* BaseWidget = Cast<URBaseWidget>(RGameInstance->RWidgetManager->GetUserWidget(widgetid));
					if (IsValid(BaseWidget))
					{
						FString subTag;
						for (FXmlNode* child1 : child->GetChildrenNodes())
						{
							subTag = child1->GetTag();
							if (subTag == TEXT("widgetblank"))
							{
								UWidget* widget = BaseWidget->FindChildWidgetWithCorrectName<UWidget>(BaseWidget, child1->GetAttribute(TEXT("name")));
								if (IsValid(widget))
								{
									widgetList.Emplace(widget);
								}
							}
							else if (subTag == TEXT("stageIcon"))
							{
								UWidget* widget = BaseWidget->FindChildWidgetWithCorrectName<UWidget>(BaseWidget, child1->GetAttribute(TEXT("name")));
								UUC_StageButton* StageBtn = Cast<UUC_StageButton>(widget);
								if (IsValid(widget))
								{
									widgetList.Emplace(StageBtn->ButtonInput);
								}
							}
							else if (subTag == TEXT("heroscrollblank"))
							{
								UUP_HeroScrollView_Bottom* herolist = UUIFunctionLibrary::GetHeroScrollView();
								UUC_BaseHeroScrollView_New*	scrollView = herolist->ScrollView;
								if (scrollView)
								{
									int32 idx = FCString::Atoi(*child1->GetAttribute(TEXT("index")));
									if (scrollView->ScrollItemList.IsValidIndex(idx))
									{
										if (IsValid(scrollView->ScrollItemList[idx]))
										{
											UWidget* widget = Cast<UWidget>(scrollView->ScrollItemList[idx]);
											if(widget)
												widgetList.Emplace(widget);
										}
									}
								}
							}
							else if (subTag == TEXT("scrollblank"))
							{
								URScrollView* scrollView = BaseWidget->FindChildWidgetWithCorrectName<URScrollView>(BaseWidget, child1->GetAttribute(TEXT("name")));
								if (IsValid(scrollView))
								{
									if (child1->GetChildrenNodes().Num() > 0)
									{
										int32 idx = FCString::Atoi(*child1->GetAttribute(TEXT("index")));
										if (scrollView->ScrollItemList.IsValidIndex(idx))
										{
											if (IsValid(scrollView->ScrollItemList[idx]))
											{
												FString subTag1;
												for (FXmlNode* child2 : child1->GetChildrenNodes())
												{
													subTag1 = child2->GetTag();
													if (subTag1 == TEXT("widgetblank"))
													{
														UWidget* widget = scrollView->ScrollItemList[idx]->FindChildWidgetWithCorrectName<UWidget>(scrollView->ScrollItemList[idx], child2->GetAttribute(TEXT("name")));
														if (IsValid(widget))
														{
															widgetList.Emplace(widget);
														}
													}
													else if (subTag1 == TEXT("stageicon"))
													{
														URTowerScrollItem* StageUI = Cast<URTowerScrollItem>(scrollView->ScrollItemList[idx]);
														if (StageUI)
														{
															int32 stageIdx = FCString::Atoi(*child2->GetAttribute(TEXT("index")));
															currEventBtn = StageUI->UP_CampaignTower->NormalStageButtonArray[stageIdx]->ButtonInput;
															if (currEventBtn)
															{
																UWidget* widget = Cast<UWidget>(currEventBtn);
																if (widget)
																	widgetList.Emplace(widget);
															}
														}
													}
												}
											}
										}
									}
									else
									{
										int32 idx = FCString::Atoi(*child1->GetAttribute(TEXT("index")));
										if (scrollView->ScrollItemList.IsValidIndex(idx))
										{
											if (IsValid(scrollView->ScrollItemList[idx]))
											{
												widgetList.Emplace(scrollView->ScrollItemList[idx]);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}

		if (ButtonsNode)
		{
			if (ButtonsNode->GetChildrenNodes().Num() > 0)
			{
				InControlBlock->SetBorder(false);
			}
			else
			{
				InControlBlock->SetBorder(true);
			}
		}

		InControlBlock->SetButtonShowClear();
		InControlBlock->SetRefresh(bRefresh);
		InControlBlock->SetCardRefresh(bCardRefresh);
		InControlBlock->SetOffsetPos(offsetx, offsety);
		InControlBlock->SetOffsetSize(offsetxwidth, offsetyheight);

		bool bMaterial = false;
		if (widgetList.Num() > 0)
		{
			if (bRefresh)
			{
				InControlBlock->SetPosWidgetsCachedList(widgetList);
			}
			InControlBlock->SetPosWidgets(widgetList, bMaterial);
		}
		else
		{
			if (bBlank)
			{
				InControlBlock->SetPositionForce(x, y, width, height, bMaterial);
			}
			else
			{
				InControlBlock->SetPositionForce(0, 0, 0, 0, bMaterial);
				InControlBlock->SetBorder(false);
			}
		}
	}
}

void UForceTutorialManager::SetButtonNode(FXmlNode* InNode, UUP_ControlBlock* InControlBlock)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (InNode && IsValid(InControlBlock) && IsValid(RGameInstance) && IsValid(RGameInstance->RWidgetManager))
	{
		float offsetx = 0.0f;
		float offsety = 0.0f;
		float offsetxwidth = 0.0f;
		float offsetyheight = 0.0f;
		bool bRefresh = false;
		bool bButton = false;
		bool bCardRefresh = false;

		TArray<UWidget*> widgetList;

		int32 x = 0;
		int32 y = 0;
		int32 width = 0;
		int32 height = 0;

		offsetx = FCString::Atof(*InNode->GetAttribute(TEXT("xoffset")));
		offsety = FCString::Atof(*InNode->GetAttribute(TEXT("yoffset")));
		offsetxwidth = FCString::Atof(*InNode->GetAttribute(TEXT("widthoffset")));
		offsetyheight = FCString::Atof(*InNode->GetAttribute(TEXT("heightoffset")));

		InControlBlock->SetButtonHide();

		FString tag;
		for (FXmlNode* child : InNode->GetChildrenNodes())
		{
			tag = child->GetTag();
			if (tag == TEXT("button"))
			{
				bButton = true;
				x = FCString::Atoi(*child->GetAttribute(TEXT("x")));
				y = FCString::Atoi(*child->GetAttribute(TEXT("y")));
				width = FCString::Atoi(*child->GetAttribute(TEXT("width")));
				height = FCString::Atoi(*child->GetAttribute(TEXT("height")));
			}
			else if (tag == TEXT("buttongachacard"))
			{
				UWorld* GameWorld = RGameInstance->GetWorld();

				ATowerLobbyLevelScriptActor* TowerLobbyLSA = Cast<ATowerLobbyLevelScriptActor>(GameWorld->GetLevelScriptActor());

				ARHeroGachaCard* pCard = nullptr;

				if (TowerLobbyLSA->HeroGachaActor->HeroCardArray.IsValidIndex(0))
				{
					pCard = TowerLobbyLSA->HeroGachaActor->HeroCardArray[0];
				}

				AActor* Actor = nullptr;
				ALobbyPlayerController * PlayerController = UUtilFunctionLibrary::GetLobbyPlayerController(GameWorld);;
				FVector2D TempLoca(0, 0);

				if (PlayerController && pCard && pCard->IsValidLowLevel())
					PlayerController->ProjectWorldLocationToScreen(pCard->GetTargetLocation(), TempLoca);

				const FVector2D viewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
				const float ViewportScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(viewportSize.X, viewportSize.Y));

				TempLoca /= ViewportScale;

				bButton = true;
				bCardRefresh = true;

				width = FCString::Atoi(*child->GetAttribute(TEXT("width")));
				height = FCString::Atoi(*child->GetAttribute(TEXT("height")));

				x = TempLoca.X - (width / 2);
				y = TempLoca.Y - (height / 2);

			}
			else if (tag == TEXT("widgetbuttons"))
			{
				EWidgetBluePrintEnum widgetid = EWidgetBluePrintEnum::WBP_None;
				UEnum* WidgetTypeEnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWidgetBluePrintEnum"), true);
				if (IsValid(WidgetTypeEnumPtr))
				{
					widgetid = (EWidgetBluePrintEnum)(WidgetTypeEnumPtr->GetValueByName(FName(*child->GetAttribute("enum"))));
				}

				if (widgetid != EWidgetBluePrintEnum::WBP_None)
				{
					if (child->GetAttribute(TEXT("refresh")) == TEXT("true"))
					{
						bRefresh = true;
					}

					URBaseWidget* BaseWidget = Cast<URBaseWidget>(RGameInstance->RWidgetManager->GetUserWidget(widgetid));
					if (IsValid(BaseWidget))
					{
						FString subTag;
						for (FXmlNode* child1 : child->GetChildrenNodes())
						{
							subTag = child1->GetTag();
							if (subTag == TEXT("widgetbutton"))
							{
								UWidget* widget = BaseWidget->FindChildWidgetWithCorrectName<UWidget>(BaseWidget, child1->GetAttribute(TEXT("name")));
								if (IsValid(widget))
								{
									widgetList.Emplace(widget);
								}
							}
							else if (subTag == TEXT("stageIcon"))
							{
								UWidget* widget = BaseWidget->FindChildWidgetWithCorrectName<UWidget>(BaseWidget, child1->GetAttribute(TEXT("name")));
								UUC_StageButton* StageBtn = Cast<UUC_StageButton>(widget);
								if (IsValid(widget))
								{
									widgetList.Emplace(StageBtn->ButtonInput);
								}
							}
							else if (subTag == TEXT("heroscrollbutton"))
							{
								UUP_HeroScrollView_Bottom* herolist = UUIFunctionLibrary::GetHeroScrollView();
								UUC_BaseHeroScrollView_New*	scrollView = herolist->ScrollView;
								if (scrollView)
								{
									int32 idx = FCString::Atoi(*child1->GetAttribute(TEXT("index")));
									if (scrollView->ScrollItemList.IsValidIndex(idx))
									{
										if (IsValid(scrollView->ScrollItemList[idx]))
										{
											UWidget* widget = Cast<UWidget>(scrollView->ScrollItemList[idx]);
											if (widget)
												widgetList.Emplace(widget);
										}
									}
								}
							}
							else if (subTag == TEXT("scrollbutton"))
							{
								URScrollView* scrollView = BaseWidget->FindChildWidgetWithCorrectName<URScrollView>(BaseWidget, child1->GetAttribute(TEXT("name")));
								if (IsValid(scrollView))
								{
									if (child1->GetChildrenNodes().Num() > 0)
									{
										int32 idx = FCString::Atoi(*child1->GetAttribute(TEXT("index")));
										if (scrollView->ScrollItemList.IsValidIndex(idx))
										{
											if (IsValid(scrollView->ScrollItemList[idx]))
											{
												FString subTag1;
												for (FXmlNode* child2 : child1->GetChildrenNodes())
												{
													subTag1 = child2->GetTag();
													if (subTag1 == TEXT("widgetbutton"))
													{
														UWidget* widget = scrollView->ScrollItemList[idx]->FindChildWidgetWithCorrectName<UWidget>(scrollView->ScrollItemList[idx], child2->GetAttribute(TEXT("name")));
														if (IsValid(widget))
														{
															widgetList.Emplace(widget);
														}
													}
													else if (subTag1 == TEXT("stageicon"))
													{
														URTowerScrollItem* StageUI = Cast<URTowerScrollItem>(scrollView->ScrollItemList[idx]);
														if (StageUI)
														{
															int32 stageIdx = FCString::Atoi(*child2->GetAttribute(TEXT("index")));
															currEventBtn = StageUI->UP_CampaignTower->NormalStageButtonArray[stageIdx]->ButtonInput;
															if (currEventBtn)
															{
																UWidget* widget = Cast<UWidget>(currEventBtn);
																if (widget)
																	widgetList.Emplace(widget);
															}
														}
													}
												}
											}
										}
									}
									else
									{
										int32 idx = FCString::Atoi(*child1->GetAttribute(TEXT("index")));
										if (scrollView->ScrollItemList.IsValidIndex(idx))
										{
											if (IsValid(scrollView->ScrollItemList[idx]))
											{
												widgetList.Emplace(scrollView->ScrollItemList[idx]);
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}

		InControlBlock->SetButtonRefresh(bRefresh);
		InControlBlock->SetCardRefresh(bCardRefresh);

		if (widgetList.Num() > 0)
		{
			if (bRefresh)
			{
				InControlBlock->SetPosWidgetsButtonCachedList(widgetList);
			}

			InControlBlock->SetPosWidgetsButton(widgetList, offsetx, offsety, offsetxwidth, offsetyheight);
		}
		else
		{
			if (bButton)
			{
				InControlBlock->SetPositionButton(x, y, width, height, offsetx, offsety, offsetxwidth, offsetyheight);
			}
			else
			{
				InControlBlock->SetButtonFull();
			}
		}

		if (IsValid(InControlBlock->BackButton))
		{
			InControlBlock->BackButton->OnPressed.RemoveDynamic(this, &UForceTutorialManager::OnExeNextNode);
			InControlBlock->BackButton->OnPressed.AddDynamic(this, &UForceTutorialManager::OnExeNextNode);
		}
	}
}

bool UForceTutorialManager::IsCheckConditionNode(FXmlNode* InNode)
{
	if (InNode)
	{
		for (int32 i = 0; i < ConditionsNodeList.Num(); ++i)
		{
			if (ConditionsNodeList[i] == InNode)
			{
				ConditionsNodeList.RemoveAt(i);
				return true;
			}
		}
	}

	return false;
}

void UForceTutorialManager::ExeConditionCheck()
{
	if (ConditionsNodeList.Num() == 0)
	{
		ExeShowForceTutorialUI();
	}
}

void UForceTutorialManager::ExeShowForceTutorialUI()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (IsValid(RGameInstance) && IsValid(RGameInstance->RWidgetManager))
	{
		if (NewCreatesNode)
		{
			if (RGameInstance->RWidgetManager->IsShowUserWidget(EWidgetBluePrintEnum::WBP_ControlBlock))
			{
				RGameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_ControlBlock);
				bNextFrameExe = true;
			}
		}

		if (InGameNode)
		{
			FString tag;
			for (FXmlNode* child : InGameNode->GetChildrenNodes())
			{
				tag = child->GetTag();
				if (tag == TEXT("pause"))
				{
					UGameplayStatics::SetGlobalTimeDilation(RGameInstance, 0.0f);
					bForcePause = true;
				}
			}
		}

		if (!bNextFrameExe)
		{
			if (SendContentsUnlockNode)
			{
				if (IsValid(RGameInstance->ContentsUnLockManager))
				{
					//RGameInstance->ContentsUnLockManager->RequestRemoveUnlockNotification();
					SendContentsUnlockNode = nullptr;
				}				
			}
			UUserWidget* userWidget = RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_ControlBlock);
			if (userWidget == nullptr)
			{
				userWidget = RGameInstance->RWidgetManager->CreateUserWidget(EWidgetBluePrintEnum::WBP_ControlBlock);
			}

			RGameInstance->RWidgetManager->ShowUserWidget(EWidgetBluePrintEnum::WBP_ControlBlock, true, FORCETUTORIAL_BLOCK_ZORDER);

			//RGameInstance->RWidgetManager->ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_ControlBlock, true);

			UUP_ControlBlock* BlockUI = Cast<UUP_ControlBlock>(RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_ControlBlock));
			if (IsValid(BlockUI))
			{
				BlockUI->Clear();
				
				BlockUI->ShowSkipButton(bSkipable);

				if (IsValid(BlockUI->BackButton))
				{
					BlockUI->BackButton->OnClicked.Clear();
				}

// 				if (DlgNode)
// 				{
// 					float xoffset = FCString::Atof(*DlgNode->GetAttribute(TEXT("offsetx")));
// 					float yoffset = FCString::Atof(*DlgNode->GetAttribute(TEXT("offsety")));
// 
// 					BlockUI->SetDlgPanelPos(xoffset, yoffset);
// 				}
				if (ShowUINode)
				{
					ShowUIPage(ShowUINode);
				}
				if (BlanksNode)
				{
					SetBlankNode(BlanksNode, BlockUI);
				}

				if (ButtonsNode)
				{
					SetButtonNode(ButtonsNode, BlockUI);
				}

				if (DlgAnimationStartNode)
				{
					float fDelay = FCString::Atof(*DlgAnimationStartNode->GetAttribute(TEXT("delay")));
					BlockUI->ShowAnimation(fDelay);
				}

				if (InGameDlgStartNode)
				{
					float fDelay = FCString::Atof(*InGameDlgStartNode->GetAttribute(TEXT("delay")));
					BlockUI->ShowIndicator(fDelay);
				}

				if (InGameDlg)
				{
					float xoffset = FCString::Atof(*InGameDlg->GetAttribute(TEXT("offsetx")));
					float yoffset = FCString::Atof(*InGameDlg->GetAttribute(TEXT("offsety")));

					UUtilFunctionLibrary::SetIndicatorOffset(xoffset, yoffset);
				}

				if (!CurSubQuest.IsNone())
				{
					UE_LOG(LogBattle, Log, TEXT("UForceTutorialManager::ExeShowForceTutorialUI %s"), *CurSubQuest.ToString());
					FForceTutorialDetailTableInfo* DetailTableInfo = UTableManager::GetInstancePtr()->GetForceTutorialDetailTableRow(CurSubQuest);
					if (DetailTableInfo)
					{
						if (DetailTableInfo->SaveIdx != 0)
						{
							if (IsValid(RGameInstance->HttpClient))
							{
								if (nCurrentTutorialStep != DetailTableInfo->SaveIdx)
								{
									UE_LOG(LogBattle, Log, TEXT("UForceTutorialManager::ExeShowForceTutorialUI saveindex : %d"), DetailTableInfo->SaveIdx);
									UPacketFunctionLibrary::AVATAR_SELECTIVE_TUTORIAL_UPDATE_RQ((int32)EFirstCheckContentsType::FirstForceTutorial, DetailTableInfo->SaveIdx);
									//UPacketFunctionLibrary::ROA_TUTORIAL_PROGRESS_RQ(DetailTableInfo->SaveIdx);
								}
							}
						}

						if (DetailTableInfo->DlgString.IsNone())
						{
							BlockUI->ShowDlg(false);
						}
						else
						{
							BlockUI->ShowDlg(!BlockUI->IsIndicator());
							BlockUI->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Dialogue, DetailTableInfo->DlgString));
						}						
					}
				}
			}
		}
	}
}

void UForceTutorialManager::ChangeAvatar(FAVATAR& avatar)
{
}

void UForceTutorialManager::SetAvatar(FAVATAR& avatar)
{
	CurAvatar = avatar;
}

void UForceTutorialManager::ExeEvents(FXmlNode* node)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (node)
	{
		FString tag;
		for (FXmlNode* child : node->GetChildrenNodes())
		{
			tag = child->GetTag();
			if (tag == TEXT("buttonevent"))
			{
				EWidgetBluePrintEnum widgetid = EWidgetBluePrintEnum::WBP_None;
				UEnum* WidgetTypeEnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWidgetBluePrintEnum"), true);
				if (IsValid(WidgetTypeEnumPtr))
				{
					widgetid = (EWidgetBluePrintEnum)(WidgetTypeEnumPtr->GetValueByName(FName(*child->GetAttribute("enum"))));
				}

				if (widgetid != EWidgetBluePrintEnum::WBP_None)
				{
					URBaseWidget* BaseWidget = Cast<URBaseWidget>(RGameInstance->RWidgetManager->GetUserWidget(widgetid));
					if (IsValid(BaseWidget))
					{
						UButton* buttonwidget = BaseWidget->FindChildWidgetWithCorrectName<UButton>(BaseWidget, child->GetAttribute(TEXT("name")));
						if (IsValid(buttonwidget))
						{
							buttonwidget->OnClicked.Broadcast();
						}
					}
				}
			}
			else if (tag == TEXT("deckslotevent"))
			{
				URBaseWidget* BaseWidget = Cast<URBaseWidget>(RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_DeckManager));
				if (BaseWidget)
				{
					UUP_DeckManager_New* umg = Cast<UUP_DeckManager_New>(BaseWidget);
					if (umg)
					{
						int32 index = FCString::Atoi(*child->GetAttribute(TEXT("slotindex")));
						if (umg->Variables.HeroDeckList.IsValidIndex(index))
							umg->Variables.HeroDeckList[index]->Button_HeroCard->OnClicked.Broadcast();
					}
				}
			}
			else if (tag == TEXT("stagebuttonevent"))
			{
				URBaseWidget* BaseWidget = Cast<URBaseWidget>(RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_Campaign));
				if (BaseWidget)
				{
					UUP_Campaign_Tower* umg = Cast<UUP_Campaign_Tower>(BaseWidget);
					if (umg)
					{
						int32 index = FCString::Atoi(*child->GetAttribute(TEXT("floorindex")));
						if (umg->NormalStageButtonArray.IsValidIndex(index))
							umg->NormalStageButtonArray[index]->ButtonInput->OnClicked.Broadcast();
					}
				}
			}
			else if (tag == TEXT("rbuttonevent"))
			{
				EWidgetBluePrintEnum widgetid = EWidgetBluePrintEnum::WBP_None;
				UEnum* WidgetTypeEnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWidgetBluePrintEnum"), true);
				if (IsValid(WidgetTypeEnumPtr))
				{
					widgetid = (EWidgetBluePrintEnum)(WidgetTypeEnumPtr->GetValueByName(FName(*child->GetAttribute("enum"))));
				}

				if (widgetid != EWidgetBluePrintEnum::WBP_None)
				{
					URBaseWidget* BaseWidget = Cast<URBaseWidget>(RGameInstance->RWidgetManager->GetUserWidget(widgetid));
					if (IsValid(BaseWidget))
					{
						URBaseButton* rbuttonwidget = BaseWidget->FindChildWidgetWithCorrectName<URBaseButton>(BaseWidget, child->GetAttribute(TEXT("name")));
						if (IsValid(rbuttonwidget))
						{
							rbuttonwidget->OnButtonClick_Delegate.Broadcast();
						}
					}
				}
			}
			else if (tag == TEXT("scrollevent"))
			{
				EWidgetBluePrintEnum widgetid = EWidgetBluePrintEnum::WBP_None;
				UEnum* WidgetTypeEnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWidgetBluePrintEnum"), true);
				if (IsValid(WidgetTypeEnumPtr))
				{
					widgetid = (EWidgetBluePrintEnum)(WidgetTypeEnumPtr->GetValueByName(FName(*child->GetAttribute("enum"))));
				}

				if (widgetid != EWidgetBluePrintEnum::WBP_None)
				{
					URBaseWidget* BaseWidget = Cast<URBaseWidget>(RGameInstance->RWidgetManager->GetUserWidget(widgetid));
					if (IsValid(BaseWidget))
					{
						URScrollView* scrollwidget = BaseWidget->FindChildWidgetWithCorrectName<URScrollView>(BaseWidget, child->GetAttribute(TEXT("name")));
						if (IsValid(scrollwidget))
						{
							if (child->GetChildrenNodes().Num() > 0)
							{
								int32 idx = FCString::Atoi(*child->GetAttribute(TEXT("index")));
								URScrollItem* scrollItem = scrollwidget->GetScrollItem(idx);
								if (IsValid(scrollItem))
								{
									FString tagSub;
									for (FXmlNode* child1 : child->GetChildrenNodes())
									{
										tagSub = child1->GetTag();
										if (tagSub == TEXT("buttonevent"))
										{
											UButton* buttonwidget = scrollItem->FindChildWidgetWithCorrectName<UButton>(scrollItem, child1->GetAttribute(TEXT("name")));
											if (IsValid(buttonwidget))
											{
												buttonwidget->OnClicked.Broadcast();
											}
										}
									}
								}
							}
							else
							{
								int32 idx = FCString::Atoi(*child->GetAttribute(TEXT("index")));
								scrollwidget->SetSelectScrollListIndex(idx);
							}
						}
					}
				}
			}
			else if (tag == TEXT("heroscrollevent"))
			{
				UUP_GenericScrollview* herolist = UUIFunctionLibrary::GetGenericScrollview();
				URScrollView*	scrollwidget = herolist->Variables.ScrollView;
				if(scrollwidget->IsValidLowLevel())
				{
					int32 idx = FCString::Atoi(*child->GetAttribute(TEXT("index")));
					//scrollwidget->SetSelectScrollListIndex(idx);
					URScrollItem* pScrollitem = scrollwidget->GetScrollItem(idx);
					if(pScrollitem)
					{
						pScrollitem->ForceSelect();
					}
				}
			}
			else if (tag == TEXT("checkboxradioevent"))
			{
				EWidgetBluePrintEnum widgetid = EWidgetBluePrintEnum::WBP_None;
				UEnum* WidgetTypeEnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWidgetBluePrintEnum"), true);
				if (IsValid(WidgetTypeEnumPtr))
				{
					widgetid = (EWidgetBluePrintEnum)(WidgetTypeEnumPtr->GetValueByName(FName(*child->GetAttribute("enum"))));
				}

				if (widgetid != EWidgetBluePrintEnum::WBP_None)
				{
					URBaseWidget* BaseWidget = Cast<URBaseWidget>(RGameInstance->RWidgetManager->GetUserWidget(widgetid));
					if (IsValid(BaseWidget))
					{
						UCheckBox_Radio* checkboxradiowidget = BaseWidget->FindChildWidgetWithCorrectName<UCheckBox_Radio>(BaseWidget, child->GetAttribute(TEXT("name")));
						if (IsValid(checkboxradiowidget))
						{
							if (checkboxradiowidget->OnCheckRadioCheck.IsBound())
							{
								checkboxradiowidget->OnCheckRadioCheck.Broadcast(true);
							}
							else
							{
								if (checkboxradiowidget->OnCheckStateChanged.IsBound())
								{
									checkboxradiowidget->OnCheckStateChanged.Broadcast(true);
								}
							}
						}
					}
				}
			}
			else if (tag == TEXT("skillevent"))
			{
				EWidgetBluePrintEnum widgetid = EWidgetBluePrintEnum::WBP_None;
				UEnum* WidgetTypeEnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWidgetBluePrintEnum"), true);
				if (IsValid(WidgetTypeEnumPtr))
				{
					widgetid = (EWidgetBluePrintEnum)(WidgetTypeEnumPtr->GetValueByName(FName(*child->GetAttribute("enum"))));
				}

				if (widgetid != EWidgetBluePrintEnum::WBP_None)
				{
					URBaseWidget* BaseWidget = Cast<URBaseWidget>(RGameInstance->RWidgetManager->GetUserWidget(widgetid));
					if (IsValid(BaseWidget))
					{
						UButton* buttonwidget = BaseWidget->FindChildWidgetWithCorrectName<UButton>(BaseWidget, child->GetAttribute(TEXT("name")));
						if (IsValid(buttonwidget))
						{
							UUP_ControlBlock* BlockUI = Cast<UUP_ControlBlock>(RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_ControlBlock));
							buttonwidget->OnClicked.Broadcast();
// 							if (IsValid(BlockUI) && IsValid(BlockUI->BackButton))
// 							{
// 								BlockUI->BackButton->SetVisibility(ESlateVisibility::Collapsed);
// 								BlockUI->Button_BlockInputManager->SetVisibility(ESlateVisibility::Collapsed);
// 							}
						}
					}
				}
			}
			else if (tag == TEXT("checkboxevent"))
			{
				EWidgetBluePrintEnum widgetid = EWidgetBluePrintEnum::WBP_None;
				UEnum* WidgetTypeEnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWidgetBluePrintEnum"), true);
				if (IsValid(WidgetTypeEnumPtr))
				{
					widgetid = (EWidgetBluePrintEnum)(WidgetTypeEnumPtr->GetValueByName(FName(*child->GetAttribute("enum"))));
				}

				if (widgetid != EWidgetBluePrintEnum::WBP_None)
				{
					URBaseWidget* BaseWidget = Cast<URBaseWidget>(RGameInstance->RWidgetManager->GetUserWidget(widgetid));
					if (IsValid(BaseWidget))
					{
						UCheckBox* checkboxwidget = BaseWidget->FindChildWidgetWithCorrectName<UCheckBox>(BaseWidget, child->GetAttribute(TEXT("name")));
						if (IsValid(checkboxwidget))
						{
							if(checkboxwidget->OnCheckStateChanged.IsBound())
							{
								checkboxwidget->SetCheckedState(ECheckBoxState::Checked);
								checkboxwidget->OnCheckStateChanged.Broadcast(true);
							}
						}
					}
				}

			}
			else if (tag == TEXT("gotutorialstage"))
			{
				RGameInstance->ChangeState(EGameStateEnum::VE_Tutorial);
			}
			else if (tag == TEXT("cardopen"))
			{
				UWorld* GameWorld = RGameInstance->GetWorld();

				ATowerLobbyLevelScriptActor* TowerLobbyLSA = Cast<ATowerLobbyLevelScriptActor>(GameWorld->GetLevelScriptActor());

				if (TowerLobbyLSA->HeroGachaActor)
				{
					ALobbyPlayerController * PlayerController = UUtilFunctionLibrary::GetLobbyPlayerController(GameWorld);;
					FVector2D TempLoca(0, 0);
					ARHeroGachaCard* pCard = nullptr;

					if (TowerLobbyLSA->HeroGachaActor->HeroCardArray.IsValidIndex(0))
						pCard = TowerLobbyLSA->HeroGachaActor->HeroCardArray[0];

					if (PlayerController && pCard && pCard->IsValidLowLevel())
						PlayerController->ProjectWorldLocationToScreen(pCard->GetTargetLocation(), TempLoca);

					const FVector2D viewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
					const float ViewportScale = GetDefault<UUserInterfaceSettings>(UUserInterfaceSettings::StaticClass())->GetDPIScaleBasedOnSize(FIntPoint(viewportSize.X, viewportSize.Y));
					TempLoca /= ViewportScale;

					TowerLobbyLSA->HeroGachaActor->CallBack_OnTouch(ETouchIndex::Touch1, FVector(TempLoca.X, TempLoca.Y, 0.0f));

				}
			}
		}
	}
}

void UForceTutorialManager::ShowUIPage(FXmlNode* InNode)
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	if (InNode)
	{
		EWidgetBluePrintEnum widgetid = EWidgetBluePrintEnum::WBP_None;
		UEnum* WidgetTypeEnumPtr = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWidgetBluePrintEnum"), true);
		if (IsValid(WidgetTypeEnumPtr))
			widgetid = (EWidgetBluePrintEnum)(WidgetTypeEnumPtr->GetValueByName(FName(*InNode->GetAttribute("enum"))));

		if (widgetid != EWidgetBluePrintEnum::WBP_None)
		{
			UUserWidget* userWidget = GameInstance->RWidgetManager->GetUserWidget(widgetid);
			if (!userWidget)
				GameInstance->RWidgetManager->CreateUserWidget(widgetid);
			UUP_ControlBlock* BlockUI = Cast<UUP_ControlBlock>(GameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_ControlBlock));
			BlockUI->Button_BlockInputManager->SetVisibility(ESlateVisibility::Collapsed);
			GameInstance->RWidgetManager->ShowUserWidget(widgetid);
		}
	}
}

void UForceTutorialManager::OnExeNextNode()
{
	if (bExeCoolTime)
		return;
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (EventsNode)
	{		
		if (IsValid(RGameInstance))
		{
			RGameInstance->GetWorld()->GetTimerManager().ClearTimer(TimeHandler_ExeNodeDelay);

			if (IsValid(RGameInstance->RWidgetManager))
			{
				UUP_ControlBlock* BlockUI = Cast<UUP_ControlBlock>(RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_ControlBlock));
				if (IsValid(BlockUI) && IsValid(BlockUI->BackButton))
				{
					BlockUI->BackButton->OnClicked.Clear();
				}
			}
		}

		if (WaitEventNode)
		{
			UE_LOG(LogBattle, Log, TEXT("UForceTutorialManager::OnExeNextNode %s %s Waiting Return"), *CurQuest.ToString(), *CurSubQuest.ToString());
			return;
		}			

		FXmlNode* AfterEventsNode = EventsNode;

		if (SubQuestList.Num() > 0)
		{
			SubQuestList.RemoveAt(0);
		}

		if (SubQuestList.Num() > 0)
		{
			CurSubQuest = SubQuestList[0];

			SetDetailForceTutorial();
		}
		else
		{
			if (!CurQuest.IsNone())
			{
				FForceTutorialTableInfo* ForceTutorialTableInfo = UTableManager::GetInstancePtr()->GetForceTutorialTableRow(CurQuest);
				if (ForceTutorialTableInfo)
				{
					if (!ForceTutorialTableInfo->LastEvent.IsNone())
					{
						SetForceTutorial(ForceTutorialTableInfo->LastEvent);
					}
					else
					{
						HideBlockUI();
						bForceTutorialing = false;
					}
				}
			}
		}

		ExeEvents(AfterEventsNode);
	}
}

void UForceTutorialManager::OnExeNodeDelay()
{
	OnExeNextNode();
}

// FROA_QUEST*	UForceTutorialManager::GetQuest(FName InQuestId)
// {
// 	FQuestTableInfo* QuestTableInfo = UTableManager::GetInstancePtr()->GetQuestRow(InQuestId);
// 	if (QuestTableInfo)
// 	{
// 		int32 day = QuestTableInfo->Day - 1;
// 		if (CurQuestData.days.IsValidIndex(day))
// 		{
// 			for (FROA_QUEST& quest : CurQuestData.days[day].quests)
// 			{
// 				if (quest.quest == QuestTableInfo->QuestUID)
// 				{
// 					return &quest;
// 				}
// 			}
// 		}
// 	}
// 
// 	return nullptr;
// }

// FROA_QUEST*	UForceTutorialManager::GetQuestID(int32 InQuestId)
// {
// 	for (FROA_CHAPTER_QUESTS& chapter : CurQuestData.days)
// 	{
// 		for (FROA_QUEST& quest : chapter.quests)
// 		{
// 			if (quest.quest == InQuestId)
// 				return &quest;
// 		}
// 	}
// 
// 	return nullptr;
// }

// FQuestTableInfo* UForceTutorialManager::GetQuestTable(int32 id)
// {
// 	TArray<FName> questList = UTableManager::GetInstancePtr()->GetQuestRowsNames();
// 	for (FName& quest : questList)
// 	{
// 		FQuestTableInfo* QuestTableInfo = UTableManager::GetInstancePtr()->GetQuestRow(quest);
// 		if (QuestTableInfo)
// 		{
// 			if (QuestTableInfo->QuestUID == id)
// 				return QuestTableInfo;
// 		}
// 	}
// 
// 	return nullptr;
// }

// void UForceTutorialManager::QuestGuideGoContent(FQuestTableInfo* InTableInfo)
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if (InTableInfo && IsValid(RGameInstance))
// 	{
// 		if (InTableInfo->GoContent == EUIMenuIDEnum::VE_DetailHeros)
// 		{
// 			if (IsValid(RGameInstance->RInventory))
// 			{
// 				FHERO* outHero = RGameInstance->RInventory->GetHeroWithID(InTableInfo->GoContentValue);
// 				if (outHero != nullptr)
// 				{
// 					RGameInstance->RInventory->SetHeroUDInHeroManagement(outHero->heroUD);
// 				}
// 			}
// 		}
// 
// 		UUIFunctionLibrary::GoContents(InTableInfo->GoContent);
// 	}
// }

// float UForceTutorialManager::GetQuestPercent()
// {
// 	if (CurQuestData.complete)
// 	{
// 		return 1.0f;
// 	}
// 	else
// 	{
// 		int32 maxCount = 0;
// 		int32 curCount = 0;
// 
// 		for (FROA_CHAPTER_QUESTS& quests : CurQuestData.days)
// 		{
// 			for (FROA_QUEST& quest : quests.quests)
// 			{
// 				++maxCount;
// 
// 				if (quest.complete && quest.rewarded)
// 				{
// 					++curCount;
// 				}
// 			}
// 		}
// 
// 		if (maxCount > 0)
// 		{
// 			return (float)(curCount) / (float)(maxCount);
// 		}
// 	}
// 
// 	return 0.0f;
// }

// void UForceTutorialManager::SetROA_STATUS_RP(FROA_ADVENTURE& Indata)
// {
// // 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// // 	if (!IsValid(RGameInstance))
// // 		return;
// // 
// // 	SetQuestData(Indata);
// 
// // 	UUP_QuestGuide* QuestGuide = RGameInstance->RWidgetManager->GetUserWidgetT<UUP_QuestGuide>(EWidgetBluePrintEnum::WBP_Quest);
// // 	if (IsValid(QuestGuide))
// // 	{
// // 		//QuestGuide->OnReceiveQuestGuideStatus();
// // 	}
// 
// 	// TODO: lobby
// 	//UUP_Lobby* Lobby = RGameInstance->RWidgetManager->GetUserWidgetT<UUP_Lobby>(EWidgetBluePrintEnum::WBP_Lobby);
// 	//if (IsValid(Lobby))
// 	//{
// 	//	Lobby->InvalidateData();
// 	//}
// }

// void UForceTutorialManager::SetQuestData(FROA_ADVENTURE& Indata)
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	CurQuestData = Indata;
// 
// 	CurExeQuest = nullptr;
// 
// 	int32 count = 0;
// 	for (int32 i = 0; i < CurQuestData.day; ++i)
// 	{
// 		FROA_CHAPTER_QUESTS& chaperquest = CurQuestData.days[i];
// 		for (FROA_QUEST& quest : chaperquest.quests)
// 		{
// 			if (!quest.rewarded && quest.complete)
// 			{
// 				++count;
// 			}
// 		}
// 
// 		if (chaperquest.day == CurQuestData.day)
// 		{
// 			for (FROA_QUEST& quest : chaperquest.quests)
// 			{
// 				if (CurExeQuest == nullptr)
// 				{
// 					if (!quest.rewarded && quest.complete)
// 					{
// 						CurExeQuest = &quest;
// 						break;
// 					}
// 				}
// 			}
// 
// 			if (CurExeQuest == nullptr)
// 			{
// 				for (FROA_QUEST& quest : chaperquest.quests)
// 				{
// 					if (!quest.rewarded && !quest.complete)
// 					{
// 						CurExeQuest = &quest;
// 						break;
// 					}
// 				}
// 			}
// 		}
// 	}	
// 
// 	if (IsValid(RGameInstance) && IsValid(RGameInstance->NotifyManager))
// 	{
// 		if (count > 0)
// 		{
// 			RGameInstance->NotifyManager->SetNotify(ENotifyStateFlagEnum::BM_Quest_Guide, true);
// 		}
// 		else
// 		{
// 			RGameInstance->NotifyManager->SetNotify(ENotifyStateFlagEnum::BM_Quest_Guide, false);
// 		}
// 	}
// }

// bool UForceTutorialManager::GetNoCompleteQuest(FROA_QUEST& OutQuest)
// {
// 	for (int32 i = 0; i < CurQuestData.day; ++i)
// 	{
// 		FROA_CHAPTER_QUESTS& chaperquest = CurQuestData.days[i];
// 		for (FROA_QUEST& quest : chaperquest.quests)
// 		{
// 			if (!quest.complete)
// 			{
// 				OutQuest = quest;
// 				return true;
// 			}
// 		}
// 	}
// 
// 	return false;
// }