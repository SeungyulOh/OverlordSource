// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "AkhanComponent.h"


#include "UtilFunctionIntegrated.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/BattleBaseAnimInstance.h"



// Sets default values for this component's properties
UAkhanComponent::UAkhanComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UAkhanComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}

void UAkhanComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsValid(BattleActor))
	{
		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(BattleActor);
		if (IsValid(BattleEntityComponent))
		{
			if (BattleEntityComponent->IsDead())
			{
				switch (CurSituationState)
				{
				case ECharacterSituationState::VE_Air:
				case ECharacterSituationState::VE_ProgressAir:
				{
					if (!bFallStart)
					{
						bFallStart = true;
						CurAniTime = 0.0f;
						CurStartHeight = BattleActor->GetActorLocation().Z;
					}
				}
				break;
				}
			}
		}

		if (bFallStart)
		{
			CurAniTime += DeltaTime;
			if (CurAniTime >= FallTime)
			{
				CurAniTime = FallTime;
			}

			float MoveDiff = FMath::InterpEaseInOut<float>(CurStartHeight, StartHeight, CurAniTime / FallTime, 2.0f);

			FVector Pos = BattleActor->GetActorLocation();
			Pos.Z = MoveDiff;

			BattleActor->SetActorLocation(Pos);
		}
		else
		{
			switch (CurSituationState)
			{
			case ECharacterSituationState::VE_ProgressAir:
			case ECharacterSituationState::VE_ProgressGround:
			{
				CurAniTime += DeltaTime;
				if (CurAniTime >= TotalAniTime)
				{
					CurAniTime = TotalAniTime;
				}

				float MoveDiff = 0.0f;
				if (CurSituationState == ECharacterSituationState::VE_ProgressAir)
				{
					MoveDiff = FMath::InterpEaseInOut<float>(CurStartHeight, StartHeight + Height, CurAniTime / TotalAniTime, 2.0f);
				}
				else
				{
					MoveDiff = FMath::InterpEaseInOut<float>(CurStartHeight, StartHeight, CurAniTime / TotalAniTime, 2.0f);
				}

				FVector Pos = BattleActor->GetActorLocation();
				Pos.Z = MoveDiff;

				BattleActor->SetActorLocation(Pos);
			}
			break;
			}
		}
	}
}

void UAkhanComponent::SetOwner(AActor* character)
{
	Super::SetOwner(character);

	if (IsValid(BattleActor))
	{
		StartHeight = BattleActor->GetActorLocation().Z;

		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(BattleActor);
		if (IsValid(BattleEntityComponent))
		{
			CurSituationState = BattleEntityComponent->GetSituationState();			
		}
	}
}

void UAkhanComponent::ChangeSituation(ECharacterSituationState InState, float InTotalTime)
{
	if (IsValid(BattleActor))
	{
		UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(BattleActor);
		UBattleBaseAnimInstance* BattleBaseAnim = UCharacterHelpFunctionLibrary::GetCharacterBattleBaseAnimInstance(BattleActor);
		
		if (IsValid(BattleEntityComponent) && IsValid(BattleBaseAnim))
		{
			CurSituationState = InState;
			switch (CurSituationState)
			{
			case ECharacterSituationState::VE_Ground:
			{
				BattleBaseAnim->SetFlyState(false);
				auto RGameInstance = RGAMEINSTANCE(this);
				if (RGameInstance)
				{
					RGameInstance->CameraActor->SetIsSequenceCameraState(false);
				}
			}
			break;
			case ECharacterSituationState::VE_Air:
			{
				BattleBaseAnim->SetFlyState(true);
				auto RGameInstance = RGAMEINSTANCE(this);
				if (RGameInstance)
				{
					RGameInstance->CameraActor->SetIsSequenceCameraState(true);
				}
			}
			break;
			case ECharacterSituationState::VE_ProgressGround:
			case ECharacterSituationState::VE_ProgressAir:			
			{
				CurAniTime = 0.0f;
				TotalAniTime = InTotalTime;
				CurStartHeight = BattleActor->GetActorLocation().Z;
			}
			break;
			case ECharacterSituationState::VE_ProgressAirHigh:
			{				
				CurAniTime = 0.0f;
				TotalAniTime = InTotalTime;
				CurStartHeight = BattleActor->GetActorLocation().Z;
			}
			break;
			case ECharacterSituationState::VE_AirHigh:
			{			
				BattleBaseAnim->SetFlyState(true);
			}
			break;
			}
		}
	}
}

bool UAkhanComponent::CanAttachSlot(EAbnormalSlotTypeEnum SlotType)
{
	if (SlotType == EAbnormalSlotTypeEnum::VE_Slot_Provoke ||
		SlotType == EAbnormalSlotTypeEnum::VE_Slot_Ice ||
		SlotType == EAbnormalSlotTypeEnum::VE_Slot_Freezing ||
		SlotType == EAbnormalSlotTypeEnum::VE_Slot_StatUpMagicalAttack ||

		SlotType == EAbnormalSlotTypeEnum::VE_Slot_Poison_Jin ||
		SlotType == EAbnormalSlotTypeEnum::VE_Slot_Poison_Yoseph ||
		SlotType == EAbnormalSlotTypeEnum::VE_Slot_Poison_Zagan ||

		SlotType == EAbnormalSlotTypeEnum::VE_Slot_Bleeding_Bartor ||
		SlotType == EAbnormalSlotTypeEnum::VE_Slot_Bleeding_Buddy ||
		SlotType == EAbnormalSlotTypeEnum::VE_Slot_Bleeding_Than ||

		SlotType == EAbnormalSlotTypeEnum::VE_Slot_Invincible_Akhan)
		
		return true;
	
	return false;
}
