// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/BossComponents/BossBaseComponent.h"
#include "Character/Components/AnimStateComponent.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"

#include "SharedConstants/GlobalConstants.h"
#include "GlobalIntegrated.h"
#include "UtilFunctionIntegrated.h"


#include "VibrationComponent.h"


// Sets default values for this component's properties
UVibrationComponent::UVibrationComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bWantsInitializeComponent = true;
}

void UVibrationComponent::InitializeComponent()
{
	Super::InitializeComponent();

	OwnerActor = GetOwner();

	CurveVibration = UUtilFunctionLibrary::GetCurveFloat("ActorVibration");
}

// Called when the game starts
void UVibrationComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
	MeshComponent		= UCharacterHelpFunctionLibrary::GetComponent<UMeshComponent>(GetOwner());
	AnimStateComponent	= UCharacterHelpFunctionLibrary::GetAnimStateComponent(GetOwner());

	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(GetOwner()->GetWorld());

	auto BattleEntityComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(GetOwner());

	if (BattleEntityComp)
	{
		if (BattleEntityComp->OnReceiveDamageDelegate.IsAlreadyBound(this, &UVibrationComponent::OnReceiveDamage))
			BattleEntityComp->OnReceiveDamageDelegate.RemoveDynamic(this, &UVibrationComponent::OnReceiveDamage);

			BattleEntityComp->OnReceiveDamageDelegate.AddDynamic(this, &UVibrationComponent::OnReceiveDamage);
	}

	IsVibration = false;
	
}

void UVibrationComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	auto BattleEntityComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(GetOwner());
	if (BattleEntityComp)
	{
		if (BattleEntityComp->OnReceiveDamageDelegate.IsAlreadyBound(this, &UVibrationComponent::OnReceiveDamage))
			BattleEntityComp->OnReceiveDamageDelegate.RemoveDynamic(this, &UVibrationComponent::OnReceiveDamage);
	}

	Super::EndPlay(EndPlayReason);
}


// Called every frame
void UVibrationComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if( !IsActivateComponent)
		return;


	if(MeshComponent == nullptr)
		return;

	if (nullptr == CurveVibration)
		return;

	// ...
	if (IsVibration)
	{
		EllapsedTime += (DeltaTime * TimeRatio);		

		if (EllapsedTime <= DurationTime)
		{			
			float curInTime			= FMath::Clamp<float>(EllapsedTime, 0, 1.0f);
			float val				= CurveVibration->GetFloatValue(curInTime);
			//float rndVal			= FMath::RandRange(0.f, val);
			FVector NewLoc			= InitLocation + (-ForwardVector * val);

			MeshComponent->SetRelativeLocation(NewLoc);
		}
		else
		{
			IsVibration = false;
			EllapsedTime = 0.0f;
			MeshComponent->SetRelativeLocation(InitLocation);
		}
	}
	
}


void UVibrationComponent::OnReceiveDamage(AActor* Invoker, float Damage, EBattleDamageResultEnum DamageResultType, AActor* FromActor)
{
	if (EBattleDamageResultEnum::VE_Dead == DamageResultType)
		return;

	if (EBattleDamageResultEnum::VE_Heal == DamageResultType)
		return;

	// Except boss
	auto BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(OwnerActor.Get());
	if (BattleEntityComponent)
	{
		if (IsValid(BattleEntityComponent->BossComponent))
			return;
	}

	if (AnimStateComponent)
	{
		if( AnimStateComponent->GetCurrentAnimStateType() == EAnimStateEnum::VE_Die || 
			AnimStateComponent->GetCurrentAnimStateType() == EAnimStateEnum::VE_Spawn)
			return;
	}

	if (BasePlayerController == nullptr)
		return;

	if(BasePlayerController->GetSelectedCharacter() == nullptr || FromActor == nullptr)
		return;

	if (BasePlayerController->GetSelectedCharacter() != FromActor)
		return;
	
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance)
	{
		if (DamageResultType == EBattleDamageResultEnum::VE_Critical)
			DurationTime = RGameInstance->GameEnvironmentVar.DurationVibrationTime_Critical;
		else
			DurationTime = RGameInstance->GameEnvironmentVar.DurationVibrationTime_Normal;

	}
	

	if (MeshComponent)
	{
		if (!IsVibration)
			InitLocation = MeshComponent->GetRelativeTransform().GetLocation();
		else
			MeshComponent->SetRelativeLocation(InitLocation);
	}
	
			
	IsVibration = true;
	EllapsedTime = 0.0f;
	TimeRatio  = 1.0f / DurationTime;
		
	
	if (IsValid(FromActor))
	{
		FVector FromActorForward = FromActor->GetTransform().GetRotation().GetForwardVector();
		ForwardVector = FromActor->GetTransform().InverseTransformVector(FromActorForward);
	}
	else
	{
		ForwardVector = MeshComponent->GetComponentTransform().GetRotation().GetForwardVector();
	}
}

void UVibrationComponent::ClearEvent()
{
	auto BattleEntityComp = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(GetOwner());

	if (BattleEntityComp)
	{
		BattleEntityComp->OnReceiveDamageDelegate.RemoveDynamic(this, &UVibrationComponent::OnReceiveDamage);
	}
}
