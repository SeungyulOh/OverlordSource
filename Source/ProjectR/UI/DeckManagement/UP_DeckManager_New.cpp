// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_DeckManager_New.h"

#include "Global/TableManager.h"
#include "Global/RGameInstance.h"
#include "Global/RealTimeModManager.h"
#include "Global/ForceTutorialManager.h"

#include "GamePlayBluePrintFunction.h"
#include "GameState/Stage/Spawner/BaseStageInfo.h"

#include "UI/RWidgetManager.h"

#include "UI/DeckManagement/UC_HeroDeck.h"
#include "UI/DeckManagement/UC_SkillForDeck.h"

#include "UI/Common/MonsterScrollItemData.h"
#include "UI/Common/ScrollView/UP_HeroScrollView_Bottom.h"

#include "UP_GenericScrollview.h"

#include "UC_PartyEffect.h"


#include "UI/UI_Common/UP_CommonPopup.h"
#include "UI/RWidgetManager.h"
#include "UtilFunctionIntegrated.h"
#include "GameInfo/RInventory.h"
#include "Network/RTSManager.h"
#include "UI/Common/UC_Item_Icons.h"

#include "UI/Common/BaseMonsterIcon.h"



void UUP_DeckManager_New::NativeConstruct()
{
	Super::NativeConstruct();

	// decide which UI deck type will be used at first time.
	UBaseStageInfo *BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	if (BaseStageInfo)
	{
		FFloorStageDetail *FloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(BaseStageInfo->StageID);
		if (FloorStageTable)
		{
			if (FloorStageTable->PlayType.ToString().Contains(TEXT("N")))
				Variables.DeckUIType = FUI_Variables::EDeckUIType::CAMPAIGN_NORMAL;
			else if (FloorStageTable->PlayType.ToString().Contains(TEXT("H")))
				Variables.DeckUIType = FUI_Variables::EDeckUIType::CAMPAIGN_HARD;
			else if (FloorStageTable->PlayType.ToString().Contains(TEXT("S")))
				Variables.DeckUIType = FUI_Variables::EDeckUIType::SINGLERAID;
		}
		else
		{
			FRaidStageTableInfo* RaidTable = UTableManager::GetInstancePtr()->GetRaidStageTableRow(BaseStageInfo->StageID);
			if(RaidTable && RaidTable->PlayMode == FName(TEXT("S")))
				Variables.DeckUIType = FUI_Variables::EDeckUIType::SINGLERAID;
		}
	}

	/*Blackboard Setting Start*/
	Blackboard = &(UUIFunctionLibrary::GetGenericScrollview()->Blackboard);
	Blackboard->SelectedHeroUDs.Init(TEXT(""), MAX_DECKCOUNT);
	Blackboard->OnBBStateChanged.AddDynamic(this, &UUP_DeckManager_New::Update_FromBlackboard);
	
	Blackboard->TargetWidgetArray.Empty();
	for (size_t i = 0; i < Variables.HeroDeckList.Num(); ++i)
	{
		URBaseDeckWidget* BaseDeckWidget = Cast<URBaseDeckWidget>(Variables.HeroDeckList[i]);
		if (BaseDeckWidget)
		{
			Blackboard->TargetWidgetArray.Emplace(BaseDeckWidget);
		}
			
	}
		
	/*Blackboard Setting End*/

	/*StateController Setting Start*/
	StateController = &(UUIFunctionLibrary::GetGenericScrollview()->StateController);
	/*StateController Setting End*/

	/*Renderer Setting Start*/
	Renderer.blackboard = Blackboard;
	Renderer.StateController = StateController;
	Renderer.variables = &Variables;
	Renderer.Prepare();
	Renderer.Render();
	/*Renderer Setting End*/

	/*Buttons Binding Start*/
	if (Variables.Button_Start)
	{
		Variables.Button_Start->OnClicked.Clear();
		Variables.Button_Start->OnClicked.AddDynamic(this, &UUP_DeckManager_New::OnClick_GameStart);
	}
	if (Variables.Button_Clear)
	{
		Variables.Button_Clear->OnClicked.Clear();
		Variables.Button_Clear->OnClicked.AddDynamic(this, &UUP_DeckManager_New::ClearDeckSetting);
	}
	if (Variables.Button_AutoSetting)
	{
		Variables.Button_AutoSetting->OnClicked.Clear();
		Variables.Button_AutoSetting->OnClicked.AddDynamic(this, &UUP_DeckManager_New::AutoDeckSetting);
	}
	if (Variables.ButtonPrev)
	{
		Variables.ButtonPrev->OnClicked.Clear();
		Variables.ButtonPrev->OnClicked.AddDynamic(this, &UUP_DeckManager_New::OnClick_ButtonPrev);
	}
	if (Variables.ButtonAfter)
	{
		Variables.ButtonAfter->OnClicked.Clear();
		Variables.ButtonAfter->OnClicked.AddDynamic(this, &UUP_DeckManager_New::OnClick_ButtonAfter);
	}
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	RGameInstance->RealTimeModManager->OnReceiveDeckSelect.RemoveDynamic(this, &UUP_DeckManager_New::onACK_DeckList);
	RGameInstance->RealTimeModManager->OnReceiveDeckSelect.AddDynamic(this, &UUP_DeckManager_New::onACK_DeckList);

	/*Buttons Binding End*/

	/*Load DeckSetting Start*/
	StateController->SetState(EUserBehaviorState::LOADDECKSETTING);
	Blackboard->OnBBStateChanged.Broadcast();
	/*Load DeckSetting End*/
	
// 	if(RGameInstance->ForceTutorialManager)
// 	{
// 		RGameInstance->ForceTutorialManager->CheckUnlockTutorial(EFirstCheckContentsType::FirstStageHard1_1);
// 		RGameInstance->ForceTutorialManager->CheckUnlockTutorial(EFirstCheckContentsType::FirstSingleRaid_DeckSetting);
// 	}
}

void UUP_DeckManager_New::NativeDestruct()
{
	Blackboard->OnBBStateChanged.RemoveDynamic(this, &UUP_DeckManager_New::Update_FromBlackboard);

	UUP_GenericScrollview* GenericScrollview = UUIFunctionLibrary::GetGenericScrollview();
	if (GenericScrollview)
	{
		GenericScrollview->Appear(false);
	}

	Super::NativeDestruct();
}

void UUP_DeckManager_New::Update_FromBlackboard()
{
	Renderer.Render();
}

void UUP_DeckManager_New::OnClick_GameStart()
{
	UBaseStageInfo *BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	if (!BaseStageInfo)
		return;

	int32 ReqStamina = 0;
	if (Variables.DeckUIType != FUI_Variables::EDeckUIType::SINGLERAID)
	{
		FFloorStageDetail *FloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(BaseStageInfo->StageID);
		if (!FloorStageTable)
			return;
		ReqStamina = FloorStageTable->Stamina;
	}
	else
	{
		FRaidStageTableInfo *RaidStageTable = UTableManager::GetInstancePtr()->GetRaidStageTableRow(BaseStageInfo->StageID);
		if (!RaidStageTable)
			return;
		ReqStamina = RaidStageTable->Stamina;
	}
	

	/*Enough Stamina ?*/
	/*Leader selected ?*/
	auto CheckStartCondition = [=](int32 RequiredStamina) -> bool {
		int32 CurrentStamina = RGAMEINSTANCE(this)->RInventory->GetStamina();
		if (CurrentStamina > 0 && CurrentStamina >= RequiredStamina)
		{
			if (!Blackboard->SelectedHeroUDs[ROLE_LEADER_INDEX].IsEmpty())
				return true;
			else
			{
				UUIFunctionLibrary::ShowCommonPopup(
					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_WARNING),
					UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, TEXT("Confirm_Message_000045"))
					, ECommonPopupType::VE_OK);
			}
		}
		else
		{
			UUIFunctionLibrary::ShowCommonPopup(
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Warning")),
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("Popup_Campaign_Start_StaminaCondition")), ECommonPopupType::VE_OK);
		}
		return false;
	};

	if (CheckStartCondition(ReqStamina))
	{
		//PlayHeroInfo
		if (Variables.DeckUIType == FUI_Variables::EDeckUIType::CAMPAIGN_NORMAL)
			PlayHeroInfo.UsingType = EDeckUsingTypeEnum::VE_Campaign;
		else if (Variables.DeckUIType == FUI_Variables::EDeckUIType::CAMPAIGN_HARD)
			PlayHeroInfo.UsingType = EDeckUsingTypeEnum::VE_PlayAlone;
		else if (Variables.DeckUIType == FUI_Variables::EDeckUIType::SINGLERAID)
			PlayHeroInfo.UsingType = EDeckUsingTypeEnum::VE_Raid;

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

		PlayHeroInfo.HeroUDs = Blackboard->SelectedHeroUDs;
		PlayHeroInfo.HeroMaxCount = MAX_DECKCOUNT;

		int32 iEmptySlot = INDEX_NONE;
		for (size_t i = 0; i < Blackboard->SelectedHeroUDs.Num(); ++i)
		{
			if (Blackboard->SelectedHeroUDs.IsValidIndex(i) && !Blackboard->SelectedHeroUDs[i].IsEmpty())
			{
				FHERO_ICON_INFO HeroIconInfo;
				UUIFunctionLibrary::GetHeroIconInfoWithUD(HeroIconInfo, Blackboard->SelectedHeroUDs[i], EHeroOwnershipType::VE_Me);
				PlayHeroInfo.HeroIconInfos.Emplace(HeroIconInfo);
				PlayHeroInfo.TotalCombatStat += UClient_BattleFunctionLibrary::GetHeroCombatStat(Blackboard->SelectedHeroUDs[i]);
			}
			else
				++iEmptySlot;

			if(Variables.DeckUIType == FUI_Variables::EDeckUIType::SINGLERAID)
				StateController->SendSelectPacket(i);
		}

		int32 iRemainHeroCount = RGAMEINSTANCE(this)->RInventory->GetHeroCount() - (MAX_DECKCOUNT - iEmptySlot);
		if (iEmptySlot > 0 && iRemainHeroCount > iEmptySlot)
		{
			UUP_CommonPopup* popup = UUIFunctionLibrary::ShowCommonPopup(
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, COMMONSTRING_WARNING),
				UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_SystemMessage, TEXT("Confirm_Message_000044"))
				, ECommonPopupType::VE_OKCANCEL);

			if (popup)
			{
				popup->OnClickOK.Clear();
				popup->OnClickOK.AddDynamic(this, &UUP_DeckManager_New::GameStart);
			}
		}
		else
			UGamePlayFunctionLibrary::RequestGameStart(PlayHeroInfo);
	}
}

void UUP_DeckManager_New::GameStart()
{
	UGamePlayFunctionLibrary::RequestGameStart(PlayHeroInfo);
}

void UUP_DeckManager_New::AutoDeckSetting()
{
	Blackboard->WaitingList.clear();
	StateController->SetState(EUserBehaviorState::AUTOSETTING_ALL);
	Blackboard->OnBBStateChanged.Broadcast();
}

void UUP_DeckManager_New::ClearDeckSetting()
{
	Blackboard->WaitingList.clear();
	StateController->SetState(EUserBehaviorState::DESELECT_ALL);
	Blackboard->OnBBStateChanged.Broadcast();
}

void UUP_DeckManager_New::onACK_DeckList(FDeckSelected DeckSelected)
{
	if (DeckSelected.crewNo == 0)
	{
		RGAMEINSTANCE(this)->RealTimeModManager->OnReceiveDeckSelect.RemoveDynamic(this, &UUP_DeckManager_New::onACK_DeckList);
		StateController->SetState(EUserBehaviorState::IDLE);
		Blackboard->OnBBStateChanged.Broadcast();
	}
	else
	{
		Blackboard->SelectedHeroUDs[DeckSelected.crewNo - 1] = DeckSelected.heroUD;
	}
}

void UUP_DeckManager_New::OnClick_ButtonPrev()
{
	UBaseStageInfo* CurrentStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	if (CurrentStageInfo)
	{
		int32 TargetStageIdx = CurrentStageInfo->StageIndex - 1;

		TArray<int32> ClearGrades = RGAMEINSTANCE(this)->CampaignManager->TowerStatus.clearGrades;
		if (ClearGrades.IsValidIndex(TargetStageIdx) && ClearGrades[TargetStageIdx] > 0)
		{
			TArray<FName> TableKeys = UTableManager::GetInstancePtr()->GetFloorStageRowNames();
			FFloorStageDetail* FloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(TableKeys[TargetStageIdx]);
			if (FloorStageTable)
			{
				checkf(CurrentStageInfo, TEXT("CurrentStageInfo is Null"));

				CurrentStageInfo->ClearRewardTableKey = FloorStageTable->ClearRewardTableKey;
				CurrentStageInfo->MapTableKey = FName(*FString::FromInt(FloorStageTable->MapTableKey));
				CurrentStageInfo->FloorInfoId = FName(*FString::FromInt(FloorStageTable->FloorInfoId));
				CurrentStageInfo->StageIndex = FloorStageTable->StageIdx;
				CurrentStageInfo->StageID = TableKeys[TargetStageIdx];

				if (FloorStageTable->PlayType.ToString().Contains(TEXT("H")))
					CurrentStageInfo->bPlayAlone = true;
				else
					CurrentStageInfo->bPlayAlone = false;
			}
		}
	}

	Renderer.Prepare();
}

void UUP_DeckManager_New::OnClick_ButtonAfter()
{
	UBaseStageInfo* CurrentStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	if (CurrentStageInfo)
	{
		int32 TargetStageIdx = CurrentStageInfo->StageIndex + 1;

		TArray<int32> ClearGrades = RGAMEINSTANCE(this)->CampaignManager->TowerStatus.clearGrades;
		if (ClearGrades.IsValidIndex(CurrentStageInfo->StageIndex) && ClearGrades[CurrentStageInfo->StageIndex] > 0)
		{
			TArray<FName> TableKeys = UTableManager::GetInstancePtr()->GetFloorStageRowNames();
			FFloorStageDetail* FloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(TableKeys[TargetStageIdx]);
			if (FloorStageTable)
			{
				checkf(CurrentStageInfo, TEXT("CurrentStageInfo is Null"));

				CurrentStageInfo->ClearRewardTableKey = FloorStageTable->ClearRewardTableKey;
				CurrentStageInfo->MapTableKey = FName(*FString::FromInt(FloorStageTable->MapTableKey));
				CurrentStageInfo->FloorInfoId = FName(*FString::FromInt(FloorStageTable->FloorInfoId));
				CurrentStageInfo->StageIndex = FloorStageTable->StageIdx;
				CurrentStageInfo->StageID = TableKeys[TargetStageIdx];

				if (FloorStageTable->PlayType.ToString().Contains(TEXT("H")))
					CurrentStageInfo->bPlayAlone = true;
				else
					CurrentStageInfo->bPlayAlone = false;
			}
		}
	}

	Renderer.Prepare();
}

void FUI_Renderer::Prepare()
{
	//Text Setting
	switch (variables->DeckUIType)
	{
	case FUI_Variables::EDeckUIType::CAMPAIGN_NORMAL:
	case FUI_Variables::EDeckUIType::SINGLERAID:

		if (variables->CrewSpirit_Text)
			variables->CrewSpirit_Text->SetText(FText::FromString(TEXT("CREW")));

		if (IsValid(variables->ChangeImage1))
			variables->ChangeImage1->SetRenderOpacity(1.f);
		if (IsValid(variables->ChangeImage2))
			variables->ChangeImage2->SetRenderOpacity(1.f);
		if (IsValid(variables->SpiritCircleImage))
			variables->SpiritCircleImage->SetVisibility(ESlateVisibility::Collapsed);

		if (IsValid(variables->StageNumber))
		{
			FSlateColor color = FSlateColor(FLinearColor(1.f, 1.f, 1.f));
			variables->StageNumber->SetColorAndOpacity(color);
		}

		break;
	case FUI_Variables::EDeckUIType::CAMPAIGN_HARD:

		if (variables->CrewSpirit_Text)
			variables->CrewSpirit_Text->SetText(FText::FromString(TEXT("SPIRIT")));

		if (IsValid(variables->ChangeImage1))
			variables->ChangeImage1->SetRenderOpacity(0.f);
		if (IsValid(variables->ChangeImage2))
			variables->ChangeImage2->SetRenderOpacity(0.f);
		if (IsValid(variables->SpiritCircleImage))
			variables->SpiritCircleImage->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

		if (IsValid(variables->StageNumber))
		{
			FSlateColor color = FSlateColor(FLinearColor(1.f, 0.f, 0.f));
			variables->StageNumber->SetColorAndOpacity(color);
		}

		break;
	}

	UBaseStageInfo *BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	if (BaseStageInfo)
	{
		FFloorStageDetail *FloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(BaseStageInfo->StageID);
		if (variables->DeckUIType != FUI_Variables::EDeckUIType::SINGLERAID)
		{
			if (IsValid(variables->Text_NeedStamina))
				variables->Text_NeedStamina->SetText(FText::AsNumber(FloorStageTable->Stamina));
			if (IsValid(variables->HorizontalAfter))
				variables->HorizontalAfter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			if (IsValid(variables->HorizontalPrev))
				variables->HorizontalPrev->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			if (IsValid(variables->StageTitle))
				variables->StageTitle->SetVisibility(ESlateVisibility::SelfHitTestInvisible);

			if (IsValid(variables->StageNumber))
			{
				variables->StageNumber->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				variables->StageNumber->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, FloorStageTable->IngameSubTitle));
			}
				
			if (IsValid(variables->TextPrev))
			{
				int32 TargetStageIdx = BaseStageInfo->StageIndex - 1;
				TArray<FName> TableKeys = UTableManager::GetInstancePtr()->GetFloorStageRowNames();
				FFloorStageDetail* CurrentFloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(TableKeys[BaseStageInfo->StageIndex]);
				FFloorStageDetail* PrevFloorStageTable = nullptr;
				if (TargetStageIdx != INDEX_NONE)
					PrevFloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(TableKeys[TargetStageIdx]);

				TArray<int32> ClearGrades = RGAMEINSTANCE(GEngine)->CampaignManager->TowerStatus.clearGrades;
				if (PrevFloorStageTable &&
					ClearGrades.IsValidIndex(TargetStageIdx) && ClearGrades[TargetStageIdx] > 0
					&& CurrentFloorStageTable->FloorInfoId == PrevFloorStageTable->FloorInfoId
					&& CurrentFloorStageTable->PlayType == PrevFloorStageTable->PlayType)
				{
					variables->HorizontalPrev->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					variables->ButtonPrev->SetIsEnabled(true);

					variables->TextPrev->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, PrevFloorStageTable->IngameSubTitle));
				}
				else
				{
					variables->HorizontalPrev->SetVisibility(ESlateVisibility::Hidden);
					variables->ButtonPrev->SetIsEnabled(false);
				}

			}
			if (IsValid(variables->TextAfter))
			{
				int32 TargetStageIdx = BaseStageInfo->StageIndex + 1;
				TArray<FName> TableKeys = UTableManager::GetInstancePtr()->GetFloorStageRowNames();
				FFloorStageDetail* CurrentFloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(TableKeys[BaseStageInfo->StageIndex]);

				FFloorStageDetail* NextFloorStageTable = nullptr;
				if (TargetStageIdx != INDEX_NONE)
					NextFloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(TableKeys[TargetStageIdx]);

				TArray<int32> ClearGrades = RGAMEINSTANCE(GEngine)->CampaignManager->TowerStatus.clearGrades;
				if (NextFloorStageTable &&
					ClearGrades.IsValidIndex(BaseStageInfo->StageIndex) && ClearGrades[BaseStageInfo->StageIndex] > 0
					&& CurrentFloorStageTable->FloorInfoId == NextFloorStageTable->FloorInfoId
					&& CurrentFloorStageTable->PlayType == NextFloorStageTable->PlayType)
				{
					variables->HorizontalAfter->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					variables->ButtonAfter->SetIsEnabled(true);

					variables->TextAfter->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, NextFloorStageTable->IngameSubTitle));
				}
				else
				{
					variables->HorizontalAfter->SetVisibility(ESlateVisibility::Hidden);
					variables->ButtonAfter->SetIsEnabled(false);
				}
			}

			if (IsValid(variables->StageTitle))
			{
				variables->StageTitle->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Campaign, FloorStageTable->Title));
			}

			if (IsValid(variables->ClearTitle))
			{
				variables->ClearTitle->SetText(UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, FloorStageTable->clearInfo1));
			}

			TArray<FITEM_ICON_INFO> ItemIconArray;
			UUIFunctionLibrary::GetRewardsIconInfoByPreview(FloorStageTable->ClearRewardTableKey, ItemIconArray);

			for (size_t i = 0; i < 5; ++i)
			{
				if (i >= ItemIconArray.Num())
				{
					variables->RewardSizeBoxes[i]->SetVisibility(ESlateVisibility::Collapsed);
					continue;
				}

				if (variables->RewardIcons.IsValidIndex(i))
					variables->RewardIcons[i]->SetItemIconStructure(ItemIconArray[i]);
			}


			for (size_t i = 0; i < variables->MonsterIcons.Num(); ++i)
			{
				if (FloorStageTable->MonsterIDList.IsValidIndex(i))
				{
					FHERO_ICON_INFO NPCIconInfo;
					UUIFunctionLibrary::GetHeroIconInfoForNpc(NPCIconInfo, FloorStageTable->MonsterIDList[i]);
					NPCIconInfo.level = 1;

					if (IsValid(variables->MonsterIcons[i]))
						variables->MonsterIcons[i]->SetMonsterIconByIconInfo(NPCIconInfo, true);
				}
			}
		}
		else
		{
			if (IsValid(variables->HorizontalAfter))
				variables->HorizontalAfter->SetVisibility(ESlateVisibility::Collapsed);
			if (IsValid(variables->HorizontalPrev))
				variables->HorizontalPrev->SetVisibility(ESlateVisibility::Collapsed);

			if (IsValid(variables->StageNumber))
				variables->StageNumber->SetVisibility(ESlateVisibility::Collapsed);
			if (IsValid(variables->StageTitle))
				variables->StageTitle->SetVisibility(ESlateVisibility::Collapsed);

			FRaidStageTableInfo* RaidTable = UTableManager::GetInstancePtr()->GetRaidStageTableRow(BaseStageInfo->StageID);

			if (IsValid(variables->Text_NeedStamina))
				variables->Text_NeedStamina->SetText(FText::AsNumber(RaidTable->Stamina));

			for (size_t i = 0; i < variables->MonsterIcons.Num(); ++i)
			{
				if (RaidTable->MonsterIDList.IsValidIndex(i))
				{
					FHERO_ICON_INFO NPCIconInfo;
					UUIFunctionLibrary::GetHeroIconInfoForNpc(NPCIconInfo, RaidTable->MonsterIDList[i]);
					NPCIconInfo.level = 1;

					if (IsValid(variables->MonsterIcons[i]))
						variables->MonsterIcons[i]->SetMonsterIconByIconInfo(NPCIconInfo, true);
				}
			}

			TArray<FITEM_ICON_INFO> ItemIconArray;
			UUIFunctionLibrary::GetRewardsIconInfoByPreview(RaidTable->RewardPreviewKey, ItemIconArray);
			for (size_t i = 0; i < 5; ++i)
			{
				if (i >= ItemIconArray.Num())
				{
					variables->RewardSizeBoxes[i]->SetVisibility(ESlateVisibility::Collapsed);
					continue;
				}

				if (variables->RewardIcons.IsValidIndex(i))
					variables->RewardIcons[i]->SetItemIconStructure(ItemIconArray[i]);
			}
		}
		
	}
	

}

void FUI_Renderer::Render()
{
	EUserBehaviorState UserState = StateController->UserState;

	switch (UserState)
	{
	case EUserBehaviorState::IDLE:
	{

		auto HideAvailableImage = [](TArray<UUC_HeroDeck *> InArray) {
			for (size_t i = 0; i < InArray.Num(); ++i)
			{
				if (InArray.IsValidIndex(i))
				{
					InArray[i]->SetAvailable(false);
					InArray[i]->SetSelecting(false);

				}
					
			}
		};

		HideAvailableImage(variables->HeroDeckList);

		//Show Selected Deck
		bool isEliteMode = variables->DeckUIType == FUI_Variables::EDeckUIType::CAMPAIGN_HARD ? true : false;
		for (int32 i = 0; i < MAX_DECKCOUNT; ++i)
		{
			FString heroUD = blackboard->SelectedHeroUDs[i];
			if (variables->HeroDeckList.IsValidIndex(i))
				variables->HeroDeckList[i]->SetHero(heroUD);

			if (variables->HeroSkillList.IsValidIndex(i))
				variables->HeroSkillList[i]->SetSkillIcon(heroUD, isEliteMode);
		}

		/*PartyEffects*/
		variables->UC_PartyEffect->Update(blackboard->SelectedHeroUDs);

		//AttackPower Render
		int32 CurrentPower = 0;

		UBaseStageInfo *BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
		if (BaseStageInfo)
		{
			int32 RecommendedPower = 0;
			if (variables->DeckUIType != FUI_Variables::EDeckUIType::SINGLERAID)
			{
				FFloorStageDetail *FloorStageTable = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(BaseStageInfo->StageID);
				if (FloorStageTable)
				{
					RecommendedPower = FloorStageTable->RecommendedPower;
					variables->TextBlock_AttackPower->SetText(FText::AsNumber(RecommendedPower));
				}
					
			}
			else
			{
				FRaidStageTableInfo *RaidStageTable = UTableManager::GetInstancePtr()->GetRaidStageTableRow(BaseStageInfo->StageID);
				if (RaidStageTable)
				{
					RecommendedPower = RaidStageTable->RecommendPowerValue;
					variables->TextBlock_AttackPower->SetText(FText::AsNumber(RecommendedPower));
				}
					
			}
			

			for (size_t i = 0; i < blackboard->SelectedHeroUDs.Num(); ++i)
			{
				if (blackboard->SelectedHeroUDs.IsValidIndex(i) && !blackboard->SelectedHeroUDs[i].IsEmpty())
				{
					CurrentPower += UClient_BattleFunctionLibrary::GetHeroCombatStat(blackboard->SelectedHeroUDs[i]);
				}
			}

			variables->TextBlock_PartyAttackPower->SetText(FText::AsNumber(CurrentPower));

			if (CurrentPower > RecommendedPower)
			{
				FSlateColor PlusColor = FSlateColor(FLinearColor(0.63f, 0.88f, 0.02f, 1.f));
				variables->TextBlock_PartyAttackPower->SetColorAndOpacity(PlusColor);
// 				variables->WidgetSwitcherDifficulty->SetActiveWidgetIndex(0);
			}
			else
			{
				FSlateColor MinusColor = FSlateColor(FLinearColor(1.f, 0.04f, 0.04f, 1.f));
				variables->TextBlock_PartyAttackPower->SetColorAndOpacity(MinusColor);
// 				variables->WidgetSwitcherDifficulty->SetActiveWidgetIndex(1);
			}
		}

		break;
	}
	case EUserBehaviorState::SELECTING_START:
	{
		auto FindAvailableDeck = [](TArray<UUC_HeroDeck *> InArray , int32 Targetidx) {
			for (size_t i = 0; i < InArray.Num(); ++i)
			{
				if (InArray.IsValidIndex(i))
				{
					if(i == Targetidx)
						InArray[i]->SetSelecting(true);
				}
			}
		};

		UUserWidget *widget = blackboard->WaitingList.front();
		URBaseDeckWidget *Waiting_Deck = Cast<URBaseDeckWidget>(widget);
		if (Waiting_Deck)
		{
			int32 iIdx = blackboard->TargetWidgetArray.Find(Waiting_Deck);
			if(iIdx != INDEX_NONE)
				FindAvailableDeck(variables->HeroDeckList , iIdx);
		}
		
		break;
	}
	case EUserBehaviorState::CHANGING_START:
	{
		if (blackboard->WaitingList.size() == 1)
		{
			UUC_HeroDeck *heroDeck = Cast<UUC_HeroDeck>(blackboard->WaitingList.front());
			if (heroDeck)
			{
				int32 iIdx = variables->HeroDeckList.Find(heroDeck);
				if (iIdx != INDEX_NONE)
				{
					auto FindAvailableDeck = [=](TArray<UUC_HeroDeck *> InArray) {
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

					FindAvailableDeck(variables->HeroDeckList);
				}
			}
		}

		break;
	}
	}
}

