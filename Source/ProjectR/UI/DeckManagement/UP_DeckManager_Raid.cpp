// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_DeckManager_Raid.h"

#include "Global/RGameInstance.h"
#include "Global/RealTimeModManager.h"
#include "UC_SkillForDeck.h"
#include "Network/HttpRequestClient.h"
#include "Network/RTSManager.h"
#include "UI/Common/ScrollView/UP_HeroScrollView_Bottom.h"
#include "UI/DeckManagement/UP_DeckManager_CS.h"
#include "UP_GenericScrollview.h"
#include "UtilFunctionIntegrated.h"
#include "Gameinfo/PlayStateInfo.h"
#include "Client_BattleFunctionLibrary.h"
#include "Table/RaidStageTableInfo.h"
#include "UC_HeroDeck.h"

#define SEATINROOM 0
#define INVITEDSLOT 1
#define EMPTYSLOT 2


void FRenderer_MultiRaid::Prepare()
{
	if (!FloorStageTable)
		return;

	variables->TextBlock_AttackPower->SetText(FText::AsNumber(FloorStageTable->RecommendPowerValue));
	variables->LocalTextBlockPartyID->SetText(FText::AsNumber(RGAMEINSTANCE(GEngine)->RealTimeModManager->RTSInfo.partyNo));

}

void FRenderer_MultiRaid::Render()
{
// 	if (variables->MySlot == INDEX_NONE)
// 		return;

	EUserBehaviorState UserState = StateController->UserState;

	switch (UserState)
	{
	case EUserBehaviorState::IDLE:
	{
		//set default deck btn slot
		for (int32 i = 0; i < MAX_DECKCOUNT; i++)
		{
			if (i == 0)
			{
				variables->UC_DeckManager_Hero_01->SetAvailable(false);
				variables->UC_DeckManager_Hero_01->SetSelecting(false);
				variables->UC_DeckManager_Hero_01->SetIsEnabled(true);
			}
			else
			{
				variables->UC_Party_Icons[i - 1]->SetAvailable(false);
				variables->UC_Party_Icons[i - 1]->SetSelecting(false);
				variables->UC_Party_Icons[i - 1]->SetIsEnabled(true);
			}
		}

		static auto FindColor = [](ECounterType type) -> FSlateColor {
			switch (type)
			{
			case ECounterType::SOLAR: return FSlateColor(FLinearColor(FColor(195, 42, 42)));
			case ECounterType::LIGHT: return FSlateColor(FLinearColor(FColor(195, 108, 6)));
			case ECounterType::DARK: return FSlateColor(FLinearColor(FColor(132, 132, 183)));
			case ECounterType::MOON: return FSlateColor(FLinearColor(FColor(81, 132, 183)));

			default:
				return FSlateColor(FLinearColor::White);
			}
		};

		TMap<ECounterType, FName> FirstCounters;
		TMap<ECounterType, FName> SecondCounters;

		for (int32 i = 0; i < MAX_DECKCOUNT; ++i)
		{
			FString heroUD = blackboard->SelectedHeroUDs[i];
			if (i == 0)
			{
				variables->UC_DeckManager_Hero_01->SetHero(heroUD);
				variables->UC_DeckManager_Hero_Skill01->SetSkillIcon(heroUD, true);

				if (heroUD.IsEmpty())
					variables->Text_MyLeaderEffectDesc->SetVisibility(ESlateVisibility::Collapsed);
				else
				{
					variables->Text_MyLeaderEffectDesc->SetVisibility(ESlateVisibility::Visible);
					
					FHERO* hero = RGAMEINSTANCE(GEngine)->RInventory->GetHeroWithUD(heroUD);

					if (hero)
					{
						FCharacterTableInfo* heroinfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
						if (heroinfo && IsValid(variables->Text_MyLeaderEffectDesc))
						{
							variables->Text_MyLeaderEffectDesc->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, heroinfo->AttributeSkillString));
							variables->Text_MyLeaderEffectDesc->SetColorAndOpacity(FindColor(heroinfo->CounterType));
						}
					}
				}
			}
			else
			{
				variables->UC_Party_Icons[i - 1]->SetHero(heroUD);

				if (heroUD.IsEmpty())
				{
					variables->Text_PartyEffectFirstArray[i - 1]->SetVisibility(ESlateVisibility::Collapsed);
					variables->Text_PartyEffectSecondArray[i - 1]->SetVisibility(ESlateVisibility::Collapsed);
				}
				else
				{
					variables->Text_PartyEffectFirstArray[i - 1]->SetVisibility(ESlateVisibility::Visible);
					variables->Text_PartyEffectSecondArray[i - 1]->SetVisibility(ESlateVisibility::Visible);

					FHERO* hero = RGAMEINSTANCE(GEngine)->RInventory->GetHeroWithUD(heroUD);
					if (hero)
					{
						FCharacterTableInfo* heroinfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
						if (heroinfo->AttributeSynergyString == TEXT("P"))
						{
							if (!FirstCounters.Find(heroinfo->CounterType))
								FirstCounters.Emplace(heroinfo->CounterType, heroinfo->AttributeSkillString);
						}
						else if (heroinfo->AttributeSynergyString == TEXT("M"))
						{
							if (!SecondCounters.Find(heroinfo->CounterType))
								SecondCounters.Emplace(heroinfo->CounterType, heroinfo->AttributeSkillString);
						}

						auto Predicate = [](const ECounterType& Source, const ECounterType& Dest)
						{
							return (int32)Source < (int32)Dest;
						};

						FirstCounters.KeySort(Predicate);
						SecondCounters.KeySort(Predicate);

						auto StringHelper = [](TArray<UTextBlock*>& TargetTextArray, const TMap<ECounterType, FName>& InMap) {

							TArray<ECounterType> KeyArray;
							InMap.GenerateKeyArray(KeyArray);
							for (size_t j = 0; j < TargetTextArray.Num(); ++j)
							{
								if (TargetTextArray.IsValidIndex(j))
								{
									if (j < InMap.Num())
									{
										FString OutString = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, InMap[KeyArray[j]]).ToString();
										TargetTextArray[j]->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
										TargetTextArray[j]->SetText(FText::FromString(OutString));
										TargetTextArray[j]->SetColorAndOpacity(FindColor(KeyArray[j]));
									}
									else
										TargetTextArray[j]->SetVisibility(ESlateVisibility::Hidden);
								}
							}
						};

						StringHelper(variables->Text_PartyEffectFirstArray, FirstCounters);
						StringHelper(variables->Text_PartyEffectSecondArray, SecondCounters);

					}
				}
			}
		}

		break;
	}
	case EUserBehaviorState::SELECTING_START:
	{
		auto FindAvailableDeck = [](TArray<URBaseDeckWidget *> InArray, int32 Targetidx) {
			for (size_t i = 0; i < InArray.Num(); ++i)
			{
				if (InArray.IsValidIndex(i))
				{
					if (i == Targetidx)
						InArray[i]->SetSelecting(true);
				}
			}
		};

		UUserWidget *widget = blackboard->WaitingList.front();
		URBaseDeckWidget *Waiting_Deck = Cast<URBaseDeckWidget>(widget);
		if (Waiting_Deck)
		{
			int32 iIdx = blackboard->TargetWidgetArray.Find(Waiting_Deck);
			if (iIdx != INDEX_NONE)
				FindAvailableDeck(blackboard->TargetWidgetArray, iIdx);
		}

		break;
	}
	case EUserBehaviorState::CHANGING_START:
	{
		if (blackboard->WaitingList.size() == 1)
		{
			URBaseDeckWidget *heroDeck = Cast<URBaseDeckWidget>(blackboard->WaitingList.front());
			if (heroDeck)
			{
				int32 iIdx = blackboard->TargetWidgetArray.Find(heroDeck);
				if (iIdx != INDEX_NONE)
				{
					auto FindAvailableDeck = [=](TArray<URBaseDeckWidget *> InArray) {
						for (size_t i = 0; i < InArray.Num(); ++i)
						{
							if (iIdx == 0)
							{
								if(i == 0)
									InArray[i]->SetSelecting(true);
								else
									InArray[i]->SetIsEnabled(false);
							}
							else
							{
								if (i == 0)
									InArray[i]->SetIsEnabled(false);
								else
								{
									if (i == iIdx)
										InArray[i]->SetSelecting(true);
									else
										InArray[i]->SetAvailable(true);
								}
							}
						}
					};

					FindAvailableDeck(blackboard->TargetWidgetArray);
				}
			}
		}

		break;
	}
	}
	SetPartyCombatStat();
}

void FRenderer_MultiRaid::SetPartyCombatStat()
{
	if (!BaseStageInfo)
		return;
	if (!FloorStageTable)
		return;

	TArray<FString> HeroUDs = StateController->blackboard->SelectedHeroUDs;
	variables->PartyCombatStat[0] = 0;
	for (FString& UD : HeroUDs)
		variables->PartyCombatStat[0] += UClient_BattleFunctionLibrary::GetHeroCombatStat(UD);
	
	variables->Text_MyPower->SetText(FText::AsNumber(variables->PartyCombatStat[0]));

	int32 PartyCombat = 0;

	for (int32 i = 0; i < 5; i++)
		PartyCombat += variables->PartyCombatStat[i];

	variables->TextBlock_PartyAttackPower->SetText(FText::AsNumber(PartyCombat));
}

void UUP_DeckManager_Raid::NativeConstruct()
{
	Super::NativeConstruct();

	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	if (!BaseStageInfo)
		return;
	BaseStageInfo->bPlayAlone = true;
	FloorStageTable = UTableManager::GetInstancePtr()->GetRaidStageTableRow(BaseStageInfo->StageID);

	//combat stat array init
	if (Variables.PartyCombatStat.Num() == 0)
	{
		for (int32 i = 0; i < 5; i++)
			Variables.PartyCombatStat.Emplace(0);
	}

	//set party ui initialize
	for (int32 i = 0; i < 4; i++)
		SetPartyOverlay(EMPTYSLOT, i);

	/*Blackboard Setting Start*/
	Blackboard = &(UUIFunctionLibrary::GetGenericScrollview()->Blackboard);
	Blackboard->SelectedHeroUDs.Init(TEXT(""), MAX_DECKCOUNT);
	Blackboard->OnBBStateChanged.AddDynamic(this, &UUP_DeckManager_Raid::Update_FromBlackboard);
	/*Blackboard Setting End*/

	/*StateController Setting Start*/
	StateController = &(UUIFunctionLibrary::GetGenericScrollview()->StateController);
	/*StateController Setting End*/

	/*Renderer*/
	Renderer.BaseStageInfo = BaseStageInfo;
	Renderer.FloorStageTable = FloorStageTable;
	Renderer.blackboard = Blackboard;
	Renderer.StateController = StateController;
	Renderer.variables = &Variables;
	Renderer.Prepare();
	Renderer.Render();
	/*Renderer End*/

	RenewPartyPublic();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	RGameInstance->RealTimeModManager->OnUpdatePartyPublic.RemoveDynamic(this, &UUP_DeckManager_Raid::RenewPartyPublic);
	RGameInstance->RealTimeModManager->OnUpdatePartyPublic.AddDynamic(this, &UUP_DeckManager_Raid::RenewPartyPublic);
	RGameInstance->RealTimeModManager->OnUpdatePartPlayer.RemoveDynamic(this, &UUP_DeckManager_Raid::PartyUpdatePlayer);
	RGameInstance->RealTimeModManager->OnUpdatePartPlayer.AddDynamic(this, &UUP_DeckManager_Raid::PartyUpdatePlayer);
	RGameInstance->RealTimeModManager->OnReceiveDeckSelect.RemoveDynamic(this, &UUP_DeckManager_Raid::OnReceiveDeckSelect);
	RGameInstance->RealTimeModManager->OnReceiveDeckSelect.AddDynamic(this, &UUP_DeckManager_Raid::OnReceiveDeckSelect);
	RGameInstance->RealTimeModManager->OnLeaderSlot.RemoveDynamic(this, &UUP_DeckManager_Raid::OnLeaderSlot);
	RGameInstance->RealTimeModManager->OnLeaderSlot.AddDynamic(this, &UUP_DeckManager_Raid::OnLeaderSlot);
	RGameInstance->RealTimeModManager->OnPartyPlayerExit.RemoveDynamic(this, &UUP_DeckManager_Raid::OnPartyPlayerExit);
	RGameInstance->RealTimeModManager->OnPartyPlayerExit.AddDynamic(this, &UUP_DeckManager_Raid::OnPartyPlayerExit);
	RGameInstance->RealTimeModManager->OnDeckReady.RemoveDynamic(this, &UUP_DeckManager_Raid::OnDeckReady);
	RGameInstance->RealTimeModManager->OnDeckReady.AddDynamic(this, &UUP_DeckManager_Raid::OnDeckReady);
	/*buttons*/
	Variables.Button_On->OnClicked.Clear();
	Variables.Button_On->OnClicked.AddDynamic(this, &UUP_DeckManager_Raid::OnClickedPublicButton);
// 	Variables.ButtonPartyPrivate->OnClicked.Clear();
// 	Variables.ButtonPartyPrivate->OnClicked.AddDynamic(this, &UUP_DeckManager_Raid::OnClickedPartyPrivate);
// 	Variables.ButtonPartyPublic->OnClicked.Clear();
// 	Variables.ButtonPartyPublic->OnClicked.AddDynamic(this, &UUP_DeckManager_Raid::OnClickedPartyPublic);
// 
	Variables.Button_Ready->OnClicked.Clear();
	Variables.Button_Ready->OnClicked.AddDynamic(this, &UUP_DeckManager_Raid::OnClickedReady);
	Variables.Button_Cancel->OnClicked.Clear();
	Variables.Button_Cancel->OnClicked.AddDynamic(this, &UUP_DeckManager_Raid::OnClickedCancel);
// 	Variables.BattleButton->OnClicked.Clear();
// 	Variables.BattleButton->OnClicked.AddDynamic(this, &UUP_DeckManager_Raid::OnClickedBattle);
// 	Variables.CancelButton->OnClicked.Clear();
// 	Variables.CancelButton->OnClicked.AddDynamic(this, &UUP_DeckManager_Raid::OnClickedCancel);
	
}

void UUP_DeckManager_Raid::NativeDestruct()
{
	Blackboard->OnBBStateChanged.RemoveDynamic(this, &UUP_DeckManager_Raid::Update_FromBlackboard);
	Variables.MySlot = INDEX_NONE;

	if (URealTimeModManager::IsRealTimeGameMode())
		URTSManager::GetInstancePtr()->REQ_MATCH_CANCEL();

	auto ScrollView = UUIFunctionLibrary::GetGenericScrollview();

	if (ScrollView)
		ScrollView->Appear(false);

	Super::NativeDestruct();
}

void UUP_DeckManager_Raid::Update_FromBlackboard()
{
	Renderer.Render();
}

void UUP_DeckManager_Raid::PartyUpdatePlayer(int32 kid)
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	int32 myKid = GameInstance->UserInfo->Avatar.kId;

	auto currentPlayer = GameInstance->RealTimeModManager->RTSPlayerList[kid];

	if (currentPlayer.kId == myKid)
	{
		Variables.Text_MyNick->SetText(FText::FromString(currentPlayer.nick));

		if (Variables.MySlot == INDEX_NONE)
		{
			if (GameInstance->HttpClient->IsValidKID(kid))
			{
				Variables.MySlot = currentPlayer.slot;

				Blackboard->TargetWidgetArray.Empty();
				URBaseDeckWidget* BaseDeckWidget = nullptr;
				for (size_t i = 0; i < MAX_DECKCOUNT; ++i)
				{
					if (i == 0)
						BaseDeckWidget = Cast<URBaseDeckWidget>(Variables.UC_DeckManager_Hero_01);
					else
						BaseDeckWidget = Cast<URBaseDeckWidget>(Variables.UC_Party_Icons[i - 1]);
					if (BaseDeckWidget)
						Blackboard->TargetWidgetArray.Emplace(BaseDeckWidget);
				}
			}
		}
	}
	else
	{
		SetPartyOverlay(SEATINROOM, currentPlayer.slot - 1);
		Variables.Text_PartyNicks[currentPlayer.slot - 1]->SetText(FText::FromString(currentPlayer.nick));
	}
}

void UUP_DeckManager_Raid::RenewPartyPublic()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	PlayAni(TEXT("on_ani"), RGameInstance->RealTimeModManager->RTSInfo.bPublic);
}

void UUP_DeckManager_Raid::OnReceiveDeckSelect(FDeckSelected InDeckSelected)
{
	if (InDeckSelected.crewNo == 0)
		return;

	int32 SlotIdx = InDeckSelected.slot;
	int32 idx = InDeckSelected.crewNo - 1;

	if (SlotIdx == Variables.MySlot)
	{
		Blackboard->SelectedHeroUDs[idx] = InDeckSelected.heroUD;
		StateController->SetState(EUserBehaviorState::IDLE);
		Blackboard->OnBBStateChanged.Broadcast();
		return;
	}

	if (idx == 0)
	{
		Variables.UC_DeckManager_Heros[SlotIdx - 1]->SetHeroWithID(InDeckSelected.heroId);
		Variables.UC_Skill_ForDecks[SlotIdx - 1]->SetSkillIconWithID(FName(*InDeckSelected.heroId), true);
	}
}

void UUP_DeckManager_Raid::OnPartyPlayerExit(int32 InSlotNumber)
{
	SetPartyOverlay(EMPTYSLOT, InSlotNumber - 1);
	//Variables.PlayerDecks[InSlotNumber-1]->DeckSwitcher->SetActiveWidgetIndex(3);
}

void UUP_DeckManager_Raid::OnLeaderSlot(int32 InSlotNumber, bool IsLeader)
{
	if (IsLeader && InSlotNumber-1 == Variables.MySlot)
		Variables.bRoomLeader = true;
}

void UUP_DeckManager_Raid::OnDeckReady(int32 InSlotNumber, bool InReady)
{
	if (InSlotNumber == Variables.MySlot)
	{
		if (InReady)
		{
			Variables.MyReadyOverlay->SetVisibility(ESlateVisibility::Visible);
			Variables.WidgetSwitcher_IsReady->SetActiveWidgetIndex(1);
			Variables.Button_Ready->SetIsEnabled(true);
		}
		else
		{
			Variables.MyReadyOverlay->SetVisibility(ESlateVisibility::Collapsed);
			Variables.WidgetSwitcher_IsReady->SetActiveWidgetIndex(0);
			Variables.Button_Cancel->SetIsEnabled(true);
		}
	}
	else 
	{
		if (InReady)
			Variables.PartyReadyOverlays[InSlotNumber - 1]->SetVisibility(ESlateVisibility::Visible);
		else
			Variables.PartyReadyOverlays[InSlotNumber - 1]->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUP_DeckManager_Raid::OnClickedPublicButton()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	URTSManager::GetInstancePtr()->REQ_PARTY_UPDATE_PUBLIC(RGameInstance->RealTimeModManager->RTSInfo.partyNo, !RGameInstance->RealTimeModManager->RTSInfo.bPublic);
}

void UUP_DeckManager_Raid::OnClickedBattle()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	URTSManager::GetInstancePtr()->REQ_PARTY_GO(RGameInstance->RealTimeModManager->RTSInfo.partyNo);
	UUIFunctionLibrary::ShowWaitingPopup(true);
}

void UUP_DeckManager_Raid::OnClickedCancel()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	URTSManager::GetInstancePtr()->REQ_DECK_SELECT_READY(RGameInstance->RealTimeModManager->RTSInfo.mSlotNo, false);
	Variables.Button_Cancel->SetIsEnabled(false);
}

void UUP_DeckManager_Raid::OnClickedReady()
{
	bool isAllHeroSelected = true;
	for (size_t i = 0; i < Blackboard->SelectedHeroUDs.Num(); ++i)
	{
		if (Blackboard->SelectedHeroUDs.IsValidIndex(i) && Blackboard->SelectedHeroUDs[i].IsEmpty())
			isAllHeroSelected = false;
	}

	if (!isAllHeroSelected)
	{
		UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY), 
			UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_HeroChoice_Warning01")));
		return;
	}

	auto RGameInstance = RGAMEINSTANCE(this);
	URTSManager::GetInstancePtr()->REQ_DECK_SELECT_READY(RGameInstance->RealTimeModManager->RTSInfo.mSlotNo, true);
	Variables.Button_Ready->SetIsEnabled(false);
}

void UUP_DeckManager_Raid::SetPartyOverlay(int32 state, int32 index)
{
	switch (state)
	{
	case SEATINROOM:
		Variables.Text_PartyNicks[index]->SetVisibility(ESlateVisibility::Visible);
		Variables.Text_PartyPowers[index]->SetVisibility(ESlateVisibility::Visible);
		Variables.UC_Skill_ForDecks[index]->SetVisibility(ESlateVisibility::Visible);
		Variables.Text_PartyEffect_Titles[index]->SetVisibility(ESlateVisibility::Visible);
		Variables.Text_PartyEffect_Descs[index]->SetVisibility(ESlateVisibility::Visible);
		break;
	case INVITEDSLOT:
		Variables.Text_PartyNicks[index]->SetVisibility(ESlateVisibility::Visible);
		Variables.Text_PartyPowers[index]->SetVisibility(ESlateVisibility::Visible);
		Variables.UC_Skill_ForDecks[index]->SetVisibility(ESlateVisibility::Collapsed);
		Variables.Text_PartyEffect_Titles[index]->SetVisibility(ESlateVisibility::Collapsed);
		Variables.Text_PartyEffect_Descs[index]->SetVisibility(ESlateVisibility::Collapsed);
		break;
	case EMPTYSLOT:
		Variables.Text_PartyNicks[index]->SetVisibility(ESlateVisibility::Collapsed);
		Variables.Text_PartyPowers[index]->SetVisibility(ESlateVisibility::Collapsed);
		Variables.UC_Skill_ForDecks[index]->SetVisibility(ESlateVisibility::Collapsed);
		Variables.Text_PartyEffect_Titles[index]->SetVisibility(ESlateVisibility::Collapsed);
		Variables.Text_PartyEffect_Descs[index]->SetVisibility(ESlateVisibility::Collapsed);
		Variables.UC_DeckManager_Heros[index]->SetHero(FString());
		Variables.UC_Skill_ForDecks[index]->SetSkillIcon(FString(), true);
		break;
	default:
		break;
	}
}

void UUC_Raid_Deck_MR::NativeConstruct()
{
	Super::NativeConstruct();

	DeckSwitcher->SetActiveWidgetIndex(3);
	ReadyText->SetVisibility(ESlateVisibility::Collapsed);
	bReady = false;
}

void UUC_Raid_Deck_MR::Init(FString nickname)
{
	NickName->SetText(FText::FromString(nickname));
	DeckSwitcher->SetActiveWidgetIndex(0);
}

void UUC_Raid_Deck_MR::SetReady(bool InReady)
{
	if (InReady)
		ReadyText->SetVisibility(ESlateVisibility::Visible);
	else
		ReadyText->SetVisibility(ESlateVisibility::Collapsed);
	bReady = InReady;
}
