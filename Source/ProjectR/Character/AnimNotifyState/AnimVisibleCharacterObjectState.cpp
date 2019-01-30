// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "AnimVisibleCharacterObjectState.h"
#include "Character/BattleBaseAnimInstance.h"
#include "Character/Components/FunctionalComponents/EntityRenderComponent.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "UtilFunctionIntegrated.h"
#include "Item/WeaponBase.h"

void UAnimVisibleCharacterObjectState::NotifyBegin(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float TotalDuration)
{
#if LOG_ANIMNOTIFY
	UE_LOG(LogBattle, Log, TEXT("Enter UAnimVisibleCharacterObjectState Begin"));
#endif
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AActor* Owner = AnimInstance->GetOwningActor();
		if (IsValid(Owner))
		{
			ApplyAnimStateImp(Owner, false);
		}
	}
}

void UAnimVisibleCharacterObjectState::NotifyTick(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation, float FrameDeltaTime)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime);
}

void UAnimVisibleCharacterObjectState::NotifyEnd(USkeletalMeshComponent * MeshComp, UAnimSequenceBase * Animation)
{
#if LOG_ANIMNOTIFY
	UE_LOG(LogBattle, Log, TEXT("Enter UAnimVisibleCharacterObjectState End"));
#endif
	Super::NotifyEnd(MeshComp, Animation);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	if (IsValid(AnimInstance))
	{
		AActor* Owner = AnimInstance->GetOwningActor();
		if (IsValid(Owner))
		{
			ApplyAnimStateImp(Owner, true);
		}
	}
}


void UAnimVisibleCharacterObjectState::ApplyAnimStateImp(AActor* character, bool IsNotifyEnd)
{
	if (!IsValid(character))
		return;

	auto ACharacter_character = Cast<ACharacter>(character);
	if (IsValid(ACharacter_character))
	{
		switch (CharacterObjectType)
		{
		case EAttachCharacterObjectEnum::VE_CharacterMesh:
		{
			ACharacter_character->GetMesh()->SetVisibility(IsNotifyEnd);
		}
		break;
		case EAttachCharacterObjectEnum::VE_AllWeapon:
		{
			UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(character);
			if (IsValid(BattleEntityComponent))
			{
				if (BattleEntityComponent->LWeapon.IsValid())
				{
					BattleEntityComponent->LWeapon->SetVisibility(IsNotifyEnd);
				}
				if (BattleEntityComponent->RWeapon.IsValid())
				{
					BattleEntityComponent->RWeapon->SetVisibility(IsNotifyEnd);
				}
			}
		}
		break;
		case EAttachCharacterObjectEnum::VE_LWeapon:
		{
			UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(character);
			if (IsValid(BattleEntityComponent))
			{
				if (BattleEntityComponent->LWeapon.IsValid())
				{
					BattleEntityComponent->LWeapon->SetVisibility(IsNotifyEnd);
				}
			}

		}
		break;
		case EAttachCharacterObjectEnum::VE_RWeapon:
		{
			UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(character);
			if (IsValid(BattleEntityComponent))
			{
				if (BattleEntityComponent->RWeapon.IsValid())
				{
					BattleEntityComponent->RWeapon->SetVisibility(IsNotifyEnd);
				}
			}
		}
		break;
		case EAttachCharacterObjectEnum::VE_DecoEffect:
		{
			UEntityRenderComponent* EntityRenderComponent = UCharacterHelpFunctionLibrary::GetEntityRenderComponent(character);
			if (IsValid(EntityRenderComponent))
			{
				for (auto& Element : EntityRenderComponent->DecoParticles)
				{
					Element->SetVisibility(IsNotifyEnd);
				}
			}
		}
		break;
		case EAttachCharacterObjectEnum::VE_All:
		{
			//EAttachCharacterObjectEnum::VE_All
			//for ()
		}
		break;
		}
	}
}