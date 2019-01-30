// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "StagePlayerController.h"
#include "ProjectRGameMode.h"
#include "GlobalIntegrated.h"
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
#include "Character/Components/AvoidanceComponent.h"
#include "UtilFunctionIntegrated.h"
#include "Object/BaseCameraActor.h"
#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"
#include "Skill_V2/SharedSkill/ISkill_V2.h"
#include "Network/HttpRequestClient.h"
#include "Input/NormalTouchInput.h"
#include "Widgets/Input/SVirtualJoystick.h"
#include "UI/UIDefs.h"
#include "UI/RWidgetManager.h"
#include "UI/UI_Battle/RCampaignHUD_ThreeControl.h"
#include "UI/UI_Battle/RSkillIcon.h"
#include "UI/UI_Battle/RAttackIcon.h"//temp
#include "UI/InGameHUD/UP_InGameHUD_Arena.h"
#include "UI/Common/UC_Particle.h"

#include "UI/UI_Battle/InGameHeroUI/HeroUIforBase.h"

#define DPAD_DEBUG 0

AStagePlayerController::AStagePlayerController(const FObjectInitializer& ObjectInitializer)
	: Super( ObjectInitializer )
{
	LeaderControlHelper = CreateDefaultSubobject<ULeaderControlHelper>(TEXT("LeaderControlHelper"));
	
}

void AStagePlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(RGAMEINSTANCE(this)->EventManager))
	{
		//RGAMEINSTANCE(this)->EventManager->OnCrewDead.Clear();
		//RGAMEINSTANCE(this)->EventManager->OnCrewDead.AddDynamic(this, &AStagePlayerController::OnCrewDead);

		RGAMEINSTANCE(this)->EventManager->OnLeaderMove.Clear();
		RGAMEINSTANCE(this)->EventManager->OnLeaderMove.AddDynamic(this, &AStagePlayerController::OnLeaderMove);
	}

	if (IsValid(LeaderControlHelper))
		LeaderControlHelper->BindingDelegates();
}

void AStagePlayerController::TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);

	if (VirtualJoystick.IsValid())
		VirtualJoystickHelper.Calculate(VirtualJoystick.Get());
	
	CheckOwnerAbnormal();
	if (IsValid(LeaderControlHelper))
		LeaderControlHelper->Tick(DeltaTime);
}

void AStagePlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(RGAMEINSTANCE(this)->EventManager))
	{
		RGAMEINSTANCE(this)->EventManager->OnCrewDead.Clear();
		RGAMEINSTANCE(this)->EventManager->OnLeaderMove.Clear();
	}
	Super::EndPlay(EndPlayReason);
}

ULeaderControlHelper* AStagePlayerController::GetLeaderControlHelper()
{
	if (!IsValid(LeaderControlHelper->GetBasePC()))
		LeaderControlHelper->SetBasePC(this);

	return LeaderControlHelper;
}

void AStagePlayerController::OnLeaderMove()
{
	for (const FHeroSlotInfo& slot : RGAMEINSTANCE(this)->PlayStateInfo->GetSelectedCharacterArray())
	{
		if (slot.CharacterRole == ECharacterRoleEnum::VE_Leader || slot.CharacterRole == ECharacterRoleEnum::VE_None)
			continue;

		if (slot.CharacterCrewState == ECharacterCrewStateEnum::VE_Battle)
		{
			UEntityAIComponent* EntityAIComp = UCharacterHelpFunctionLibrary::GetEntityAIComponent(slot.SpawnedTargetActor);
			if (EntityAIComp)
				EntityAIComp->SetPause(false);
		}
	}

	RGAMEINSTANCE(this)->EventManager->OnLeaderMove.Clear();
}

void AStagePlayerController::SetCrewPosition(int32 InCrewIndex)
{
	if (RGAMEINSTANCE(this) == nullptr)
		return;

	//UPlayStateInfo* PlayStateInfo = RGAMEINSTANCE(this)->PlayStateInfo;

	FHeroSlotInfo& Leader = RGAMEINSTANCE(this)->PlayStateInfo->GetSelectedCharacterSlot(0);
	FHeroSlotInfo& CrewSlot = RGAMEINSTANCE(this)->PlayStateInfo->GetSelectedCharacterSlot(InCrewIndex);
	if (!IsValid(Leader.SpawnedTargetActor) ||
		!IsValid(CrewSlot.SpawnedTargetActor))
		return;

	AActor* LeaderActor = Leader.SpawnedTargetActor;
	AActor*	CrewActor = CrewSlot.SpawnedTargetActor;

	UBattleEntityComponent* LeaderBattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(LeaderActor);
	UBattleEntityComponent* CrewBattleComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(CrewActor);
	USkillEntityComponent* CrewSkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(CrewActor);
	UISkill_V2* SkillInst = CrewSkillComp ? CrewSkillComp->GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_ActiveSkill03) : nullptr;
	FSkillTableInfo_V2* SkillInfo = SkillInst ? SkillInst->GetSkillInfo() : nullptr;

	if (IsValid(CrewBattleComp))
	{
		CrewBattleComp->ClearAllAbnormalState();
	}

	FVector ResultLocation;
	bool bChangePosition = false;
	float fEaseValue = 2.0f;
	float fLifetime = 15.0f;
#ifdef DRAW_DEBUG
	FRotator rotator = FRotator::ZeroRotator;
	rotator.Pitch = 90.0f;
	FMatrix matrix = FRotationTranslationMatrix::Make(rotator, LeaderActor->GetActorLocation());
	DrawDebugCircle(RGAMEINSTANCE(this)->GetWorld(), matrix, RGAMEINSTANCE(this)->GameEnvironmentVar.FollowLeaderDistance, 32, FColorList::VioletRed, false, fLifetime);
#endif // DRAW_DEBUG
	float Dist = FVector::Distance(LeaderActor->GetActorLocation(), CrewActor->GetActorLocation());
	// call crew in my area
	if (RGAMEINSTANCE(this)->GameEnvironmentVar.FollowLeaderDistance <= Dist)
	{
		float distance = (Dist - RGAMEINSTANCE(this)->GameEnvironmentVar.FollowLeaderDistance);
		ResultLocation = FMath::Lerp<FVector>(CrewActor->GetActorLocation(), LeaderActor->GetActorLocation(), (distance / Dist));
		bChangePosition = true;
	}

	// in crew's skill area
	AActor* TargetActor = LeaderBattleComp->GetCurrentAttackTarget();
	if (TargetActor != nullptr)
	{
		FVector CurrentCrewLocation = bChangePosition ? ResultLocation : CrewActor->GetActorLocation();
		Dist = FVector::Distance(CurrentCrewLocation, TargetActor->GetActorLocation());
		if (SkillInfo && SkillInfo->CastingRange <= Dist)
		{
			float distance = (Dist - SkillInfo->CastingRange);			
			ResultLocation = FMath::Lerp<FVector>(CurrentCrewLocation, TargetActor->GetActorLocation(), (distance / Dist));
			bChangePosition = true;
		}
	}

	if (bChangePosition && UMapFunctionLibrary::IsProjectionPointOnNavyMesh(RGAMEINSTANCE(this), ResultLocation))
	{
		UCapsuleComponent* ActorCapsuleComponent = UCharacterHelpFunctionLibrary::GetCapsuleComponent(CrewSlot.SpawnedTargetActor);
		if (ActorCapsuleComponent)
		{
			ResultLocation.Z += ActorCapsuleComponent->GetScaledCapsuleHalfHeight();
		}

#ifdef DRAW_DEBUG
		DrawDebugPoint(RGAMEINSTANCE(this)->GetWorld(), CrewSlot.SpawnedTargetActor->GetActorTransform().GetLocation(), 10.0f, FColor::Green, false, fLifetime);
		DrawDebugPoint(RGAMEINSTANCE(this)->GetWorld(), ResultLocation, 10.0f, FColor::Red, false, fLifetime);
#endif // DRAW_DEBUG

		CrewSlot.SpawnedTargetActor->SetActorLocation(ResultLocation, false, nullptr, ETeleportType::TeleportPhysics);

	}
}

AActor* AStagePlayerController::SpawnHeroAtLocation(int32 SelectedCommandSlot, const FVector& WorldLocation, const FRotator& CharRotation, bool IsSpawnEffectFromCrew /*= false*/)
{
	
	FHeroSlotInfo SlotInfo = RGAMEINSTANCE(this)->PlayStateInfo->GetSelectedCharacterSlot(SelectedCommandSlot);
	FTransform SpawnTransform;
	FHeroSlotInfo SlotInfoFirst = RGAMEINSTANCE(this)->PlayStateInfo->GetSelectedCharacterSlot(dMAIN_SLOT_HERO);
	if (IsValid(SlotInfoFirst.SpawnedTargetActor))
		SpawnTransform = SlotInfoFirst.SpawnedTargetActor->GetActorTransform();
	
	SpawnTransform.SetLocation(WorldLocation);
	AActor* SpawnedCharacter = nullptr;
	int32 UserKID = RGAMEINSTANCE(this)->HttpClient->GetKID();
	SpawnedCharacter = UObjectFunctionLibrary::SpawnCharacter(SpawnTransform, SlotInfo.HeroIDName, SlotInfo.CharacterCategoryType,
		UserKID, SlotInfo.HeroUD, GroupKey_SINGLE_MY, PartyID_PC_MY, PLAYER_UK_NONE,
		SlotInfo.HeroLevel, SlotInfo.HeroGrade, true, SelectedCommandSlot, false, IsSpawnEffectFromCrew);

	UBaseStageInfo* BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	if (BaseStageInfo)
		BaseStageInfo->SetPlayer(SpawnedCharacter);

	if (!IsValid(SpawnedCharacter))
	{
		UUtilFunctionLibrary::ShowIndicator(FText::FromString(TEXT("Fail to summon")));
		return nullptr;
	}
	SpawnedCharacter->SetActorRotation(CharRotation);

	RGAMEINSTANCE(this)->PlayStateInfo->SetSpawnedCharacter(SelectedCommandSlot, SpawnedCharacter);

	if (SelectedCommandSlot == dMAIN_SLOT_HERO)
	{
		SetSelectedCharacter(SpawnedCharacter);
		bool bPlayAlone = false;
		if (BaseStageInfo)
			bPlayAlone = BaseStageInfo->bPlayAlone;
		
		
	}

	RGAMEINSTANCE(this)->PlayStateInfo->SetSpawnState(SelectedCommandSlot, EHeroSpawnStateEnum::VE_Spawned);

	auto SkillEntityComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(SpawnedCharacter);
	if (SkillEntityComp)
	{
		SkillEntityComp->PrepareAICastingSkills();
	}


	if (IsValid(RGAMEINSTANCE(this)->BattleRecordManager))
	{
		if (RGAMEINSTANCE(this)->CurState == EGameStateEnum::VE_Play)
		{
			RGAMEINSTANCE(this)->BattleRecordManager->SetHeroBattleInfoDelegate(SpawnedCharacter);
		}
	}

	return SpawnedCharacter;
}

void AStagePlayerController::SetSelectedCharacter(AActor* InSelectedCharacter)
{
	if (RGAMEINSTANCE(this) == nullptr)
		return;
	bool SelectedCharacterChange = (LeaderCharacter != InSelectedCharacter);
	LeaderCharacter = InSelectedCharacter;
	if (LeaderCharacter.IsValid() && SelectedCharacterChange)
	{
		if (IsCustomGame == false)
		{
			if (CurInputCommandType == EInputCommandTypeEnum::VE_AttackRelease)
				CurInputCommandType = EInputCommandTypeEnum::VE_None;
			UAIFunctionLibrary::Stop(LeaderCharacter.Get());
			//SetLeaderCharacterAIAutoMode(EAIControlModeEnum::VE_SemiAutoMode);
		}
		URenewal_BattleFunctionLibrary::VisibleCharacterIndicatorMesh(LeaderCharacter.Get(), false);
		auto BattleEntityComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(LeaderCharacter.Get());
		if (IsValid(BattleEntityComp))
		{
			auto CurrentTarget = BattleEntityComp->GetCurrentAttackTarget();
			URenewal_BattleFunctionLibrary::ComputeTargetDecal(CurrentTarget, false);
		}

		UEntityRenderComponent* EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(LeaderCharacter.Get());
		if (EntityRenderComponent)
			EntityRenderComponent->OutLine(false);
	}

	if (IsValid(InSelectedCharacter))
	{
		if (SelectedCharacterChange)
		{
			UUtilFunctionLibrary::ChangeZOrder(LeaderCharacter.Get());
			if ((Particle_Hero_Gather || Particle_Hero_StageDirection) && LeaderCharacter.IsValid())
			{
				ACharacter* SelectCharacterPtr = Cast<ACharacter>(LeaderCharacter.Get());
				if (Particle_Hero_Gather)
				{
					Particle_Hero_Gather->AttachToComponent(SelectCharacterPtr->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SOCKET_ROOT);
				}
				if (IsStageDirectionProcess())
				{
					SetStageDirection(true);
				}
			}

			if (GetLeaderAIModeType() == EAIControlModeEnum::VE_ManualMode)
			{
				SetLeaderCharacterAIAutoMode(EAIControlModeEnum::VE_ManualMode);

				UAIFunctionLibrary::Stop(LeaderCharacter.Get());

				UCharacterMovementComponent* CharacterMovementComponent = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(InSelectedCharacter);
				if (IsValid(CharacterMovementComponent))
				{
					CharacterMovementComponent->StopActiveMovement();
				}

			}

			URenewal_BattleFunctionLibrary::VisibleCharacterIndicatorMesh(LeaderCharacter.Get(), true);

			UEntityRenderComponent* EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(LeaderCharacter.Get());
			if (EntityRenderComponent)
				EntityRenderComponent->OutLine(false);
			int32 SlotIndex = RGAMEINSTANCE(this)->PlayStateInfo->GetSelectedCharacterSlotIndex(LeaderCharacter.Get());
			RGAMEINSTANCE(this)->PlayStateInfo->SelectSlot(SlotIndex);
		}
	}
	else
	{
		//del? error?
		LeaderCharacter = nullptr;
	}
}

void AStagePlayerController::CheckOwnerAbnormal()
{
	AActor*	pOwner = RGAMEINSTANCE(this)->PlayStateInfo->GetOwnerCharacter(0);
	auto OwnerActor_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(pOwner);
	if (OwnerActor_BattleEntityComponent)
	{
		bool bSetAbnormal = !OwnerActor_BattleEntityComponent->IsAbleToUseSkill();

		if (IsAbnormalState != bSetAbnormal)
		{
			IsAbnormalState = bSetAbnormal;

			if (DPadSkillInterface.IsValid())
			{
				//DPadSkillInterface->SetIconAbnormalState(bSetAbnormal);
			}

			if (IsAbnormalState)
			{
				//if (LeaderControlHelper->GetLeaderSkillControl())
				//{
// 					ULeaderSkillControl* SkillControl = LeaderControlHelper->GetLeaderSkillControl();
// 					if (SkillControl->SkillOrigin == (int32)ESkillOrigin::VE_Leader)
// 					{
// 						SkillControl->IsCancelSkillFire = true;
// 						SkillControl->InputSkillRelease(SkillControl->InputSkillIndex, SkillControl->InputSkillName);
// 					}
				//}
			}
		}
	}
}

void FVirtualJoyStickHelper::Calculate(SVirtualJoystick* InVirtualJoystick)
{
	FVector2D ThumbPos = InVirtualJoystick->GetThumbPosition();
	FVector2D VisualCenter = InVirtualJoystick->GetVisualCenter();
// 	FVector2D ThumbPos;
// 	FVector2D VisualCenter;
	
	//VisualCenter
	URBaseIngameHUD* MyInGameHUD = Cast<URBaseIngameHUD>(UUIFunctionLibrary::GetInGameHUD());
	if (MyInGameHUD && MyInGameHUD->JoyStickDirection)
	{
		MyInGameHUD->JoyStickDirection->SetRenderTranslation(VisualCenter);

		//Set Rotation Math
		if (!ThumbPos.IsZero())
		{
			FVector2D Dir2D = ThumbPos;
			Dir2D.Normalize();
			float cosValue = FVector2D::DotProduct(Dir2D, FVector2D(0.f, -1.f));
			float Angle = 0.f;
			if (Dir2D.X > 0)
				Angle = FMath::Acos(cosValue);
			else
				Angle = 2 * PI - FMath::Acos(cosValue);

			Angle = FMath::RadiansToDegrees<float>(Angle);
			MyInGameHUD->JoyStickDirection->SetRenderAngle(Angle);
			MyInGameHUD->JoyStickDirection->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
		}
		else
			MyInGameHUD->JoyStickDirection->SetVisibility(ESlateVisibility::Collapsed);
	}
}
