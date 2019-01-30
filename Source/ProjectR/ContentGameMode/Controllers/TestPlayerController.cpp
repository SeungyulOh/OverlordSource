// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "TestPlayerController.h"
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
#include "Character/Components/AvoidanceComponent.h"
#include "Object/BaseCameraActor.h"
#include "CustomStructures/SharedStructures/CharacterBattleInfo.h"
#include "Skill_V2/SharedSkill/ISkill_V2.h"
#include "Network/HttpRequestClient.h"
#include "Input/NormalTouchInput.h"
#include "LeaderControlHelper.h"
#include "UI/UIDefs.h"
#include "UI/RWidgetManager.h"
#include "UI/UI_Battle/RCampaignHUD_ThreeControl.h"
#include "UI/UI_Battle/RSkillIcon.h"
#include "UI/UI_Battle/RAttackIcon.h"//temp
#include "UI/InGameHUD/UP_InGameHUD_Arena.h"
#include "UI/Common/UC_Particle.h"


#define DPAD_DEBUG 0

ATestPlayerController::ATestPlayerController(const FObjectInitializer& ObjectInitializer)
	: Super( ObjectInitializer )
{
}

void ATestPlayerController::BeginPlay()
{
	Super::BeginPlay();
	

	UKismetSystemLibrary::ControlScreensaver(false);

	if (InputComponent)
	{
		InputComponent->BindAction("DebugView", IE_Released, this, &ATestPlayerController::ShowDebugPanel);
		InputComponent->BindAxis("ZoomInOut", this, &ATestPlayerController::EditorUseZoomInOut);
		InputComponent->BindAction("TestKey", IE_Released, this, &ATestPlayerController::EditorTestKey);
		InputComponent->BindAction("ActiveSkill_1", IE_Released, this, &ATestPlayerController::EditorActiveSkill1);
		InputComponent->BindAction("ActiveSkill_2", IE_Released, this, &ATestPlayerController::EditorActiveSkill2);
		InputComponent->BindAction("ActiveSkill_3", IE_Released, this, &ATestPlayerController::EditorActiveSkill3);
		InputComponent->BindAction("NormalAttack", IE_Released, this, &ATestPlayerController::EditorNormalAttack);
	}

}

void ATestPlayerController::TickActor(float DeltaTime, enum ELevelTick TickType, FActorTickFunction& ThisTickFunction)
{
	Super::TickActor(DeltaTime, TickType, ThisTickFunction);
}

void ATestPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void ATestPlayerController::ShowDebugPanel()
{
	IsShowDebug = !(IsShowDebug);
	if (IsShowDebug)
	{
		RGAMEINSTANCE(this)->RWidgetManager->ShowUserWidgetAutoCreate(EWidgetBluePrintEnum::WBP_DebugPanel);
	}
	else
	{
		RGAMEINSTANCE(this)->RWidgetManager->HideUserWidget(EWidgetBluePrintEnum::WBP_DebugPanel);
	}
}

void ATestPlayerController::EditorTestKey(FKey InKey)
{
	PlayCameraShake(nullptr, 10);
}

void ATestPlayerController::EditorActiveSkill1()
{
	if (LeaderCharacter.IsValid())
	{
		if (RGAMEINSTANCE(this)->EventManager)
		{
			auto SkillEntComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(LeaderCharacter.Get());
			auto BattleEntComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(LeaderCharacter.Get());
			if (IsValid(SkillEntComp) && IsValid(BattleEntComp))
			{
				UISkill_V2* ActiveSkill = SkillEntComp->GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_ActiveSkill01);
				if (ActiveSkill)
				{
					FName SkillID = ActiveSkill->GetSkillID();
					AActor* CurTarget = BattleEntComp->GetCurrentAttackTarget();
					if (CurTarget)
					{
						FVector Location = UMapFunctionLibrary::GetNavAgentLocation(CurTarget);
						SkillEntComp->CastInputSkill(SkillID, LeaderCharacter.Get(), CurTarget, Location);
					}
				}
			}
		}
	}
}

void ATestPlayerController::EditorActiveSkill2()
{
	if (LeaderCharacter.IsValid())
	{
		if (RGAMEINSTANCE(this)->EventManager)
		{
			auto SkillEntComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(LeaderCharacter.Get());
			auto BattleEntComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(LeaderCharacter.Get());
			if (IsValid(SkillEntComp) && IsValid(BattleEntComp))
			{
				UISkill_V2* ActiveSkill = SkillEntComp->GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_ActiveSkill02);
				if (ActiveSkill)
				{
					FName SkillID = ActiveSkill->GetSkillID();
					AActor* CurTarget = BattleEntComp->GetCurrentAttackTarget();
					if (CurTarget)
					{
						FVector Location = UMapFunctionLibrary::GetNavAgentLocation(CurTarget);
						SkillEntComp->CastInputSkill(SkillID, LeaderCharacter.Get(), CurTarget, Location);
					}
				}
			}
		}
	}
}

void ATestPlayerController::EditorActiveSkill3()
{
	if (LeaderCharacter.IsValid())
	{
		if (RGAMEINSTANCE(this)->EventManager)
		{
			auto SkillEntComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(LeaderCharacter.Get());
			auto BattleEntComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(LeaderCharacter.Get());
			if (IsValid(SkillEntComp) && IsValid(BattleEntComp))
			{
				UISkill_V2* ActiveSkill = SkillEntComp->GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_ActiveSkill03);
				if (ActiveSkill)
				{
					FName SkillID = ActiveSkill->GetSkillID();
					AActor* CurTarget = BattleEntComp->GetCurrentAttackTarget();
					if (CurTarget)
					{
						FVector Location = UMapFunctionLibrary::GetNavAgentLocation(CurTarget);
						SkillEntComp->CastInputSkill(SkillID, LeaderCharacter.Get(), CurTarget, Location);
					}
				}
			}
		}
	}
}

void ATestPlayerController::EditorNormalAttack()
{
	//LeaderControlHelper->GetLeaderMoveControl()->CallbackInputAttackValue(EInputCommandTypeEnum::VE_Attack);
	//LeaderControlHelper->GetLeaderMoveControl()->CallbackInputAttackValue(EInputCommandTypeEnum::VE_AttackRelease);
}

void ATestPlayerController::EditorUseZoomInOut(float InOutValue)
{
	if (IsSetCameraArm)
		return;

	if (InOutValue == 0)
		return;

	if (IsValid(RGAMEINSTANCE(this)->CameraActor))
		RGAMEINSTANCE(this)->CameraActor->ZoomInAndOut(InOutValue);
}

void ATestPlayerController::TempDelayTestPlay()
{
	RGAMEINSTANCE(this)->InitBeforeLaunch();
	RGAMEINSTANCE(this)->InitAfterLaunch();

	RGAMEINSTANCE(this)->HttpClient->SetKID(KID_TEST_MY);

	TArray<uint8> inarray;
	inarray.Reserve(CommandTestSlotCategory.Num());
	for (auto Element : CommandTestSlotCategory)
	{
		uint8 Value = static_cast<uint8>(Element);
		inarray.Emplace(Value);
	}

	RGAMEINSTANCE(this)->PlayStateInfo->InitCharacterSlotForTestPlay(CommandTestInfo, inarray, CommandTestSlotLevel, CommandTestSlotGrade);

	int32 Start = (int32)EGameModeStateEnum::VE_Init;
	int32 Max = (int32)EGameModeStateEnum::VE_End;
	for (; Start < Max; ++Start)
	{
		EGameModeStateEnum CurrentState = (EGameModeStateEnum)Start;
		UUtilFunctionLibrary::ChangeGameModeState(CurrentState);
		UUtilFunctionLibrary::CheckGameModeState();
	}

	UUIFunctionLibrary::AttachHUDPage(GetWorld());
	UUIFunctionLibrary::ActivateJoystick(RGAMEINSTANCE(this)->GetWorld(), true);

	FVector SpawnPos = FVector(0, 0, 0);
	if (CameraActor)
		SpawnPos = CameraActor->GetActorLocation();

	UUtilFunctionLibrary::FirstSpawnPos(GetWorld(), SpawnPos);	
}

void ATestPlayerController::ActorTest()
{
	for (TActorIterator<AActor> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AActor* TargetActor = *ActorItr;
		
		if (TargetActor != this)
		{
			TargetActor->MarkPendingKill();
		}
	}
}
