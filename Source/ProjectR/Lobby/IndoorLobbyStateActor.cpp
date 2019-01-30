#include "ProjectR.h"
#include "IndoorLobbyStateActor.h"

#include "GlobalIntegrated.h"
#include "GameInfo/RInventory.h"
#include "Network/PacketDataStructures.h"
#include "Network/HttpRequestClient.h"
#include "Character/LobbyCharacter.h"

#include "Character/AnimInstance/AnimInstanceDefs.h"
#include "Character/BattleBaseAnimInstance.h"
#include "Character/AnimInstance/LobbyAnimInstance.h"

#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"

#include "Table/LobbyCharacterTableInfo.h"
#include "Table/IndoorHeroManagementViewTableInfo.h"
#include "Table/ResourceSequenceTableInfo.h"
#include "Character/BattleBaseCharacter.h"
#include "GameFramework/Character.h"
#include "UtilFunctionIntegrated.h"
#include "SaveObject/RSaveGame.h"


void AIndoorLobbyStateActor::EnterSublevel()
{
	Super::EnterSublevel();

	UMapFunctionLibrary::LoadSubLevel(FName("Lobby_In"), FName("LoadLobbyInLevelCompleted"), this);
}

void AIndoorLobbyStateActor::LeaveSublevel()
{
	Super::LeaveSublevel();

	DestroyCurrentLobbyCharacter();

	// Loose Tower actor
	UMapFunctionLibrary::UnLoadSubLevel(FName("Lobby_In"), FName("OnLeaveSublevelComplete"), this );
}

void AIndoorLobbyStateActor::BeginPlay()
{
	Super::BeginPlay();

	URSaveGame* SaveGame = RGAMEINSTANCE(this)->SaveGame;
	if (IsValid(SaveGame))
	{
		//SaveGame = Cast<URSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SAVEDATA"), 0));
		if (IsValid(SaveGame))
			CurrentHeroUD = SaveGame->LastPlayedHeroUD;
	}
}

void AIndoorLobbyStateActor::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	URSaveGame* SaveGame = RGAMEINSTANCE(this)->SaveGame;
	if (IsValid(SaveGame))
	{
		SaveGame->LastPlayedHeroUD = CurrentHeroUD;
		SaveGame->Save();
		//UGameplayStatics::SaveGameToSlot(SaveGame, TEXT("SAVEDATA"), 0);
	}
	
	Super::EndPlay(EndPlayReason);
}

void AIndoorLobbyStateActor::BeginSublevel()
{
	Super::BeginSublevel();

	// This superfluous looking thing is added to patch the issue where the hero character has incorrect, darker lighting upon state transition on mobile view,
	// but is corrected when the actor is moved.
	// So we give the actor a jerk here just to fix the lighting.
	// Probably lighting invalidation issue related to Level Streaming.
	if (IsValid(CurrentLobbyCharacter))
	{
		RotateHero(1.0f);
		RotateHero(-1.0f);
	}

	GoToNormalView();
	//PlayHeroTouchActionAnimationAndSequence();
}

void AIndoorLobbyStateActor::EndSublevel()
{
	RGAMEINSTANCE(this)->SequenceManager->ClearSequence();
	ClearCharacterLoadRequest();

	Super::EndSublevel();
}

void AIndoorLobbyStateActor::LoadLobbyInLevelCompleted()
{
	/*Decide which Hero will be shown*/
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	TArray<FString> HeroUDs;
	RGameInstance->RInventory->GetHeroUDArray(HeroUDs);

	for (size_t i = 0; i < HeroUDs.Num(); )
	{
		FHERO* hero = RGameInstance->RInventory->GetHeroWithUD(HeroUDs[i]);
		if (hero)
		{
			FCharacterTableInfo* tableinfo = UTableManager::GetInstancePtr()->GetCharacterRow(hero->heroId);
			if (tableinfo && !tableinfo->IsHero)
			{
				HeroUDs.RemoveAt(i);
				continue;
			}
		}
		++i;
	}

	if (CurrentHeroUD.IsEmpty())
	{
		if (HeroUDs.IsValidIndex(0))
			CurrentHeroUD = HeroUDs[0];
	}
	else
	{
		int32 FoundIdx = HeroUDs.Find(CurrentHeroUD);
		if (FoundIdx == INDEX_NONE)
		{
			if (HeroUDs.IsValidIndex(0))
				CurrentHeroUD = HeroUDs[0];
		}
	}

	SetLobbyHeroInfo(CurrentHeroUD);
	ChangeHero_Owned(CurrentHeroUD);

	OnEnterSublevelComplete.Broadcast();
}


void AIndoorLobbyStateActor::SetCurrentLobbyHeroInfo()
{
	SetLobbyHeroInfo(CurrentHeroUD);
}

void AIndoorLobbyStateActor::SetLobbyHeroInfo(FString& InHeroUD)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	FHERO* pHero = RGameInstance->RInventory->GetHeroWithUD(InHeroUD);
	if (pHero)
	{
		FCharacterTableInfo* pCharacter = UTableManager::GetInstancePtr()->GetCharacterRow(pHero->heroId);
		if (pCharacter)
		{
			heroName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, pCharacter->DisplayName);
// 			heroNickName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, pCharacter->Nick);
			heroSprite = UResourceFunctionLibrary::GetHeroClassTypeSprite(pCharacter->Class);
			heroGrade = pCharacter->Grade;
		}
	}
}

void AIndoorLobbyStateActor::ChangeState(ETowerLobbySubstateEnum InSubstate)
{
	check(InSubstate != ETowerLobbySubstateEnum::None);

	if (InSubstate == CurrentSubstate)
	{
		OnStateChanged.Broadcast();
		return;
	}

	CurrentSubstate = InSubstate;
	switch (CurrentSubstate)
	{
	case ETowerLobbySubstateEnum::NormalView:
		// request from art
		//ChangeToRandomHero();
		SetCurrentLobbyHeroInfo();
		GoToNormalView();
		break;
	case ETowerLobbySubstateEnum::HeroManagementView:
		GoToHeroInventoryView();
		break;
	case ETowerLobbySubstateEnum::HeroShop: //Don't need to do any initializing tasks, ready to move on
		OnStateChanged.Broadcast();
		OnStateChanged.Clear();
		break;
	case ETowerLobbySubstateEnum::RaidTowerBoss:
	{
		RGAMEINSTANCE(this)->SequenceManager->ClearSequence();

		APlayerCameraManager * PCM = UGameplayStatics::GetPlayerCameraManager(RGAMEINSTANCE(this)->GetWorld(), 0);
		PCM->SetViewTarget(IndoorNormalViewCameraActor.Get());
		OnStateChanged.Broadcast();
		OnStateChanged.Clear();
	}
		break;
	default:
		UE_LOG(LogLobby, Log, TEXT("AIndoorLobbyStateActor::ChangeState: unexpected state"));
		return;
	}
}

void AIndoorLobbyStateActor::ChangeToRandomHero()
{
	if (HeroNameOverride.IsNone())
	{
		FLobbyCharacterTableInfo* LobbyCharacterInfo = FLobbyCharacterTableInfo::ChooseRandomly();
		ChangeHero_LobbyCharacterTable(LobbyCharacterInfo->DisplayName);
	}
	else
	{
		ChangeHero_LobbyCharacterTable(HeroNameOverride);
	}
}

void AIndoorLobbyStateActor::ChangeToHeroName(FName HeroName)
{
	ChangeHero_LobbyCharacterTable(HeroName);
}

void AIndoorLobbyStateActor::ChangeHero_LobbyCharacterTable(FName HeroName)
{
	UE_LOG(LogLobby, Log, TEXT("AIndoorLobbyStateActor::ChangeHero_LobbyCharacterTable: %s"), *HeroName.ToString());

	DestroyCurrentLobbyCharacter();

	//RGAMEINSTANCE(this)->EventManager->IndoorLobbyHeroChange.Broadcast(HeroName);
	FLobbyCharacterTableInfo* LobbyCharacterInfo = FLobbyCharacterTableInfo::LookUp(HeroName);
	if (LobbyCharacterInfo == nullptr)
	{
		UE_LOG(LogLobby, Log, TEXT("AIndoorLobbyStateActor::ChangeHero_LobbyCharacterTable: failed to get LobbyCharacterInfo"));
		return;
	}

	heroName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, LobbyCharacterInfo->DisplayName);
// 	heroNickName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, LobbyCharacterInfo->Nick);
	FCharacterTableInfo* pUITableInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum::VE_Hero, LobbyCharacterInfo->HeroID);
	heroSprite = UResourceFunctionLibrary::GetHeroClassTypeSprite(pUITableInfo->Class);
	heroGrade = pUITableInfo->Grade;
	// Async asset loading with StreamableManager

	ClearCharacterLoadRequest();
	
	LoadLobbyHeroCharacter(HeroName, LobbyCharacterInfo->LobbyHeroClass, LobbyCharacterInfo->RWeaponType, LobbyCharacterInfo->LWeaponType);
}

void AIndoorLobbyStateActor::ChangeHero_Owned(FString InHeroUD)
{
	// dubious: skipping if same HeroUD
	// TODO
	if (!PlayingLobbyTouchSequenceName.IsNone())
		RGAMEINSTANCE(this)->SequenceManager->CleanUpSequence(PlayingLobbyTouchSequenceName);

	DestroyCurrentLobbyCharacter();

	int32 UserKID = RGAMEINSTANCE(this)->HttpClient->GetKID();

	FHERO* HeroInvenData = RGAMEINSTANCE(this)->RInventory->GetHeroWithUD(InHeroUD);
	if (HeroInvenData == nullptr)
		return;
	FCharacterTableInfo* CharacterInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroInvenData->heroId);
	check(CharacterInfo != nullptr);


	FTransform CharacterTransform;
	CharacterTransform.SetLocation(IndoorOriginLocation);

// 	AActor* SpawnedActor = ObjectMgr->SpawnCharacter(CharacterTransform, HeroInvenData->heroId, ECharacterCategoryTypeEnum::VE_UICharacter,
// 		UserKID, HeroInvenData->heroUD, GroupKey_SINGLE_MY, PartyID_PC_MY, PLAYER_UK_NONE);
	AActor* SpawnedActor = UObjectFunctionLibrary::SpawnCharacter(CharacterTransform, HeroInvenData->heroId, ECharacterCategoryTypeEnum::VE_UICharacter,
																	UserKID, HeroInvenData->heroUD, GroupKey_SINGLE_MY, PartyID_PC_MY, PLAYER_UK_NONE);
	//CurrentLobbyCharacter = Cast<ALobbyCharacter>(SpawnedActor);
	CurrentLobbyCharacter = SpawnedActor;
	if (!IsValid(CurrentLobbyCharacter))
	{
		UE_LOG(LogLobby, Log, TEXT("AIndoorLobbyStateActor::ChangeHero: Failed to get ALobbyCharacter"));
		return;
	}

	CurrentHeroName = CharacterInfo->DisplayName;
	CurrentHeroUD = InHeroUD;

	ApplyIndoorHeroManagementViewTableHero();
	if (CurrentSubstate == ETowerLobbySubstateEnum::HeroManagementView)
	{
		ApplyIndoorHeroManagementViewTableCamera();
	}
	else if (CurrentSubstate == ETowerLobbySubstateEnum::NormalView || CurrentSubstate == ETowerLobbySubstateEnum::HeroShop)
	{
		ApplyLobbyCharacterViewTableCamera();
	}
}

void AIndoorLobbyStateActor::PlayHeroLobbySocialAnimation()
{
	if (!IsValid(CurrentLobbyCharacter))
		return;

	ResetHeroTouchInteraction();

	auto LobbyCharacter = Cast<ALobbyCharacter>(CurrentLobbyCharacter);
	if (!IsValid(LobbyCharacter))
		return;

	UAnimInstance* CharacterAnimInstance = LobbyCharacter->GetMesh()->GetAnimInstance();
	if (IsValid(CharacterAnimInstance))
	{
		UBattleBaseAnimInstance* BattleBaseAnimInstance = Cast< UBattleBaseAnimInstance >(CharacterAnimInstance);
		if (IsValid(BattleBaseAnimInstance))
		{
			BattleBaseAnimInstance->PlayAnimMontageEventSection(ANIM_MONTAGE_EVENT_LOBBYSOCIAL, ANIM_MONTAGE_EVENT_LOBBYIDLE);
		}
		else
		{
			ULobbyAnimInstance* LobbyAnimInstance = Cast< ULobbyAnimInstance >(CharacterAnimInstance);
			if (IsValid(LobbyAnimInstance))
			{
				LobbyAnimInstance->PlayAnimMontageEventSection(ANIM_MONTAGE_EVENT_LOBBYSOCIAL);
			}
		}
	}
}

void AIndoorLobbyStateActor::PlayHeroTouchActionAnimationAndSequence()
{
	if (!IsValid(CurrentLobbyCharacter))
		return;

	// Ignore if already playing touch action sequence
	if (!PlayingLobbyTouchSequenceName.IsNone())
	{
		if (RGAMEINSTANCE(this)->SequenceManager->IsPlaying(PlayingLobbyTouchSequenceName))
			return;
	}

	ResetHeroTouchInteraction();

	// Play ANIM_MONTAGE_LOBBY_SOCIAL

	// Bloody hell

	//auto LobbyCharacter = Cast<ALobbyCharacter>(CurrentLobbyCharacter);
	//if (IsValid(LobbyCharacter))
	//{
	UAnimInstance* CharacterAnimInstance = UCharacterHelpFunctionLibrary::GetCharacterAnimInstance(CurrentLobbyCharacter);
	if (IsValid(CharacterAnimInstance))
	{
		UBattleBaseAnimInstance* BattleBaseAnimInstance = Cast< UBattleBaseAnimInstance >(CharacterAnimInstance);
		if (IsValid(BattleBaseAnimInstance))
		{
			//BattleBaseAnimInstance->PlayAnimMontageLobbySection(ANIM_MONTAGE_LOBBY_SOCIAL);
			UE_LOG(LogLobby, Log, TEXT("AIndoorLobbyStateActor::BeginSublevel: this ALobbyCharacter has UBattleBaseAnimInstance."))
		}
		else
		{
			ULobbyAnimInstance* LobbyAnimInstance = Cast< ULobbyAnimInstance >(CharacterAnimInstance);
			if (IsValid(LobbyAnimInstance))
			{
				LobbyAnimInstance->PlayAnimMontageLobbySection(ANIM_MONTAGE_LOBBY_SOCIAL);
			}
		}
	}

	if (CurrentHeroName.IsNone())
		return;

	FLobbyCharacterTableInfo* LobbyCharacterTableInfo = FLobbyCharacterTableInfo::LookUp(CurrentHeroName);
	if (LobbyCharacterTableInfo == nullptr)
	{
		UE_LOG(LogLobby, Log, TEXT("AIndoorLobbyStateActor::BeginSublevel: failed to get LobbyCharacterTableInfo"));
		return;
	}

	// Tentative: utilizing new feature of UE4.19 "Level Sequence Dynamic Transform Origin" to offset Touch Action sequence to IndoorOriginLocation

	// Get the sequence
	FResourceSequenceTableInfo* SequenceInfo = UTableManager::GetInstancePtr()->GetResourceSequenceRow(LobbyCharacterTableInfo->LobbyTouchSequence);
	if (SequenceInfo == nullptr)
	{
		UE_LOG(LogLobby, Log, TEXT("AIndoorLobbyStateActor::PlayHeroTouchActionAnimationAndSequence: sequence missing in resource table"));
		return;
	}

	FTransform SequenceOffset;
	SequenceOffset.SetLocation(IndoorOriginLocation);
	RGAMEINSTANCE(this)->SequenceManager->SpawnSequenceWithTransformOrigin(LobbyCharacterTableInfo->LobbyTouchSequence, SequenceOffset);
	PlayingLobbyTouchSequenceName = LobbyCharacterTableInfo->LobbyTouchSequence;
}

void AIndoorLobbyStateActor::RotateHero(float YawDelta)
{
	if (!IsValid(CurrentLobbyCharacter))
		return;

	// ignore interaction when playing sequence
	if (!PlayingLobbyTouchSequenceName.IsNone())
	{
		if (RGAMEINSTANCE(this)->SequenceManager->IsPlaying(PlayingLobbyTouchSequenceName))
			return;
	}

	FRotator Rotator = CurrentLobbyCharacter->GetActorRotation();
	Rotator.Yaw += YawDelta;
	CurrentLobbyCharacter->SetActorRotation(Rotator);
}

void AIndoorLobbyStateActor::ZoomInToHero(float LerpAlphaDelta)
{
	if (!IsValid(CurrentLobbyCharacter))
		return;

	if (CurrentSubstate != ETowerLobbySubstateEnum::NormalView)
		return;

	// ignore interaction when playing sequence
	if (PlayingLobbyTouchSequenceName.IsValid())
	{
		if (RGAMEINSTANCE(this)->SequenceManager->IsPlaying(PlayingLobbyTouchSequenceName))
			return;
	}

	CurrentZoomInLerpAlpha += LerpAlphaDelta;
	
	CurrentZoomInLerpAlpha = FMath::Clamp(CurrentZoomInLerpAlpha, 0.0f, 1.0f);

	UpdateNormalViewCameraTransform();
}

void AIndoorLobbyStateActor::ResetHeroTouchInteraction()
{
	if (!IsValid(CurrentLobbyCharacter))
		return;

	// Character rotation
	FRotator Rotator = CurrentLobbyCharacter->GetActorRotation();
	Rotator.Yaw = LobbyCharacterYawBase;
	CurrentLobbyCharacter->SetActorRotation(Rotator);

	// Camera Zoom in
	CurrentZoomInLerpAlpha = 0.0f;
	UpdateNormalViewCameraTransform();
}

void AIndoorLobbyStateActor::HeroTapAction()
{
	switch (CurrentSubstate)
	{
	case ETowerLobbySubstateEnum::NormalView:
		PlayHeroTouchActionAnimationAndSequence();
		break;
	case ETowerLobbySubstateEnum::HeroManagementView:
		PlayHeroLobbySocialAnimation();
		break;
	default:
		UE_LOG(LogLobby, Log, TEXT("AIndoorLobbyStateActor::HeroTapAction: unexpected CurrentIndoorLobbyState"));
		break;
	}
}

void AIndoorLobbyStateActor::GoToHeroInventoryView()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	RGameInstance->SequenceManager->ClearSequence();

	ApplyIndoorHeroManagementViewTableCamera();

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(Cast<AActor>(IndoorHeroManagementViewCameraActor));
	//UKismetSystemLibrary::Delay(GetWorld(), 0, InitLatentActionInfo(FName("GoToHeroInventoryViewComplete"), this, 1, 0));
	GoToHeroInventoryViewComplete();
}

void AIndoorLobbyStateActor::GoToHeroInventoryViewComplete()
{
	OnStateChanged.Broadcast();
}

void AIndoorLobbyStateActor::GoToNormalView()
{
	ApplyLobbyCharacterViewTableCamera();

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(Cast<AActor>(IndoorNormalViewCameraActor));
	//UKismetSystemLibrary::Delay(GetWorld(), 0, InitLatentActionInfo(FName("GoToNormalViewComplete"), this, 1, 0));
	GoToNormalViewComplete();
}

void AIndoorLobbyStateActor::GoToNormalViewComplete()
{
	URWidgetManager* WidgetManager = RGAMEINSTANCE(GEngine)->RWidgetManager;
	if (!WidgetManager->UIEventQueue.Num())
	{
		PlayHeroTouchActionAnimationAndSequence();
	}
	OnStateChanged.Broadcast();
}

void AIndoorLobbyStateActor::SetVisibilityAll(bool IsVisible)
{
	/*Super::SetVisibilityAll(!IsVisible);

	TArray<class ULevelStreaming*> LSArray = RGAMEINSTANCE(GEngine)->GetWorld()->StreamingLevels;
	for (size_t i = 0; i < LSArray.Num(); ++i)
	{
		if (LSArray[i]->bShouldBeLoaded == true)
			LSArray[i]->bShouldBeVisible = IsVisible;
	}*/

	SetVisibilityHero(IsVisible);
}

void AIndoorLobbyStateActor::SetVisibilityHero(bool IsVisible)
{
	if (IsValid(CurrentLobbyCharacter))
	{
		 //TODO: fix
		auto BattleCharacter = Cast<ABattleBaseCharacter>(CurrentLobbyCharacter);
		if (IsValid(BattleCharacter))
		{
			UEntityRenderComponent* EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(BattleCharacter);
			if (IsValid(EntityRenderComponent))
			{
				EntityRenderComponent->CharacterChangeOpacity(IsVisible ? 1.0f : 0.0f);
			}
			else
			{
				CurrentLobbyCharacter->SetActorHiddenInGame(!IsVisible);
			}
		}
		else
		{
			CurrentLobbyCharacter->SetActorHiddenInGame(!IsVisible);
		}

		TArray<UActorComponent*> ActorComponents = CurrentLobbyCharacter->GetComponentsByClass(UActorComponent::StaticClass());
		for (size_t i = 0; i < ActorComponents.Num(); ++i)
		{
			UParticleSystemComponent* PSC = Cast<UParticleSystemComponent>(ActorComponents[i]);
			if (PSC)
				PSC->SetVisibility(IsVisible);
		}
	}

	if (IsValid(CurrentRWeapon))
		CurrentRWeapon->SetActorHiddenInGame(!IsVisible);
	if (IsValid(CurrentLWeapon))
		CurrentLWeapon->SetActorHiddenInGame(!IsVisible);
}

void AIndoorLobbyStateActor::LoadLobbyHeroCharacter(FName HeroName, TSoftClassPtr<ABattleBaseCharacter> LobbyHeroClass, TSoftClassPtr<AWeaponBase> RWeaponType, TSoftClassPtr<AWeaponBase> LWeaponType)
{
	RGAMEINSTANCE(this)->SequenceManager->ClearSequence();
	DestroyCurrentLobbyCharacter();

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	FTransform CharacterTransform;
	CharacterTransform.SetLocation(IndoorOriginLocation);

	ABattleBaseCharacter* SpawnedCharacter = GetWorld()->SpawnActor<ABattleBaseCharacter>(LobbyHeroClass.LoadSynchronous(), CharacterTransform, SpawnParams);
	if (!IsValid(SpawnedCharacter))
	{
		UE_LOG(LogLobby, Log, TEXT("AIndoorLobbyStateActor::OnHeroCharacterAsyncLoaded: failed to spawn ABattleBaseCharacter"));
		return;
	}

	// This prevents the character from falling
	SpawnedCharacter->GetCharacterMovement()->GravityScale = 0.0f;

	// Attach weapons
	if (!RWeaponType.IsNull())
	{
		AActor* WeaponActor = GetWorld()->SpawnActor(RWeaponType.LoadSynchronous());
		check(IsValid(WeaponActor));
		USkeletalMeshComponent* MeshComp = SpawnedCharacter->GetMesh();
		check(IsValid(MeshComp));
		FAttachmentTransformRules rules = FAttachmentTransformRules::KeepRelativeTransform;
		rules.bWeldSimulatedBodies = true;
		WeaponActor->AttachToComponent(MeshComp, rules, TEXT("Bone_Weapon_R"));
		CurrentRWeapon = WeaponActor;
	}
	if (!LWeaponType.IsNull())
	{
		AActor* WeaponActor = GetWorld()->SpawnActor(LWeaponType.LoadSynchronous());
		check(IsValid(WeaponActor));
		USkeletalMeshComponent* MeshComp = SpawnedCharacter->GetMesh();
		check(IsValid(MeshComp));
		FAttachmentTransformRules rules = FAttachmentTransformRules::KeepRelativeTransform;
		rules.bWeldSimulatedBodies = true;
		WeaponActor->AttachToComponent(MeshComp, rules, TEXT("Bone_Weapon_L"));
		CurrentLWeapon = WeaponActor;
	}


	//SpawnedCharacter->InitHeroAfterCreation();

	CurrentLobbyCharacter = SpawnedCharacter;
	CurrentHeroName = HeroName;

	ApplyIndoorHeroManagementViewTableHero();
	if (CurrentSubstate == ETowerLobbySubstateEnum::HeroManagementView)
	{
		ApplyIndoorHeroManagementViewTableCamera();
	}
	else if (CurrentSubstate == ETowerLobbySubstateEnum::NormalView)
	{
		ApplyLobbyCharacterViewTableCamera();
	}

	OnEnterSublevelComplete.Broadcast();
}

void AIndoorLobbyStateActor::ApplyIndoorHeroManagementViewTableCamera()
{
	if (CurrentHeroName.IsNone())
		return;

	// Adjust IndoorHeroManagementViewCamera and hero transforms

	FIndoorHeroManagementViewTableInfo* ViewInfo = UTableManager::GetInstancePtr()->GetIndoorHeroManagementViewTableRow(CurrentHeroName);

	IndoorHeroManagementViewCameraActor->SetActorLocation(ViewInfo->CameraLocation + IndoorOriginLocation);
	IndoorHeroManagementViewCameraActor->SetActorRotation(ViewInfo->CameraRotation);
	IndoorHeroManagementViewCameraActor->GetCameraComponent()->SetFieldOfView(ViewInfo->Fov);

	UGameplayStatics::GetPlayerController(this, 0)->SetViewTargetWithBlend(IndoorHeroManagementViewCameraActor.Get(), 0.2);
}

void AIndoorLobbyStateActor::ApplyIndoorHeroManagementViewTableHero()
{
	if (CurrentHeroName.IsNone())
		return;

	// Adjust IndoorHeroManagementViewCamera and hero transforms

	FIndoorHeroManagementViewTableInfo* ViewInfo = UTableManager::GetInstancePtr()->GetIndoorHeroManagementViewTableRow(CurrentHeroName);
	checkf(ViewInfo != nullptr, TEXT("FIndoorHeroManagementViewTableInfo(%s)"), *CurrentHeroName.ToString());
	if (!ensure(IsValid(CurrentLobbyCharacter)))
		return;
	CurrentLobbyCharacter->SetActorLocation(ViewInfo->CharacterLocation + IndoorOriginLocation);
	CurrentLobbyCharacter->SetActorRotation(ViewInfo->CharacterRotation);
	LobbyCharacterYawBase = ViewInfo->CharacterRotation.Yaw;
}

void AIndoorLobbyStateActor::ApplyLobbyCharacterViewTableCamera()
{
	if (CurrentHeroName.IsNone())
		return;

	FLobbyCharacterTableInfo* LobbyCharacterTableInfo = FLobbyCharacterTableInfo::LookUp(CurrentHeroName);
	if (LobbyCharacterTableInfo == nullptr)
	{
		UE_LOG(LogLobby, Log, TEXT("AIndoorLobbyStateActor::ApplyLobbyCharacterViewTableCamera: failed to get LobbyCharacterTableInfo"));
		return;
	}

	// apply DefaultCameraPosition, DefaultCameraRotator, Fov
	DefaultCameraLocation = LobbyCharacterTableInfo->DefaultCameraPosition + IndoorOriginLocation;
	DefaultCameraRotation = LobbyCharacterTableInfo->DefaultCameraRotator;
	ZoomInCameraLocation = LobbyCharacterTableInfo->ZoomInCameraPosition + IndoorOriginLocation;
	ZoomInCameraRotation = LobbyCharacterTableInfo->ZoomInCameraRotator;

	if (LobbyCharacterTableInfo->ZoonInInvert)
	{
		CurrentZoomInLerpAlpha = 1.0f;
	}
	else
	{
		CurrentZoomInLerpAlpha = 0.0f;
	}

	IndoorNormalViewCameraActor->GetCameraComponent()->SetFieldOfView(LobbyCharacterTableInfo->FieldOfView);

	UpdateNormalViewCameraTransform();

	//UGameplayStatics::GetPlayerController(this, 0)->SetViewTargetWithBlend(IndoorNormalViewCameraActor.Get(), 0.2);

	if (!PlayingLobbyTouchSequenceName.IsNone())
	{
		RGAMEINSTANCE(this)->SequenceManager->StopSequence(PlayingLobbyTouchSequenceName);
		PlayingLobbyTouchSequenceName = NAME_None;
	}

	FTransform SequenceOffset;
	SequenceOffset.SetLocation(IndoorOriginLocation);
	RGAMEINSTANCE(this)->SequenceManager->SpawnSequenceWithTransformOrigin(LobbyCharacterTableInfo->LobbyIdleSequence, SequenceOffset, true, true);
}

//void AIndoorLobbyStateActor::RevertFromIndoorHeroManagementViewTable()
//{
//	// Hero to the origin
//	CurrentLobbyCharacter->SetActorLocation(LobbyCharacterTransform);
//	CurrentLobbyCharacter->SetActorRotation(FRotator::ZeroRotator);
//}

FName AIndoorLobbyStateActor::HeroIDToLobbyCharacterTableIndex(FName HeroID)
{
	int32 id = FCString::Atoi(*HeroID.ToString());
	int32 index = id % 1000;
	return FName(*FString::Printf(TEXT("%d"), index));
}

FName AIndoorLobbyStateActor::GetCurrentLightingInFName()
{
	if (RGAMEINSTANCE(this)->GameEnvironmentVar.LobbyIsNight)
		return FName("Lobby_In_NightTime_Ls");
	else
		return FName("Lobby_In_DayTime_Ls");
}

void AIndoorLobbyStateActor::UpdateNormalViewCameraTransform()
{
	FVector Location = FMath::Lerp(DefaultCameraLocation, ZoomInCameraLocation, CurrentZoomInLerpAlpha);
	FRotator Rotation = FMath::Lerp(DefaultCameraRotation, ZoomInCameraRotation, CurrentZoomInLerpAlpha);

	IndoorNormalViewCameraActor->SetActorLocation(Location);
	IndoorNormalViewCameraActor->SetActorRotation(Rotation);
}

void AIndoorLobbyStateActor::DestroyCurrentLobbyCharacter()
{
	if (CurrentLobbyCharacter && IsValid(CurrentLobbyCharacter))
	{
		CurrentLobbyCharacter->K2_DestroyActor();
		CurrentLobbyCharacter = nullptr;
	}
	if (CurrentRWeapon && IsValid(CurrentRWeapon))
	{
		CurrentRWeapon->K2_DestroyActor();
		CurrentRWeapon = nullptr;
	}
	if (CurrentLWeapon && IsValid(CurrentLWeapon))
	{
		CurrentLWeapon->K2_DestroyActor();
		CurrentLWeapon = nullptr;
	}
}

void AIndoorLobbyStateActor::ClearCharacterLoadRequest()
{
	if (CharacterLoadRequestHandle.IsValid())
	{
		CharacterLoadRequestHandle->CancelHandle();
		CharacterLoadRequestHandle.Reset();
	}
}
