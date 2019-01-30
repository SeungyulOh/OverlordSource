// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "BasePlayerController.h"
#include "ProjectRGameMode.h"
#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"
#include "GameInfo/PlayStateInfo.h"
#include "Character/BaseAIController.h"
#include "Character/Components/SkillAreaIndicatorMeshComponent.h"
#include "Character/Components/AnimStateComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "Character/Components/SelectionIndicatorMeshComponent.h"
#include "Character/Components/WidgetDecoratorComponent.h"
#include "Character/AnimState/IAnimState.h"
#include "UI/UIDefs.h"
#include "UI/RWidgetManager.h"
#include "UI/UI_Battle/RCampaignHUD_ThreeControl.h"
#include "UI/UI_Battle/RSkillIcon.h"
#include "UI/UI_Battle/RAttackIcon.h"//temp
#include "UI/UI_Battle/RCampaignHUD_OneControl.h"
#include "UI/UI_Battle/RBattleOptionMenu.h"

#include "UI/InGameHUD/UP_InGameHUD_Arena.h"
#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"
#include "Object/BaseCameraActor.h"
#include "Character/Components/AvoidanceComponent.h"
#include "Skill_V2/SharedSkill/ISkill_V2.h"
#include "Network/HttpRequestClient.h"
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "Input/NormalTouchInput.h"
#include "ContentGameMode/Modes/StageGameMode.h"
#include "LeaderControlHelper.h"
#include "UI/Common/UC_Particle.h"


#define DPAD_DEBUG 0

ABasePlayerController::ABasePlayerController(const FObjectInitializer& ObjectInitializer)
	: Super( ObjectInitializer )
{
	static ConstructorHelpers::FObjectFinder<UClass> BP_Shake_Hit(TEXT("Blueprint'/Game/Blueprints/Cameras/BP_Shake_Hit.BP_Shake_Hit_C'"));
	if (BP_Shake_Hit.Succeeded())
		CameraShakeEffect.Emplace(BP_Shake_Hit.Object);
#if WITH_EDITOR
	this->bShowMouseCursor = true;
#endif
	
}

FString	ABasePlayerController::ConsoleCommand(const FString& Command, bool bWriteToLog)
{	
	RGAMEINSTANCE(this)->ConsoleCheatManager->ConsoleCommand(Command, GetWorld());

	return Super::ConsoleCommand(Command, bWriteToLog);
}

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	ZPos_SkillPickArea = RGAMEINSTANCE(this)->GameEnvironmentVar.ZPos_SkillPickArea;
}

void ABasePlayerController::TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	if (IsAvoidButton)
	{
		AvoidRestTime -= DeltaTime;
		if (AvoidRestTime <= 0.0f)
			SetAvoidState(false);
	}

	AStageGameMode* StageGameMode = Cast<AStageGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (StageGameMode)
	{
		if(StageGameMode->GameModeState == EGameModeStateEnum::VE_Start)
			StageDirectionProcess();
	}
	
}

void ABasePlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (InputComponent)
		InputComponent->ClearActionBindings();

	RGAMEINSTANCE(this)->EventManager->OnInputSkillNameDelegate.Clear();

	if (Particle_Hero_Gather)
	{
		Particle_Hero_Gather->DestroyComponent();
		Particle_Hero_Gather = nullptr;
	}
	SetStageDirection(false);
	Super::EndPlay(EndPlayReason);
}

//////////////////////////////////////////////////////////////////////////

void ABasePlayerController::ProcessPlayerInput(const float DeltaTime, const bool bGamePaused)
{
	if (!bGamePaused && PlayerInput && InputHandler)
		InputHandler->UpdateDetection(DeltaTime);

	Super::ProcessPlayerInput(DeltaTime, bGamePaused);
}

void ABasePlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent)
	{
#if !PLATFORM_IOS
		FInputActionBinding& PrevKeyBinding = InputComponent->BindAction(TEXT("PrevKey"), IE_Pressed, this, &ABasePlayerController::CheckAndroidBack);
		PrevKeyBinding.bExecuteWhenPaused = true;
#endif
	}
	InputHandler = NewObject<UNormalTouchInput>(this);
	BIND_2P_ACTION(InputHandler, EGameKey::Pinch, IE_Pressed, &ABasePlayerController::OnPinchStarted);
	BIND_2P_ACTION(InputHandler, EGameKey::Pinch, IE_Repeat, &ABasePlayerController::OnPinchUpdate);
	BIND_2P_ACTION(InputHandler, EGameKey::Pinch, IE_Released, &ABasePlayerController::OnPinchReleased);
}

void ABasePlayerController::OnPinchStarted(const FVector2D& AnchorPosition1, const FVector2D& AnchorPosition2, float DownTime)
{
// 	if (RGAMEINSTANCE(this) == nullptr)
// 		return;
// 	UE_LOG(LogInputMgr, Log, TEXT("OnPinchStarted Position1 %s Position2 %s"), *(AnchorPosition1.ToString()), *(AnchorPosition2.ToString()));
// 	RGAMEINSTANCE(this)->EventManager->OnTwoPointInput_Pinch.Broadcast(AnchorPosition1, AnchorPosition2, EInputEvent::IE_Pressed);
}

void ABasePlayerController::OnPinchUpdate(const FVector2D& ScreenPosition1, const FVector2D& ScreenPosition2, float DownTime)
{
// 	if (RGAMEINSTANCE(this) == nullptr)
// 		return;
// 	UE_LOG(LogInputMgr, Log, TEXT("OnPinchUpdate Position1 %s Position2 %s"), *(ScreenPosition1.ToString()), *(ScreenPosition2.ToString()));
// 	RGAMEINSTANCE(this)->EventManager->OnTwoPointInput_Pinch.Broadcast(ScreenPosition1, ScreenPosition2, EInputEvent::IE_Repeat);
}

void ABasePlayerController::OnPinchReleased(const FVector2D& ScreenPosition1, const FVector2D& ScreenPosition2, float DownTime)
{
// 	if (RGAMEINSTANCE(this) == nullptr)
// 		return;
// 	UE_LOG(LogInputMgr, Log, TEXT("OnPinchReleased Position1 %s Position2 %s"), *(ScreenPosition1.ToString()), *(ScreenPosition2.ToString()));
// 	RGAMEINSTANCE(this)->EventManager->OnTwoPointInput_Pinch.Broadcast(ScreenPosition1, ScreenPosition2, EInputEvent::IE_Released);
}

void ABasePlayerController::CheckAndroidBack()
{
	if (RGAMEINSTANCE(this)->ForceTutorialManager->IsForceTutorialing())
		return;
	UUserWidget* InGameHudWidget = UUIFunctionLibrary::GetInGameHUD();
	if (InGameHudWidget)
	{
		URBaseIngameHUD* basehud = Cast<URBaseIngameHUD>(InGameHudWidget);
		if (basehud)
		{
			if (!UGamePlayBluePrintFunction::IsPause())
				basehud->OptionMenu->OnOptionMenuOpen();
			else
				basehud->OptionMenu->OnOptionMenuClose();
		}

	}
}

void ABasePlayerController::PlayCameraShake(TSubclassOf<class UCameraShake> ShakeEffect, float Intensity)
{
	TSubclassOf<class UCameraShake> TargetShake = ShakeEffect;

	if (TargetShake == nullptr)
		TargetShake = CameraShakeEffect[0];

	this->ClientPlayCameraShake(TargetShake, Intensity);// , ECameraAnimPlaySpace::CameraLocal, FRotator(0, 0, 0));
}

void ABasePlayerController::SetCameraActor(ABaseCameraActor* actor)
{ 
	
	CameraActor = actor;
	if (IsValid(CameraActor))
	{
		SetViewTargetWithBlend(CameraActor);		
	}
}

void ABasePlayerController::ActorSettingOnTheCamera(AActor* character, float InCameraMove, float InCameraExp)
{
	TouchCount = 0;
		
	if (IsValid(CameraActor))
	{
		CameraActor->SetCameraMoveEnable(true);
		if(IsValid(character))
		{
			FTransform TargetTransform = character->GetTransform();
			CameraActor->TransformStageCamera(TargetTransform, InCameraMove, InCameraExp);
			CameraActor->SetOwnerCharacter(character);
		}
	}	
}

void ABasePlayerController::HideAvoidState()
{
	AvoidRestTime = 0.0f;
	if (Particle_Hero_Gather)
	{
		Particle_Hero_Gather->DestroyComponent();
		Particle_Hero_Gather = nullptr;
	}
}

void ABasePlayerController::SetAvoidState(bool InAvoidState)
{
	IsAvoidButton = InAvoidState;

	if (IsAvoidButton)
	{
		AvoidRestTime = AvoidTime;

		if (LeaderCharacter.IsValid())
		{
			FName EffectId = FName(TEXT("Hero_gather"));
			Particle_Hero_Gather = UUtilFunctionLibrary::SpawnEmitterAttachedWithTableID(EffectId, LeaderCharacter.Get(), SOCKET_ROOT);			
		}
	}
	else
	{
		AvoidRestTime = 0.0f;

		if (Particle_Hero_Gather)
		{
			Particle_Hero_Gather->DestroyComponent();
			Particle_Hero_Gather = nullptr;
		}
	}
}

void ABasePlayerController::StageDirectionProcess()
{
	if (UUtilFunctionLibrary::isTutorialGameMode())
		return;

	UBaseStageInfo* stageinfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	ALevelActorPhase* CurrentLevelActor = nullptr;
	if (IsValid(stageinfo))
	{
		ALevelActor* levelActor = stageinfo->GetLevelActor();
		if (IsValid(levelActor))
			CurrentLevelActor = levelActor->GetLevelActorPhase();
	}

	if (!IsValid(CurrentLevelActor))
		return;

	if (!Particle_Hero_StageDirection)
	{
		if (CurrentLevelActor)
		{
			if (CurrentLevelActor->ClearCondition == EPhaseClearEnumType::VE_Move)
				SetStageDirection(true);
		}
	}
	else
	{
		if (CurrentLevelActor)
		{
			if (CurrentLevelActor->ClearCondition != EPhaseClearEnumType::VE_Move)
			{
				SetStageDirection(false);
				return;
			}
		}

		if (LeaderCharacter.IsValid() == false)
			return;

		FSpawnerTransformInfo NextTransformInfo = RGAMEINSTANCE(GEngine)->AutoPlayManager->GetNextWayPointTransform();
		StageLocation = NextTransformInfo.Position;


		FVector SelectedLocation = LeaderCharacter->GetActorLocation();
		SelectedLocation.Z = 0;

		//Rotation
		FVector DirectionVector = StageLocation - SelectedLocation;
		DirectionVector.Z = 0;
		FRotator lookatRotation = FRotationMatrix::MakeFromX(DirectionVector).Rotator();
		Particle_Hero_StageDirection->SetWorldRotation(lookatRotation);
	}
}

void ABasePlayerController::SetStageDirection(bool InAttach, FVector InTargetLocation /*= FVector(0,0,0)*/)
{
	if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::NONE))
		return;
	if (UUtilFunctionLibrary::isTutorialGameMode())
		return;

	if (InAttach)
	{
		if (InTargetLocation.IsZero() == false)
		{
			StageLocation = InTargetLocation;
			StageLocation.Z = 0;
		}

		if (LeaderCharacter.IsValid())
		{
			ACharacter* SelectCharacterPtr = Cast<ACharacter>(LeaderCharacter.Get());
			FName SocketName = SOCKET_ROOT;//((IsStageDirection_Head) ? SOCKET_STATE : SOCKET_ROOT);

			if (Particle_Hero_StageDirection)
				Particle_Hero_StageDirection->AttachToComponent(SelectCharacterPtr->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketName);
			else
			{
				FName EffectId = FName(TEXT("StageDirection"));
				Particle_Hero_StageDirection = UUtilFunctionLibrary::SpawnEmitterAttachedWithTableID(EffectId, LeaderCharacter.Get(), SocketName);
			}
			StageDirectionProcess();
		}
	}
	else
	{
		if (Particle_Hero_StageDirection)
		{
			Particle_Hero_StageDirection->DestroyComponent();
			Particle_Hero_StageDirection = nullptr;
		}
	}
}



bool ABasePlayerController::IsAroundEnemy()
{
	if (LeaderCharacter.IsValid() == false)
		return false;

	float fStageDirectionDist = RGAMEINSTANCE(this)->GameEnvironmentVar.StageDirectionDist;
	FVector OwnerLocation = LeaderCharacter->GetActorLocation();
	TArray< TWeakObjectPtr<AActor> > CheckBattleCharacter;
	URenewal_BattleFunctionLibrary::FindTarget(CheckBattleCharacter, LeaderCharacter.Get(), OwnerLocation, EFriendshipTypeEnum::VE_Enemy, ESearchShapeTypeEnum::VE_Circle, 
		FVector(fStageDirectionDist, fStageDirectionDist, fStageDirectionDist));

	return (0 < CheckBattleCharacter.Num());
}


void ABasePlayerController::CreateJoystick()
{
	IsJoystickActivate = true;

	if (CurrentTouchInterface)
	{
		SetVirtualJoystickVisibility(true);
		return;
	}

	FStringAssetReference TouchInterfaceName(FString("/Game/UMG/BP_GameTouchInterface.BP_GameTouchInterface"));
	UTouchInterface* ManualTouchInterface = LoadObject<UTouchInterface>(NULL, *TouchInterfaceName.ToString());
	if (IsValid(ManualTouchInterface))
	{
		ActivateTouchInterface(ManualTouchInterface);
	}

}

void ABasePlayerController::RemoveJoystick()
{
	IsJoystickActivate = false;

	ActivateTouchInterface(nullptr);
}

void ABasePlayerController::ResetJoystickForChannelSkillMovingCast()
{
	FVector2D DPadMoveValue = RGAMEINSTANCE(this)->InputManager->GetDPadMoveValue();

	if(false == DPadMoveValue.IsZero())
	{
		if (InputComponent)
		{
			InputComponent->ClearBindingValues();
		}
		RemoveJoystick();
		CreateJoystick();

		RGAMEINSTANCE(this)->InputManager->SetDPadMoveValue(FVector2D::ZeroVector);
	}
}

void ABasePlayerController::SetLeaderCharacterAIAutoMode(EAIControlModeEnum InControlType)
{
	URGameInstance* GameInstance = RGAMEINSTANCE(this);

	auto ChangeAIControlMode = [](AActor* InActor, EAIControlModeEnum ControlMode) {
		auto SpawnedCharacter_EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(InActor);
		if (SpawnedCharacter_EntityAIComponent == nullptr)
			return;
		if (SpawnedCharacter_EntityAIComponent->GetAIControlMode() != ControlMode)
			SpawnedCharacter_EntityAIComponent->SetAIControlMode(ControlMode);
	};

	ChangeAIControlMode(LeaderCharacter.Get() , InControlType);

	if (InControlType != EAIControlModeEnum::VE_ManualMode)
	{
		AActor* Crew1 = GameInstance->PlayStateInfo->GetBattleActiveCrewActor(ECharacterRoleEnum::VE_Crew1);
		if(Crew1)
			ChangeAIControlMode(Crew1 , EAIControlModeEnum::VE_AutoMode);

		AActor* Crew2 = GameInstance->PlayStateInfo->GetBattleActiveCrewActor(ECharacterRoleEnum::VE_Crew2);
		if (Crew2)
			ChangeAIControlMode(Crew2, EAIControlModeEnum::VE_AutoMode);
	}
}



EAIControlModeEnum ABasePlayerController::GetLeaderAIModeType()
{
	auto SpawnedCharacter_EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(LeaderCharacter.Get());
	if (SpawnedCharacter_EntityAIComponent == nullptr)
		return EAIControlModeEnum::VE_ManualMode;

	return SpawnedCharacter_EntityAIComponent->GetAIControlMode();
}