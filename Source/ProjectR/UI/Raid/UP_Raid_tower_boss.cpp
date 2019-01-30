// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "ContentIntergrated.h"
#include "ContentGameMode/Modes/TowerLobbyGameMode.h"
#include "UI/Raid/UC_Raid_tower_boss_reward.h"
#include "Lobby/IndoorLobbyStateActor.h"
#include "Network/PacketDataStructures.h"
#include "Network/PacketFunctionLibrary.h"
#include "Network/RTSManager.h"
#include "UI/Common/ItemBaseScrollItemData.h"
#include "UI/Common/MonsterScrollItemData.h"
#include "UP_Raid_tower_boss.h"


void UUP_Raid_tower_boss::NativeConstruct()
{
	Super::NativeConstruct();

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	if (IsValid(Button_SEntrance))
	{
		Button_SEntrance->OnClicked.RemoveDynamic(this, &UUP_Raid_tower_boss::OnClickSingleEntrance);
		Button_SEntrance->OnClicked.AddDynamic(this, &UUP_Raid_tower_boss::OnClickSingleEntrance);
	}
	if (IsValid(Button_MPartyCreate))
	{
		Button_MPartyCreate->OnClicked.RemoveDynamic(this, &UUP_Raid_tower_boss::OnClickMultiPartyCreate);
		Button_MPartyCreate->OnClicked.AddDynamic(this, &UUP_Raid_tower_boss::OnClickMultiPartyCreate);
	}
	if (IsValid(Button_MPartyNumber))
	{
		Button_MPartyNumber->OnClicked.RemoveDynamic(this, &UUP_Raid_tower_boss::OnClickMultiPartyNumber);
		Button_MPartyNumber->OnClicked.AddDynamic(this, &UUP_Raid_tower_boss::OnClickMultiPartyNumber);
	}
	if (IsValid(Button_MFastEntrance))
	{
		Button_MFastEntrance->OnClicked.RemoveDynamic(this, &UUP_Raid_tower_boss::OnClickMultiFastEntrance);
		Button_MFastEntrance->OnClicked.AddDynamic(this, &UUP_Raid_tower_boss::OnClickMultiFastEntrance);
	}
	if (IsValid(Button_MGuildParty))
	{
		Button_MGuildParty->OnClicked.RemoveDynamic(this, &UUP_Raid_tower_boss::OnClickMultiGuildParty);
		Button_MGuildParty->OnClicked.AddDynamic(this, &UUP_Raid_tower_boss::OnClickMultiGuildParty);
	}

	RGameInstance->EventManager->OnTowerBossRefersh.RemoveDynamic(this, &UUP_Raid_tower_boss::OnTowerBossRefersh);
	RGameInstance->EventManager->OnTowerBossRefersh.AddDynamic(this, &UUP_Raid_tower_boss::OnTowerBossRefersh);
	RGameInstance->EventManager->OnUpdateTowerBossUIPage.RemoveDynamic(this, &UUP_Raid_tower_boss::OnUpdateTowerBossUIPage);
	RGameInstance->EventManager->OnUpdateTowerBossUIPage.AddDynamic(this, &UUP_Raid_tower_boss::OnUpdateTowerBossUIPage);

	Init();
	SetBossList();
}

void UUP_Raid_tower_boss::NativeDestruct()
{
	Super::NativeDestruct();
// 	ATowerLobbyGameMode* CurrentMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
// 	if (CurrentMode)
// 	{
// 		AIndoorLobbyStateActor* LobbyStateActor = Cast<AIndoorLobbyStateActor>(CurrentMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
// 		if (LobbyStateActor)
// 			LobbyStateActor->DestroyCurrentLobbyCharacter();
// 	}
// 	auto TowerGameMode = RGAMEINSTANCE(this)->GetTowerLobbyGameMode();
// 	if (TowerGameMode)
// 	{
// 		auto IndoorActor = Cast<AIndoorLobbyStateActor>(TowerGameMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
// 		IndoorActor->LeaveSublevel();
// 	}
}

void UUP_Raid_tower_boss::Init()
{
	UC_party_keypad->SetVisibility(ESlateVisibility::Collapsed);

	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	SelectFloor = RGameInstance->CampaignManager->TowerStatus.currentFloor;

	int32	SingleStage		=	SelectFloor * 12;
	int32	MultiStage		=	SelectFloor * 13;
	FName SingleStageKey = FName();
	FName MultiStageKey = FName();
	FName SingleKey = FName("S");
	FName MultiKey = FName("M");
	FloorStageIDs = UTableManager::GetInstancePtr()->GetFloorStageRowNames();
	for (FName FloorStageID : FloorStageIDs)
	{
		FFloorStageDetail* StageDetail = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(FloorStageID);
		if (StageDetail->FloorInfoId != SelectFloor)
			continue;
		if (StageDetail->PlayType == SingleKey)
			SingleStageKey = FloorStageID;
		if (StageDetail->PlayType == MultiKey)
			MultiStageKey = FloorStageID;
	}
	
	SingleBossTable		= UTableManager::GetInstancePtr()->GetBossStage(SingleStageKey);
	SingleBossKey		= SingleStageKey;
	SingleFloorStage	= UTableManager::GetInstancePtr()->GetFloorStageDetailRow(SingleStageKey);

	MultiBossTable		= UTableManager::GetInstancePtr()->GetBossStage(MultiStageKey);
	MultiBossKey		= MultiStageKey;
	MultiFloorStage		= UTableManager::GetInstancePtr()->GetFloorStageDetailRow(MultiStageKey);

	
	
	checkf(SingleBossTable != nullptr, TEXT("UUP_Raid_tower_boss::Init[SingleBossTable Is Empty]"));
	checkf(MultiBossTable != nullptr, TEXT("UUP_Raid_tower_boss::Init[MultiBossTable Is Empty]"));
	checkf(SingleFloorStage != nullptr, TEXT("UUP_Raid_tower_boss::Init[SingleFloorStage Is Empty]"));
	checkf(MultiFloorStage != nullptr, TEXT("UUP_Raid_tower_boss::Init[MultiFloorStage Is Empty]"));
	checkf(RGameInstance != nullptr, TEXT("UUP_Raid_tower_boss::Init[RGameInstance Is Empty]"));
//	
	ATowerLobbyGameMode* CurrentMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (CurrentMode)
	{
		AIndoorLobbyStateActor* LobbyStateActor = Cast<AIndoorLobbyStateActor>(CurrentMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
		FFloorInfo* FloorInfo = UTableManager::GetInstancePtr()->GetFloorInfoRow(FName(*FString::FromInt(SelectFloor)));
		if (FloorInfo && LobbyStateActor != nullptr)
		{
			FCharacterTableInfo* ChracterTable = UTableManager::GetInstancePtr()->GetCharacterRow(FloorInfo->FloorBoss);
			if (ChracterTable)
			{
				LobbyStateActor->ChangeToHeroName(ChracterTable->DisplayName);
				UUtilFunctionLibrary::SetTowerBossCameraLocation(ChracterTable->DisplayName);
			}
		}
	}

	OnClickSingle_Monster();
	OnClickMulti_Monster();
	SetSingleText();
	SetMultiText();
// 	SetSingleScroll();
// 	SetMultiScroll();

// 	if (RGameInstance->ForceTutorialManager)
// 	{
// 		RGameInstance->ForceTutorialManager->CheckUnlockTutorial(EFirstCheckContentsType::FirstSingleRaid);
// 	}
}

void UUP_Raid_tower_boss::SetSingleText()
{
	// 싱글
	checkf(SingleBossTable != nullptr, TEXT("UUP_Raid_tower_boss::SetSingleText[SingleBossTable Is Empty]"));
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	FText SingleTimeText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Min"));
	FText Text = FText::Format(FText::FromString("{0}{1}"), FText::AsNumber(SingleBossTable->CommonPlayTime), SingleTimeText);
	if (IsValid(Text_SPersonnel))
		Text_SPersonnel->SetText(FText::FromName(SingleBossTable->CommonEntryCount));
	if (IsValid(Text_SLimitTime))
		Text_SLimitTime->SetText(Text);
	if (IsValid(Text_SReCommandedPower))
		Text_SReCommandedPower->SetText(FText::AsNumber(SingleBossTable->CommonRecommendPowerValue));
	if (IsValid(Text_SRemainPlayCount))
		Text_SRemainPlayCount->SetText(FText::AsNumber(RGameInstance->RInventory->GetCurrencyWithKey(SingleBossTable->UseCurrency)));
	if (IsValid(Text_SMaxPlayCount))
		Text_SMaxPlayCount->SetText(FText::FromName(SingleBossTable->SingleTrialMax));
	if (IsValid(Btn_Single_Monster))
	{
		Btn_Single_Monster->OnClicked.RemoveDynamic(this, &UUP_Raid_tower_boss::OnClickSingle_Monster);
		Btn_Single_Monster->OnClicked.AddDynamic(this, &UUP_Raid_tower_boss::OnClickSingle_Monster);
	}
	if (IsValid(Btn_Single_Item))
	{
		Btn_Single_Item->OnClicked.RemoveDynamic(this, &UUP_Raid_tower_boss::OnClickSingle_Item);
		Btn_Single_Item->OnClicked.AddDynamic(this, &UUP_Raid_tower_boss::OnClickSingle_Item);
	}
}

void UUP_Raid_tower_boss::SetMultiText()
{
	// 멀티
	checkf(MultiBossTable != nullptr, TEXT("UUP_Raid_tower_boss::SetMultiText[MultiBossTable Is Empty]"));

	FText MultiTimeText = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_UI, TEXT("UI_Min"));
	FText Text = FText::Format(FText::FromString("{0}{1}"), FText::AsNumber(MultiBossTable->CommonPlayTime), MultiTimeText);

	if (IsValid(Text_MPersonnel))
		Text_MPersonnel->SetText(FText::FromName(MultiBossTable->CommonEntryCount));
	if (IsValid(Text_MLimitTime))
		Text_MLimitTime->SetText(Text);
	if (IsValid(Text_MReCommandedPower))
		Text_MReCommandedPower->SetText(FText::AsNumber(MultiBossTable->CommonRecommendPowerValue));
	if (IsValid(Btn_Multi_Monster))
	{
		Btn_Multi_Monster->OnClicked.RemoveDynamic(this, &UUP_Raid_tower_boss::OnClickMulti_Monster);
		Btn_Multi_Monster->OnClicked.AddDynamic(this, &UUP_Raid_tower_boss::OnClickMulti_Monster);
	}
	if (IsValid(Btn_Multi_Item))
	{
		Btn_Multi_Item->OnClicked.RemoveDynamic(this, &UUP_Raid_tower_boss::OnClickMulti_Item);
		Btn_Multi_Item->OnClicked.AddDynamic(this, &UUP_Raid_tower_boss::OnClickMulti_Item);
	}
}

void UUP_Raid_tower_boss::SetSingleScroll()
{
	checkf(SingleFloorStage != nullptr, TEXT("UUP_Raid_tower_boss::SetSingleScroll[SingleFloorStage Is Empty]"));
	if (IsValid(Single_ScrollView_Item))
		Single_ScrollView_Item->SetVisibility(ESlateVisibility::Collapsed);
	if (IsValid(Single_ScrollView_Monster))
		Single_ScrollView_Monster->SetVisibility(ESlateVisibility::Collapsed);
	if (eTowerSingleScrollType == ETowerScrollType::VE_ITEMS)
	{
		if (IsValid(Single_ScrollView_Item))
		{
			Single_ScrollView_Item->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Single_ScrollView_Item->Init();
			Single_ScrollView_Item->ClearScrollItemData();
			FFloorStageDetail* StageInfo = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(SingleBossKey);
			if (StageInfo != nullptr)
			{
				TArray<FITEM_ICON_INFO> ItemIconArray;
				UUIFunctionLibrary::GetRewardsIconInfoByPreview(StageInfo->ClearRewardTableKey, ItemIconArray);
				if (ItemIconArray.Num() > 0)
				{
					for (size_t i = 0; i < ItemIconArray.Num(); ++i)
					{
						UItemBaseScrollItemData* ScrollItemData = Cast<UItemBaseScrollItemData>(Single_ScrollView_Item->AddScrollItemData(UItemBaseScrollItemData::StaticClass()));
						if (!IsValid(ScrollItemData))
							continue;
						ScrollItemData->ItemIconInfo = ItemIconArray[i];
					}
				}
			}
			Single_ScrollView_Item->InvalidateData();
		}
	}
	if (eTowerSingleScrollType == ETowerScrollType::VE_MONSTERS)
	{
		if (IsValid(Single_ScrollView_Monster))
		{
			Single_ScrollView_Monster->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Single_ScrollView_Monster->Init();
			Single_ScrollView_Monster->ClearScrollItemData();
			FFloorStageDetail* StageInfo = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(SingleBossKey);
			if (StageInfo != nullptr)
			{
				TArray<FName> MonsterArray = StageInfo->MonsterIDList;
				if (MonsterArray.Num() > 0)
				{
					for (size_t i = 0; i < MonsterArray.Num(); ++i)
					{
						UMonsterScrollItemData* ScrollItemData = Cast<UMonsterScrollItemData>(Single_ScrollView_Monster->AddScrollItemData(UMonsterScrollItemData::StaticClass()));
						FCharacterTableInfo* NPCInfo = UTableManager::GetInstancePtr()->GetCharacterNpcRow(MonsterArray[i]);
						if (!IsValid(ScrollItemData))
							continue;
						ScrollItemData->NPCID = MonsterArray[i];
						ScrollItemData->MonsterLevel = NPCInfo->Level;
						ScrollItemData->IsBossMonster = UUIFunctionLibrary::IsBossNpc(MonsterArray[i]);
					}
				}
			}
			Single_ScrollView_Monster->InvalidateData();
		}
	}
}

void UUP_Raid_tower_boss::SetMultiScroll()
{
	checkf(SingleFloorStage != nullptr, TEXT("UUP_Raid_tower_boss::SetSingleScroll[SingleFloorStage Is Empty]"));
	if (IsValid(Multi_ScrollView_Item))
		Multi_ScrollView_Item->SetVisibility(ESlateVisibility::Collapsed);
	if (IsValid(Multi_ScrollView_Monster))
		Multi_ScrollView_Monster->SetVisibility(ESlateVisibility::Collapsed);
	if (eTowerMultiScrollType == ETowerScrollType::VE_ITEMS)
	{
		if (IsValid(Multi_ScrollView_Item))
		{
			Multi_ScrollView_Item->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Multi_ScrollView_Item->Init();
			Multi_ScrollView_Item->ClearScrollItemData();
			FFloorStageDetail* StageInfo = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(MultiBossKey);
			if (StageInfo != nullptr)
			{
				TArray<FITEM_ICON_INFO> ItemIconArray;
				UUIFunctionLibrary::GetRewardsIconInfoByPreview(StageInfo->ClearRewardTableKey, ItemIconArray);
				if (ItemIconArray.Num() > 0)
				{
					for (size_t i = 0; i < ItemIconArray.Num(); ++i)
					{
						UItemBaseScrollItemData* ScrollItemData = Cast<UItemBaseScrollItemData>(Multi_ScrollView_Item->AddScrollItemData(UItemBaseScrollItemData::StaticClass()));
						if (!IsValid(ScrollItemData))
							continue;
						ScrollItemData->ItemIconInfo = ItemIconArray[i];
					}
				}
			}
			Multi_ScrollView_Item->InvalidateData();
		}
	}
	if (eTowerMultiScrollType == ETowerScrollType::VE_MONSTERS)
	{
		if (IsValid(Multi_ScrollView_Monster))
		{
			Multi_ScrollView_Monster->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			Multi_ScrollView_Monster->Init();
			Multi_ScrollView_Monster->ClearScrollItemData();
			FFloorStageDetail* StageInfo = UTableManager::GetInstancePtr()->GetFloorStageDetailRow(MultiBossKey);
			if (StageInfo != nullptr)
			{
				TArray<FName> MonsterArray = StageInfo->MonsterIDList;
				if (MonsterArray.Num() > 0)
				{
					for (size_t i = 0; i < MonsterArray.Num(); ++i)
					{
						UMonsterScrollItemData* ScrollItemData = Cast<UMonsterScrollItemData>(Multi_ScrollView_Monster->AddScrollItemData(UMonsterScrollItemData::StaticClass()));
						FCharacterTableInfo* NPCInfo = UTableManager::GetInstancePtr()->GetCharacterNpcRow(MonsterArray[i]);
						if (!IsValid(ScrollItemData))
							continue;
						ScrollItemData->NPCID = MonsterArray[i];
						ScrollItemData->MonsterLevel = NPCInfo->Level;
						ScrollItemData->IsBossMonster = UUIFunctionLibrary::IsBossNpc(MonsterArray[i]);
					}
				}
			}
			Multi_ScrollView_Monster->InvalidateData();
		}
	}
}

void UUP_Raid_tower_boss::SetBossList()
{
	
}

void UUP_Raid_tower_boss::OnClickSingle_Monster()
{
	eTowerSingleScrollType = ETowerScrollType::VE_MONSTERS;
	UWidgetSwitcher* MonsterSwiter = FindChildNameWidget<UWidgetSwitcher>(this, TEXT("monster_tab"));
	MonsterSwiter->SetActiveWidgetIndex(0);
	UWidgetSwitcher* ItemSwiter = FindChildNameWidget<UWidgetSwitcher>(this, TEXT("item_tab"));
	ItemSwiter->SetActiveWidgetIndex(1);
	SetSingleScroll();
}

void UUP_Raid_tower_boss::OnClickSingle_Item()
{
	eTowerSingleScrollType = ETowerScrollType::VE_ITEMS;
	UWidgetSwitcher* MonsterSwiter = FindChildNameWidget<UWidgetSwitcher>(this, TEXT("monster_tab"));
	MonsterSwiter->SetActiveWidgetIndex(1);
	UWidgetSwitcher* ItemSwiter = FindChildNameWidget<UWidgetSwitcher>(this, TEXT("item_tab"));
	ItemSwiter->SetActiveWidgetIndex(0);
	SetSingleScroll();
}

void UUP_Raid_tower_boss::OnClickMulti_Monster()
{
	eTowerMultiScrollType = ETowerScrollType::VE_MONSTERS;
	UWidgetSwitcher* MonsterSwiter = FindChildNameWidget<UWidgetSwitcher>(this, TEXT("multi_monster_tab"));
	MonsterSwiter->SetActiveWidgetIndex(0);
	UWidgetSwitcher* ItemSwiter = FindChildNameWidget<UWidgetSwitcher>(this, TEXT("multi_item_tab"));
	ItemSwiter->SetActiveWidgetIndex(1);
	SetMultiScroll();
}

void UUP_Raid_tower_boss::OnClickMulti_Item()
{
	eTowerMultiScrollType = ETowerScrollType::VE_ITEMS;
	UWidgetSwitcher* MonsterSwiter = FindChildNameWidget<UWidgetSwitcher>(this, TEXT("multi_monster_tab"));
	MonsterSwiter->SetActiveWidgetIndex(1);
	UWidgetSwitcher* ItemSwiter = FindChildNameWidget<UWidgetSwitcher>(this, TEXT("multi_item_tab"));
	ItemSwiter->SetActiveWidgetIndex(0);
	SetMultiScroll();
}

void UUP_Raid_tower_boss::OnClickSingleEntrance()
{
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	BaseStageInfo->Reset();
	BaseStageInfo->StageID = SingleBossKey;
	UPacketFunctionLibrary::TOWER_BOSS_PLAY_CHECK_RQ(SingleBossKey.ToString());
}

void UUP_Raid_tower_boss::OnClickMultiPartyCreate()
{
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	BaseStageInfo->Reset();
	BaseStageInfo->StageID = MultiBossKey;
	UPacketFunctionLibrary::TOWER_BOSS_PLAY_CHECK_RQ(MultiBossKey.ToString());
	//	멀티 파티 생성
}

void UUP_Raid_tower_boss::OnClickMultiPartyNumber()
{
	//	멀티 파티 넘버
	if (UC_party_keypad->GetVisibility() == ESlateVisibility::Collapsed)
	{
		UC_party_keypad->Init();
		UC_party_keypad->OnEnterPartyNumber.RemoveDynamic(this, &UUP_Raid_tower_boss::OnEnterPartyNumber);
		UC_party_keypad->OnEnterPartyNumber.AddDynamic(this, &UUP_Raid_tower_boss::OnEnterPartyNumber);
		UC_party_keypad->SetVisibility(ESlateVisibility::Visible);
	}
}

void UUP_Raid_tower_boss::OnClickMultiFastEntrance()
{
	//temp for stage id
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	BaseStageInfo->Reset();
	BaseStageInfo->StageID = MultiBossKey;
	RGAMEINSTANCE(this)->RealTimeModManager->JoinNumberString = TEXT("FastEnterance");

	UPacketFunctionLibrary::TOWER_BOSS_PLAY_CHECK_RQ(MultiBossKey.ToString());
	//	멀티 빠른 입장
	/*URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	URTSManager::GetInstancePtr()->REQ_MATCH(EPVP::MR, MultiBossKey.ToString(), false, false);*/
}

void UUP_Raid_tower_boss::OnClickMultiGuildParty()
{
	//	멀티 길드 파티
}

void UUP_Raid_tower_boss::OnEnterPartyNumber(FString PartyID)
{
	if (PartyID.IsEmpty())
		return;
	
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	RGameInstance->RealTimeModManager->JoinNumberString = PartyID;
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	BaseStageInfo->Reset();
	BaseStageInfo->StageID = MultiBossKey;
	UPacketFunctionLibrary::TOWER_BOSS_PLAY_CHECK_RQ(MultiBossKey.ToString());
}

void UUP_Raid_tower_boss::OnTowerBossRefersh(int32 changedFloor)
{
	//UPacketFunctionLibrary::TOWER_CHANGE_FLOOR_RQ(changedFloor);
}

void UUP_Raid_tower_boss::OnUpdateTowerBossUIPage()
{
	Init();
}

void UUC_Party_Keypad::NativeConstruct()
{
	Super::NativeConstruct();
	KeyButton0->OnClicked.RemoveDynamic(this, &UUC_Party_Keypad::OnClickedNumber0);
	KeyButton1->OnClicked.RemoveDynamic(this, &UUC_Party_Keypad::OnClickedNumber1);
	KeyButton2->OnClicked.RemoveDynamic(this, &UUC_Party_Keypad::OnClickedNumber2);
	KeyButton3->OnClicked.RemoveDynamic(this, &UUC_Party_Keypad::OnClickedNumber3);
	KeyButton4->OnClicked.RemoveDynamic(this, &UUC_Party_Keypad::OnClickedNumber4);
	KeyButton5->OnClicked.RemoveDynamic(this, &UUC_Party_Keypad::OnClickedNumber5);
	KeyButton6->OnClicked.RemoveDynamic(this, &UUC_Party_Keypad::OnClickedNumber6);
	KeyButton7->OnClicked.RemoveDynamic(this, &UUC_Party_Keypad::OnClickedNumber7);
	KeyButton8->OnClicked.RemoveDynamic(this, &UUC_Party_Keypad::OnClickedNumber8);
	KeyButton9->OnClicked.RemoveDynamic(this, &UUC_Party_Keypad::OnClickedNumber9);
	Button_Clear->OnClicked.RemoveDynamic(this, &UUC_Party_Keypad::Clear);
	Button_Cancel->OnClicked.RemoveDynamic(this, &UUC_Party_Keypad::OnClickedCancel);
	Button_OK->OnClicked.RemoveDynamic(this, &UUC_Party_Keypad::OnClickedOK);
	KeyButton0->OnClicked.AddDynamic(this, &UUC_Party_Keypad::OnClickedNumber0);
	KeyButton1->OnClicked.AddDynamic(this, &UUC_Party_Keypad::OnClickedNumber1);
	KeyButton2->OnClicked.AddDynamic(this, &UUC_Party_Keypad::OnClickedNumber2);
	KeyButton3->OnClicked.AddDynamic(this, &UUC_Party_Keypad::OnClickedNumber3);
	KeyButton4->OnClicked.AddDynamic(this, &UUC_Party_Keypad::OnClickedNumber4);
	KeyButton5->OnClicked.AddDynamic(this, &UUC_Party_Keypad::OnClickedNumber5);
	KeyButton6->OnClicked.AddDynamic(this, &UUC_Party_Keypad::OnClickedNumber6);
	KeyButton7->OnClicked.AddDynamic(this, &UUC_Party_Keypad::OnClickedNumber7);
	KeyButton8->OnClicked.AddDynamic(this, &UUC_Party_Keypad::OnClickedNumber8);
	KeyButton9->OnClicked.AddDynamic(this, &UUC_Party_Keypad::OnClickedNumber9);
	Button_Clear->OnClicked.AddDynamic(this, &UUC_Party_Keypad::Clear);
	Button_Cancel->OnClicked.AddDynamic(this, &UUC_Party_Keypad::OnClickedCancel);
	Button_OK->OnClicked.AddDynamic(this, &UUC_Party_Keypad::OnClickedOK);
}

void UUC_Party_Keypad::NativeDestruct()
{
	Super::NativeDestruct();
}

void UUC_Party_Keypad::Init()
{
	Clear();
}

void UUC_Party_Keypad::NumberPrinting()
{
	Party_Number_Label->SetText(FText::FromString(PartyNumberString));
}

void UUC_Party_Keypad::Clear()
{
	PartyNumberString = TEXT("");
	NumberPrinting();
}

void UUC_Party_Keypad::OnClickedOK()
{
	if (OnEnterPartyNumber.IsBound())
		OnEnterPartyNumber.Broadcast(PartyNumberString);
	SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_Party_Keypad::OnClickedCancel()
{
	SetVisibility(ESlateVisibility::Collapsed);
}

void UUC_Party_Keypad::OnClickedNumber0()
{
	if (PartyNumberString.Len() < NumberLen)
		PartyNumberString += TEXT("0");
	NumberPrinting();
}

void UUC_Party_Keypad::OnClickedNumber1()
{
	if (PartyNumberString.Len() < NumberLen)
		PartyNumberString += TEXT("1");
	NumberPrinting();
}
void UUC_Party_Keypad::OnClickedNumber2()
{
	if(PartyNumberString.Len() < NumberLen)
		PartyNumberString += TEXT("2");
	NumberPrinting();
}
void UUC_Party_Keypad::OnClickedNumber3()
{
	if (PartyNumberString.Len() < NumberLen)
		PartyNumberString += TEXT("3");
	NumberPrinting();
}
void UUC_Party_Keypad::OnClickedNumber4()
{
	if (PartyNumberString.Len() < NumberLen)
		PartyNumberString += TEXT("4");
	NumberPrinting();
}
void UUC_Party_Keypad::OnClickedNumber5()
{
	if (PartyNumberString.Len() < NumberLen)
		PartyNumberString += TEXT("5");
	NumberPrinting();
}
void UUC_Party_Keypad::OnClickedNumber6()
{
	if (PartyNumberString.Len() < NumberLen)
		PartyNumberString += TEXT("6");
	NumberPrinting();
}
void UUC_Party_Keypad::OnClickedNumber7()
{
	if (PartyNumberString.Len() < NumberLen)
		PartyNumberString += TEXT("7");
	NumberPrinting();
}
void UUC_Party_Keypad::OnClickedNumber8()
{
	if (PartyNumberString.Len() < NumberLen)
		PartyNumberString += TEXT("8");
	NumberPrinting();
}
void UUC_Party_Keypad::OnClickedNumber9()
{
	if (PartyNumberString.Len() < NumberLen)
		PartyNumberString += TEXT("9");
	NumberPrinting();
}