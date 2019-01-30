// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "GameInfo/RInventory.h"

#include "Global/RGameInstance.h"
#include "Global/TableManager.h"
#include "Global/RGameInstance.h"
#include "Global/RealTimeModManager.h"

#include "GamePlayBluePrintFunction.h"
#include "GameState/Stage/Spawner/BaseStageInfo.h"


#include "UI/RWidgetManager.h"
#include "UI/Common/Renewal_BaseHeroIcon.h"
#include "UI/UI_Common/UP_CommonPopup.h"
#include "UI/RWidgetManager.h"
#include "UI/HeroManagement/UP_HeroInventory.h"
#include "UI/Common/ScrollView/UC_Hero_SortingFilter.h"

#include "UtilFunctionIntegrated.h"
#include "GameInfo/RInventory.h"
#include "Network/RTSManager.h"


#include "UP_HeroScrollView_Bottom.h"
#include "Global/ArenaManager.h"


const int32 MaxScrollItem = 16;

const int32 MAX_LISTCOUNT = 2;

void UUP_HeroScrollView_Bottom::NativeConstruct()
{
	Super::NativeConstruct();

	/*1. ScrollView Settings*/
	ScrollView->IsSelectEnable = true;
		/*1.1 ScrollItem Create*/
		if (ScrollView)
		{
			if(!ScrollView->ScrollItemList.Num())
				ScrollView->Init_NoClear(MaxScrollItem);
		}
		/*1.1 ScrollItem Create End*/

		/*1.2 ScrollItemData Create*/
		TArray<FString> HeroUDs;
		RGAMEINSTANCE(this)->RInventory->GetHeroUDArray(HeroUDs);

		if (HeroUDs.Num() != cachedItemDataList.Num())
		{
			ScrollView->ClearScrollItemData(HeroUDs.Num());
			for (size_t i = 0; i < HeroUDs.Num(); ++i)
			{
				UHeroScrollItemData_New* itemdata = Cast<UHeroScrollItemData_New>(ScrollView->AddScrollItemData(UHeroScrollItemData_New::StaticClass()));
				if (itemdata)
				{
					itemdata->HeroUD = HeroUDs[i];
					itemdata->eSelectType = UHeroScrollItemData_New::IDLE;
					cachedItemDataList.Emplace(itemdata);
				}
			}
		}
		/*1.2 ScrollItemData Create End*/

	ScrollView->InvalidateData();
	ScrollView->CurrentUIPage = RGAMEINSTANCE(this)->RWidgetManager->GetCurrentUIPage();

	switch (ScrollView->CurrentUIPage)
	{
	case EUIPageEnum::UIPage_DeckSetting: 
		ScrollView->MultiSelect = true;
		break;
	}
	/*1. ScrollView Settings End*/

	
	/*2. State Machine Setting*/
	StateController.blackboard = &Blackboard;

	Blackboard.OnBBStateChanged.RemoveDynamic(this, &UUP_HeroScrollView_Bottom::Update_StateController);
	Blackboard.OnBBStateChanged.AddDynamic(this, &UUP_HeroScrollView_Bottom::Update_StateController);
	/*2. State Machine Setting End*/

	/*3. Delegate Settings*/
	if (IsValid(Button_Sorting_Show))
	{
		Button_Sorting_Show->OnClicked.Clear();
		Button_Sorting_Show->OnClicked.AddDynamic(this, &UUP_HeroScrollView_Bottom::OnClick_ButtonSortingShow);
	}

	if (IsValid(Button_Sorting_Hide))
	{
		Button_Sorting_Hide->OnClicked.Clear();
		Button_Sorting_Hide->OnClicked.AddDynamic(this, &UUP_HeroScrollView_Bottom::OnClick_ButtonSortingHide);
	}

	if (IsValid(Button_Filter_Show))
	{
		Button_Filter_Show->OnClicked.Clear();
		Button_Filter_Show->OnClicked.AddDynamic(this, &UUP_HeroScrollView_Bottom::OnClick_ButtonFilterShow);
	}
	/*3. Delegate Settings End*/

	if (IsValid(UC_Filter))
		UC_Filter->OnClicked_Button_Reset();

}


void UUP_HeroScrollView_Bottom::NativeDestruct()
{
	if (IsValid(UC_Sorting))
		UC_Sorting->SetVisibility(ESlateVisibility::Collapsed);
	if (IsValid(UC_Filter))
		UC_Filter->SetVisibility(ESlateVisibility::Collapsed);

	Super::NativeDestruct();
}

void UUP_HeroScrollView_Bottom::Update_StateController()
{
	StateController.ChangeState();
	StateController.Process();
}

void UUP_HeroScrollView_Bottom::OnClick_ButtonSortingShow()
{
	if (IsValid(UC_Sorting))
	{
		UC_Sorting->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		Switcher_Sorting->SetActiveWidgetIndex(1);
	}
}

void UUP_HeroScrollView_Bottom::OnClick_ButtonSortingHide()
{
	if (IsValid(UC_Sorting))
	{
		UC_Sorting->SetVisibility(ESlateVisibility::Collapsed);
		Switcher_Sorting->SetActiveWidgetIndex(0);
	}
}

void UUP_HeroScrollView_Bottom::OnClick_ButtonFilterShow()
{
	if (IsValid(UC_Filter))
	{
		UC_Filter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void UUP_HeroScrollView_Bottom::Recompose_Itemdatalist(TArray<FString> HeroUDs)
{
	if (ScrollView)
	{
		ScrollView->ClearScrollItemData(HeroUDs.Num());

		for (size_t i = 0; i < HeroUDs.Num(); ++i)
		{
			UHeroScrollItemData_New** FoundItem = cachedItemDataList.FindByPredicate([HeroUDs , i](UHeroScrollItemData_New* Source) {
				if (Source->HeroUD == HeroUDs[i])
					return true;
				else
					return false;
			});

			if (FoundItem && *FoundItem)
			{
				ScrollView->ItemDataList.Emplace(*FoundItem);
			}
		}

		ScrollView->InvalidateData();
	}
}

void UHeroScrollItem_New::InvalidateData()
{
	Super::InvalidateData();

	IsSelectEnable = true;
	UHeroScrollItemData_New* itemdata = Cast<UHeroScrollItemData_New>(ItemData);
	if (itemdata)
	{
		FString CurrentHeroUD = itemdata->HeroUD;
		if (HeroIcon)
			HeroIcon->SetHeroIconInfoWithUD(CurrentHeroUD, EHeroOwnershipType::VE_Me);

		if (itemdata->eSelectType == UHeroScrollItemData_New::IDLE)
		{
			HeroIcon->WidgetSwitcher_Select->SetVisibility(ESlateVisibility::Collapsed);
		}
		else if (itemdata->eSelectType == UHeroScrollItemData_New::SELECTING)
		{
			HeroIcon->WidgetSwitcher_Select->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			HeroIcon->WidgetSwitcher_Select->SetActiveWidgetIndex(1);
		}
		else
		{
			HeroIcon->WidgetSwitcher_Select->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			HeroIcon->WidgetSwitcher_Select->SetActiveWidgetIndex(0);
		}
	}
}

void UHeroScrollItem_New::Selected()
{
	UUC_BaseHeroScrollView_New* ScrollView = Cast<UUC_BaseHeroScrollView_New>(ParentScrollView);
	if (!ScrollView)
		return;

	if (!ScrollView->IsSelectEnable)
		return;

	switch (ScrollView->CurrentUIPage)
	{
		case EUIPageEnum::UIPage_DeckSetting: 
		case EUIPageEnum::UIPage_ColosseumSetting:
		case EUIPageEnum::UIPage_PartyRaidUI:
		{
			UUP_HeroScrollView_Bottom* parent = Cast<UUP_HeroScrollView_Bottom>(ScrollView->GetParentWidget());
			if (parent)
				parent->Blackboard.SetData_OnBlackBoardData(this);

			break;
		}
		default:
		{
			Super::Selected();
			InvalidateData();
			return;
		}
	}
}

void UUC_BaseHeroScrollView_New::ClearAllSelected()
{
	for (size_t i = 0; i < SelectedScrollItemDataList.Num(); ++i)
	{
		UHeroScrollItemData_New* itemdata = Cast<UHeroScrollItemData_New>(SelectedScrollItemDataList[i]);
		if (itemdata)
			itemdata->eSelectType = UHeroScrollItemData_New::IDLE;
	}
	for (size_t i = 0; i < ScrollItemList.Num(); ++i)
		ScrollItemList[i]->InvalidateData();

	Super::ClearAllSelected();

}

void UUC_BaseHeroScrollView_New::ScrollItemList_ChangeItemDataList(int32 dataIndex)
{
	UUP_HeroScrollView_Bottom* parent = Cast<UUP_HeroScrollView_Bottom>(ParentWidget);
	if (parent)
	{
		if (parent->Blackboard.WaitingList.size())
		{
			UHeroScrollItem_New* scrollitem = Cast<UHeroScrollItem_New>(parent->Blackboard.WaitingList.front());
			if (scrollitem)
			{
				URScrollItem* Item = nullptr;
				int32 ItemListLength = ScrollItemList.Num();
				int32 DataListLength = ItemDataList.Num();
				for (int32 i = 0; i < ItemListLength; ++i)
				{
					Item = ScrollItemList[i];
					if (IsValid(Item) && Item == scrollitem)
					{
						int32 PrevItemCount = Item->Count;
						Item->Count = dataIndex + i;
						if (Item->Count < DataListLength)
						{
							if (Item->Count != PrevItemCount)
							{
								UHeroScrollItemData_New* itemdata = Cast<UHeroScrollItemData_New>(scrollitem->ItemData);
								if (itemdata)
								{
									itemdata->eSelectType = UHeroScrollItemData_New::IDLE;
									scrollitem->InvalidateData();
								}
								parent->StateController.SetState(EUserBehaviorState::IDLE);
								parent->Blackboard.WaitingList.clear();
								parent->Blackboard.OnBBStateChanged.Broadcast();
							}
						}
					}
				}
			}
		}
	}
	

	Super::ScrollItemList_ChangeItemDataList(dataIndex);
		
}

void UUC_BaseHeroScrollView_New::Update_ChangeItemDataList()
{
	URScrollItem* Item = nullptr;
	int32 ItemListLength = ScrollItemList.Num();

	for (int32 i = 0; i < ItemListLength; ++i)
	{
		Item = ScrollItemList[i];
		if (IsValid(Item))
		{
			Item->IsSelected = false;
			Item->ItemData->bSelected = false;
			Item->SetSelected(false);
		}
	}

	for (int32 idx : SelectedScrollItemList)
	{
		for (URScrollItem* ItemElem : ScrollItemList)
		{
			if (IsValid(ItemElem))
			{
				if (ItemElem->Count == idx)
				{
					UHeroScrollItem_New* ScrollItem = Cast<UHeroScrollItem_New>(ItemElem);
					if (ScrollItem)
					{
						ScrollItem->IsSelected = true;
						ScrollItem->ItemData->bSelected = true;

						UHeroScrollItemData_New* itemdata = Cast<UHeroScrollItemData_New>(ScrollItem->ItemData);
						if (itemdata)
						{
							itemdata->eSelectType = UHeroScrollItemData_New::SELECTED;
							ScrollItem->InvalidateData();
						}
					}
				}
			}
		}
	}

}

void FDeckManager_Blackboard::SetData_OnBlackBoardData(UUserWidget *InWidget)
{
	if (WaitingList.size() < MAX_LISTCOUNT)
	{
		WaitingList.push_back(InWidget);
		OnBBStateChanged.Broadcast();
	}
}


void FUI_StateController::ChangeState()
{
	if (blackboard->WaitingList.size() == 1)
	{
		UUserWidget* widget = blackboard->WaitingList.front();
		URBaseDeckWidget* Waiting_Deck = Cast<URBaseDeckWidget>(widget);
		
		if (Waiting_Deck)
		{
			if (Waiting_Deck->GetCurHeroUD().IsEmpty())
				SetState(EUserBehaviorState::SELECTING_START);
			else
				SetState(EUserBehaviorState::CHANGING_START);
		}
	}
	else if (blackboard->WaitingList.size() == 2)
	{
		if (blackboard->WaitingList.front() == blackboard->WaitingList.back())
			SetState(EUserBehaviorState::DESELECT_DECK);
		else if (Cast<URBaseDeckWidget>(blackboard->WaitingList.front()) && Cast<UUC_GenericScrollItem>(blackboard->WaitingList.back()))
		{
			SetState(EUserBehaviorState::SELECTING_END);
		}
		else if (Cast<URBaseDeckWidget>(blackboard->WaitingList.front()))
		{
			URBaseDeckWidget* FrontDeck = Cast<URBaseDeckWidget>(blackboard->WaitingList.front());
			if (FrontDeck)
			{
				if (!FrontDeck->GetCurHeroUD().IsEmpty())
					SetState(EUserBehaviorState::CHANGING_END);
				else
					blackboard->WaitingList.pop_back();
			}
		}
	}
}

/*work to do*/
/*1. blackboard->waitingList manage*/
/*2. blackboard->selectedheroUDs manage*/
void FUI_StateController::Process()
{
	switch (UserState)
	{
	case EUserBehaviorState::IDLE:
	{
		break;
	}

	case EUserBehaviorState::SELECTING_START:
	{
		while (blackboard->WaitingList.size() > 1)
		{
		
			UUserWidget *widget = blackboard->WaitingList.front();
			UUC_GenericScrollItem *Waiting_ScrollItem = Cast<UUC_GenericScrollItem>(widget);
			if (Waiting_ScrollItem)
			{
				blackboard->WaitingList.pop_front();
			}
		}

		UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
		if (GenericScrollview && GenericScrollview->Variables.State == EScrollviewType::VE_END)
		{
			TArray<FString> HeroUDs;
			RGAMEINSTANCE(GEngine)->RInventory->GetProtectedHeros(HeroUDs);
			for (size_t i = 0; i < HeroUDs.Num(); )
			{
				int32 FoundIdx = blackboard->SelectedHeroUDs.Find(HeroUDs[i]);
				if (FoundIdx != INDEX_NONE)
				{
					HeroUDs.RemoveAt(i);
					continue;
				}
				++i;
			}
			bool bExistFavoriteHeros = HeroUDs.Num() != 0;


			UUserWidget* MirrorDeckWidget = RGAMEINSTANCE(GEngine)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_MirrorDeckSetting);
			if (IsValid(MirrorDeckWidget) && MirrorDeckWidget->IsInViewport())
			{
				GenericScrollview->SetState(EScrollviewType::VE_HERO_DECK_SHOWHP);
			}
			else
			{
				if (bExistFavoriteHeros)
					GenericScrollview->SetState(EScrollviewType::VE_HERO_DECK_FAVORITE);
				else
					GenericScrollview->SetState(EScrollviewType::VE_HERO_DECK);
			}

			GenericScrollview->Appear(true);
		}
		break;
	}

	case EUserBehaviorState::SELECTING_END:
	{
		UUserWidget *widget = blackboard->WaitingList.front();
		URBaseDeckWidget *Waiting_Deck = Cast<URBaseDeckWidget>(widget);
		if (Waiting_Deck)
		{
			UUC_GenericScrollItem *TargetItem = Cast<UUC_GenericScrollItem>(blackboard->WaitingList.back());
			int32 iIdx = blackboard->TargetWidgetArray.Find(Waiting_Deck);
			if (iIdx != INDEX_NONE)
			{
				UUC_GenericScrollItemData *itemdata = Cast<UUC_GenericScrollItemData>(TargetItem->ItemData);
				if (itemdata)
				{
					FHERO* heroinfo = RGAMEINSTANCE(GEngine)->RInventory->GetHeroWithUD(itemdata->ItemUD);
					FCharacterTableInfo* info = UTableManager::GetInstancePtr()->GetCharacterRow(heroinfo->heroId);
					if (info && !info->IsHero && iIdx == ROLE_LEADER_INDEX)
					{
						UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
							UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Campaign_Leader_Not_Spirit"))));

						blackboard->WaitingList.pop_back();
						return;
					}
					else if (isSameHeroInDeck(itemdata->ItemUD, Waiting_Deck->GetCurHeroUD()))
					{
						UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
							UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Message_Cannot_SameHero"))));

						blackboard->WaitingList.pop_back();
						return;
					}
					else
					{
							blackboard->SelectedHeroUDs[iIdx] = itemdata->ItemUD;

						if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS) ||
							URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::MR))
							SendSelectPacket(iIdx);
					}
				}
			}

			//SelectComplete(Waiting_ScrollItem->ItemData, true);
		}

		blackboard->WaitingList.clear();
		SetState(EUserBehaviorState::IDLE);

		UUIFunctionLibrary::GetGenericScrollview()->Appear(false);

		break;
	}

	case EUserBehaviorState::CHANGING_START:
	{
		UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
		if (GenericScrollview)
		{
			UUserWidget* MirrorDeckWidget = RGAMEINSTANCE(GEngine)->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_MirrorDeckSetting);
			if (IsValid(MirrorDeckWidget) && MirrorDeckWidget->IsInViewport())
				GenericScrollview->SetState(EScrollviewType::VE_HERO_DECK_SHOWHP);
			else
				GenericScrollview->SetState(EScrollviewType::VE_HERO_DECK);

			GenericScrollview->Appear(true);
		}
		break;
	}

	case EUserBehaviorState::CHANGING_END:
	{
		if (Cast<URBaseDeckWidget>(blackboard->WaitingList.back()))
		{
			URBaseDeckWidget *SourceDeck = Cast<URBaseDeckWidget>(blackboard->WaitingList.front());
			int32 SourceIdx = blackboard->TargetWidgetArray.Find(SourceDeck);
			URBaseDeckWidget *TargetDeck = Cast<URBaseDeckWidget>(blackboard->WaitingList.back());
			int32 TargetIdx = blackboard->TargetWidgetArray.Find(TargetDeck);

			FHERO* heroinfo = RGAMEINSTANCE(GEngine)->RInventory->GetHeroWithUD(blackboard->SelectedHeroUDs[SourceIdx]);
			FCharacterTableInfo* info = UTableManager::GetInstancePtr()->GetCharacterRow(heroinfo->heroId);
			if (!info->IsHero && TargetIdx == ROLE_LEADER_INDEX)
			{
				UUIFunctionLibrary::ShowCommonPopupSplash(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_NOTIFY),
					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FName(TEXT("UI_Campaign_Leader_Not_Spirit"))));

				if (Cast<URBaseDeckWidget>(blackboard->WaitingList.back()))
					blackboard->WaitingList.pop_back();

				return;
			}

			if (SourceIdx != INDEX_NONE && TargetIdx != INDEX_NONE)
			{
				FString cacheUD = blackboard->SelectedHeroUDs[SourceIdx];
				blackboard->SelectedHeroUDs[SourceIdx] = blackboard->SelectedHeroUDs[TargetIdx];
				blackboard->SelectedHeroUDs[TargetIdx] = cacheUD;

				blackboard->WaitingList.clear();

			}

			SetState(EUserBehaviorState::IDLE);

			if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS) ||
				URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::MR))
			{
				SendSelectPacket(SourceIdx);
				SendSelectPacket(TargetIdx);
			}

			UUIFunctionLibrary::GetGenericScrollview()->Appear(false);
		}

		break;
	}
	case EUserBehaviorState::DESELECT_SCROLLITEM:
	{
		int32 iIdx = 0;
		UUserWidget *widget = blackboard->WaitingList.front();
		UHeroScrollItem_New *Waiting_ScrollItem = Cast<UHeroScrollItem_New>(widget);
		if (Waiting_ScrollItem)
		{
			UHeroScrollItemData_New *itemdata = Cast<UHeroScrollItemData_New>(Waiting_ScrollItem->ItemData);
			if (itemdata)
			{
				itemdata->eSelectType = UHeroScrollItemData_New::IDLE;
				Waiting_ScrollItem->InvalidateData();

				iIdx = blackboard->SelectedHeroUDs.Find(itemdata->HeroUD);
				if (iIdx != INDEX_NONE)
				{
					blackboard->SelectedHeroUDs[iIdx] = TEXT("");
					if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS) ||
						URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::MR))
						SendSelectPacket(iIdx);
				}
					
			}
		}


		blackboard->WaitingList.clear();
		SetState(EUserBehaviorState::IDLE);

		break;
	}

	case EUserBehaviorState::DESELECT_DECK:
	{
		if (blackboard->WaitingList.size() == 1)
		{
			blackboard->WaitingList.pop_back();
			SetState(EUserBehaviorState::IDLE);
		}
		else if (blackboard->WaitingList.size() == 2)
		{
			URBaseDeckWidget *deck = Cast<URBaseDeckWidget>(blackboard->WaitingList.back());
			int32 iIdx = blackboard->TargetWidgetArray.Find(deck);
			if (deck)
			{
				//Deselect_ScrollItem(blackboard->SelectedHeroUDs[iIdx]);
				blackboard->SelectedHeroUDs[iIdx] = TEXT("");
			}

			blackboard->WaitingList.clear();
			SetState(EUserBehaviorState::IDLE);

			UUIFunctionLibrary::GetGenericScrollview()->Appear(false);

			if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS) ||
				URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::MR))
				SendSelectPacket(iIdx);
		}

		break;
	}

	case EUserBehaviorState::DESELECT_ALL:
	{
		int32 idx = 0;
		for (auto &Element : blackboard->SelectedHeroUDs)
		{
			Element = TEXT("");
			
			if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS) ||
				URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::MR))
				SendSelectPacket(idx);

			++idx;
		}

		SetState(EUserBehaviorState::IDLE);

		

		break;
	}

	case EUserBehaviorState::AUTOSETTING_ALL:
	{
		auto SortingHelper = [](FString SourceUD, FString DestUD) -> bool {
			int32 SourcePower = UClient_BattleFunctionLibrary::GetHeroCombatStat(SourceUD);
			int32 DestPower = UClient_BattleFunctionLibrary::GetHeroCombatStat(DestUD);

			return SourcePower > DestPower;
		};

		bool bSpiritSelectable = UGamePlayBluePrintFunction::GetBaseStageInfo()->bPlayAlone;

		TArray<FString> UDlist;// = UUIFunctionLibrary::GetGenericScrollview()->Variables.CachedItemUDList;
		if (!UDlist.Num())
		{
			RGAMEINSTANCE(GEngine)->RInventory->GetHeroUDArray(UDlist);
		}

		Algo::Sort(UDlist, SortingHelper);

		for (auto &Element : blackboard->SelectedHeroUDs)
			Element = TEXT("");

		int32 iCount = 0;
		for (size_t i = 0; i < UDlist.Num(); ++i)
		{
			FHERO* hero = RGAMEINSTANCE(GEngine)->RInventory->GetHeroWithUD(UDlist[i]);
			if (hero)
			{
				FCharacterTableInfo* tableinfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
				if (!bSpiritSelectable)
				{
					if (!tableinfo->IsHero)
						continue;
				}
				else
				{
					if (i == ROLE_LEADER_INDEX && !tableinfo->IsHero)
						continue;
					else if (i != ROLE_LEADER_INDEX && tableinfo->IsHero)
						continue;
				}
			}

			if (!isSameHeroInDeck(UDlist[i], TEXT("")))
			{
				blackboard->SelectedHeroUDs[iCount] = UDlist[i];

				if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS) ||
					URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::MR))
					SendSelectPacket(iCount);

				++iCount;

				if (iCount == MAX_DECKCOUNT)
					break;
			}
		}

		SetState(EUserBehaviorState::IDLE);
		break;
	}

	case EUserBehaviorState::LOADDECKSETTING:
	{
		FDECKS decks = RGAMEINSTANCE(GEngine)->RInventory->GetHeroDeck();

		TArray<FString> DeckUd;
		// decide which UI deck type will be used at first time.
		UBaseStageInfo *BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
		if (BaseStageInfo)
		{
			FFloorStageDetail *FloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(BaseStageInfo->StageID);
			if (FloorStageTable)
			{
				if (FloorStageTable->PlayType.ToString().Contains(TEXT("N")))
					DeckUd = decks.campaignThree;
				else if (FloorStageTable->PlayType.ToString().Contains(TEXT("H")))
					DeckUd = decks.campaignOne;
			}
		}

		for (size_t i = 0; i < DeckUd.Num(); ++i)
			blackboard->SelectedHeroUDs[i] = DeckUd[i];


		SetState(EUserBehaviorState::IDLE);
		break;
	}
	}
}

void FUI_StateController::SendSelectPacket(int32 RoleIdx)
{
	UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
	if (GenericScrollview)
	{
		TArray<FString> SelectedUDs = GenericScrollview->Blackboard.SelectedHeroUDs;
		if(!SelectedUDs[RoleIdx].IsEmpty())
		{
			FHERO* heroinfo = RGAMEINSTANCE(GEngine)->RInventory->GetHeroWithUD(SelectedUDs[RoleIdx]);
			if (heroinfo)
			{
				if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS))
					URTSManager::GetInstancePtr()->REQ_CS_DECK_SELECT(RoleIdx + 1, SelectedUDs[RoleIdx], heroinfo->heroId.ToString(), heroinfo->level);
				else
					URTSManager::GetInstancePtr()->REQ_DECK_SELECT(RGAMEINSTANCE(GEngine)->RealTimeModManager->RTSInfo.mSlotNo, RoleIdx + 1, SelectedUDs[RoleIdx], heroinfo->heroId.ToString(), heroinfo->level);
			}
		}
		else
		{
			if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS))
				URTSManager::GetInstancePtr()->REQ_CS_DECK_SELECT(RoleIdx + 1, TEXT(""), TEXT(""), 1);
			else
				URTSManager::GetInstancePtr()->REQ_DECK_SELECT(RGAMEINSTANCE(GEngine)->RealTimeModManager->RTSInfo.mSlotNo, RoleIdx + 1, TEXT(""), TEXT(""), 1);
		}
	}
}


bool FUI_StateController::isSameHeroInDeck(FString heroUD, FString TargetUD)
{
	FHERO *SourceTarget = RGAMEINSTANCE(GEngine)->RInventory->GetHeroWithUD(heroUD);
	FHERO *DestTarget = RGAMEINSTANCE(GEngine)->RInventory->GetHeroWithUD(TargetUD);
	if (SourceTarget && DestTarget)
	{
		FCharacterTableInfo *SourceTargetTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(SourceTarget->heroId);
		FCharacterTableInfo *DestTargetTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(DestTarget->heroId);

		if (SourceTargetTableInfo->DisplayName == DestTargetTableInfo->DisplayName)
			return false;
	}

	//check if there is already the same heroUD
	for (size_t i = 0; i < blackboard->SelectedHeroUDs.Num(); ++i)
	{
		if (blackboard->SelectedHeroUDs[i].IsEmpty())
			continue;

		SourceTarget = RGAMEINSTANCE(GEngine)->RInventory->GetHeroWithUD(heroUD);
		DestTarget = RGAMEINSTANCE(GEngine)->RInventory->GetHeroWithUD(blackboard->SelectedHeroUDs[i]);
		if (SourceTarget == nullptr || DestTarget == nullptr)
			continue;

		FCharacterTableInfo *SourceTargetTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(SourceTarget->heroId);
		FCharacterTableInfo *DestTargetTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(DestTarget->heroId);

		if (SourceTargetTableInfo == nullptr || DestTargetTableInfo == nullptr)
			continue;

		if (SourceTargetTableInfo->DisplayName == DestTargetTableInfo->DisplayName)
			return true;
	}

	return false;
}