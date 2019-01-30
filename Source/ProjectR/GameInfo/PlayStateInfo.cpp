// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"
#include "PlayStateInfo.h"

#include "GlobalIntegrated.h"
#include "ContentIntergrated.h"
#include "UtilFunctionIntegrated.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/AvoidanceComponent.h"
#include "UI/UI_Battle/RCampaignHUD_ThreeControl.h"
#include "UI/UI_Battle/RCampaignHUD_RaidThreeControl.h"
#include "Network/RTSManager.h"
#include "Skill_V2/SharedSkill/ISkill_V2.h"

UPlayStateInfo::UPlayStateInfo()
{
	
}

void UPlayStateInfo::Init(/*URGameInstance* RGameInstance*/)
{
	ClearSelectedCharacter();
	ReadyToPlay();
}

void UPlayStateInfo::ReadyToPlay()
{
	SelectedSlotIndex = INVALID_INDEXNUM;
	bIsAllHeroDie = false;
}

TArray<FHeroSlotInfo>& UPlayStateInfo::GetSelectedCharacterArray()
{
	return SelectedCharacterArray;
}

FHeroSlotInfo& UPlayStateInfo::GetSelectedCharacterSlot(int32 SlotIndex)
{
	if (SelectedCharacterArray.IsValidIndex(SlotIndex))
	{
		return (SelectedCharacterArray[SlotIndex]);
	}
	
	static FHeroSlotInfo EmptyHeroSlotInfo;
	EmptyHeroSlotInfo.SlotSpawnState = EHeroSpawnStateEnum::VE_None;
	return EmptyHeroSlotInfo;
}

FHeroSlotInfo* UPlayStateInfo::GetSelectedCharacterSlotHeroUD(FString heroUD)
{
	for (int32 i = 0; i < SelectedCharacterArray.Num(); ++i)
	{
		if (SelectedCharacterArray[i].HeroUD == heroUD)
			return &SelectedCharacterArray[i];
	}

	return nullptr;
}

FHeroSlotInfo*	UPlayStateInfo::GetSelectedCharacterSlotCharacter(AActor* character)
{
	if (character)
	{
		for (int32 i = 0; i < SelectedCharacterArray.Num(); ++i)
		{
			if (SelectedCharacterArray[i].SpawnedTargetActor == character)
				return &SelectedCharacterArray[i];
		}
	}

	return nullptr;
}

int32 UPlayStateInfo::GetSelectedCharacterSlotIndex(AActor* character)
{
	if (character)
	{
		for (int32 i = 0; i < SelectedCharacterArray.Num(); ++i)
		{
			if (SelectedCharacterArray[i].SpawnedTargetActor == character)
				return i;
		}
	}

	return INVALID_CREW_INDEX;
}

AActor* UPlayStateInfo::GetNextCombineSkillOwnerActor(AActor* InActor)
{
	int32 nIndex = GetSelectedCharacterSlotIndex(InActor);
	ECharacterRoleEnum eRole = UGamePlayFunctionLibrary::IndexToRoleEnum(nIndex);
	switch (eRole)
	{
	case ECharacterRoleEnum::VE_Leader:
		return GetBattleActiveCrewActor(ECharacterRoleEnum::VE_Crew1);
	case ECharacterRoleEnum::VE_Crew1:
		return GetBattleActiveCrewActor(ECharacterRoleEnum::VE_Crew2);

 	default:
		break;
	}

	return nullptr;
}

AActor* UPlayStateInfo::GetBattleActiveCrewActor(ECharacterRoleEnum InRole)
{
	for (int i = 0; i < SelectedCharacterArray.Num(); ++i)
	{
		FHeroSlotInfo& slot = SelectedCharacterArray[i];
		if (slot.CharacterRole == InRole)
		{
			if (slot.CharacterCrewState == ECharacterCrewStateEnum::VE_Battle && slot.SlotSpawnState != EHeroSpawnStateEnum::VE_Dead)
				return slot.SpawnedTargetActor;
		}
	}

	return nullptr;
}

AActor* UPlayStateInfo::GetRestActiveCrewActor(ECharacterRoleEnum InRole)
{
	for (int i = 0; i < SelectedCharacterArray.Num(); ++i)
	{
		FHeroSlotInfo& slot = SelectedCharacterArray[i];
		if (slot.CharacterRole == InRole)
		{
			if (slot.CharacterCrewState == ECharacterCrewStateEnum::VE_Rest && slot.SlotSpawnState != EHeroSpawnStateEnum::VE_Dead)
				return slot.SpawnedTargetActor;
		}
	}

	return nullptr;
}

FHeroSlotInfo* UPlayStateInfo::GetBattleActiveCrewSlot(ECharacterRoleEnum InRole)
{
	for (int i = 0; i < SelectedCharacterArray.Num(); ++i)
	{
		FHeroSlotInfo& slot = SelectedCharacterArray[i];
		if (slot.CharacterRole == InRole)
		{
			if (slot.CharacterCrewState == ECharacterCrewStateEnum::VE_Battle && slot.SlotSpawnState != EHeroSpawnStateEnum::VE_Dead)
				return &SelectedCharacterArray[i];
		}
	}

	return nullptr;
}

FHeroSlotInfo* UPlayStateInfo::GetRestActiveCrewSlot(ECharacterRoleEnum InRole)
{
	for (int i = 0; i < SelectedCharacterArray.Num(); ++i)
	{
		FHeroSlotInfo& slot = SelectedCharacterArray[i];
		if (slot.CharacterRole == InRole)
		{
			if (slot.CharacterCrewState == ECharacterCrewStateEnum::VE_Rest && slot.SlotSpawnState != EHeroSpawnStateEnum::VE_Dead)
				return &SelectedCharacterArray[i];
		}
	}

	return nullptr;
}

FHeroSlotInfo* UPlayStateInfo::GetBattleSlot(ECharacterRoleEnum InRole)
{
	for (int i = 0; i < SelectedCharacterArray.Num(); ++i)
	{
		FHeroSlotInfo& slot = SelectedCharacterArray[i];
		if (slot.CharacterRole == InRole)
		{
			if (slot.CharacterCrewState == ECharacterCrewStateEnum::VE_Battle)
				return &SelectedCharacterArray[i];
		}
	}

	return nullptr;
}

FHeroSlotInfo* UPlayStateInfo::GetRestSlot(ECharacterRoleEnum InRole)
{
	for (int i = 0; i < SelectedCharacterArray.Num(); ++i)
	{
		FHeroSlotInfo& slot = SelectedCharacterArray[i];
		if (slot.CharacterRole == InRole)
		{
			if (slot.CharacterCrewState == ECharacterCrewStateEnum::VE_Rest)
				return &SelectedCharacterArray[i];
		}
	}

	return nullptr;
}

int32 UPlayStateInfo::GetBattleActiveCrewIdx(ECharacterRoleEnum InRole)
{
	for (int i = 0; i < SelectedCharacterArray.Num(); ++i)
	{
		FHeroSlotInfo& slot = SelectedCharacterArray[i];
		if (slot.CharacterRole == InRole)
		{
			if (slot.CharacterCrewState == ECharacterCrewStateEnum::VE_Battle && slot.SlotSpawnState != EHeroSpawnStateEnum::VE_Dead)
				return i;
		}
	}

	return INDEX_NONE;
}

int32 UPlayStateInfo::GetRestActiveCrewIdx(ECharacterRoleEnum InRole)
{
	for (int i = 0; i < SelectedCharacterArray.Num(); ++i)
	{
		FHeroSlotInfo& slot = SelectedCharacterArray[i];
		if (slot.CharacterRole == InRole)
		{
			if (slot.CharacterCrewState == ECharacterCrewStateEnum::VE_Rest && slot.SlotSpawnState != EHeroSpawnStateEnum::VE_Dead)
				return i;
		}
	}

	return INDEX_NONE;
}

FHeroSlotInfo* UPlayStateInfo::GetCurrentSelectedCharacterSlotCharacter()
{
	if (SelectedSlotIndex < 0)
		return nullptr;

	if (SelectedCharacterArray.Num() > SelectedSlotIndex)
	{
		return &SelectedCharacterArray[SelectedSlotIndex];
	}

	return nullptr;
}

AActor* UPlayStateInfo::GetCurrentSelectedCharacterActor()
{
	if (SelectedSlotIndex < 0)
		return nullptr;

	return GetOwnerCharacter(SelectedSlotIndex);
}

EHeroSpawnStateEnum	UPlayStateInfo::GetSpawnState(int32 SlotIndex)
{
	if (SelectedCharacterArray.IsValidIndex(SlotIndex))
	{
		return SelectedCharacterArray[SlotIndex].SlotSpawnState;
	}

	return EHeroSpawnStateEnum::VE_Ready;
}

void UPlayStateInfo::SetSpawnState(int32 SlotIndex, EHeroSpawnStateEnum SpawnState)
{
	if (SelectedCharacterArray.IsValidIndex(SlotIndex) == false)
		return;

	SelectedCharacterArray[SlotIndex].SetSpawnState( SpawnState );
}

void UPlayStateInfo::SetSpawnedCharacter(int32 SlotIndex, AActor* SpawnedCharacter)
{

	if (SelectedCharacterArray.IsValidIndex(SlotIndex))
	{
		URGameInstance* GameInstance = RGAMEINSTANCE(this);
		EHeroSpawnStateEnum beforeSpawnState = SelectedCharacterArray[SlotIndex].SlotSpawnState;

		SelectedCharacterArray[SlotIndex].SetSpawnedCharacter(SpawnedCharacter);

		UAvoidanceComponent* AvoidanceComponent = UCharacterHelpFunctionLibrary::GetComponent<UAvoidanceComponent>(SpawnedCharacter);
		if (IsValid(AvoidanceComponent))
		{
			AvoidanceComponent->SetEnableStopSearch(true);
		}
	}
}

void UPlayStateInfo::SetDeSpawnedCharacter(int32 SlotIndex)
{
	if (SelectedCharacterArray.IsValidIndex(SlotIndex) == false)
	{
		SelectedCharacterArray.RemoveAt(SlotIndex);
	}
}

AActor*	UPlayStateInfo::GetOwnerCharacter(int32 SlotIndex)
{
	if (SelectedCharacterArray.IsValidIndex(SlotIndex) && SelectedCharacterArray[SlotIndex].CharacterCategoryType == ECharacterCategoryTypeEnum::VE_Hero )
	{
		return SelectedCharacterArray[SlotIndex].SpawnedTargetActor;
	}

	return nullptr;
}

bool UPlayStateInfo::IsLeaderDead()
{
	FHeroSlotInfo* pLeaderSlot = GetCurrentSelectedCharacterSlotCharacter();
	if (pLeaderSlot)
	{
		if (pLeaderSlot->SlotSpawnState == EHeroSpawnStateEnum::VE_Dead)
		{
			bIsAllHeroDie = true;
			return true;
		}
	}

	return false;
}

int32 UPlayStateInfo::GetHeroDieCount()
{
	int32 DieCount = 0;
	for (auto HeroSlotItor = SelectedCharacterArray.CreateIterator(); HeroSlotItor; ++HeroSlotItor)
	{
		auto& SpawnedTargetActorCategoryType = HeroSlotItor->CharacterCategoryType;
		auto& SlotSpawnState = HeroSlotItor->SlotSpawnState;

		if (SpawnedTargetActorCategoryType != ECharacterCategoryTypeEnum::VE_Hero)
		{
			continue;
		}

		if (SlotSpawnState == EHeroSpawnStateEnum::VE_Dead)
		{
			++DieCount;
		}
	}

	return DieCount;
}

int32 UPlayStateInfo::GetHeroSpawnCount()
{
	int32 SpawnCount = 0;

	for (auto HeroSlotItor = SelectedCharacterArray.CreateIterator(); HeroSlotItor; ++HeroSlotItor)
	{
		auto& SpawnedTargetActorCategoryType = HeroSlotItor->CharacterCategoryType;
		auto& SlotSpawnState = HeroSlotItor->SlotSpawnState;

		if (SpawnedTargetActorCategoryType != ECharacterCategoryTypeEnum::VE_Hero)
		{
			continue;
		}

		if (SlotSpawnState != EHeroSpawnStateEnum::VE_Ready && SlotSpawnState != EHeroSpawnStateEnum::VE_None)
		{
			++SpawnCount;
		}
	}

	return SpawnCount;
}

int32 UPlayStateInfo::GetSelectedCharacterCount()
{
	return SelectedCharacterArray.Num();
}

int32 UPlayStateInfo::GetHeroRemainSpawnCount()
{
	int32 SpawnCount = 0;

	for (auto HeroSlotItor = SelectedCharacterArray.CreateIterator(); HeroSlotItor; ++HeroSlotItor)
	{
		auto& SpawnedTargetActorCategoryType = HeroSlotItor->CharacterCategoryType;
		auto& SlotSpawnState = HeroSlotItor->SlotSpawnState;

		if (SpawnedTargetActorCategoryType != ECharacterCategoryTypeEnum::VE_Hero)
		{
			continue;
		}

		if (SlotSpawnState ==  EHeroSpawnStateEnum::VE_Ready)
		{
			++SpawnCount;
		}
	}

	return SpawnCount;
}


void UPlayStateInfo::DespawnAll()
{
	int32 SlotIndex = 0;
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	for (auto HeroSlotItor = SelectedCharacterArray.CreateIterator(); HeroSlotItor; ++HeroSlotItor )
	{
		FHeroSlotInfo& SlotInfo = *HeroSlotItor;		
		SlotInfo.DespawnAll();
//		GameInstance->EventManager->OnUpdateCommandSlot.Broadcast(SlotIndex++);
  	}
}

ESpawnResultTypeEnum UPlayStateInfo::CanSpawnHero(int32 SlotIndex)
{	
	if(SelectedCharacterArray.IsValidIndex(SlotIndex) == false)
		return ESpawnResultTypeEnum::VE_Fail;

	FHeroSlotInfo Slot = GetSelectedCharacterSlot(SlotIndex);
	
	return Slot.CanSpawn();
}


/*
TODO: TestPlay
*/
void UPlayStateInfo::InitCharacterSlotForTestPlay(TArray<FName> SelectedCharacterRows, TArray<uint8> selectArr, TArray<int32> InHeroLevels, TArray<int32> InHeroGrades)
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	
	ClearSelectedCharacter(true);
	
	for (auto Itor = SelectedCharacterRows.CreateConstIterator(); Itor; ++Itor)
	{
		FName CharacterRowID = *Itor;
		FHeroSlotInfo CharacterSlot;
		FCharacterTableInfo* CharacterTableInfo = nullptr;

		if (selectArr[Itor.GetIndex()] == static_cast<uint8>(ECharacterCategoryTypeEnum::VE_Hero))
		{
			CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(CharacterRowID);
			CharacterSlot.CharacterCategoryType = ECharacterCategoryTypeEnum::VE_Hero;
		}
		else if ((selectArr[Itor.GetIndex()] == static_cast<uint8>(ECharacterCategoryTypeEnum::VE_Minion)) ||
			(selectArr[Itor.GetIndex()] == static_cast<uint8>(ECharacterCategoryTypeEnum::VE_Npc)) ||
			(selectArr[Itor.GetIndex()] == static_cast<uint8>(ECharacterCategoryTypeEnum::VE_BattleObject)))
		{
			CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterNpcRow(CharacterRowID);
			CharacterSlot.CharacterCategoryType = ECharacterCategoryTypeEnum::VE_Npc;
		}

		if (CharacterTableInfo == nullptr)
		{
			continue;
		}

		FString CharacterKey = Itor->ToString();
		
		FHERO* HeroInvenData = GameInstance->RInventory->GetHeroWithUD(CharacterKey);
		if (HeroInvenData != nullptr)
		{
			CharacterSlot.HeroUD = HeroInvenData->heroUD;
		}
		else
		{
			CharacterSlot.HeroUD = CharacterRowID.ToString();
		}
				
		int32 Index = Itor.GetIndex();		
		CharacterSlot.HeroLevel			= InHeroLevels[Index];
		CharacterSlot.HeroGrade			= InHeroGrades[Index];
		CharacterSlot.PortraitTextureKey = CharacterTableInfo->PortraitTextureKey;
		CharacterSlot.HeroIDName		= CharacterRowID;
		CharacterSlot.SlotSpawnState	= EHeroSpawnStateEnum::VE_Ready;
// 		CharacterSlot.MaxSpawnCount		= CharacterTableInfo->SpawnCount;
// 		CharacterSlot.SpawnCount		= CharacterSlot.MaxSpawnCount;
		CharacterSlot.CharacterClassType = CharacterTableInfo->Class;
		SelectedCharacterArray.Emplace(CharacterSlot);
		
// 		MaxSpawnCount += CharacterSlot.SpawnCount;
	}
}

void UPlayStateInfo::AddCharacterSlotForTestPlay(FName characterid, int32 level, TArray<int32>& InSkillLevelList, TArray<FTestItemInfo>& InItemList , ECharacterRoleEnum role)
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	FName CharacterRowID = characterid;
	FString CharacterKey = CharacterRowID.ToString();
	FHeroSlotInfo CharacterSlot;
	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(CharacterRowID);

	if (CharacterTableInfo)
	{
		CharacterSlot.CharacterCategoryType = ECharacterCategoryTypeEnum::VE_Hero;

		FHERO* HeroInvenData = GameInstance->RInventory->GetHeroWithID(CharacterRowID);
		if (HeroInvenData != nullptr)
		{
			CharacterSlot.HeroUD = HeroInvenData->heroUD;
		}
		else
		{
			CharacterSlot.HeroUD = CharacterRowID.ToString();
		}

		CharacterSlot.HeroLevel = level;
		CharacterSlot.HeroGrade = 1;
		CharacterSlot.PortraitTextureKey = CharacterTableInfo->PortraitTextureKey;
		CharacterSlot.HeroIDName = CharacterRowID;
		CharacterSlot.SlotSpawnState = EHeroSpawnStateEnum::VE_Ready;
// 		CharacterSlot.MaxSpawnCount = CharacterTableInfo->SpawnCount;
// 		CharacterSlot.SpawnCount = CharacterSlot.MaxSpawnCount;
		CharacterSlot.CharacterClassType = CharacterTableInfo->Class;
		CharacterSlot.TestSkillLevelList = InSkillLevelList;
		CharacterSlot.TestItemList = InItemList;
		CharacterSlot.CharacterRole = role;
		SelectedCharacterArray.Emplace(CharacterSlot);

// 		MaxSpawnCount += CharacterSlot.SpawnCount;
	}
}
 
void UPlayStateInfo::AddSelectedCharacter(FHeroSlotInfo* HeroSlot, bool InsertFront)
{
	//if (HeroSlot->HeroIDName != EXCEPTION_NONE)
	{
		if (InsertFront)
		{
			SelectedCharacterArray.Insert(*HeroSlot, 0);
		}
		else
		{
			SelectedCharacterArray.Emplace(*HeroSlot);
		}
	}
}

void UPlayStateInfo::ClearSelectedCharacter(bool InIsDestory /*= false*/)
{
	if(InIsDestory)
	{
		for (auto Element : SelectedCharacterArray)
		{
			if (Element.SpawnedTargetActor)
			{
				Element.SpawnedTargetActor->K2_DestroyActor();
			}
		}
	}
	SelectedCharacterArray.Empty();
	SelectedSlotIndex = INVALID_INDEXNUM;
}

bool UPlayStateInfo::IsPlayHero(AActor* character)
{
	for (int32 i = 0; i < SelectedCharacterArray.Num(); ++i)
	{
		FHeroSlotInfo info = SelectedCharacterArray[i];
		if (IsValid(info.SpawnedTargetActor) && info.SpawnedTargetActor == character)
		{
			return true;
		}
	}

	return false;
}

bool UPlayStateInfo::SelectSlot(int32 SlotIndex)
{	
	SelectedSlotIndex = SlotIndex;
	return true;
}
//////////////////////////////////////////////////////////////////////////

void UPlayStateInfo::ActivateSlotSpawnedHeroRVO(bool bIsActive)
{
	//UE_LOG(LogBattle, Log, TEXT("UPlayStateInfo::ActivateSlotSpawnedHeroRVO Start"));
	for (auto& HeroSlotInfo : SelectedCharacterArray)
	{
		auto SpawnedTargetActor = HeroSlotInfo.SpawnedTargetActor;
		if (IsValid(HeroSlotInfo.SpawnedTargetActor) == false)
			continue;

		auto CastCharacter_SpawnedCharacter = Cast<ACharacter>(SpawnedTargetActor);
		if (IsValid(CastCharacter_SpawnedCharacter) == false)
			return;

		auto CharacterMovementComp = Cast<UCharacterMovementComponent>(CastCharacter_SpawnedCharacter->GetMovementComponent());
		if (IsValid(CharacterMovementComp) == false)
			return;

		CharacterMovementComp->SetRVOAvoidanceWeight(0.0f);
		CharacterMovementComp->SetAvoidanceEnabled(bIsActive);			
	}
}



void UPlayStateInfo::SpawnCharacters(FTransform SpawnTransform)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	checkf(RGameInstance != nullptr, TEXT("[%s]URGameInstance is Empty"), __FUNCTION__);
	checkf(RGameInstance->EventManager != nullptr, TEXT("[%s]RGameInstance->EventManager is Empty"), __FUNCTION__);
	UWorld*			GameWorld = GEngine->GetWorldFromContextObject(RGameInstance, EGetWorldErrorMode::Assert);
	checkf(GameWorld != nullptr, TEXT("[%s]UWorld is Empty"), __FUNCTION__);
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();

	AStagePlayerController*	BasePlayerController = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(GameWorld));
	if (IsValid(BasePlayerController) == false)
		return;

	FTransform SelectedCharacterTransform = SpawnTransform;
	AActor* SpawnedCharacter = nullptr;
	for (int32 i = 0; i < COMMANDSLOT_MAX; i++)
	{
		if (BaseStageInfo->bPlayAlone && i != ROLE_LEADER_INDEX)
			break;

		ESpawnResultTypeEnum CanSpawnHeroResult = CanSpawnHero(i);
		// Spawn new hero
		if (CanSpawnHeroResult == ESpawnResultTypeEnum::VE_Success)
		{
			FHeroSlotInfo& slot = GetSelectedCharacterSlot(i);
			FCharacterTableInfo* characterinfo = UTableManager::GetInstancePtr()->GetCharacterRow(slot.HeroIDName);
			checkf(characterinfo != nullptr, TEXT("[%s]characterinfo is Empty"), __FUNCTION__);
			FCrewSpotTableInfo* characterspotInfo = UTableManager::GetInstancePtr()->GetCrewSpotTableRow(characterinfo->CrewSpot);
			checkf(characterspotInfo != nullptr, TEXT("[%s]characterspotInfo is Empty"), __FUNCTION__);
			FVector2D spot2d;
			spot2d.Set(0, 0);
			if (slot.CharacterRole == ECharacterRoleEnum::VE_Crew1)
				spot2d = characterspotInfo->Crew1Spot;
			else if (slot.CharacterRole == ECharacterRoleEnum::VE_Crew2)
				spot2d = characterspotInfo->Crew2Spot;

			FTransform TargetTransform = SelectedCharacterTransform;
			FSpawnerTransformInfo Element;

			Element.Position.Set(spot2d.X, spot2d.Y, 0);
			Element.Rotation = TargetTransform.Rotator();

			FVector SpawnPos = Element.Position;
			SpawnPos = SpawnPos.RotateAngleAxis(TargetTransform.Rotator().Yaw, FVector::UpVector);
			TargetTransform.AddToTranslation(SpawnPos);

			FRotator TargetRot = TargetTransform.Rotator();
			//TargetRot += Element.Rotation;
			TargetTransform.SetRotation(TargetRot.Quaternion());

			FVector TargetLocation = TargetTransform.GetLocation();

			UMapFunctionLibrary::GetAdjacentPosNearestWithTargetPositionOnNavyMesh(Element.Position, TargetLocation, 1, 5);

			SpawnedCharacter = BasePlayerController->SpawnHeroAtLocation(i, TargetLocation, TargetRot);
		}
	}

	URBaseIngameHUD* OneGameHUD = Cast<URBaseIngameHUD>(UUIFunctionLibrary::GetInGameHUD());
	if (OneGameHUD)
		RGameInstance->EventManager->OnRefreshAutoSkillIcon.Broadcast();
}

/*
Make HeroSlotInfo
*/
void UPlayStateInfo::MakeHeroSlot(TArray<FString>& HeroUDList)
{
	ClearSelectedCharacter();
	for (int32 nIndex = 0; nIndex < HeroUDList.Num(); ++nIndex) {
		AddHeroSlotInfo(HeroUDList[nIndex], nIndex);
	}
}
bool UPlayStateInfo::AddHeroSlotInfo(FString& HeroUD, int32 index)
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);

	if (HeroUD.IsEmpty())
	{
		FHeroSlotInfo CommandSlot;
		SelectedCharacterArray.Emplace(CommandSlot);
		return true;
	}

	int32 MyKID = GameInstance->HttpClient->GetKID();

	FHeroSlotInfo* SlotInfo = GetSelectedCharacterSlotHeroUD(HeroUD);
	if (nullptr == SlotInfo)
	{
		FHERO* HeroInvenData = GameInstance->RInventory->GetHeroWithUD(HeroUD);
		if (HeroInvenData)
		{
			FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroInvenData->heroId);
			if (CharacterTableInfo)
			{
				FHeroSlotInfo CommandSlot;
				CommandSlot.UserKID = MyKID;
				CommandSlot.HeroUD = HeroInvenData->heroUD;
				CommandSlot.HeroLevel = HeroInvenData->level;
//				CommandSlot.HeroGrade = HeroInvenData->grade;
				CommandSlot.HeroIDName = HeroInvenData->heroId;
				CommandSlot.HeroExp = HeroInvenData->exp;

				CommandSlot.CharacterCategoryType = ECharacterCategoryTypeEnum::VE_Hero;
				CommandSlot.SlotSpawnState = EHeroSpawnStateEnum::VE_Ready;

				CommandSlot.PortraitTextureKey = CharacterTableInfo->PortraitTextureKey;
				CommandSlot.CharacterClassType = CharacterTableInfo->Class;
// 				CommandSlot.MaxSpawnCount = CharacterTableInfo->SpawnCount;
// 				CommandSlot.SpawnCount = CommandSlot.MaxSpawnCount;
				CommandSlot.CharacterCrewState = UGamePlayFunctionLibrary::IndexToCrewState(index);
				CommandSlot.CharacterRole = UGamePlayFunctionLibrary::IndexToRoleEnum(index);
				AddSelectedCharacter(&CommandSlot);
				return true;
			}
		}
	}

	return false;
}

void UPlayStateInfo::MakeRealTimeHeroSlot()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);
	if(!URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::MR))
		ClearSelectedCharacter();

	int32 MyKID = GameInstance->HttpClient->GetKID();
	

	TArray<int32> MyTeamUKList;
	for (auto& Element : GameInstance->RealTimeModManager->ActorList)
	{
		if (Element.Value.team == UObjectFunctionLibrary::GetMyTeamKey())
			MyTeamUKList.Emplace(Element.Value.actorUK);
	}
	
	int32 UKidx = 0;
	for (int32 i = 0; i < 5; i++)
	{
		FHeroSlotInfo CommandSlot;
		
		int32 actorUK = MyTeamUKList.IsValidIndex(UKidx)? MyTeamUKList[UKidx] : INDEX_NONE;
		FRTS_ACTOR* fActor = GameInstance->RealTimeModManager->Get_RTS_Actor(actorUK);
		if (fActor == nullptr || i != fActor->slot - 1)
		{
			if (!URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::MR))
				AddSelectedCharacter(&CommandSlot);
			continue;
		}
		UKidx++;

		CommandSlot.UserKID = fActor->kId;
		CommandSlot.HeroUD = fActor->heroUD;
		CommandSlot.PlayerUK = fActor->actorUK;
		CommandSlot.HeroLevel = fActor->level;
		CommandSlot.HeroGrade = fActor->grade;
		CommandSlot.HeroIDName = fActor->actorId;

		CommandSlot.CharacterCategoryType = ECharacterCategoryTypeEnum::VE_Hero;
		CommandSlot.SlotSpawnState = EHeroSpawnStateEnum::VE_Ready;
		CommandSlot.CharacterRole = UGamePlayFunctionLibrary::IndexToRoleEnum(fActor->slot - 1);
		CommandSlot.CharacterCrewState = UGamePlayFunctionLibrary::IndexToCrewState(fActor->slot - 1);
		FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(CommandSlot.HeroIDName);
		if (CharacterTableInfo)
		{
			CommandSlot.PortraitTextureKey = CharacterTableInfo->PortraitTextureKey;
			CommandSlot.CharacterClassType = CharacterTableInfo->Class;
		}
		AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(GameInstance));
		if (pGameMode)
		{
			TWeakObjectPtr<AActor> ObjectActor = pGameMode->ObjectHelper.GetObjectByPlayerUK(CommandSlot.PlayerUK);
			if (ObjectActor.IsValid())
			{
				CommandSlot.SpawnedTargetActor = ObjectActor.Get();
			}
		}

		bool InsertFront = false;
		if (MyKID == CommandSlot.UserKID && CommandSlot.CharacterRole == ECharacterRoleEnum::VE_Leader)
		{
			InsertFront = true;
			SelectedSlotIndex = 0;

		}
		if (!URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::MR))
			AddSelectedCharacter(&CommandSlot, InsertFront);
	}
}