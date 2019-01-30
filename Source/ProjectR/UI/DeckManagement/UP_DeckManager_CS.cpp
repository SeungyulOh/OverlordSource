// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_DeckManager_CS.h"

#include "Global/RGameInstance.h"
#include "Global/RealTimeModManager.h"

#include "Network/HttpRequestClient.h"
#include "Network/RTSManager.h"

#include "UI/Common/ScrollView/UP_HeroScrollView_Bottom.h"
#include "UI/Common/Renewal_BaseHeroIcon.h"


#include "UtilFunctionIntegrated.h"
#include "UP_GenericScrollview.h"
#include "UC_HeroDeck.h"


const int32 Stamina = 5;

void UUP_DeckManager_CS::NativeConstruct()
{
	Super::NativeConstruct();

	/*Blackboard Setting Start*/
	Blackboard = &(UUIFunctionLibrary::GetGenericScrollview()->Blackboard);
	Blackboard->SelectedHeroUDs.Init(TEXT(""), MAX_DECKCOUNT);
	Blackboard->OnBBStateChanged.AddDynamic(this, &UUP_DeckManager_CS::Update_FromBlackboard);
	
	Blackboard->TargetWidgetArray.Empty();
	for (size_t i = 0; i < Variables.MyGroupDeck->Decks.Num(); ++i)
	{
		URBaseDeckWidget* BaseDeckWidget = Cast<URBaseDeckWidget>(Variables.MyGroupDeck->Decks[i]);
		if (BaseDeckWidget)
		{
			Blackboard->TargetWidgetArray.Emplace(BaseDeckWidget);
		}
			
	}
	/*Blackboard Setting End*/

	/*StateController Setting Start*/
	StateController = &(UUIFunctionLibrary::GetGenericScrollview()->StateController);
	/*StateController Setting End*/

	/*Renderer*/
	Renderer.blackboard = Blackboard;
	Renderer.StateController = StateController;
	Renderer.variables = &Variables;
	Renderer.Prepare();
	Renderer.Render();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	for (auto& Element : RGameInstance->RealTimeModManager->RTSPlayerList)
	{
		Render_First(Element.Value.kId);
	}

	/*Renderer End*/
	RGAMEINSTANCE(GEngine)->ColosseumManager->OnUpdateColosseumTeamAvatar.RemoveDynamic(this, &UUP_DeckManager_CS::Render_First);
	RGAMEINSTANCE(GEngine)->ColosseumManager->OnUpdateColosseumTeamAvatar.AddDynamic(this, &UUP_DeckManager_CS::Render_First);

	RGAMEINSTANCE(GEngine)->RealTimeModManager->OnReceiveCSDeckSelect.RemoveDynamic(this, &UUP_DeckManager_CS::Render_EnemyDeck);
	RGAMEINSTANCE(GEngine)->RealTimeModManager->OnReceiveCSDeckSelect.AddDynamic(this, &UUP_DeckManager_CS::Render_EnemyDeck);

	RGAMEINSTANCE(GEngine)->RealTimeModManager->OnCSDeckSelectTime.RemoveDynamic(this, &UUP_DeckManager_CS::Render_Time);
	RGAMEINSTANCE(GEngine)->RealTimeModManager->OnCSDeckSelectTime.AddDynamic(this, &UUP_DeckManager_CS::Render_Time);

	RGAMEINSTANCE(GEngine)->RealTimeModManager->OnCSStepAndGo.RemoveDynamic(this, &UUP_DeckManager_CS::OnStepGo);
	RGAMEINSTANCE(GEngine)->RealTimeModManager->OnCSStepAndGo.AddDynamic(this, &UUP_DeckManager_CS::OnStepGo);

	RGAMEINSTANCE(GEngine)->RealTimeModManager->OnReceiveCSReady.RemoveDynamic(this, &UUP_DeckManager_CS::OnReceiveReady);
	RGAMEINSTANCE(GEngine)->RealTimeModManager->OnReceiveCSReady.AddDynamic(this, &UUP_DeckManager_CS::OnReceiveReady);

	if (IsValid(Variables.Button_QuickStart))
	{
		Variables.Button_QuickStart->OnClicked.Clear();
		Variables.Button_QuickStart->OnClicked.AddDynamic(this, &UUP_DeckManager_CS::AutoSettings);
	}

	if (IsValid(Variables.Button_SelectHero))
	{
		Variables.Button_SelectHero->OnClicked.Clear();
		Variables.Button_SelectHero->OnClicked.AddDynamic(this, &UUP_DeckManager_CS::OnClick_Start);
	}

	if (IsValid(Variables.Button_DeselectAll))
	{
		Variables.Button_DeselectAll->OnClicked.Clear();
		Variables.Button_DeselectAll->OnClicked.AddDynamic(this, &UUP_DeckManager_CS::DeselectAll);
	}

	if (IsValid(Variables.Button_Back))
	{
		Variables.Button_Back->OnClicked.Clear();
		Variables.Button_Back->OnClicked.AddDynamic(this, &UUP_DeckManager_CS::Onclick_BackButton);
	}


	IsReady = false;
}

void UUP_DeckManager_CS::NativeDestruct()
{
	Blackboard->OnBBStateChanged.RemoveDynamic(this, &UUP_DeckManager_CS::Update_FromBlackboard);

	UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
	if (GenericScrollview)
	{
		GenericScrollview->Appear(false);
	}

	Super::NativeDestruct();
}

void UUP_DeckManager_CS::Update_FromBlackboard()
{
	Renderer.Render();
}

void UUP_DeckManager_CS::Render_First(int32 UserKID)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	FRTS_PLAYER* Player = RGameInstance->RealTimeModManager->Get_RTS_Player(UserKID);
	if (Player)
	{
		if (RGameInstance->HttpClient->GetKID() == UserKID)
		{
			if (IsValid(Variables.LAvatarNameText))
				Variables.LAvatarNameText->SetText(FText::FromString(Player->nick));
			if (IsValid(Variables.LGuildName))
				Variables.LGuildName->SetText(FText::FromString(Player->guild));
			if (IsValid(Variables.LAvatarLevel))
				Variables.LAvatarLevel->SetText(FText::FromString(FString::FromInt(Player->level)));
		}
		else
		{
			if (IsValid(Variables.RAvatarNameText))
				Variables.RAvatarNameText->SetText(FText::FromString(Player->nick));
			if (IsValid(Variables.RGuildName))
				Variables.RGuildName->SetText(FText::FromString(Player->guild));
			if (IsValid(Variables.RAvatarLevel))
				Variables.RAvatarLevel->SetText(FText::FromString(FString::FromInt(Player->level)));
		}
	}
}

void UUP_DeckManager_CS::Render_EnemyDeck(FCSDeckSelected InDeckSelected)
{
	if (RGAMEINSTANCE(GEngine)->HttpClient->IsValidKID(InDeckSelected.kId))
		return;

	int32 idx = InDeckSelected.crewNo - 1;
	if (Variables.EnemyGroupDeck->Decks.IsValidIndex(idx))
	{
		UUC_Colosseum_Deck_New* deck = Cast<UUC_Colosseum_Deck_New>(Variables.EnemyGroupDeck->Decks[idx]);
		if (deck && deck->HeroIcon)
		{
			if (!InDeckSelected.heroId.IsEmpty())
				deck->SelectingSwitcher->SetActiveWidgetIndex(1);
			else
				deck->SelectingSwitcher->SetActiveWidgetIndex(0);
			
			deck->HeroIcon->SetHeroIcon(InDeckSelected.heroId, InDeckSelected.level);
		}
		else
		{
			UUC_HeroDeck* herodeck = Cast<UUC_HeroDeck>(Variables.EnemyGroupDeck->Decks[idx]);
			if (herodeck)
			{
				herodeck->SetHeroWithID(InDeckSelected.heroId);
			}
		}
	}
}

void UUP_DeckManager_CS::Render_Time(int32 Time)
{
	FString MinText, SecText;
	UDescriptionFunctionLibrary::GetTimeStr((float)Time, MinText, SecText);
	if (IsValid(Variables.TextBlock_PickTimer))
		Variables.TextBlock_PickTimer->SetText(FText::Format(FText::FromString(TEXT("{0}")), FText::AsNumber(Time)));
}

void UUP_DeckManager_CS::OnClick_Start()
{
	/*Enough Stamina ?*/
	/*Leader selected ?*/
	auto CheckStartCondition = [=]() -> bool {
		if (!Blackboard->SelectedHeroUDs[ROLE_LEADER_INDEX].IsEmpty())
			return true;
		else
		{
			UUIFunctionLibrary::ShowCommonPopup(
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_WARNING),
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, TEXT("Confirm_Message_000045"))
				, ECommonPopupType::VE_OK);
		}
		return false;
	};

	if (CheckStartCondition())
	{
		/*Rearrange*/
		if (Blackboard->SelectedHeroUDs[ROLE_CREW1_BATTLE_INDEX].IsEmpty() && !Blackboard->SelectedHeroUDs[ROLE_CREW1_REST_INDEX].IsEmpty())
		{
			Swap<FString>(Blackboard->SelectedHeroUDs[ROLE_CREW1_BATTLE_INDEX], Blackboard->SelectedHeroUDs[ROLE_CREW1_REST_INDEX]);
		}

		if (Blackboard->SelectedHeroUDs[ROLE_CREW2_BATTLE_INDEX].IsEmpty() && !Blackboard->SelectedHeroUDs[ROLE_CREW2_REST_INDEX].IsEmpty())
		{
			Swap<FString>(Blackboard->SelectedHeroUDs[ROLE_CREW2_BATTLE_INDEX], Blackboard->SelectedHeroUDs[ROLE_CREW2_REST_INDEX]);
		}
		/**/

		for (size_t i = 0; i < Blackboard->SelectedHeroUDs.Num(); ++i)
			StateController->SendSelectPacket(i);

		URTSManager::GetInstancePtr()->REQ_CS_DECK_SELECT_READY(true);
	}
		
}

void UUP_DeckManager_CS::OnStepGo(int32 Sequence)
{
	if (!IsReady)
	{
		AutoSettings();
		URTSManager::GetInstancePtr()->REQ_CS_DECK_SELECT_READY(true);
	}
	URTSManager::GetInstancePtr()->ACK_STEP_AND_GO(Sequence);
}

void UUP_DeckManager_CS::OnReceiveReady(int32 InKid, bool InReady)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance->HttpClient->GetKID() == InKid)
	{
		IsReady = InReady;

		if (IsReady)
		{
			Variables.Text_Ready->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UP_Guild_Management_Menu36"))));

			URScrollView* Scrollview = UUIFunctionLibrary::GetGenericScrollview()->Variables.ScrollView;
			Scrollview->IsSelectEnable = false;

			for (size_t i = 0; i < Variables.MyGroupDeck->Decks.Num(); ++i)
			{
				UUC_Colosseum_Deck_New* deck = Cast<UUC_Colosseum_Deck_New>(Variables.MyGroupDeck->Decks[i]);
				if (IsValid(deck))
					deck->InputButton->OnClicked.Clear();
			}
			
		}
		else
			Variables.Text_Ready->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Colosseum_Complete"))));
	}
}

void UUP_DeckManager_CS::AutoSettings()
{
	Blackboard->WaitingList.clear();
	StateController->SetState(EUserBehaviorState::AUTOSETTING_ALL);
	Blackboard->OnBBStateChanged.Broadcast();
}

void UUP_DeckManager_CS::DeselectAll()
{
	Blackboard->WaitingList.clear();
	StateController->SetState(EUserBehaviorState::DESELECT_ALL);
	Blackboard->OnBBStateChanged.Broadcast();
}

void UUP_DeckManager_CS::Onclick_BackButton()
{
	URTSManager::GetInstancePtr()->REQ_MATCH_CANCEL();
	RGAMEINSTANCE(this)->RWidgetManager->ChangeUIPage(EUIPageEnum::UIPage_ColosseumLobby);
}

void FRenderer_CSDeckManager::Prepare()
{
	variables->MyGroupDeck->TeamSwitcher->SetActiveWidgetIndex(0);
	variables->EnemyGroupDeck->TeamSwitcher->SetActiveWidgetIndex(1);
}

void FRenderer_CSDeckManager::Render()
{
	EUserBehaviorState UserState = StateController->UserState;

	switch (UserState)
	{
	case EUserBehaviorState::IDLE:
	{
		auto HideAvailableImage = [](TArray<URBaseDeckWidget*> InArray) {
			for (size_t i = 0; i < InArray.Num(); ++i)
			{
				if (InArray.IsValidIndex(i))
				{
					InArray[i]->SetAvailable(false);
					InArray[i]->SetSelecting(false);
				}
					
			}
		};

		HideAvailableImage(variables->MyGroupDeck->Decks);
		HideAvailableImage(variables->EnemyGroupDeck->Decks);

		for (int32 i = 0; i < MAX_DECKCOUNT; ++i)
		{
			FString heroUD = blackboard->SelectedHeroUDs[i];
			if (variables->MyGroupDeck->Decks.IsValidIndex(i))
				variables->MyGroupDeck->Decks[i]->SetHero(heroUD);
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
					if(i==Targetidx)
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
				FindAvailableDeck(variables->MyGroupDeck->Decks, iIdx);
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
				int32 iIdx = variables->MyGroupDeck->Decks.Find(heroDeck);
				if (iIdx != INDEX_NONE)
				{
					auto FindAvailableDeck = [=](TArray<URBaseDeckWidget *> InArray) {
						for (size_t i = 0; i < InArray.Num(); ++i)
						{
							if (i == iIdx)
							{
								InArray[i]->SetSelecting(true);
								continue;
							}
								

							if (InArray.IsValidIndex(i))
								InArray[i]->SetAvailable(true);
						}
					};

					FindAvailableDeck(variables->MyGroupDeck->Decks);
				}
			}
		}

		break;
	}
	}
}

void UUC_Colosseum_GroupDeck::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUC_Colosseum_GroupDeck::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_Colosseum_Deck_New::NativeConstruct()
{
	Super::NativeConstruct();
	
	SelectingSwitcher->SetActiveWidgetIndex(0);
	HeroIcon->SetVisibility(ESlateVisibility::Collapsed);

	if (IsValid(InputButton))
	{
		InputButton->OnClicked.Clear();
		InputButton->OnClicked.AddDynamic(this, &UUC_Colosseum_Deck_New::OnClick_Deck);
	}
}

void UUC_Colosseum_Deck_New::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_Colosseum_Deck_New::SetAvailable(bool inAvailable)
{
	if (inAvailable)
		PlayAni(TEXT("Deck_Available") , false, 0);
	else
		StopAni(TEXT("Deck_Available"));
}

void UUC_Colosseum_Deck_New::SetSelecting(bool bSelecting)
{
	if (bSelecting)
	{
		Image_Selecting->SetVisibility(ESlateVisibility::HitTestInvisible);
		UWidgetAnimation* Anim = GetWidgetAnimFromName(TEXT("Deck_Selecting"));
		PlayAnimation(Anim, 0.f, 0);
	}
	else
	{
		Image_Selecting->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUC_Colosseum_Deck_New::SetHero(FString heroUD)
{
	CurHeroUD = heroUD;

	if (!heroUD.IsEmpty())
	{
		SelectingSwitcher->SetActiveWidgetIndex(1);
		HeroIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		HeroIcon->SetHeroIconInfoWithUD(heroUD , EHeroOwnershipType::VE_Me);
	}
	else
	{
		SelectingSwitcher->SetActiveWidgetIndex(0);
		HeroIcon->SetVisibility(ESlateVisibility::Collapsed);
	}
}
