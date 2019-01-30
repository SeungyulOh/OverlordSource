// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UtilFunctionIntegrated.h"

#include "SharedConstants/GlobalConstants.h"
#include "GlobalIntegrated.h"
//
#include "GameInfo/UserInfo.h"
#include "ProjectRGameMode.h"
#include "HUDBase.h"

#include "GameInfo/PlayStateInfo.h"
#include "GameInfo/RInventory.h"

#include "Lobby/IndoorLobbyStateActor.h"
#include "Lobby/OutdoorLobbyStateActor.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "ContentGameMode/Controllers/StagePlayerController.h"
#include "Character/BattleBaseAnimInstance.h"
#include "Character/BattleObject/BattleBaseObject.h"
#include "Character/Components/AnimStateComponent.h"
#include "Character/Components/BossComponents/BossBaseComponent.h"
#include "Character/Components/WidgetDecoratorComponent.h"

#include "Character/Components/CustomWidgetComponent.h"
#include "Character/Components/FunctionalComponents/EntityAIComponent.h"
#include "Character/Components/FunctionalComponents/PathFindingComponent.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/SelectionIndicatorMeshComponent.h"
#include "Character/Components/SpeechBubbleComponent.h"
#include "Character/Interfaces/EntityBaseProperty.h"
//#include "Character/Party/Party.h"
#include "Character/AnimInstance/AnimInstanceDefs.h"

#include "DirectionTrackManager.h"

// #include "Utils/ConsoleCheatManager.h"

#include "Controllers/DPadController.h"
#include "GameState/Pvp/VictoryPosition.h"

#include "JsonHelper.h"
#include "AI/Navigation/NavigationPath.h"
#include "JsonObjectConverter.h"
#include "DescriptionFunctionLibrary.h"
#include "Runtime/Engine/Classes/Matinee/MatineeActor.h"
#include "Runtime/CoreUObject/Public/UObject/UObjectGlobals.h"

#include "Table/StringTableInfo.h"
#include "Table/ItemTableInfo.h"
#include "Table/CharacterTableInfo.h"
#include "Table/LevelAppliedTables/WorkedItemTableInfo.h"
#include "Table/ResourceMaterialTableInfo.h"
#include "Table/ResourceParticleTableInfo.h"
#include "Table/ResourceTexture2DTableInfo.h"
#include "Table/ResourceSoundTableInfo.h"
#include "Table/ItemTableInfo.h"

#include "Network/PacketDataStructures.h"
#include "Network/RTSManager.h"
#include "UI/RWidgetManager.h"
#include "UI/CharacterUI/CharacterUserWidget.h"
#include "UI/Raid/UP_RaidBossSpawn.h"
#include "UI/Common/UP_CampaignBossSpawn.h"
#include "UI/Common/UC_TextIndicator.h"
#include "UI/Shop/UP_HeroShop.h"

#include "Object/NextTargetActor.h"
#include "ContentGameMode/Controllers/LobbyPlayerController.h"
#include "Object/LobbyCameraActor.h"
#include "Object/CharacterSetCameraActor.h"
#include "Object/EmptyBasicActor.h"

#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Runtime/Engine/Classes/AI/Navigation/RecastNavMesh.h"
#include "Runtime/UMG/Public/Blueprint/WidgetLayoutLibrary.h"

#include "Base64.h"
#include "LeaderControlHelper.h"
#include "AutoSpawner.h"

#include "Runtime/AIModule/Classes/BehaviorTree/BehaviorTree.h"

#include "Classes/Engine/BlockingVolume.h"
#include "Animation/AnimMontage.h" //by danzang

#include "ContentGameMode/Modes/StageGameMode.h"
#include "ProjectRGameMode.h"

#include "UtilFunctionIntegrated.h"

#include "UI/UIDefs.h"
#include "ContentGameMode/Modes/TowerLobbyGameMode.h"
#include "Object.h"
#include "Global/RGameInstance.h"
#include "GameState/PlayState.h"
#include "TutorialGameMode.h"
#include "TutorialLevelScriptActor.h"
// #if LOG_TIME
// #include "Utils/UtilsStructures/ScopedTimeElapsedLog.h"
// #endif

//#define TESTCOW
//#define DRAW_DEBUG_INFO
//#define NOT_SPAWN_EFFECT PLATFORM_IOS
#define NOT_SPAWN_EFFECT 0
#define LOAD_PARTICLE_RESOURCE 1
#define LOAD_SOUND_RESOURCE 1

void UUtilFunctionLibrary::ShowIndicator(FText DisplayText, float InDuration/* = 0.0f*/, bool bNoAni/* = false*/, bool bBottomMsg/* = false*/)
{
	auto Widget = GetIndicatorWidget();

	if (Widget)
	{
		auto TextIndicator = Cast<UUC_TextIndicator>(Widget);
		if (TextIndicator)
		{
			if (bBottomMsg)
			{
				if (0.0f == InDuration)
					TextIndicator->ShowIndicatorStringBottom(DisplayText, 1.0f);
				else
					TextIndicator->ShowIndicatorStringBottom(DisplayText, InDuration);
			}
			else
			{
				if (bNoAni)
					TextIndicator->ShowIndicatorStringNoAni(DisplayText, InDuration);
				else
					TextIndicator->ShowIndicatorString(DisplayText, InDuration);
			}
		}
	}

	/*APlayerController* PlayerController = World->GetFirstPlayerController();
	if (PlayerController)
	{
		AHUDBase* HUDBase = Cast< AHUDBase >(PlayerController->GetHUD());
		if (IsValid(HUDBase))
		{
			HUDBase->OnShowIndicatorText(DisplayText);
		}
	}*/
}

void UUtilFunctionLibrary::HideIndicator()
{
	auto Widget = GetIndicatorWidget();
	if (Widget)
	{
		auto TextIndicator = Cast<UUC_TextIndicator>(Widget);
		if (TextIndicator)
		{
			TextIndicator->HideWidget();
		}
	}
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RWidgetManager Is Empty ]"), __FUNCTION__);
	RGameInstance->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_TextIndicator);
}

void UUtilFunctionLibrary::SetIndicatorOffset(float InOffsetX, float InOffsetY)
{
	auto Widget = GetIndicatorWidget();
	if (Widget)
	{
		auto TextIndicator = Cast<UUC_TextIndicator>(Widget);
		if(TextIndicator)
		{
			TextIndicator->SetIndicatorOffset(InOffsetX, InOffsetY);
		}
	}
}

FSkillTableInfo_V2* UUtilFunctionLibrary::GetSkillTableFromHeroID(const FName& InHeroID)
{
	FCharacterTableInfo* CharacterTableInfo1 = UTableManager::GetInstancePtr()->GetCharacterRow(InHeroID);
	FString SkillIDName = CharacterTableInfo1->ActiveSkill02.ToString() + "_1";
	FSkillTableInfo_V2* skillData = UTableManager::GetInstancePtr()->GetSkillRow_V2(FName(*SkillIDName));
	return skillData;
}

// FSkillTableInfo_V2* UUtilFunctionLibrary::GetSkillTableFromHeroUD(const FString& InHeroUD)
// {
// 	FHERO* pHero = RGAMEINSTANCE(GEngine)->RInventory->GetHeroWithUD(InHeroUD);
// 	FCharacterTableInfo* CharacterTableInfo1 = UTableManager::GetInstancePtr()->GetCharacterRow(pHero->heroId);
// 	FString SkillIDName = CharacterTableInfo1->ActiveSkill02.ToString() + "_1";
// 	FSkillTableInfo_V2* skillData = UTableManager::GetInstancePtr()->GetSkillRow_V2(FName(*SkillIDName));
// 	return skillData;
// }

UUserWidget* UUtilFunctionLibrary::GetIndicatorWidget()
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RWidgetManager Is Empty ]"), __FUNCTION__);
	UUserWidget* userWidget = RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_TextIndicator);
	if (userWidget == nullptr)
	{
		userWidget = RGameInstance->RWidgetManager->CreateUserWidget(EWidgetBluePrintEnum::WBP_TextIndicator);
	}

	RGameInstance->RWidgetManager->ShowUserWidget(EWidgetBluePrintEnum::WBP_TextIndicator, true, FORCETUTORIAL_BLOCK_ZORDER + 1);

	return RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_TextIndicator);
}

bool UUtilFunctionLibrary::IsShowIndicator()
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->RWidgetManager != nullptr, TEXT("%s[ RWidgetManager Is Empty ]"), __FUNCTION__);
	RGameInstance->RWidgetManager->ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_TextIndicator);
	auto Widget = RGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_TextIndicator);
	if (Widget)
	{
		auto TextIndicator = Cast<UUC_TextIndicator>(Widget);
		if (TextIndicator)
		{
			return TextIndicator->IsShowIndicator();
		}
	}

	return false;
}

// bool UUtilFunctionLibrary::IsRealTimeGameMode(UWorld* World)
// {
// 
// 	if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::BF) ||
// 		URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::BG) ||
// 		URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::GR) ||
// 		URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS) ||
// 		URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::SR) ||
// 		URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::MR))
// 		return true;
// 	return false;
// }

EGamePlayModeTypeEnum UUtilFunctionLibrary::GetGamePlayModeEnum()
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	if (RGameInstance)
	{
		UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
		return BaseStageInfo->GetGameMode();
	}
	return EGamePlayModeTypeEnum::VE_None;
}

// AProjectRGameMode* UUtilFunctionLibrary::GetGameModeByRGameInstance(URGameInstance* RGameInstance)
// {
// 	auto GameMode = UGameplayStatics::GetGameMode(RGameInstance);
// 	if (IsValid(GameMode))
// 	{
// 		return Cast<AProjectRGameMode>(GameMode);
// 	}
// 
// 	return nullptr;
// }

AProjectRGameMode* UUtilFunctionLibrary::GetGamePlayMode()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(GEngine);

	auto GameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(GameInstance));
	if (IsValid(GameMode))
		return GameMode;

	return nullptr;
}

void UUtilFunctionLibrary::CheckGameModeState()
{
	URGameInstance* GameInstance = RGAMEINSTANCE(GEngine);

	auto GameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(GameInstance));
	if (IsValid(GameMode))
		GameMode->CheckGameModeState();
}

void UUtilFunctionLibrary::ChangeGameModeState(EGameModeStateEnum  InModeState, bool InIsForce /*= false*/)
{
	URGameInstance* GameInstance = RGAMEINSTANCE(GEngine);

	auto GameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(GameInstance));
	if (IsValid(GameMode))
		GameMode->ChangeGameModeState(InModeState, InIsForce);
}

//URGameInstance* UUtilFunctionLibrary::GetRGameInstance(UWorld* World)
//{
//	if (IsValid(World) == false)
//		return nullptr;
//
//	URGameInstance* RGameInstance = Cast<URGameInstance>(World->GetGameInstance());
//	return RGameInstance;
//}
//
//URGameInstance* UUtilFunctionLibrary::GetRGameInstanceFromActor(AActor* Actor)
//{
//	if (IsValid(Actor) == false)
//		return nullptr;
//
//	return RGAMEINSTANCE(GEngine);
//}

//URGameInstance* UUtilFunctionLibrary::GetRGameInstanceFromWidget(UUserWidget* Widget)
//{
//	if (IsValid(Widget) == false)
//		return nullptr;
//	return RGAMEINSTANCE(GEngine);
//}

// UGameObjectManager* UUtilFunctionLibrary::GetGameObjectManagerFromWorld(UWorld* World)
// {
// 	URGameInstance* instance = RGAMEINSTANCE(GEngine);
// 	if (!IsValid(instance))
// 		return nullptr;
// 
// 	return instance->GameObjectMgr;
// }
// 
// UGameObjectManager* UUtilFunctionLibrary::GetGameObjectManagerFromActor(AActor* Actor)
// {
// 	URGameInstance* instance = RGAMEINSTANCE(GEngine);
// 	if (!IsValid(instance))
// 		return nullptr;
// 
// 	return instance->GameObjectMgr;
// }

UParticleSystemComponent* UUtilFunctionLibrary::SpawnEmitterAttachedWithTableID(FName& InTableID, AActor* Target, FName AttachPointName, FVector InLocation, FRotator Rotation, EAttachLocation::Type LocationType, bool bAutoDestroy)
{
	UParticleSystem* EmitterTemplate = GetParticleSystemFromTable(InTableID);
	return SpawnEmitterAttached(EmitterTemplate, Target, AttachPointName, InLocation, Rotation, LocationType, bAutoDestroy);
}

UParticleSystemComponent* UUtilFunctionLibrary::SpawnEmitterAttachedWithAssetID(TAssetPtr< UParticleSystem > InAssetID, AActor* Target, FName AttachPointName , FVector InLocation , FRotator Rotation , EAttachLocation::Type LocationType , bool bAutoDestroy )
{
	UParticleSystem* EmitterTemplate = GetParticleSystemFromAssetID(InAssetID);
	return SpawnEmitterAttached( EmitterTemplate, Target, AttachPointName, InLocation, Rotation, LocationType, bAutoDestroy);
}

UParticleSystemComponent* UUtilFunctionLibrary::SpawnEmitterAttached(UParticleSystem* EmitterTemplate, AActor* Target, FName AttachPointName, FVector InLocation, FRotator Rotation, EAttachLocation::Type LocationType, bool bAutoDestroy)
{
#if NOT_SPAWN_EFFECT
	return nullptr;
#endif
	if (!IsValid(EmitterTemplate))
		return nullptr;

	if (!IsValid(Target))
		return nullptr;

	UBattleEntityComponent* Target_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Target);
	if (Target_BattleEntityComponent)
	{
		if (Target_BattleEntityComponent->IsInAbnormalState(EAbonormalStateFlagEnum::BM_Hide))
			return nullptr;
	}

	UParticleSystemComponent* particle = nullptr;
	ABattleBaseObject* baseObj = Cast<ABattleBaseObject>(Target);
	if (IsValid(baseObj))
	{
		USceneComponent* sceneComp = baseObj->GetSocketSceneComponent(AttachPointName);
		particle = CustomSpawnEmitterAttached(EmitterTemplate, sceneComp, AttachPointName, InLocation, Rotation, LocationType, bAutoDestroy);
	}
	else
	{
		USkeletalMeshComponent* Target_SkeletalMeshComponent = UCharacterHelpFunctionLibrary::GetSkeletalMeshComponent(Target);
		if(IsValid(Target_SkeletalMeshComponent))
			particle = CustomSpawnEmitterAttached(EmitterTemplate, Target_SkeletalMeshComponent, AttachPointName, InLocation, Rotation, LocationType, bAutoDestroy);
	}

	if (IsValid(particle))
	{
		ComputeEffectSpeed(Target, particle);
	}

	return particle;
}

UParticleSystemComponent* UUtilFunctionLibrary::CustomSpawnEmitterAttached(UParticleSystem* EmitterTemplate, USceneComponent* AttachToComponent, FName AttachPointName, FVector Location, FRotator Rotation, EAttachLocation::Type LocationType, bool bAutoDestroy )
{
#if NOT_SPAWN_EFFECT
	return nullptr;
#endif

	return UGameplayStatics::SpawnEmitterAttached(EmitterTemplate, AttachToComponent, AttachPointName, Location, Rotation, LocationType, bAutoDestroy );
}

UParticleSystemComponent* UUtilFunctionLibrary::CustomSpawnEmitterAtLocation(UObject* WorldContextObject, class UParticleSystem* EmitterTemplate, FVector Location, FRotator Rotation, bool bAutoDestroy )
{
#if NOT_SPAWN_EFFECT
	return nullptr;
#endif

	return UGameplayStatics::SpawnEmitterAtLocation(WorldContextObject, EmitterTemplate, Location, Rotation, bAutoDestroy);
}

bool UUtilFunctionLibrary::ComputeEffectSpeed(AActor* InTargetActor, UParticleSystemComponent* InTargetParticle)
{
	if (IsValid(InTargetActor) == false || IsValid(InTargetParticle) == false)
		return false;

	UEntityAIComponent* TargetEntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(InTargetActor);
	if (TargetEntityAIComponent == nullptr)
		return false;

	if (TargetEntityAIComponent->GetModifiedAnimSpeed() != 1.0)
	{
		InTargetParticle->CustomTimeDilation = TargetEntityAIComponent->GetModifiedAnimSpeed();
	}
	return true;
}


float UUtilFunctionLibrary::GetBetweenDegree(FVector SourceDir, FVector DestDir)
{
	float Value = FVector::DotProduct(SourceDir, DestDir);
	float Degree = FMath::Acos(Value) * (180.f / PI);

	FVector crossVec = FVector::CrossProduct(SourceDir, DestDir);
	if (crossVec.Z < 0.0f)
	{
		Degree *= -1.0f;
	}

	return Degree;
}

ECharacterDirection4REnum UUtilFunctionLibrary::GetBetweenDirFrontBack(FVector SourceDir, FVector DestDir)
{
	float Value = FVector::DotProduct(SourceDir, DestDir);
	float Degree = FMath::Acos(Value) * (180.f / PI);

	if ( FMath::Abs( Degree ) < 90.0f )
		return ECharacterDirection4REnum::VE_Front;

	return ECharacterDirection4REnum::VE_Back;
}

ECharacterDirection4REnum UUtilFunctionLibrary::GetBetweenDirLeftRight(FVector SourceDir, FVector DestDir)
{
	FVector crossVec = FVector::CrossProduct(SourceDir, DestDir);
	if (crossVec.Z < 0.0f)
	{
		return ECharacterDirection4REnum::VE_Left;
	}
	return ECharacterDirection4REnum::VE_Right;

// 	float fDegree = GetBetweenDegree(SourceDir, DestDir);
// 
// 	if (fDegree >= 0.0f && fDegree <= 180.0f)
// 		return ECharacterDirection4REnum::VE_Right;
// 
// 	return ECharacterDirection4REnum::VE_Left;
}

ECharacterDirection4Enum UUtilFunctionLibrary::GetBetweenDir4(FVector SourceDir, FVector DestDir)
{
	float fDegree = GetBetweenDegree(SourceDir, DestDir);

	if (fDegree >= 0.0f && fDegree <= 90.0f)
		return ECharacterDirection4Enum::VE_FrontRight;
	else if (fDegree >= 90.0f && fDegree <= 180.0f)
		return ECharacterDirection4Enum::VE_BackRight;
	else if (fDegree < 0.0f && fDegree >= -90.0f)
		return ECharacterDirection4Enum::VE_FrontLeft;
	else
		return ECharacterDirection4Enum::VE_BackLeft;
}

ECharacterDirection4REnum UUtilFunctionLibrary::GetBetweenDir4R(FVector SourceDir, FVector DestDir)
{
	float fDegree = GetBetweenDegree(SourceDir, DestDir);

	if (fDegree > -45.0f && fDegree <= 45.0f)
		return ECharacterDirection4REnum::VE_Front;
	else if (fDegree > 45.0f && fDegree <= 135.0f)
		return ECharacterDirection4REnum::VE_Right;
	else if (fDegree <= -45.0f && fDegree >= -135.0f)
		return ECharacterDirection4REnum::VE_Left;
	else
		return ECharacterDirection4REnum::VE_Back;
}

FVector UUtilFunctionLibrary::GetCameraPos(FVector SourcePos, FVector CamPos, FVector CamDir)
{
	FVector basePos = SourcePos;
	FVector newCamPos = basePos;
	newCamPos.Z = CamPos.Z;

	FVector diff = basePos - newCamPos;
	float fHeight = diff.Size();
	diff.Normalize();

	float Value = FVector::DotProduct(CamDir, diff);

	float fLength = fHeight / Value;

	FVector newPos = basePos - fLength * CamDir;

	return newPos;
}

// AActor* UUtilFunctionLibrary::GetCommandSlotOwnerCharacter(AActor* Actor, int32 SlotIndex)
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
// 	if (RGameInstance)
// 	{
// 		return RGameInstance->PlayStateInfo->GetOwnerCharacter(SlotIndex);
// 	}
// 
// 	return nullptr;
// }

// EHeroSpawnStateEnum UUtilFunctionLibrary::GetSpawnState(AActor* Actor, int32 SlotIndex)
// {
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
// 	if (RGameInstance)
// 	{
// 		return RGameInstance->PlayStateInfo->GetSpawnState(SlotIndex);
// 	}
// 
// 	return EHeroSpawnStateEnum::VE_None;
// }

APlayerController* UUtilFunctionLibrary::GetPlayerController(UWorld* World)
{
	if (World)
		return Cast<APlayerController>(World->GetFirstPlayerController());
	return nullptr;
// 	if (!IsValid(World))
// 	{
// 		return nullptr;
// 	}
// 
// 	APlayerController* PlayerController = World->GetFirstPlayerController();
// 
// 	if (!IsValid(PlayerController))
// 	{
// 		return nullptr;
// 	}
// 
// 	return PlayerController;
}

ABasePlayerController* UUtilFunctionLibrary::GetBasePlayerController(UWorld* World)
{
	if(World)
		return Cast<ABasePlayerController>(World->GetFirstPlayerController());
	return nullptr;
// 	APlayerController* PlayerController = GetPlayerController(World);
// 	if (IsValid(PlayerController))
// 	{
// 		return Cast<ABasePlayerController>(PlayerController);
// // 		ABasePlayerController* BasePlayerController = Cast<ABasePlayerController>(PlayerController);
// // 		if (IsValid(BasePlayerController))
// // 		{
// // 			return BasePlayerController;
// // 		}
// 	}
// 
// 	return nullptr;
}

ALobbyPlayerController* UUtilFunctionLibrary::GetLobbyPlayerController(UWorld* World)
{
	if (World)
		return Cast<ALobbyPlayerController>(World->GetFirstPlayerController());
	return nullptr;
// 	if (IsValid(PlayerController))
// 	{
//		return Cast<ALobbyPlayerController>(PlayerController);
// 		if (IsValid(LobbyPlayerController))
// 		{
// 			return LobbyPlayerController;
// 		}
//	}
//
//	return nullptr;
}

ADPadController* UUtilFunctionLibrary::GetDPadController(UWorld* World)
{
	APlayerController* PlayerController = GetPlayerController(World);
	if (IsValid(PlayerController))
	{
		ADPadController* DPadController = Cast<ADPadController>(PlayerController);
		if (IsValid(DPadController))
		{
			return DPadController;
		}
	}

	return nullptr;
}

bool UUtilFunctionLibrary::IsLeaderCharacter(AActor* actor)
{
	if (IsValid(actor))
	{
		ABasePlayerController* BasePlayerController = GetBasePlayerController(actor->GetWorld());
		if (IsValid(BasePlayerController))
		{
			return BasePlayerController->IsSelectedCharacter(actor);
		}
	}

	return false;
}

bool UUtilFunctionLibrary::IsMyPlayCharacter(AActor* actor)
{
	if (IsValid(actor))
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
		checkf(RGameInstance->PlayStateInfo != nullptr, TEXT("%s[ PlayStateInfo Is Empty ]"), __FUNCTION__);
		if (RGameInstance->PlayStateInfo->GetSelectedCharacterSlotIndex(actor) != INVALID_CREW_INDEX)
		{
			return true;
		}
	}
	return false;
}

bool UUtilFunctionLibrary::AIComponentAttach(AActor* actor, FName ainame)
{
	if (IsValid(actor))
	{
		return true;
	}

	return false;
}


// bool UUtilFunctionLibrary::RandPercentage(int32 ClearPercent)
// {
// 	int32 DiceNumber = (rand() % 100) + 1;
// 
// 	if(DiceNumber <= ClearPercent)
// 	{
// 		return true;
// 	}
// 	return false;
// }

void UUtilFunctionLibrary::DestroyUObject(UObject* object)
{
	if (object == nullptr || !object->IsValidLowLevel())
		return;

	object->ConditionalBeginDestroy();
	object = nullptr;
}

bool UUtilFunctionLibrary::IsInUnActableState(AActor* TargetActor)
{
	if (IsValid(TargetActor))
	{
		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(TargetActor);
		if (BattleEntityComponent)
		{
			if (BattleEntityComponent->IsInUnActableState())
				return true;
		}
	}

	return false;
}

UDirectionTrackManager* UUtilFunctionLibrary::GetDirectionTrackManager()
{
	UWorld* world = RGAMEINSTANCE(GEngine)->GetWorld();
	if (world)
	{
		AProjectRGameMode* ProjectRGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(world));
		if (ProjectRGameMode)
			return ProjectRGameMode->DirectionTrackManager;
	}

	return nullptr;
}

//AActor* UUtilFunctionLibrary::GetCommandSlotOwnerCharacterByWidget(UWidget* Widget, int32 SlotIndex)
//{
//	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
//	if (RGameInstance)
//	{
//		return RGameInstance->PlayStateInfo->GetOwnerCharacter(SlotIndex);
//	}
//
//	return nullptr;
//}

UObject* UUtilFunctionLibrary::CreateObjectFromClass(UObject* WorldContextObject, TSubclassOf<UObject> USourceClass)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	UObject* tempObject = StaticConstructObject_Internal(USourceClass);

	return tempObject;
}

// void UUtilFunctionLibrary::SetChangeLocation(AActor* baseCharacter, FVector pos)
// {
// 	if (IsValid(baseCharacter))
// 	{
// 		if (UMapFunctionLibrary::IsEnableNavigationPos(baseCharacter->GetWorld(), pos))
// 		{
// 			baseCharacter->SetActorLocation(pos);
// 		}
// 	}
// }

FString UUtilFunctionLibrary::GetEnumToString(const TCHAR* Name, int32 Value)
{
	const UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, Name, true);
	if (EnumPtr)
	{
		return EnumPtr->GetNameByValue(Value).ToString();
	}

	return TEXT("");
}

FText UUtilFunctionLibrary::GetEnumToDisplayName(const TCHAR* Name, int32 Value)
{
	UEnum* EnumPtr = FindObject<UEnum>(ANY_PACKAGE, Name, true);
	if (EnumPtr)
	{
		return EnumPtr->GetDisplayNameTextByValue(Value);
	}

	return FText();
}

FName UUtilFunctionLibrary::GetEnumToName(const TCHAR* Name, int32 Value)
{
	UEnum* EnumType = FindObject<UEnum>(ANY_PACKAGE, Name, true);
	FString	EnumString;

	EnumString = EnumType->GetNameStringByIndex(Value);
	auto EnumName = FName(*EnumString);

	return EnumName;
}

int32 UUtilFunctionLibrary::StringToEnum(const TCHAR* EnumTypeName, FString ValueString)
{
	UEnum* EnumClass = FindObject<UEnum>(ANY_PACKAGE, EnumTypeName, true);
	if (!IsValid(EnumClass))
	{
		UE_LOG(LogGamePlay, Log, TEXT("UUtilFunctionLibrary::StringToEnum: Coding error: Cannot find UEnum type for %s"), EnumTypeName);
		return INDEX_NONE;
	}

	int32 ValueInt = (int32)EnumClass->GetValueByNameString(ValueString);

	if (ValueInt == INDEX_NONE)
	{
		UE_LOG(LogGamePlay, Log, TEXT("UUtilFunctionLibrary::StringToEnum: Unexpected string value \"%s\" for enum type %s"),
			*ValueString,
			*EnumClass->GetName());
		return INDEX_NONE;
	}

	return ValueInt;
}

FName UUtilFunctionLibrary::GetResultSkillID(ECharacterCategoryTypeEnum CharacterCategory, const FName InSkillID, int32 InLevel)
{
	switch (CharacterCategory)
	{
	case ECharacterCategoryTypeEnum::VE_Hero:
		{
			FString SkillLevelString = InSkillID.ToString() + FString("_") + FString::FromInt(InLevel == 0 ? 1 : InLevel);
			return FName(*SkillLevelString);
		}
		break;
	default:
		{
			return InSkillID;
		}
		break;
	}
}

/*
String UtilFunction
*/

FText UUtilFunctionLibrary::GetLocalizedString(EStringTableType TableType, FName StringID)
{
	if(StringID.IsNone())
		return FText();
	
	FText StringValue;

	if (ULocalStringTableManager::GetInstancePtr()->GetLocalizedText(TableType, StringID, StringValue) == false)
		return FText::Format(FText::FromString(TEXT("NotFoundString[{0}]")), FText::FromString(StringID.ToString()));

#if !WITH_EDITOR
	RGAMEINSTANCE(GEngine)->RWidgetManager->AddLocalizingKey(StringID, TableType);
#endif

	return StringValue;
}

// bool UUtilFunctionLibrary::Base64Decode(FString& Source, FString& dest)
// {
// 	FString strSource = Source;
// 	int32 rest = strSource.Len() % 4;
// 	for (int32 i = 0; i < (4 - rest); ++i)
// 	{
// 		strSource += " ";
// 	}
// 
// 	return FBase64::Decode(strSource, dest);
// }
// 
// bool UUtilFunctionLibrary::Base64DecodeTArray(const FString& Source, TArray<uint8>& Dest)
// {
// 	FString strSource = Source;
// 	int32 rest = strSource.Len() % 4;
// 	for (int32 i = 0; i < (4 - rest); ++i)
// 	{
// 		strSource += " ";
// 	}
// 
// 	return FBase64::Decode(Source, Dest);
// }
// 
// FString UUtilFunctionLibrary::Base64Encode(FString& Source)
// {
// 	return FBase64::Encode(Source);
// }
// 
// FString UUtilFunctionLibrary::Base64EncodeTArray(const TArray<uint8>& Source)
// {
// 	return FBase64::Encode(Source);
// }
// 


//Item

// TODO: Have to check for invalid ItemId because UC_Item_Icons can host non-item things. Fix UC_Item_Icons

FName UUtilFunctionLibrary::GetItemGradeText(FName itemid)
{
	FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(itemid);
	if (ItemTableInfo == nullptr)
		return NAME_None;
	FItemGradeTableInfo* ItemGradeInfo = UTableManager::GetInstancePtr()->GetItemGradeTableRow(UUIFunctionLibrary::GetItemGradeName(ItemTableInfo->ItemGrade));
	return ItemGradeInfo->UIStringKey;
}

UTexture2D* UUtilFunctionLibrary::GetItemGradeIconImg(FName itemid)
{
	FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(itemid);
	if (ItemTableInfo == nullptr)
		return nullptr;
	FItemGradeTableInfo* ItemGradeInfo = UTableManager::GetInstancePtr()->GetItemGradeTableRow(UUIFunctionLibrary::GetItemGradeName(ItemTableInfo->ItemGrade));
	UTexture2D* Brush = ItemGradeInfo->IconImg.Get();
	if (!IsValid(Brush))
	{
		Brush = ItemGradeInfo->IconImg.LoadSynchronous();
	
		if (!GEngine || !RGAMEINSTANCE(GEngine) || !IsValid(RGAMEINSTANCE(GEngine)))
			return Brush;

		UWorld* World = RGAMEINSTANCE(GEngine)->GetWorld();
		if (World)
		{
			AProjectRGameMode* RGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(World));
			if (RGameMode)
				RGameMode->AddTryCntLoadSync(itemid.ToString());
		}
	}
	return Brush;
}

FSlateColor UUtilFunctionLibrary::GetItemGradeColor(FName itemid)
{
	FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(itemid);
	if (ItemTableInfo == nullptr)
		return FSlateColor();
	FItemGradeTableInfo* ItemGradeInfo = UTableManager::GetInstancePtr()->GetItemGradeTableRow(UUIFunctionLibrary::GetItemGradeName(ItemTableInfo->ItemGrade));
	return FSlateColor(FLinearColor(ItemGradeInfo->Color));
}

// Optimistic
bool UUtilFunctionLibrary::CanHeroEquipItem(FString HeroUD, FString ItemUD)
{
	FHERO* HeroData = RGAMEINSTANCE(GEngine)->RInventory->GetHeroWithUD(HeroUD);
	if (HeroData == nullptr)
	{
		UE_LOG(LogUI, Log, TEXT("UUtilFunctionLibrary::CanHeroEquipItem: failed to look up HeroData"));
		return true;
	}
	FCharacterTableInfo* HeroInfo = UTableManager::GetInstancePtr()->GetCharacterRow(HeroData->heroId);
	if (HeroInfo == nullptr)
	{
		UE_LOG(LogUI, Log, TEXT("UUtilFunctionLibrary::CanHeroEquipItem: failed to look up HeroInfo"));
		return true;
	}

	FITEM* ItemData = RGAMEINSTANCE(GEngine)->RInventory->GetItemDataWithUD(ItemUD);
	if (ItemData == nullptr)
	{
		UE_LOG(LogUI, Log, TEXT("UUtilFunctionLibrary::CanHeroEquipItem: failed to look up ItemData"));
		return true;
	}
	FItemTableInfo* ItemInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(ItemData->itemId);
	if (ItemInfo == nullptr)
	{
		UE_LOG(LogUI, Log, TEXT("UUtilFunctionLibrary::CanHeroEquipItem: failekd to look up ItemInfo"));
		return true;
	}

	return FItemTableInfo::CanEquip(HeroInfo->Class, ItemInfo->EquipType);
}

int32 UUtilFunctionLibrary::GetCurrentHaveItemCount(UWorld* World, FName itemName)
{
	return 0;
}

int32 UUtilFunctionLibrary::GetCurrentHaveRuneCount(UWorld* World, FName InCharacterName)
{
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
// 	if ( IsValid(RGameInstance->HeroInventory))
// 	{
// 		FHERO* OutHeroData = RGameInstance->HeroInventory->GetInventoryDataWithID(InCharacterName);
// 		if (OutHeroData != nullptr)
// 		{
// 			return OutHeroData->rune;
// 		}
// 	}
	return 0;
}

int32 UUtilFunctionLibrary::GetItemManufactureEnableCount(UWorld* World, FName manufactueid)
{	
	FItemManufatureTableInfo* ManufatureTableInfo = UTableManager::GetInstancePtr()->GetItemManufactureRow(manufactueid);
	if (ManufatureTableInfo)
	{
		URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
		checkf(RGameInstance->RInventory != nullptr, TEXT("%s[ RInventory Is Empty ]"), __FUNCTION__);
// 		if ( IsValid(RGameInstance->RInventory))
// 		{
			TArray<int32> haveList;
			TArray<int32> enableCountList;
			if (!ManufatureTableInfo->material1.IsNone())
			{
				int32 getCount = 0;// RGameInstance->RInventory->GetItemDataCount(ManufatureTableInfo->material1);
				haveList.Emplace(getCount);
				if (ManufatureTableInfo->material1Count > 0)
					enableCountList.Emplace(getCount / ManufatureTableInfo->material1Count);
				else
					enableCountList.Emplace(getCount);
			}
			if (!ManufatureTableInfo->material2.IsNone())
			{
				int32 getCount = 0;// RGameInstance->RInventory->GetItemDataCount(ManufatureTableInfo->material2);
				haveList.Emplace(getCount);
				if (ManufatureTableInfo->material2Count > 0)
					enableCountList.Emplace(getCount / ManufatureTableInfo->material2Count);
				else
					enableCountList.Emplace(getCount);
			}
			if (!ManufatureTableInfo->material3.IsNone())
			{
				int32 getCount = 0;// RGameInstance->RInventory->GetItemDataCount(ManufatureTableInfo->material3);
				haveList.Emplace(getCount);
				if (ManufatureTableInfo->material3Count > 0)
					enableCountList.Emplace(getCount / ManufatureTableInfo->material3Count);
				else
					enableCountList.Emplace(getCount);
			}
			if (!ManufatureTableInfo->material4.IsNone())
			{
				int32 getCount = 0;// RGameInstance->RInventory->GetItemDataCount(ManufatureTableInfo->material4);
				haveList.Emplace(getCount);
				if (ManufatureTableInfo->material4Count > 0)
					enableCountList.Emplace(getCount / ManufatureTableInfo->material4Count);
				else
					enableCountList.Emplace(getCount);
			}
			if (!ManufatureTableInfo->material5.IsNone())
			{
				int32 getCount = 0;// RGameInstance->RInventory->GetItemDataCount(ManufatureTableInfo->material5);
				haveList.Emplace(getCount);
				if (ManufatureTableInfo->material5Count > 0)
					enableCountList.Emplace(getCount / ManufatureTableInfo->material5Count);
				else
					enableCountList.Emplace(getCount);
			}

			if (!ManufatureTableInfo->targetHero.IsNone())
			{
				FCharacterTableInfo* CharTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(ManufatureTableInfo->targetHero);
				if (CharTableInfo)
				{
					int32 getCount = UUtilFunctionLibrary::GetCurrentHaveRuneCount(World, ManufatureTableInfo->targetHero);
					haveList.Emplace(getCount);
					if (ManufatureTableInfo->runeCount > 0)
						enableCountList.Emplace(getCount / ManufatureTableInfo->runeCount);
					else
						enableCountList.Emplace(getCount);
				}
			}

			if (enableCountList.Num() > 0)
			{
				enableCountList.Sort();

				return enableCountList[0];
			}
//		}
	}
	return 0;
}

/*
Hero
*/
void UUtilFunctionLibrary::MakeEquipedHeroData(FEQUIPED_HERO& EquipedHero, FString& InHeroHashKey, URGameInstance* RGameInstance)
{
	EquipedHero.heroUD.Empty();
	EquipedHero.heroId = FName("");
	
	if (RGameInstance == nullptr)
		return;
	FHERO* HeroData = RGameInstance->RInventory->GetHeroWithUD(InHeroHashKey);
	if (HeroData == nullptr)
		return;

	EquipedHero.heroUD = HeroData->heroUD;
	EquipedHero.heroId = HeroData->heroId;
	EquipedHero.level = HeroData->level;
	//EquipedHero.grade = HeroData->grade;
	EquipedHero.skills = HeroData->skills;

	int32 MaxEquip = (int32)EItemEquipPositionEnum::VE_Max;
	for (int32 n = 0; n < MaxEquip; ++n)
	{
		FITEM* ItemData = nullptr;
		if (HeroData->equipedItemUDs.IsValidIndex(n))
		{
			ItemData	=	RGameInstance->RInventory->GetItemDataWithUD(HeroData->equipedItemUDs[n]);
		}
		
		EquipedHero.equipedItems.Emplace(*(ItemData));
	}
}

void UUtilFunctionLibrary::FirstSpawnPos(UWorld* World, FVector location, FRotator Rotator)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);

	UPlayStateInfo* PSInfo = RGameInstance->PlayStateInfo;
	if (PSInfo)
	{
		FTransform spawnTransform = FTransform(Rotator, location);
		PSInfo->SpawnCharacters(spawnTransform);
	}
	
}

AActor* UUtilFunctionLibrary::FindActor(UWorld* world, FName ActorName)
{
	TArray<class ULevelStreaming*> LSArray = world->StreamingLevels;
	
	for (size_t i = 0; i < LSArray.Num(); ++i)
	{
		if (LSArray[i]->bShouldBeLoaded == 1 && LSArray[i]->bShouldBeVisible == 1)
		{
			ULevel* LoadedLevel = LSArray[i]->GetLoadedLevel();
			for (size_t j = 0; j < LoadedLevel->Actors.Num(); ++j)
			{
				if (LoadedLevel->Actors[j]->GetFName() == ActorName)
					return LoadedLevel->Actors[j];
			}
		}
	}

	return nullptr;

}

/*
Actor Finder
*/
// AActor* UUtilFunctionLibrary::FindActor(UObject* WorldContextObject, TSubclassOf<AActor> ActorClass, FName name)
// {
// 	UWorld* InWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
// 	if (InWorld == nullptr)
// 		return nullptr;
// 	for (TActorIterator<AActor> ActorItr(InWorld); ActorItr; ++ActorItr)
// 	{
// 		AActor* Actor = *ActorItr;
// 		if (IsValid(Actor) && Actor->GetFName() == name)
// 		{
// 			return Actor;
// 		}
// 	}
// 	return nullptr;
// // 	TArray<AActor*> OutActors;
// // 	UGameplayStatics::GetAllActorsOfClass(WorldContextObject, ActorClass, OutActors);
// // 
// // 	for (AActor* actor : OutActors)
// // 	{
// // 		if (IsValid(actor))
// // 		{
// // 			if (actor->GetFName() == name)
// // 				return actor;
// // 		}
// // 	}
// // 
// // 	return nullptr;
// }
//
//
// bool UUtilFunctionLibrary::FindActorByTag(UObject* WorldContextObject, FName tag, TArray<AActor*>& OutActors)
// {
// 	UWorld* InWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
// 	if (InWorld == nullptr)
// 		return false;
// 
// 	bool IsExistActor = false;
// 	for (TActorIterator<AActor> ActorItr(InWorld); ActorItr; ++ActorItr)
// 	{
// 		AActor* Actor = *ActorItr;
// 		if (IsValid(Actor) && Actor->Tags.Contains(tag)) 
// 		{
// 			OutActors.Emplace(Actor);
// 			IsExistActor = true;
// 		}
// 	}
// 	
// 	if(!IsExistActor)
// 		return false;
// 
// 	return true;
// }

AActor* UUtilFunctionLibrary::FindSpawnBox(FName name)
{
	UPlayState* Playstate = Cast<UPlayState>(RGAMEINSTANCE(GEngine)->GetCurrentState());
	if (Playstate)
	{
		ASpawnBox** FoundSpawnBox = Playstate->SpawnBoxMap.Find(name);
		if (FoundSpawnBox)
			return *FoundSpawnBox;
	}
	
	return nullptr;
}

AActor* UUtilFunctionLibrary::FindWaypoint(FName name)
{
	UPlayState* Playstate = Cast<UPlayState>(RGAMEINSTANCE(GEngine)->GetCurrentState());
	if (Playstate)
	{
		ASpawnerTransform** FoundWaypoint = Playstate->WaypointMap.Find(name);
		if (FoundWaypoint)
			return *FoundWaypoint;
	}

	return nullptr;
}

AWallActor* UUtilFunctionLibrary::FindWall(FName name)
{
	UPlayState* Playstate = Cast<UPlayState>(RGAMEINSTANCE(GEngine)->GetCurrentState());
	if (Playstate)
	{
		AWallActor** FoundWall = Playstate->WallMap.Find(name);
		if (FoundWall)
			return *FoundWall;
	}

	return nullptr;
}

/*
related matenee
*/

bool UUtilFunctionLibrary::PlayMatineeActor(UWorld* InWorld, FString MatineeActorName, FString MatineeCameraName, AActor* TargetActor)
{
	AMatineeActor* MatineeActor = GetMatineeActor(InWorld, MatineeActorName);
	if (IsValid(MatineeActor))
	{
		if (IsValid(TargetActor))
		{
			ACameraActor* MatineeCameraActor = GetMatineeCameraActor(InWorld, MatineeCameraName);
			if (MatineeCameraActor != nullptr)
			{
				AEmptyBasicActor* MatineePosition = InWorld->SpawnActor<AEmptyBasicActor>(AEmptyBasicActor::StaticClass(), TargetActor->GetTransform());
				if (MatineePosition)
				{
					MatineeCameraActor->AttachToActor(MatineePosition, FAttachmentTransformRules::KeepRelativeTransform);
					UE_LOG(LogGamePlay, Log, TEXT(" %f %f %f"), MatineePosition->GetActorLocation().X, MatineePosition->GetActorLocation().Y, MatineePosition->GetActorLocation().Z);
				}
			}
		}
		MatineeActor->SetPosition(0.0f);
		MatineeActor->Play();


		auto RGameInstance = RGAMEINSTANCE(GEngine);
		RGameInstance->RWidgetManager->CreateUserWidget(EWidgetBluePrintEnum::WBP_SkipButton);
		RGameInstance->RWidgetManager->ShowUserWidget(EWidgetBluePrintEnum::WBP_SkipButton);

		ViewTargetBaseCamera(InWorld);

		return true;
	}
	UE_LOG(LogGamePlay, Warning, TEXT("MatineeActor fail - %s"), *(MatineeActorName));
	return false;
}

bool UUtilFunctionLibrary::PlayNoneSkipMatineeActor(UWorld* InWorld, FString MatineeActorName, FString MatineeCameraName, AActor* TargetActor)
{
	AMatineeActor* MatineeActor = GetMatineeActor(InWorld, MatineeActorName);
	if (IsValid(MatineeActor))
	{
		if (IsValid(TargetActor))
		{
			ACameraActor* MatineeCameraActor = GetMatineeCameraActor(InWorld, MatineeCameraName);
			if (MatineeCameraActor != nullptr)
			{
				AEmptyBasicActor* MatineePosition = InWorld->SpawnActor<AEmptyBasicActor>(AEmptyBasicActor::StaticClass(), TargetActor->GetTransform());
				if (MatineePosition)
				{
					MatineeCameraActor->AttachToActor(MatineePosition, FAttachmentTransformRules::KeepRelativeTransform);
					UE_LOG(LogGamePlay, Log, TEXT(" %f %f %f"), MatineePosition->GetActorLocation().X, MatineePosition->GetActorLocation().Y, MatineePosition->GetActorLocation().Z);
				}
			}
		}
		MatineeActor->SetPosition(0.0f);
		MatineeActor->Play();

		ViewTargetBaseCamera(InWorld);

		return true;
	}
	UE_LOG(LogGamePlay, Warning, TEXT("MatineeActor fail - %s"), *(MatineeActorName));
	return false;
}

bool UUtilFunctionLibrary::StopMatineeActor(UWorld* InWorld, FString MatineeActorName)
{
	AMatineeActor* MatineeActor = GetMatineeActor(InWorld, MatineeActorName);
	if (IsValid(MatineeActor))
	{
		MatineeActor->Stop();
		return true;
	}	

	return false;
}

AMatineeActor* UUtilFunctionLibrary::GetPlayingMatineeActor(UObject* WorldContextObject)
{
	UWorld* InWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	if (InWorld == nullptr)
		return nullptr;

	for (TActorIterator<AMatineeActor> ActorItr(InWorld); ActorItr; ++ActorItr)
	{
		AMatineeActor* MatineeActor = *ActorItr;
		if (IsValid(MatineeActor) && MatineeActor->bIsPlaying)
		{
			return MatineeActor;
		}
	}
	return nullptr;
}


FString UUtilFunctionLibrary::GetPlayingMatineeActorName(UObject* WorldContextObject)
{
	UWorld* InWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	if (InWorld == nullptr)
		return FString("");

	for (TActorIterator<AMatineeActor> ActorItr(InWorld); ActorItr; ++ActorItr)
	{
		AMatineeActor* MatineeActor = *ActorItr;
		if (IsValid(MatineeActor) && MatineeActor->bIsPlaying)
		{
			return MatineeActor->GetName();
		}
	}
	return FString("");
}

bool UUtilFunctionLibrary::SkipMatineeActor(UObject* WorldContextObject, FString MatineeActorName)
{
	UWorld* InWorld = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	if (InWorld == nullptr)
		return false;

	AMatineeActor* MatineeActor = GetMatineeActor(InWorld, MatineeActorName);
	if (IsValid(MatineeActor))
	{
		MatineeActor->Stop();
		MatineeActor->NotifyEventTriggered(MATINEE_NOTI_END, true);
		MatineeActor->NotifyEventTriggered(MATINEE_NOTI_FINISHED, true);

		return true;
	}

	return false;
}


AMatineeActor* UUtilFunctionLibrary::GetMatineeActor(UWorld* InWorld, FString MatineeActorName)
{
	if (InWorld == nullptr)
		return nullptr;

	//TArray<AMatineeActor*> tempList;
	//for (TActorIterator<AMatineeActor> ActorItr(InWorld); ActorItr; ++ActorItr)
	//{
	//	AMatineeActor* MatineeActor = *ActorItr;
	//	if (IsValid(MatineeActor))
	//	{
	//		tempList.Add(MatineeActor);
	//	}
	//}

	for (TActorIterator<AMatineeActor> ActorItr(InWorld); ActorItr; ++ActorItr)
	{
		AMatineeActor* MatineeActor = *ActorItr;
		if (IsValid(MatineeActor) && MatineeActor->GetName().Contains(MatineeActorName))
		{
			return MatineeActor;
		}
	}
	return nullptr;
}

ACameraActor* UUtilFunctionLibrary::GetMatineeCameraActor(UWorld* InWorld, FString MatineeCameraName)
{
	for (TActorIterator<ACameraActor> ActorItr(InWorld); ActorItr; ++ActorItr)
	{
		ACameraActor* MatineeCameraActor = *ActorItr;
		if(IsValid(MatineeCameraActor) && MatineeCameraActor->GetName().Contains(MatineeCameraName))
		{
			return MatineeCameraActor;
		}
	}

	return nullptr;
}

void UUtilFunctionLibrary::DestroyMatineeActorByName(UWorld* InWorld, FString MatineeActorName)
{
	AMatineeActor* MatineeActor = GetMatineeActor(InWorld, MatineeActorName);
	DestroyMatineeActorByActor(MatineeActor);
}

void UUtilFunctionLibrary::DestroyMatineeActorByActor(AMatineeActor* MatineeActor)
{
	if (IsValid(MatineeActor))
	{
		TArray<AActor*> OutActors;
		MatineeActor->GetControlledActors(OutActors);
		int32 actor_num = OutActors.Num();
		//UE_LOG(LogGamePlay, Log, TEXT("======================================"));
		//UE_LOG(LogGamePlay, Log, TEXT("===>>>> DestroyMatineeActor Actors(%d)"), actor_num);
		for (int32 n = 0; n < actor_num; ++n)
		{
			if (IsValid(OutActors[n]))
			{
				TArray<AActor*> OutAttachedActors;
				OutActors[n]->GetAttachedActors(OutAttachedActors);
				for (auto AttachedActor : OutAttachedActors)
				{
					if (IsValid(AttachedActor))
					{
						AttachedActor->Destroy();
					}
					AttachedActor = nullptr;
				}

				//UE_LOG(LogGamePlay, Log, TEXT("===>>>> DestroyMatineeActor Actor[%d] = %s"), n, *(OutActors[n]->GetName()));
				OutActors[n]->Destroy();
			}
		}
		MatineeActor->Destroy();
	}
}

AActor* UUtilFunctionLibrary::SetActorVictoryPosition(UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	if (World == nullptr)
		return nullptr;

	for (TActorIterator<AVictoryPosition> ActorItr(World); ActorItr; ++ActorItr)
	{
		AVictoryPosition* VictoryPosition = *ActorItr;
		if (IsValid(VictoryPosition))
		{
			VictoryPosition->SetActorVictoryPosition();
			return VictoryPosition;
		}
	}

	return nullptr;
}

void UUtilFunctionLibrary::ShowColosseumTeamAnimMontageEvent(URGameInstance* InRGameInstance, const FName SectionName, bool PositionRight)
{
	int32 PartyID = PositionRight ? PartyID_COLOSSEUM_RIGHT : PartyID_COLOSSEUM_LEFT;

	TWeakObjectPtr< UParty > TeamData = InRGameInstance->PartyManager->GetPartyByPartyID(PartyID);
	if (false == TeamData.IsValid())
		return;

	PlayAnimMontageEventFromTeam(TeamData, SectionName);
}

void UUtilFunctionLibrary::PlayAnimMontageEventFromTeam(TWeakObjectPtr<UParty> TeamData, const FName SectionName)
{
	if (false == TeamData.IsValid())
		return;
	
	TArray< TWeakObjectPtr < AActor > > TeamMemberList;
	TeamData->GetPartyMember(TeamMemberList);

	for (auto TeamMember : TeamMemberList)
	{
		if (false == TeamMember.IsValid())
			continue;

		ACharacter* SpawnedCharacter = Cast< ACharacter >(TeamMember.Get());
		if (false == IsValid(SpawnedCharacter))
			continue;
		UBattleBaseAnimInstance* BattleBaseAnimInstance = UCharacterHelpFunctionLibrary::GetCharacterBattleBaseAnimInstance(SpawnedCharacter);
		if (false == IsValid(BattleBaseAnimInstance))
			continue;
		if (BattleBaseAnimInstance->AnimState == EAnimStateEnum::VE_Die)
			continue;

		BattleBaseAnimInstance->PlayAnimMontageEventSection(SectionName);
	}
	TeamData->StopSkillEffect();
}


/*
Sequence
*/
// 
// ALevelSequenceActor* UUtilFunctionLibrary::GetSequenceActor(UWorld* InWorld, FString SequenceActorName)
// {
// 	if (InWorld == nullptr)
// 		return nullptr;
// 
// 	for (TActorIterator<ALevelSequenceActor> ActorItr(InWorld); ActorItr; ++ActorItr)
// 	{
// 		ALevelSequenceActor* SequenceActor = *ActorItr;
// 		if (IsValid(SequenceActor) && SequenceActor->GetName().Contains(SequenceActorName))
// 		{
// 			return SequenceActor;
// 		}
// 	}
// 
// 	return nullptr;
// }
// 
// ALevelSequenceActor* UUtilFunctionLibrary::GetFirstFindSequenceActor(UWorld* InWorld)
// {
// 	if (InWorld == nullptr)
// 		return nullptr;
// 
// 	for (TActorIterator<ALevelSequenceActor> ActorItr(InWorld); ActorItr; ++ActorItr)
// 	{
// 		ALevelSequenceActor* SequenceActor = *ActorItr;
// 		if (IsValid(SequenceActor))
// 		{
// 			return SequenceActor;
// 		}
// 	}
// 
// 	return nullptr;
// }


void UUtilFunctionLibrary::PauseSequence(URGameInstance* InRGameInstance)
{
	InRGameInstance->SequenceManager->PauseSequence();
}

void UUtilFunctionLibrary::ShowVictorySequence(URGameInstance* InRGameInstance)
{
	UWorld* World = InRGameInstance->GetWorld();
	if (!IsValid(World))
		return;

	UUIFunctionLibrary::OnShowInGameHUD(World, false);
	UUIFunctionLibrary::ActivateJoystick(World, false);
	InRGameInstance->SetIsVictoryState(true);

	// Victory Position
	AActor* InActor = SetActorVictoryPosition(InRGameInstance);
	if (IsValid(InActor))
	{
		// Victory Sequence
		InRGameInstance->SequenceManager->PlaySequenceAtTargetActorLocation(SEQUENCE_VICTORY, ESequenceActorType::VE_General, InActor);

		APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(InRGameInstance->GetWorld(), 0);
		if (PCM)
			PCM->StopCameraFade();
	}
}

void UUtilFunctionLibrary::ShowVictoryAnim(URGameInstance* InRGameInstance)
{
	TWeakObjectPtr< UParty > TeamData = InRGameInstance->PartyManager->GetMyParty();
	if (!TeamData.IsValid())
		return;

	// PlayAnimation
	PlayAnimMontageEventFromTeam(TeamData, ANIM_MONTAGE_EVENT_BATTLEWIN);
}

// AStageBase* UUtilFunctionLibrary::GetStageSpawner(URGameInstance* InRGameInstance)
// {
// 	if (!IsValid(InRGameInstance))
// 		return nullptr;
// 	
// 	AStageBase*	StageSpawner = nullptr;
// 	auto RGamePlayMode = GetGamePlayModeByRGameInstance(InRGameInstance);
// 	if (IsValid(RGamePlayMode))
// 	{
// 		UPhaseMode* PhaseMode = Cast<UPhaseMode>(RGamePlayMode);
// 		if (IsValid(PhaseMode))
// 		{
// 			StageSpawner = PhaseMode->GetCurStageBase();
// 		}
// 		else
// 		{
// 			StageSpawner = RGamePlayMode->GetStageSpawner();
// 		}
// 	}
// 
// 	return StageSpawner;
// }

void UUtilFunctionLibrary::BossName(AActor* InBossActor)
{
	if (!IsValid(InBossActor))
		return;

	RGAMEINSTANCE(GEngine)->RWidgetManager->ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_CampaignBossSpawn);

	UUP_CampaignBossSpawn* CampaignBossSpawn = RGAMEINSTANCE(GEngine)->RWidgetManager->GetUserWidgetT<UUP_CampaignBossSpawn>(EWidgetBluePrintEnum::WBP_CampaignBossSpawn);
	if (IsValid(CampaignBossSpawn))
		CampaignBossSpawn->SetBossActor(InBossActor);
}

void UUtilFunctionLibrary::RaidBossName(URGameInstance* InRGameInstance, bool Show)
{
	AProjectRGameMode* GameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(InRGameInstance->GetWorld()));


	if (IsValid(GameMode))
	{
		FGameModeSequenceTableInfo* GameModeSequenceInfo = UTableManager::GetInstancePtr()->GetGameModeSequenceRow(GameMode->GameModeSequenceKey);
		if (GameModeSequenceInfo)
		{
			if (InRGameInstance->RWidgetManager->GetUserWidget(EWidgetBluePrintEnum::WBP_RaidBossSpawn) != nullptr)
			{
				UUP_RaidBossSpawn* RaidBossSpawn = InRGameInstance->RWidgetManager->GetUserWidgetT<UUP_RaidBossSpawn>(EWidgetBluePrintEnum::WBP_RaidBossSpawn);
				if (IsValid(RaidBossSpawn))
				{
					RaidBossSpawn->BP_ShowName(GameModeSequenceInfo->GameModeSequenceType, Show);
				}
			}
		}
	}
}

void UUtilFunctionLibrary::StageClearSequenceEnd(URGameInstance* InRGameInstance)
{
	bool bExe = true;
	AStageGameMode* RGamePlayMode = Cast<AStageGameMode>(UUtilFunctionLibrary::GetGamePlayMode());
	if (IsValid(RGamePlayMode))
	{
		bExe = !false;
		//End_Movie Event?
	}

	if(bExe)
	{
		ShowVictorySequence(InRGameInstance);
	}
	return;
	
	
}



/*
Component Helper
*/
void UUtilFunctionLibrary::SettingStaticMeshComponent(AActor* InOwner, UStaticMeshComponent* InTarget,
	FName InStaticMeshTableName /*= FName()*/, float InOffsetZ /*= 0*/)
{
	if (InOwner == nullptr || InTarget == nullptr)
		return;

	if (InStaticMeshTableName.IsNone() == false)
	{
		UStaticMesh* StaticMesh = UUtilFunctionLibrary::GetStaticMeshFromTable(InStaticMeshTableName);
		InTarget->SetStaticMesh(StaticMesh);
	}

	InTarget->OnComponentCreated();
	InTarget->RegisterComponent();
	InTarget->AttachToComponent(InOwner->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	if (InOffsetZ != 0)
	{
		ACharacter* OwnerCharacter = Cast< ACharacter >(InOwner);
		if (OwnerCharacter)
		{
			//auto CapsuleComp = OwnerCharacter->GetCapsuleComponent();
			//if (CapsuleComp)
			{
				FVector Offset(0, 0, -(OwnerCharacter->GetDefaultHalfHeight()) + InOffsetZ);
				InTarget->SetRelativeLocation(Offset);
				InTarget->SetAbsolute(false, true);
			}
		}
	}

	UUtilFunctionLibrary::OptimizeStaticMeshComponent(InTarget);
	InTarget->SetComponentTickEnabled(false);
	InTarget->SetVisibility(false);
}

void UUtilFunctionLibrary::OptimizeStaticMeshComponent(UStaticMeshComponent* InTarget)
{
	if (InTarget == nullptr)
		return;
	
	//Collisiton
	InTarget->bGenerateOverlapEvents = false;
	InTarget->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InTarget->SetCanEverAffectNavigation(false);

	//Physics
	InTarget->SetEnableGravity(false);
	
	//Light
	InTarget->SetCastShadow(false);
	InTarget->IndirectLightingCacheQuality = EIndirectLightingCacheQuality::ILCQ_Off;
	
	//Rendering
	InTarget->bReceivesDecals = false;
	InTarget->bVisibleInReflectionCaptures = false;
	InTarget->bUseAttachParentBound = true;
}


/*
related Asset
*/

UClass* UUtilFunctionLibrary::GetCharacterClassFromAssetID( TAssetSubclassOf< ABattleBaseCharacter > AssetID)
{
	if (AssetID.IsNull())
		return nullptr;

	UClass* CharacterClass = AssetID.Get();
	if (CharacterClass == nullptr)
	{
		UE_LOG(LogGamePlay, Log, TEXT(" <><><><><><><> Get asset id : %s"), *(AssetID.ToSoftObjectPath().ToString()));
		AssetID.ToSoftObjectPath().TryLoad();
		CharacterClass = AssetID.Get();

		if (!GEngine || !RGAMEINSTANCE(GEngine) || !IsValid(RGAMEINSTANCE(GEngine)))
			return CharacterClass;

		UWorld* World = RGAMEINSTANCE(GEngine)->GetWorld();
		if (World)
		{
			AProjectRGameMode* RGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(World));
			if (RGameMode)
				RGameMode->AddTryCntLoadSync(AssetID.ToString());
		}
	}
	return CharacterClass;
}

// UClass* UUtilFunctionLibrary::GetLobbyActorClassFromAssetID(TAssetSubclassOf< ALobbyActor > AssetID)
// {
// 	UClass* LobbyActorClass = AssetID.Get();
// 	if (LobbyActorClass == nullptr)
// 	{
// 		AssetID.ToSoftObjectPath().TryLoad();
// 		LobbyActorClass = AssetID.Get();
// 	}
// 	return LobbyActorClass;
// }

UClass* UUtilFunctionLibrary::GetWeaponBaseFromAssetID( TAssetSubclassOf< AWeaponBase > AssetID)
{
	if (AssetID.IsNull())
		return nullptr;

	UClass* WeaponUClass = AssetID.Get();
	if (WeaponUClass == nullptr)
	{
		AssetID.ToSoftObjectPath().TryLoad();
		WeaponUClass = AssetID.Get();

		if (!GEngine || !RGAMEINSTANCE(GEngine) || !IsValid(RGAMEINSTANCE(GEngine)))
			return WeaponUClass;

		UWorld* World = RGAMEINSTANCE(GEngine)->GetWorld();
		if (World)
		{
			AProjectRGameMode* RGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(World));
			if (RGameMode)
				RGameMode->AddTryCntLoadSync(AssetID.ToString());
		}
	}
	return WeaponUClass;
}

UClass* UUtilFunctionLibrary::GetBulletBaseFromAssetID( TAssetSubclassOf< ABulletBase > AssetID)
{
	if (AssetID.IsNull())
		return nullptr;

	UClass* BulletUClass = AssetID.Get();
	if (BulletUClass == nullptr)
	{
		AssetID.ToSoftObjectPath().TryLoad();
		BulletUClass = AssetID.Get();

		if (!GEngine || !RGAMEINSTANCE(GEngine) || !IsValid(RGAMEINSTANCE(GEngine)))
			return BulletUClass;

		UWorld* World = RGAMEINSTANCE(GEngine)->GetWorld();
		if (World)
		{
			AProjectRGameMode* RGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(World));
			if (RGameMode)
				RGameMode->AddTryCntLoadSync(AssetID.ToString());
		}
	}
	return BulletUClass;
}


UClass* UUtilFunctionLibrary::GetBaseWidgetFromAssetID(TAssetSubclassOf< URBaseWidget > AssetID)
{
	if (AssetID.IsNull())
		return nullptr;

	UClass* WidgetUClass = AssetID.Get();
	if (WidgetUClass == nullptr)
	{
		AssetID.ToSoftObjectPath().TryLoad();
		WidgetUClass = AssetID.Get();

		if (!GEngine || !RGAMEINSTANCE(GEngine) || !IsValid(RGAMEINSTANCE(GEngine)))
			return WidgetUClass;

		UWorld* World = RGAMEINSTANCE(GEngine)->GetWorld();
		if (World)
		{
			AProjectRGameMode* RGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(World));
			if (RGameMode)
				RGameMode->AddTryCntLoadSync(AssetID.ToString());
		}
	}
	return WidgetUClass;
}


UStaticMesh* UUtilFunctionLibrary::GetStaticMeshFromAssetID(TAssetPtr< UStaticMesh > AssetID)
{
	if (AssetID.IsNull())
		return nullptr;

	UStaticMesh* StaticMesh = AssetID.Get();
	if (StaticMesh == nullptr)
	{
		StaticMesh = AssetID.LoadSynchronous();


		if (!GEngine || !RGAMEINSTANCE(GEngine) || !IsValid(RGAMEINSTANCE(GEngine)))
			return StaticMesh;

		UWorld* World = RGAMEINSTANCE(GEngine)->GetWorld();
		if (World)
		{
			AProjectRGameMode* RGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(World));
			if (RGameMode)
				RGameMode->AddTryCntLoadSync(AssetID.ToString());
		}
	}

	return StaticMesh;
}

USkeletalMesh* UUtilFunctionLibrary::GetSkeletalMeshFromAssetID(TAssetPtr< USkeletalMesh > AssetID)
{
	if (AssetID.IsNull())
		return nullptr;

	USkeletalMesh* SkeletalMesh = AssetID.Get();
	if (SkeletalMesh == nullptr)
	{
		SkeletalMesh = AssetID.LoadSynchronous();

		if (!GEngine || !RGAMEINSTANCE(GEngine) || !IsValid(RGAMEINSTANCE(GEngine)))
			return SkeletalMesh;

		UWorld* World = RGAMEINSTANCE(GEngine)->GetWorld();
		if (World)
		{
			AProjectRGameMode* RGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(World));
			if (RGameMode)
				RGameMode->AddTryCntLoadSync(AssetID.ToString());
		}

	}

	return SkeletalMesh;
}

UMaterial* UUtilFunctionLibrary::GetMaterialFromAssetID(TAssetPtr< UMaterial > AssetID)
{
	if (AssetID.IsNull())
		return nullptr;

	UMaterial* Material = AssetID.Get();
	if (Material == nullptr)
	{
		Material = AssetID.LoadSynchronous();

		if (!GEngine || !RGAMEINSTANCE(GEngine) || !IsValid(RGAMEINSTANCE(GEngine)))
			return Material;

		UWorld* World = RGAMEINSTANCE(GEngine)->GetWorld();
		if (World)
		{
			AProjectRGameMode* RGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(World));
			if (RGameMode)
				RGameMode->AddTryCntLoadSync(AssetID.ToString());
		}
	}

	return Material;
}

UMaterialInstance* UUtilFunctionLibrary::GetMaterialInstanceFromAssetID(TAssetPtr< UMaterialInstance > AssetID)
{
	if (AssetID.IsNull())
		return nullptr;

	UMaterialInstance* MaterialInstance = AssetID.Get();
	if (MaterialInstance == nullptr)
	{
		MaterialInstance = AssetID.LoadSynchronous();

		if (!GEngine || !RGAMEINSTANCE(GEngine) || !IsValid(RGAMEINSTANCE(GEngine)))
			return MaterialInstance;

		UWorld* World = RGAMEINSTANCE(GEngine)->GetWorld();
		if (World)
		{
			AProjectRGameMode* RGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(World));
			if (RGameMode)
				RGameMode->AddTryCntLoadSync(AssetID.ToString());
		}

	}

	return MaterialInstance;
}

UParticleSystem* UUtilFunctionLibrary::GetParticleSystemFromAssetID(TAssetPtr< UParticleSystem > AssetID)
{
#if !LOAD_PARTICLE_RESOURCE
	return nullptr;
#endif
	if (AssetID.IsNull())
		return nullptr;

	UParticleSystem* ParticleSystem = AssetID.Get();
	if (ParticleSystem == nullptr)
	{
		ParticleSystem = AssetID.LoadSynchronous();

		if (!GEngine || !RGAMEINSTANCE(GEngine) || !IsValid(RGAMEINSTANCE(GEngine)))
			return ParticleSystem;

		UWorld* World = RGAMEINSTANCE(GEngine)->GetWorld();
		if (World)
		{
			AProjectRGameMode* RGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(World));
			if (RGameMode)
				RGameMode->AddTryCntLoadSync(AssetID.ToString());
		}

	}

	return ParticleSystem;
}

UTexture2D* UUtilFunctionLibrary::GetTexture2DFromAssetID(TAssetPtr< UTexture2D > AssetID)
{
	if (AssetID.IsNull())
		return nullptr;

	UTexture2D* Texture2D = AssetID.Get();
	if (Texture2D == nullptr)
	{
		Texture2D = AssetID.LoadSynchronous();
		

		if (!GEngine || !RGAMEINSTANCE(GEngine) || !IsValid(RGAMEINSTANCE(GEngine)))
			return Texture2D;

		UWorld* World = RGAMEINSTANCE(GEngine)->GetWorld();
		if (World)
		{
			AProjectRGameMode* RGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(World));
			if (RGameMode)
				RGameMode->AddTryCntLoadSync(AssetID.ToString());
		}
	}

	return Texture2D;
}

UPaperSprite* UUtilFunctionLibrary::GetSpriteFromAssetID(TAssetPtr< UPaperSprite > AssetID)
{
	if (AssetID.IsNull())
		return nullptr;

	UPaperSprite* Sprite = AssetID.Get();
	if (Sprite == nullptr)
	{
		Sprite = AssetID.LoadSynchronous();


		if (!GEngine || !RGAMEINSTANCE(GEngine) || !IsValid(RGAMEINSTANCE(GEngine)))
			return Sprite;

		UWorld* World = RGAMEINSTANCE(GEngine)->GetWorld();
		if (World)
		{
			AProjectRGameMode* RGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(World));
			if (RGameMode)
				RGameMode->AddTryCntLoadSync(AssetID.ToString());
		}
	}

	return Sprite;
}

USoundCue* UUtilFunctionLibrary::GetSoundCueFromAssetID(TAssetPtr< USoundCue > AssetID)
{
#if !LOAD_SOUND_RESOURCE
	return nullptr;
#endif
	if (AssetID.IsNull())
		return nullptr;

	USoundCue* SoundCue = AssetID.Get();
	if (SoundCue == nullptr)
	{
		SoundCue = AssetID.LoadSynchronous();

		if (!GEngine || !RGAMEINSTANCE(GEngine) || !IsValid(RGAMEINSTANCE(GEngine)))
			return SoundCue;


		UWorld* World = RGAMEINSTANCE(GEngine)->GetWorld();
		if (World)
		{
			AProjectRGameMode* RGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(World));
			if (RGameMode)
				RGameMode->AddTryCntLoadSync(AssetID.ToString());
		}
	}

	return SoundCue;
}

UAnimationAsset* UUtilFunctionLibrary::GetAnimationAssetFromAssetID(TAssetPtr< UAnimationAsset > AssetID)
{
	if (AssetID.IsNull())
		return nullptr;

	UAnimationAsset* AnimationAsset = AssetID.Get();
	if (AnimationAsset == nullptr)
	{
		AnimationAsset = AssetID.LoadSynchronous();


		if (!GEngine || !RGAMEINSTANCE(GEngine) || !IsValid(RGAMEINSTANCE(GEngine)))
			return AnimationAsset;

		UWorld* World = RGAMEINSTANCE(GEngine)->GetWorld();
		if (World)
		{
			AProjectRGameMode* RGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(World));
			if (RGameMode)
				RGameMode->AddTryCntLoadSync(AssetID.ToString());
		}

	}

	return AnimationAsset;
}

UAnimMontage* UUtilFunctionLibrary::GetAnimMontageFromAssetID(TAssetPtr< UAnimMontage > AssetID)
{
#if LOG_TIME
	//FScopedTimeElapsedLog TimeLog(AssetID.GetAssetName());
#endif
	if (AssetID.IsNull())
		return nullptr;


	UAnimMontage* AnimMontage = AssetID.Get();
	if (AnimMontage == nullptr)
	{
		AnimMontage = AssetID.LoadSynchronous();	

// 		if (!GEngine || !RGAMEINSTANCE(GEngine) || !IsValid(RGAMEINSTANCE(GEngine)) || !IsValid(RGAMEINSTANCE(GEngine)))
// 			return AnimMontage;
// 
// 		UWorld* World = RGAMEINSTANCE(GEngine)->GetWorld();
// 		if (World)
// 		{
// 			AProjectRGameMode* RGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(World));
// 			if (RGameMode)
// 				RGameMode->AddTryCntLoadSync(AssetID.ToString());
// 		}

	}

	return AnimMontage;
}

UCurveFloat* UUtilFunctionLibrary::GetCurveFloat(FName AssetName)
{
	FResourceCurveTableInfo* info = UTableManager::GetInstancePtr()->GetResourceCurveRow(AssetName);
	if (info)
		return Cast<UCurveFloat>(GetCurveBaseAssetID(info->Asset));

	return nullptr;
}

UCurveVector* UUtilFunctionLibrary::GetCurveVector(FName AssetName)
{
	FResourceCurveTableInfo* info = UTableManager::GetInstancePtr()->GetResourceCurveRow(AssetName);
	if (info)
		return Cast<UCurveVector>(GetCurveBaseAssetID(info->Asset));

	return nullptr;
}

UCurveBase* UUtilFunctionLibrary::GetCurveBaseAssetID(TAssetPtr< UCurveBase > AssetID)
{
	if (AssetID.IsNull())
		return nullptr;

	UCurveBase* Curve = AssetID.Get();
	if (Curve == nullptr)
	{
		Curve = AssetID.LoadSynchronous();

		if (!GEngine || !RGAMEINSTANCE(GEngine) || !IsValid(RGAMEINSTANCE(GEngine)))
			return Curve;

		UWorld* World = RGAMEINSTANCE(GEngine)->GetWorld();
		if (World)
		{
			AProjectRGameMode* RGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(World));
			if (RGameMode)
				RGameMode->AddTryCntLoadSync(AssetID.ToString());
		}

	}

	return Curve;
}

UStaticMesh* UUtilFunctionLibrary::GetStaticMeshFromTable( FName AssetName)
{
	FResourceStaticMeshTableInfo* info = UTableManager::GetInstancePtr()->GetResourceStaticMeshRow(AssetName);
	if (info)
		return GetStaticMeshFromAssetID(info->Asset);

	return nullptr;
}

UMaterialInstance*	UUtilFunctionLibrary::GetMaterialInstanceFromTable( FName AssetName)
{
	FResourceMaterialTableInfo* info = UTableManager::GetInstancePtr()->GetResourceMaterialRow(AssetName);
	if (info)
		return GetMaterialInstanceFromAssetID(info->Asset);

	return nullptr;
}

UParticleSystem* UUtilFunctionLibrary::GetParticleSystemFromTable( FName AssetName)
{
	FResourceParticleTableInfo* info = UTableManager::GetInstancePtr()->GetResourceParticleRow(AssetName);
	if (info)
		return GetParticleSystemFromAssetID(info->Asset);

	return nullptr;
}

UTexture2D* UUtilFunctionLibrary::GetTexture2DFromTable( FName AssetName)
{
	FResourceTexture2DTableInfo* info = UTableManager::GetInstancePtr()->GetResourceTexture2DRow(AssetName);
	if (info)
		return GetTexture2DFromAssetID(info->Asset);

	return nullptr;
}

UPaperSprite* UUtilFunctionLibrary::GetSpriteFromTable(FName AssetName)
{
	FResourceSpriteTableInfo* info = UTableManager::GetInstancePtr()->GetResourceSpriteRow(AssetName);
	if (info)
		return GetSpriteFromAssetID(info->Asset);

	return nullptr;
}


FSlateBrush* UUtilFunctionLibrary::GetSpriteToBrush(UPaperSprite* Sprite)
{
	TSharedPtr<FSlateBrush> brush = MakeShareable(new FSlateBrush());
	brush->SetResourceObject(Sprite);

	return brush.Get();
}

FSlateBrush* UUtilFunctionLibrary::GetTexture2DToBrush(UTexture2D* Texture2D)
{
	TSharedPtr<FSlateBrush> brush = MakeShareable(new FSlateBrush());
	brush->SetResourceObject(Texture2D);

	return brush.Get();
}

/*
related Userwidget
*/
UUserWidget* UUtilFunctionLibrary::CreateUserWidget( UWorld* InWorld, FName WidgetID)
{
	FWidgetBluePrintTableInfo* Result = UTableManager::GetInstancePtr()->GetWidgetBluePrintRow(WidgetID);
	if (Result)
	{
		UUserWidget* userWidget = CreateWidget<UUserWidget>(InWorld, Result->GetWidgetClass());
		return userWidget;
	}
	return nullptr;
}

// UHpBarWidgetComponent* UUtilFunctionLibrary::AttachHpBarComp(AActor* InCharacter, FName InWidgetID)
// {
// 	UHpBarWidgetComponent* MyWidgetComponent = NewObject< UHpBarWidgetComponent >(InCharacter);
// 	if (MyWidgetComponent)
// 	{
// 		UEntityRenderComponent* RenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(InCharacter);
// 		if (RenderComponent)
// 		{
// 			MyWidgetComponent->AttachToComponent(RenderComponent->HeadUpWidget, FAttachmentTransformRules::KeepRelativeTransform);
// 		}
// 		else
// 		{
// 			MyWidgetComponent->AttachToComponent(InCharacter->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
// 		}
// 
// 		MyWidgetComponent->OnComponentCreated();
// 		MyWidgetComponent->RegisterComponent();
// 		MyWidgetComponent->WidgetInit();
// 		
// 	}
// 	return MyWidgetComponent;
// }

UWidgetDecoratorComponent* UUtilFunctionLibrary::AttachCharacterWidget(AActor* Character, FName WidgetID, FTransform Pivot, ECharacterCategoryTypeEnum InCharacterCategory)
{
	if (!IsValid(Character))
		return nullptr;

	// Add Component
	UWidgetDecoratorComponent* MyWidgetComponent = NewObject< UWidgetDecoratorComponent >(Character);
	if (MyWidgetComponent)
	{
		MyWidgetComponent->SetWidgetID(WidgetID);
		MyWidgetComponent->SetCharacterCategory(InCharacterCategory);

		MyWidgetComponent->OnComponentCreated();
		MyWidgetComponent->RegisterComponent();

		MyWidgetComponent->AttachToComponent(Character->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

		MyWidgetComponent->SetRelativeTransform(Pivot);
		return MyWidgetComponent;
	}

	return nullptr;
}

USpeechBubbleComponent*	UUtilFunctionLibrary::AttachCharacterSpeechBubbleWidget(AActor* Character, FName WidgetID, FTransform Pivot)
{
	if (!IsValid(Character))
		return nullptr;

	// Add Component
	USpeechBubbleComponent* MyWidgetComponent = NewObject< USpeechBubbleComponent >(Character);
	if (MyWidgetComponent)
	{
		MyWidgetComponent->SetWidgetID(WidgetID);

		MyWidgetComponent->OnComponentCreated();
		MyWidgetComponent->RegisterComponent();

		MyWidgetComponent->AttachToComponent(Character->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

		MyWidgetComponent->SetRelativeTransform(Pivot);
		return MyWidgetComponent;
	}

	return nullptr;
}


// UCustomWidgetComponent* UUtilFunctionLibrary::AttachWidgetComponent(AActor* Character, FName WidgetID, FTransform Pivot)
// {
// 	if (!IsValid(Character))
// 		return nullptr;
// 
// 	// Add Component
// 	UCustomWidgetComponent* MyWidgetComponent = NewObject< UCustomWidgetComponent >(Character);
// 	if (MyWidgetComponent)
// 	{
// 		UUserWidget* WidgetDecorator = CreateUserWidget(Character->GetWorld(), WidgetID);
// 		//MyWidgetComponent->SetWidgetSpace(true);
// 		//MyWidgetComponent->SetWidgetID(WidgetID);
// 		//MyWidgetComponent->SetRelativeRotation(FRotator(90, 0, 0));
// 		//MyWidgetComponent->SetBlendMode(EWidgetBlendMode::Transparent);
// 		MyWidgetComponent->OnComponentCreated();
// 		MyWidgetComponent->RegisterComponent();
// 		MyWidgetComponent->SetWidget(WidgetDecorator);
// 		
// 		MyWidgetComponent->AttachToComponent(Character->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
// 		
// 		UCharacterUserWidget* CharacterWidget = Cast< UCharacterUserWidget >(WidgetDecorator);
// 		if (CharacterWidget)
// 		{
// 			CharacterWidget->SetOwner(Character);
// 		}
// 		else
// 		{
// 			UE_LOG(LogBattle, Log, TEXT("CharacterUserWidget Cast fail"));
// 		}
// 		MyWidgetComponent->SetRelativeTransform(Pivot);
// 		MyWidgetComponent->SetRelativeRotation(FRotator(90, 0, 0));
// 		MyWidgetComponent->SetAbsolute(false, true);
// 		
// 		MyWidgetComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
// 
// 		return MyWidgetComponent;
// 	}
// 	else
// 	{
// 		UE_LOG(LogBattle, Log, TEXT("UCustomWidgetComponent NewObject fail"));
// 	}
// 
// 	return nullptr;
// }

void UUtilFunctionLibrary::ChangeZOrder(AActor* InCharacter)
{
	if (IsValid(InCharacter))
	{
		UEntityRenderComponent* RenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(InCharacter);
		if (IsValid(RenderComponent))
		{
			UWidgetDecoratorComponent* WidgetComponent = RenderComponent->GetHpBarWidgetComponent();
			if (IsValid(WidgetComponent))
			{
				WidgetComponent->RefreshWidgetZOrder();
			}
		}
	}
}

/*
Camera
*/
ACharacterSetCameraActor* UUtilFunctionLibrary::GetCharacterSetCamera(UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	if (World == nullptr)
		return nullptr;

	ACharacterSetCameraActor* CharacterSetCamera = nullptr;
	for (TActorIterator<ACharacterSetCameraActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		if ((*ActorItr)->ActorHasTag(FName(TEXT("UICharacterPivot"))))
		{
			CharacterSetCamera = *ActorItr;
			break;
		}
	}
	if (!IsValid(CharacterSetCamera))
		return nullptr;

	return CharacterSetCamera;
}

ALobbyCameraActor* UUtilFunctionLibrary::GetLobbyCameraActor(UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	if (World == nullptr)
		return nullptr;

	ALobbyCameraActor* LobbyCameraActor = nullptr;
	for (TActorIterator<ALobbyCameraActor> ActorItr(World); ActorItr; ++ActorItr)
	{
		if ((*ActorItr)->ActorHasTag(FName(TEXT("LobbyCharacterCamera"))))
		{
			LobbyCameraActor = *ActorItr;
			break;
		}
	}

	return LobbyCameraActor;
}

bool UUtilFunctionLibrary::ViewTargetBaseCamera(UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	if (World == nullptr)
		return false;

	auto RGameInstance = RGAMEINSTANCE(GEngine);
	if (RGameInstance == nullptr)
		return false;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!IsValid(PlayerController))
		return false;

	PlayerController->SetViewTarget(RGameInstance->CameraActor);

	return true;
}


UCameraComponent* UUtilFunctionLibrary::GetActiveCameraComponent(UWorld* InWorld, int32 InPlayerIdx /*= 0*/)
{
	APlayerCameraManager* PlayerCameraManager = UGameplayStatics::GetPlayerCameraManager(InWorld, InPlayerIdx);
	if (IsValid(PlayerCameraManager))
	{
		TArray<UCameraComponent*> Cameras;
		PlayerCameraManager->GetViewTarget()->GetComponents<UCameraComponent>(Cameras);

		if (Cameras.IsValidIndex(0))
			return Cameras[0];
	}

	return nullptr;
}

void UUtilFunctionLibrary::SetLobbyViewTarget_PCM()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	if (!RGameInstance)
		return;
	UWorld* world = RGameInstance->GetWorld();
	if (!world)
		return;
	
	APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(world, 0);
	if (PCM)
	{
		FViewTargetTransitionParams BlendParams;
		BlendParams.BlendTime = 1.f;

		ATowerLobbyGameMode* CurrentMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(world));
		AActor* CamActor = nullptr;
		if (CurrentMode->CurrentLobbyState == ETowerLobbyStateEnum::Outdoor)
		{
			AOutdoorLobbyStateActor* OutdoorStateActor = Cast<AOutdoorLobbyStateActor>(CurrentMode->GetLobbyStateActor(ETowerLobbyStateEnum::Outdoor));
			if (OutdoorStateActor)
				CamActor = OutdoorStateActor->OutdoorCameraActor.Get();
		}
		else if (CurrentMode->CurrentLobbyState == ETowerLobbyStateEnum::Indoor)
		{
			ATowerLobbyGameMode* TowerLobbyGameMode = RGameInstance->GetTowerLobbyGameMode();
			AIndoorLobbyStateActor* IndoorActor = nullptr;
			if (TowerLobbyGameMode)
				IndoorActor = Cast<AIndoorLobbyStateActor>(TowerLobbyGameMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
			if(IndoorActor)
				CamActor = IndoorActor->IndoorNormalViewCameraActor.Get();
		}
		PCM->SetViewTarget(CamActor, BlendParams);
		PCM->StopCameraFade();	
	}
}

void UUtilFunctionLibrary::SetIndoorViewTarget_PCM()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	if (!RGameInstance)
		return;
	UWorld* world = RGameInstance->GetWorld();
	if (!world)
		return;

	APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(world, 0);
	if (PCM)
	{
		FViewTargetTransitionParams BlendParams;
		BlendParams.BlendTime = 1.f;

		ATowerLobbyGameMode* CurrentMode = Cast<ATowerLobbyGameMode>(UGameplayStatics::GetGameMode(world));
		AActor* CamActor = nullptr;

		ATowerLobbyGameMode* TowerLobbyGameMode = RGameInstance->GetTowerLobbyGameMode();
		AIndoorLobbyStateActor* IndoorActor = nullptr;
		if (TowerLobbyGameMode)
			IndoorActor = Cast<AIndoorLobbyStateActor>(TowerLobbyGameMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
		if (IndoorActor)
			CamActor = IndoorActor->IndoorNormalViewCameraActor.Get();

		PCM->SetViewTarget(CamActor, BlendParams);
		PCM->StopCameraFade();
	}
}

void UUtilFunctionLibrary::SetTowerBossLobby()
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	if (!RGameInstance)
		return;
	UWorld* world = RGameInstance->GetWorld();
	if (!world)
		return;

	APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(world, 0);
	if (PCM)
	{
		FViewTargetTransitionParams BlendParams;
		BlendParams.BlendTime = 1.f;
		AActor* CamActor = RGameInstance->GetTowerLobbyGameMode()->IndoorLobbyStateAct->IndoorNormalViewCameraActor.Get();
		if (CamActor)
		{
// 			PCM->SetViewTarget(CamActor, BlendParams);
// 			PCM->StopCameraFade();
// 			//temp setting
// 			FVector newLoca = RGameInstance->GetTowerLobbyGameMode()->IndoorLobbyStateAct->IndoorOriginLocation;
// 			CamActor->SetActorLocation(newLoca + FVector(0, 700, 50));
		}
	}
}

void UUtilFunctionLibrary::SetTowerBossCameraLocation(FName BossName)
{
	FLobbyCharacterTableInfo* LobbyCharacterTableInfo = FLobbyCharacterTableInfo::LookUp(BossName);
	AIndoorLobbyStateActor* LobbyStateActor = Cast<AIndoorLobbyStateActor>(RGAMEINSTANCE(GEngine)->GetTowerLobbyGameMode()->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));

	if (LobbyCharacterTableInfo && LobbyStateActor)
	{
		FVector DefaultCameraLocation = LobbyCharacterTableInfo->DefaultCameraPosition;
		FRotator DefaultCameraRotation = LobbyCharacterTableInfo->DefaultCameraRotator;
		FVector ZoomInCameraLocation = LobbyCharacterTableInfo->ZoomInCameraPosition;
		FRotator ZoomInCameraRotation = LobbyCharacterTableInfo->ZoomInCameraRotator;
		float CurrentZoomInLerpAlpha;
		if (LobbyCharacterTableInfo->ZoonInInvert)
		{
			CurrentZoomInLerpAlpha = 1.0f;
		}
		else
		{
			CurrentZoomInLerpAlpha = 0.0f;
		}
		FVector Location = FMath::Lerp(DefaultCameraLocation, ZoomInCameraLocation, CurrentZoomInLerpAlpha);
		FRotator Rotation = FMath::Lerp(DefaultCameraRotation, ZoomInCameraRotation, CurrentZoomInLerpAlpha);
		LobbyStateActor->IndoorNormalViewCameraActor->GetCameraComponent()->SetFieldOfView(LobbyCharacterTableInfo->FieldOfView);
		LobbyStateActor->IndoorNormalViewCameraActor->SetActorLocation(Location);
		LobbyStateActor->IndoorNormalViewCameraActor->SetActorRotation(Rotation);
	}
}

void UUtilFunctionLibrary::SetGachaSHopViewTarget_PCM(float InBlendTime)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	if (!RGameInstance)
		return;
	UWorld* world = RGameInstance->GetWorld();
	if (!world)
		return;

	APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(world, 0);
	if (PCM)
	{
		FViewTargetTransitionParams BlendParams;
		BlendParams.BlendTime = InBlendTime;

		UUP_HeroShop* UP_HeroShop = RGameInstance->RWidgetManager->GetUserWidgetT<UUP_HeroShop>(EWidgetBluePrintEnum::WBP_ShopHero);
		if (UP_HeroShop)
		{
			AActor* CamActor = UP_HeroShop->GetGachaViewTarget();
			PCM->SetViewTarget(CamActor, BlendParams);
		}
	}
}


/*
Cheat
*/
bool UUtilFunctionLibrary::IsCheatCoolTimeZero(UWorld* InWorld)
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	if (RGameInstance == nullptr)
		return false;
	return RGameInstance->ConsoleCheatManager->bCoolTimeZero;
}

// bool UUtilFunctionLibrary::IsCheatSkillCostZero(UWorld* InWorld)
// {
// 	auto RGameInstance = RGAMEINSTANCE(GEngine);
// 	if (RGameInstance == nullptr)
// 		return false;
// 
// 	return RGameInstance->ConsoleCheatManager->bSkillCostZero;
// }

void UUtilFunctionLibrary::CheatGlobalTimeDilation(UObject* WorldContextObject, float GlobalTime)
{
#if WITH_EDITOR
	UGameplayStatics::SetGlobalTimeDilation(WorldContextObject, GlobalTime);
#endif
}

bool UUtilFunctionLibrary::IsCheatVisibleInGameHPBar()
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	if (RGameInstance == nullptr)
		return false;

	return RGameInstance->ConsoleCheatManager->bVisibleInGameHPBar;
}

/*
related FX
*/
void UUtilFunctionLibrary::SpawnFX(TWeakObjectPtr<UParticleSystemComponent> &Result, FName FXKey, AActor* TargetActor, FVector SpawnLocation, FRotator SpawnRotator, FName SocketName)
{
#if LOG_TIME
	double StartTime = FPlatformTime::Seconds();
#endif
	FSpawnFxInfo FxInfo;
	FxInfo.Result = Result;
	FxInfo.FXKey = FXKey;
	FxInfo.TargetActor = TargetActor;
	FxInfo.SpawnLocation = SpawnLocation;
	FxInfo.SpawnRotator = SpawnRotator;
	FxInfo.SocketName = SocketName;
	URAsyncLoader::GetInstancePtr()->AddLoadFx(FxInfo);
}

void UUtilFunctionLibrary::PlaySound2D(UWorld* InWorld, FName SoundKey, float VolumeMultiplier, float PitchMultiplier)
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(InWorld != nullptr, TEXT("%s[ InWorld Is Empty ]"), __FUNCTION__);
	checkf(RGameInstance->OptionManager != nullptr, TEXT("%s[ OptionManager Is Empty ]"), __FUNCTION__);
	if (RGameInstance->OptionManager->IsMuteSound())
		return;
// 	FResourceSoundTableInfo* info = UTableManager::GetInstancePtr()->GetResourceSoundRow(SoundKey);
// 	if (info)
// 	{
		USoundManager::GetInstancePtr()->PlaySound2D(SoundKey, VolumeMultiplier, PitchMultiplier);
//		UGameplayStatics::PlaySound2D(InWorld, UUtilFunctionLibrary::GetSoundCueFromAssetID(info->Sound), VolumeMultiplier, PitchMultiplier);
//	}
}

void UUtilFunctionLibrary::PlaySoundSceneComponent(FName SoundKey, USceneComponent* Comp, FName SocketName, float VolumeMultiplier, float PitchMultiplier)
{
	if (IsValid(Comp))
	{
		auto RGameInstance = RGAMEINSTANCE(GEngine);
		checkf(RGameInstance->OptionManager != nullptr, TEXT("%s[ OptionManager Is Empty ]"), __FUNCTION__);
		if (RGameInstance->OptionManager->IsMuteSound())
			return;

// 		FResourceSoundTableInfo* info = UTableManager::GetInstancePtr()->GetResourceSoundRow(SoundKey);
// 		if (info)
// 		{
			USoundManager::GetInstancePtr()->SpawnSoundAttached(SoundKey, Comp, SocketName, EAttachLocation::KeepRelativeOffset, false, VolumeMultiplier, PitchMultiplier);
//			UGameplayStatics::SpawnSoundAttached(UUtilFunctionLibrary::GetSoundCueFromAssetID(info->Sound), Comp, SocketName, FVector(ForceInit), EAttachLocation::KeepRelativeOffset, false, VolumeMultiplier, PitchMultiplier);
//		}
	}
}

void UUtilFunctionLibrary::PlaySoundLocation(UWorld* InWorld, FName SoundKey, FVector Location, float VolumeMultiplier, float PitchMultiplier)
{
	if (IsValid(InWorld))
	{
		// Below causes Animation Editor to crash
		//auto RGameInstance = RGAMEINSTANCE(GEngine);
		//checkf(RGameInstance->OptionManager != nullptr, TEXT("%s[ OptionManager Is Empty ]"), __FUNCTION__);
		//if (RGameInstance->OptionManager->IsMuteSound())
		//	return;

		URGameUserSetting* RGameUserSetting = URGameUserSetting::GetRGameUserSettings();
		if (IsValid(RGameUserSetting) && RGameUserSetting->MuteSound)
			return;

// 		FResourceSoundTableInfo* info = UTableManager::GetInstancePtr()->GetResourceSoundRow(SoundKey);
// 		if (info)
// 		{
			USoundManager::GetInstancePtr()->PlaySoundAtLocation(SoundKey, Location, VolumeMultiplier, PitchMultiplier);
//			UGameplayStatics::PlaySoundAtLocation(InWorld, UUtilFunctionLibrary::GetSoundCueFromAssetID(info->Sound), Location, VolumeMultiplier, PitchMultiplier);
//		}
	}
}

void UUtilFunctionLibrary::PauseBGSound()
{
//	auto RGameInstance = RGAMEINSTANCE(GEngine);
//	checkf(RGameInstance->SoundManager != nullptr, TEXT("%s[ SoundManager Is Empty ]"), __FUNCTION__);
	USoundManager::GetInstancePtr()->PauseBGM();
}

void UUtilFunctionLibrary::ResumeBGSound()
{
// 	auto RGameInstance = RGAMEINSTANCE(GEngine);
// 	checkf(RGameInstance->SoundManager != nullptr, TEXT("%s[ SoundManager Is Empty ]"), __FUNCTION__);
	USoundManager::GetInstancePtr()->ResumeBGM();
}

void UUtilFunctionLibrary::FadeInBGM()
{
// 	auto RGameInstance = RGAMEINSTANCE(GEngine);
// 	checkf(RGameInstance->SoundManager != nullptr, TEXT("%s[ SoundManager Is Empty ]"), __FUNCTION__);
	USoundManager::GetInstancePtr()->FadeInBGM();
}

void UUtilFunctionLibrary::FadeOutBGM()
{
// 	auto RGameInstance = RGAMEINSTANCE(GEngine);
// 	checkf(RGameInstance->SoundManager != nullptr, TEXT("%s[ SoundManager Is Empty ]"), __FUNCTION__);
	USoundManager::GetInstancePtr()->FadeOutBGM();
}


void UUtilFunctionLibrary::NextTargetEffect(AActor* InSourceActor, AActor* InDestActor, float InLifeTime, bool InCheckPrevHide /*=true*/)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->GameDataManager != nullptr, TEXT("%s[ GameDataManager Is Empty ]"), __FUNCTION__);
	if (IsValid(InSourceActor) && IsValid(InDestActor))
	{
		ANextTargetActor* NextTargetActor = RGameInstance->GameDataManager->GetNextTargetActor(InCheckPrevHide);
		if (IsValid(NextTargetActor))
		{
			NextTargetActor->SetTarget(InSourceActor, InDestActor, InLifeTime);
		}
	}
}

// void UUtilFunctionLibrary::ScreenLog(UObject* WorldContextObject, FString message)
// {
// 	UKismetSystemLibrary::PrintString(WorldContextObject, message, true, true, FLinearColor(0.0, 0.66, 1.0), 10.0f);
// }

// void UUtilFunctionLibrary::GetGameEnvironmentVar(UObject* WorldContextObject, FGameEnvironmentVar& OutGameVars)
// {
// 	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
// 	if (World == nullptr)
// 		return;
// 
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
// 	if (RGameInstance == nullptr)
// 		return;
// 
// 	OutGameVars = RGameInstance->GameEnvironmentVar;
// }



FString UUtilFunctionLibrary::GetBlueprintPath(FString path)
{
	FString outputStr = TEXT("");

	const TCHAR* Delims[] = { TEXT("/") };
	FString strLower = path;
	TArray<FString> Parsed;
	strLower.ParseIntoArray(Parsed, Delims, 1);

	if (Parsed.Num() > 0)
	{
		outputStr += path;
		outputStr += TEXT(".");
		outputStr += Parsed[Parsed.Num() - 1];
		outputStr += TEXT("_C");
	}

	return outputStr;
}

AActor* UUtilFunctionLibrary::SpawnPath(UWorld* InWorld, FTransform transform, FString path)
{
	FStringAssetReference itemRef = GetBlueprintPath(path);

	TAssetSubclassOf<AActor> assetSubclass(itemRef);

	UClass* Class = assetSubclass.Get();
	if (Class == nullptr)
	{
		assetSubclass.ToSoftObjectPath().TryLoad();
		Class = assetSubclass.Get();

		UWorld* World = RGAMEINSTANCE(GEngine)->GetWorld();
		if (World)
		{
			AProjectRGameMode* RGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(World));
			if (RGameMode)
				RGameMode->AddTryCntLoadSync(assetSubclass.ToString());
		}
	}

	if (IsValid(Class) && IsValid(InWorld))
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AActor* actor = InWorld->SpawnActor(Class, &transform, SpawnParams);
		return actor;
	}

	

	return nullptr;
}

FName UUtilFunctionLibrary::GetActorFileName(AActor* InActor)
{
	if (IsValid(InActor))
	{
		UClass* baseClass = InActor->GetClass();
		if (IsValid(baseClass))
		{
			if (IsValid(baseClass->ClassGeneratedBy))
			{
				UPackage* package = Cast<UPackage>(baseClass->ClassGeneratedBy->GetOuter());
				if (IsValid(package))
				{
					return package->FileName;
				}
			}
		}
	}

	return TEXT("");
}

bool UUtilFunctionLibrary::IsRareItem(EItemGradeEnum eItemGradeEnum)
{
	if (eItemGradeEnum == EItemGradeEnum::Normal)
		return false;
	return true;
}

bool UUtilFunctionLibrary::IsCustomGame(UWorld* InWorld)
{
	ABasePlayerController* BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(InWorld);
	if (BasePlayerController)
	{
		return BasePlayerController->IsCustomGame;
	}

	return false;
}


bool UUtilFunctionLibrary::isTutorialGameMode()
{
	URGameInstance* GameInst = RGAMEINSTANCE(GEngine);
	if (IsValid(GameInst))
	{
		AGameModeBase* GameModeBase = UGameplayStatics::GetGameMode(GameInst->GetWorld());
		if (IsValid(GameModeBase))
			return GameModeBase->GetClass()->IsChildOf<ATutorialGameMode>();
	}

	return false;
}

class ATutorialLevelScriptActor* UUtilFunctionLibrary::GetTutorialLevelScriptActor()
{
	URGameInstance* GameInst = RGAMEINSTANCE(GEngine);
	if (IsValid(GameInst))
	{
		return Cast<ATutorialLevelScriptActor>(GameInst->GetWorld()->GetLevelScriptActor());
	}

	return nullptr;
}

// int32 UUtilFunctionLibrary::GetObjReferenceCount(UObject* Obj, TArray<UObject*>* OutReferredToObjects /*= nullptr*/, bool InIsLog /*= false*/)
// {
// 	if (!Obj || !Obj->IsValidLowLevel())
// 	{
// 		return -1;
// 	}
// 
// 	TArray<UObject*> ReferredToObjects;				//req outer, ignore archetype, recursive, ignore transient
// 	FReferenceFinder ObjectReferenceCollector(ReferredToObjects, Obj, false, true, true, false);
// 	ObjectReferenceCollector.FindReferences(Obj);
// 
// 	if (OutReferredToObjects)
// 	{
// 		OutReferredToObjects->Append(ReferredToObjects);
// 	}
// 
// 	if (InIsLog)
// 	{
// 		UE_LOG(LogGamePlay, Warning, TEXT("TargetObj - %s"), *Obj->GetName());
// 		for (UObject* Each : ReferredToObjects)
// 		{
// 			if (Each)
// 			{
// 				UE_LOG(LogGamePlay, Warning, TEXT("%s"), *Each->GetName());
// 			}
// 		}
// 	}
// 
// 	return ReferredToObjects.Num();
// }

bool UUtilFunctionLibrary::IsSameType(EItemEquipArmorTypeEnum InArmorType, EItemEquipTypeEnum InItemEquipType)
{
	if (InArmorType == EItemEquipArmorTypeEnum::VE_EveryOne)
		return true;

	if (InItemEquipType == EItemEquipTypeEnum::VE_None)
		return true; // ?
	
	switch (InArmorType)
	{
	case EItemEquipArmorTypeEnum::VE_Heavy:
		return InItemEquipType == EItemEquipTypeEnum::VE_HeavyArmor || InItemEquipType == EItemEquipTypeEnum::VE_HeavyPants;
		break;
	case EItemEquipArmorTypeEnum::VE_Light:
		return InItemEquipType == EItemEquipTypeEnum::VE_HeavyArmor || InItemEquipType == EItemEquipTypeEnum::VE_HeavyPants;
		break;
	case EItemEquipArmorTypeEnum::VE_Leather:
		return InItemEquipType == EItemEquipTypeEnum::VE_HeavyArmor || InItemEquipType == EItemEquipTypeEnum::VE_HeavyPants;
		break;
	case EItemEquipArmorTypeEnum::VE_Fabric:
		return InItemEquipType == EItemEquipTypeEnum::VE_HeavyArmor || InItemEquipType == EItemEquipTypeEnum::VE_HeavyPants;
		break;
	}

	// TODO: Shouldn't reach here
	return false;
}

bool UUtilFunctionLibrary::CanCharacterEquipItem(FName InCharacterID, FName InItemID)
{
	FItemTableInfo* ItemTableInfo = UTableManager::GetInstancePtr()->GetCharacterItemRow(InItemID);
	if (!ItemTableInfo)
		return true;
	FCharacterTableInfo* CharacterTableInfo = UTableManager::GetInstancePtr()->GetCharacterRow(InCharacterID);
	if (!CharacterTableInfo)
		return true;

	return FItemTableInfo::CanEquip(CharacterTableInfo->Class, ItemTableInfo->EquipType);
}

EREWARD_TYPE UUtilFunctionLibrary::GetRewardTypeFName(FName InType)
{
	FCurrencyPropertyTableInfo* currencyTableInfo = UTableManager::GetInstancePtr()->GetCurrencyPropertyRow(InType);
	if (currencyTableInfo)
	{
		return currencyTableInfo->CurrencyType;
	}

	return EREWARD_TYPE::NONE;
}

int32 UUtilFunctionLibrary::GetCurrencyHave(UWorld* InWorld, FName InCurrencyType)
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->UserInfo != nullptr, TEXT("%s[ UserInfo Is Empty ]"), __FUNCTION__);
	checkf(RGameInstance->RInventory != nullptr, TEXT("%s[ RInventory Is Empty ]"), __FUNCTION__);
	return RGameInstance->RInventory->GetCurrency(InCurrencyType);
}

FText UUtilFunctionLibrary::ItemBasicOptionStatValueToText(EItemStatOption ItemStatOption, int32 Value)
{
	if (FItemTableInfo::IsItemStatPerMil(ItemStatOption))
	{
		// "+10.0%"
		return FText::FromString(FString::Printf(TEXT("+%.1f%%"), (float)Value / 10.0));
	}
	else
	{
		// "15"
		return FText::FromString(FString::Printf(TEXT("%d"), Value));
	}
}

FText UUtilFunctionLibrary::ItemAdditionalOptionStatValueToText(EItemStatOption ItemStatOption, int32 Value)
{
	if (FItemTableInfo::IsItemStatPerMil(ItemStatOption))
	{
		// "+10.0%"
		return FText::FromString(FString::Printf(TEXT("+%.1f%%"), (float)Value / 10.0));
	}
	else
	{
		// "+15"
		return FText::FromString(FString::Printf(TEXT("+%d"), Value));
	}
}

bool UUtilFunctionLibrary::IsCurrencyHave(UWorld* InWorld, FName InCurrencyType, int32 InQuantity)
{
	auto RGameInstance = RGAMEINSTANCE(GEngine);
	checkf(RGameInstance->UserInfo != nullptr, TEXT("%s[ UserInfo Is Empty ]"), __FUNCTION__);
	int32 Value = GetCurrencyHave(InWorld, InCurrencyType);
	if (Value >= InQuantity)
		return true;
	return false;
}


int32 UUtilFunctionLibrary::GetTouchCount(UObject* WorldContextObject)
{
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
	if (World)
	{
		APlayerController* PlayerController = GetPlayerController(World);
		if (IsValid(PlayerController))
		{
			int32 touchCount = 0;
			for (int32 i = (int32)ETouchIndex::Touch1; i <= (int32)ETouchIndex::Touch10; ++i)
			{
				float x, y;
				bool bState = false;
				PlayerController->GetInputTouchState((ETouchIndex::Type)i, x, y, bState);

				if (bState)
				{
					++touchCount;
				}
			}

			return touchCount;
		}
	}	

	return 0;
}
// 
// void UUtilFunctionLibrary::ShowViewportScale(UObject* WorldContextObject)
// {
// 	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::Assert);
// 	if (World)
// 	{
// 		float scale = UWidgetLayoutLibrary::GetViewportScale(WorldContextObject);
// 
// 		UGameViewportClient* ViewportClient = World->GetGameViewport();
// 		if (IsValid(ViewportClient))
// 		{
// 			FVector2D ViewportSize;
// 			ViewportClient->GetViewportSize(ViewportSize);
// 
// 			UE_LOG(LogBattle, Log, TEXT("**** GetViewportScale %f [%f:%f]"), scale, ViewportSize.X, ViewportSize.Y);
// 
// 			FString strText;
// 			strText = FString::Printf(TEXT("**** GetViewportScale %f [%f:%f]"), scale, ViewportSize.X, ViewportSize.Y);
// 			UKismetSystemLibrary::PrintString(World, strText);
// 		}
// 	}
// }

bool UUtilFunctionLibrary::CheckSelectCharacterAttackAnim(AActor* InActor)
{
	if (IsValid(InActor))
	{
		auto BasePlayerController = Cast<AStagePlayerController>(GetBasePlayerController(InActor->GetWorld()));
		if (BasePlayerController)
		{
			return BasePlayerController->GetLeaderControlHelper()->CheckLeaderCharacterAttackAnim();
		}
	}

	return true;
}

void UUtilFunctionLibrary::SetDamageFloaterVisible(UWorld* InWorld, bool InVisible)
{
	auto BasePlayerController = GetBasePlayerController(InWorld);
	if (BasePlayerController)
	{
		AActor* SelectedCharacter = BasePlayerController->GetSelectedCharacter();
		if (IsValid(SelectedCharacter))
		{
			UEntityRenderComponent* EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(SelectedCharacter);
			if (EntityRenderComponent)
			{
				EntityRenderComponent->SetDamageFloaterVisible(InVisible);
				EntityRenderComponent->OutLine(false);
			}
		}
	}
}

FString UUtilFunctionLibrary::GetCurrentIndoorLobbyHero()
{
	AIndoorLobbyStateActor* LobbyStateActor = Cast<AIndoorLobbyStateActor>(RGAMEINSTANCE(GEngine)->GetTowerLobbyGameMode()->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
	if (LobbyStateActor)
	{
		return LobbyStateActor->CurrentHeroUD;
	}

	return TEXT("");
}

bool UUtilFunctionLibrary::ComputeSynergy(TArray< TWeakObjectPtr< AActor > >& outEnemyList, AActor* InActor, FName InSkillTableID)
{
//	auto RGameInstance = RGAMEINSTANCE(GEngine);
// 	auto ObjManager = GetGameObjectManagerFromActor(InActor);
// 	TArray< TWeakObjectPtr< AActor > > outEnemyList;
// 	ObjManager->GetEnemyList(outEnemyList, InActor);

	TArray< TWeakObjectPtr<AActor> > OutTargets;
	USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(InActor);
	bool bIsSynergy = false;
	if(IsValid(SkillEntityComponent))
		bIsSynergy= SkillEntityComponent->GetSynergySkillTarget(InActor, OutTargets, InSkillTableID);

	for (auto Element : OutTargets)
	{
		auto Render = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(Element.Get());
		if (Render)
		{
			auto SynergyMesh = Render->GetSynergyMeshComponent();
			if (SynergyMesh)
			{
				SynergyMesh->SetVisibility(true);
				outEnemyList.Remove(Element);
			}
		}
	}

	for (auto Element : outEnemyList)
	{
		auto Render = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(Element.Get());
		if (Render)
		{
			auto SynergyMesh = Render->GetSynergyMeshComponent();
			if (SynergyMesh)
			{
				SynergyMesh->SetVisibility(false);
			}
		}
	}

		
	return bIsSynergy;
}


bool UUtilFunctionLibrary::CheckCampaignStorySkip(URGameInstance* InRGameInstance)
{
// 	if (InRGameInstance == nullptr)
// 		return false;
// 
// 	if (InRGameInstance->IsAutoPlayMode() || InRGameInstance->IsRetryPlay())
// 		return true;
// 	
// 	if (InRGameInstance->OptionManager->IsStoryMatineeView() == false)
// 	{
// 		UBaseCampMode* RGamePlayMode = Cast<UBaseCampMode>(GetGamePlayModeByRGameInstance(InRGameInstance));
// 		if (RGamePlayMode)
// 		{
// 			if (UGamePlayBluePrintFunction::GetBaseStageInfo()->EpisodeType != EEpisodeTypeEnum::VE_Normal)
// 				return false;
// 
// 			int32 CheckClearStage = InRGameInstance->CampaignManager->GetCurrentStageGrade();
// 			if (CheckClearStage != 0)
// 			{
// 				return true;
// 			}
// 		}
// 	}
	return false;
}

//EItemGenerateOptionType UUtilFunctionLibrary::GetItemOptionType(FString InValue)
//{
//	EItemGenerateOptionType EResult = EItemGenerateOptionType::VE_None;
//
//	if (InValue == "AccuracyValue")				EResult = EItemGenerateOptionType::VE_AccuracyValue;
//	else if (InValue == "CriticalValue")		EResult = EItemGenerateOptionType::VE_CriticalValue;
//	else if (InValue == "MaxHP")				EResult = EItemGenerateOptionType::VE_MaxHP;
//	else if (InValue == "DodgeValue")			EResult = EItemGenerateOptionType::VE_DodgeValue;
//	else if (InValue == "Armor")				EResult = EItemGenerateOptionType::VE_Armor;
//	else if (InValue == "ParryValue")			EResult = EItemGenerateOptionType::VE_ParryValue;
//	else if (InValue == "MagicResistance")		EResult = EItemGenerateOptionType::VE_MagicResistance;
//	else if (InValue == "PierceValue")			EResult = EItemGenerateOptionType::VE_PierceValue;
//	else if (InValue == "MagicAttackPower")		EResult = EItemGenerateOptionType::VE_MagicAttackPower;
//	else if (InValue == "PhysicalAttackPower")  EResult = EItemGenerateOptionType::VE_PhysicalAttackPower;
//
//	return EResult;
//}

void UUtilFunctionLibrary::UpdateGoodsInfo(URGameInstance* InRGameInstance, FUPDATE_PACKET_SLIM& InUpdate, bool bCheckHeroUpdate)
{
	InRGameInstance->UserInfo->SetAvatarData(InUpdate.avatar);
	UE_LOG(LogRInventory, Log, TEXT("UUtilFunctionLibrary::UpdateGoodsInfo 1. SetAvatarData"));
	InRGameInstance->RInventory->UpdateInventory(InUpdate.inventoryUpdate, bCheckHeroUpdate);
	UE_LOG(LogRInventory, Log, TEXT("UUtilFunctionLibrary::UpdateGoodsInfo 2. UpdateInventory"));
	if (InUpdate.achieveUpdate.GetAllocatedSize() > 0)
	{
		InRGameInstance->AchievementManager->SetAchievementUpdate(InUpdate.achieveUpdate);
		UE_LOG(LogRInventory, Log, TEXT("UUtilFunctionLibrary::UpdateGoodsInfo 3. SetAchievementUpdate"));
	}

	if (InUpdate.unlockNotify.notify.GetAllocatedSize() > 0)
	{
		InRGameInstance->ContentsUnLockManager->SetUnlockNotifyList(InUpdate.unlockNotify);
		UE_LOG(LogRInventory, Log, TEXT("UUtilFunctionLibrary::UpdateGoodsInfo 4. SetUnlockNotifyList"));
	}

	InRGameInstance->EventManager->OnUpdateUserInfo.Broadcast();
	UE_LOG(LogRInventory, Log, TEXT("UUtilFunctionLibrary::UpdateGoodsInfo 5. OnUpdateUserInfo"));
}

// void UUtilFunctionLibrary::UpdateGoodsInfo(URGameInstance* InRGameInstance, FUPDATE_PACKET& InUpdate)
// {
// 	if (IsValid(InRGameInstance) == false)
// 		return;
// 
// 	FUPDATE_PACKET_SLIM slim;
// 	slim.avatar = InUpdate.avatar;
// 	slim.heros = InUpdate.heros;
// 	slim.items = InUpdate.items;
// 
// 	UpdateGoodsInfo(InRGameInstance, slim);
// }

bool UUtilFunctionLibrary::IsPvPContents(UWorld* World)
{
	EGamePlayModeTypeEnum modeType = UUtilFunctionLibrary::GetGamePlayModeEnum();
	if (modeType == EGamePlayModeTypeEnum::VE_Arena ||
		modeType == EGamePlayModeTypeEnum::VE_BattleGround ||
		modeType == EGamePlayModeTypeEnum::VE_Colosseum ||
		modeType == EGamePlayModeTypeEnum::VE_MirrorDungeon ||
		modeType == EGamePlayModeTypeEnum::VE_RaidKillBoss)
	{
		return true;
	}
	else
	{
		return false;
	}
}

URGameInstance* UUtilFunctionLibrary::GetRGameInstanceWithBluePrint()
{
	return RGAMEINSTANCE(GEngine);
}

FText UUtilFunctionLibrary::GetBossName(AActor* InBossActor)
{
	FName						TargetID		=	UCharacterHelpFunctionLibrary::GetCharacterID(InBossActor);
	ECharacterCategoryTypeEnum	TargetChategory =	UCharacterHelpFunctionLibrary::GetCharacterCategory(InBossActor);
	FText						BossNameText	=	FText();

	auto CharacterInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(TargetChategory, TargetID);
	if (CharacterInfo)
	{
		EStringTableType		TableType;
		switch (TargetChategory)
		{
		case ECharacterCategoryTypeEnum::VE_Hero:
			TableType = EStringTableType::VE_Hero;
			break;
		default:
			TableType = EStringTableType::VE_Npc;
			break;
		}

		if (BossNameText.IsEmpty())
		{
			BossNameText = GetLocalizedString(TableType, CharacterInfo->DisplayName);
		}
	}

	return BossNameText;
}

bool UUtilFunctionLibrary::IsNightingWithRealTime()
{
	FDateTime DT = FDateTime::Now();
	int32 currentHour = DT.GetHour();
	UE_LOG(LogUI, Log, TEXT("%d"), currentHour);

	if (currentHour > 9 && currentHour < 21)
		return false;
	else
		return true;
}
