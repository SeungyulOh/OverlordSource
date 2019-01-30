// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GigantComponent.h"


#include "UtilFunctionIntegrated.h"
#include "Character/Components/AvoidanceComponent.h"



// Sets default values for this component's properties
UGigantComponent::UGigantComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGigantComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
}


void UGigantComponent::SetOwner(AActor* character)
{
	Super::SetOwner(character);

	if (IsValid(BattleActor))
	{
		FrontVector = BattleActor->GetActorForwardVector().GetSafeNormal2D();

		UCharacterMovementComponent* CharacterMovementComponent = UCharacterHelpFunctionLibrary::GetCharacterMovementComponent(BattleActor);
		if (IsValid(CharacterMovementComponent))
		{
			CharacterMovementComponent->SetMovementMode(MOVE_Flying);
			CharacterMovementComponent->MaxCustomMovementSpeed = 0.0f;
			CharacterMovementComponent->MaxWalkSpeed = 0.0f;
			CharacterMovementComponent->MaxWalkSpeedCrouched = 0.0f;
			CharacterMovementComponent->MaxFlySpeed = 0.0f;
			CharacterMovementComponent->MaxSwimSpeed = 0.0f;
		}

		UAvoidanceComponent* AvoidanceComponent = UCharacterHelpFunctionLibrary::GetComponent<UAvoidanceComponent>(BattleActor);
		if (IsValid(AvoidanceComponent))
		{
			AvoidanceComponent->SetEnableAIMovementCondition(false);
		}
	}
}

FVector UGigantComponent::GetGigantDir()
{
	return FrontVector;
}

void UGigantComponent::ChangeGigantDir()
{
	FrontVector *= -1.0f;
}

bool UGigantComponent::CanAttachSlot(EAbnormalSlotTypeEnum SlotType)
{
	if (SlotType == EAbnormalSlotTypeEnum::VE_Slot_StatUpPhysicalAttack ||
		SlotType == EAbnormalSlotTypeEnum::VE_Slot_Provoke ||
		SlotType == EAbnormalSlotTypeEnum::VE_Slot_DamageShieldRecovery ||

		SlotType == EAbnormalSlotTypeEnum::VE_Slot_Ice ||
		SlotType == EAbnormalSlotTypeEnum::VE_Slot_Freezing ||

		SlotType == EAbnormalSlotTypeEnum::VE_Slot_Fire_Flame ||
		SlotType == EAbnormalSlotTypeEnum::VE_Slot_Fire_Scarlet ||
		SlotType == EAbnormalSlotTypeEnum::VE_Slot_Fire_Akhan ||

		SlotType == EAbnormalSlotTypeEnum::VE_Slot_Ignite_Flame ||
		SlotType == EAbnormalSlotTypeEnum::VE_Slot_Ignite_Scarlet
		)
		return true;

	return false;
}
