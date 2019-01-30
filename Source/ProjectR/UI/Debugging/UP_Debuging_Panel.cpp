// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_Debuging_Panel.h"

#include "GlobalIntegrated.h"
#include "Character/Components/FunctionalComponents/EntityAIComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/AnimStateComponent.h"
#include "Skill_V2/SharedSkill/SkillSlotContainer.h"
#include "Skill_V2/SharedSkill/SkillPassiveSlotContainer.h"
#include "Skill_V2/SharedSkill/SkillSlot.h"
#include "UtilFunctionIntegrated.h"

void UUP_Debuging_Panel::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUP_Debuging_Panel::NativeDestruct()
{	
	Super::NativeDestruct();
}

void UUP_Debuging_Panel::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	InvalidateData();
}

void UUP_Debuging_Panel::InvalidateData()
{
	if (!IsValid(TextBlock_DebugText))
		return;
	
	if (OwnerCharacter.IsValid() == false)
		return;

	UEntityAIComponent* EntityAIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerCharacter.Get());
	USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerCharacter.Get());
	if (IsValid(EntityAIComponent) == false || IsValid(SkillEntityComponent) == false)
		return;

	FString ResultString;
	if (bShowForceTarget)
	{
		AActor* ForceTargetActor = EntityAIComponent->GetBlackboardActorValue(ERAIBalckboardValueEnum::VE_ForceTarget);
		if (ForceTargetActor)
		{
			ResultString += FString::Printf(TEXT("ForceTarget :  %s\n"), *(ForceTargetActor->GetName()));
		}
	}

	if (bShowAnimState)
	{
		UAnimStateComponent* AnimStateComponent = UCharacterHelpFunctionLibrary::GetAnimStateComponent(OwnerCharacter.Get());
		if (AnimStateComponent)
		{
			FString	stringAnimState = UUtilFunctionLibrary::GetEnumToString(TEXT("EAnimStateEnum"), (int32)AnimStateComponent->GetCurrentAnimStateType());
			ResultString += FString::Printf(TEXT("AnimState :  %s\n"), *stringAnimState);
		}
	}

	if (bShowAIState)
	{
		FString	stringAIState = UUtilFunctionLibrary::GetEnumToString(TEXT("EAIStateTypeEnum"), (int32)EntityAIComponent->GetCurrentAIState());

		ResultString += FString::Printf(TEXT("AI State :  %s\n"), *stringAIState);
	}

#if WITH_EDITOR
	if (bShowBuffList)
	{
		if (SkillEntityComponent->GetSkillSlotContainer())
		{
			TArray< USkillSlot* >& AllSlots = SkillEntityComponent->GetSkillSlotContainer()->GetAllSlots();
			int32 index = 1;
			for (auto skillSlot : AllSlots)
			{
				if (skillSlot->IsActivated())
				{
					ResultString += FString::Printf(TEXT("Buff : %d - %s( %.2f:%.2f )\n"), index, *(UUtilFunctionLibrary::GetEnumToDisplayName(TEXT("EAbnormalSlotTypeEnum"), (int32)(skillSlot->GetSlotType())).ToString()), skillSlot->GetDuration(), skillSlot->GetDurationElapsedTime());
					++index;
				}
			}
		}

		if (SkillEntityComponent->GetPassiveSkillSlotContainer())
		{
			TArray< USkillSlot* >& AllSlots = SkillEntityComponent->GetPassiveSkillSlotContainer()->GetAllSlots();
			int32 index = 1;
			for (auto skillSlot : AllSlots)
			{
				if (skillSlot->IsActivated())
				{
					TArray< FSkillApplierBase > Appliers = skillSlot->GetApplierList();
					for (FSkillApplierBase& SkillApplier : Appliers)
					{
						ResultString += FString::Printf(TEXT("Passive : %d - %s\n"), index, *(UUtilFunctionLibrary::GetEnumToDisplayName(TEXT("ESkillApplierTypeEnum"), (int32)(SkillApplier.SkillApplierType)).ToString()));
						++index;
					}
				}
			}
		}
	}
#endif

	TextBlock_DebugText->SetText(FText::FromString(ResultString));

}