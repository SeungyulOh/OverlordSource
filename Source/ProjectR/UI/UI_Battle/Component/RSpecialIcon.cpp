// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"

#include "classes/Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "Character/BattleBaseCharacter.h"
#include "Character/BattleBaseAnimInstance.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Table/CharacterTableInfo.h"
#include "Item/WeaponBase.h"
#include "UtilFunctionIntegrated.h"
#include "RSpecialIcon.h"
#include "RSkillSet_New.h"

void URSpecialIcon::NativeConstruct()
{
	Super::NativeConstruct();
	
	if (Button_On)
	{
		Button_On->OnClicked.RemoveDynamic(this, &URSpecialIcon::BtnEventOnClicked);
		Button_On->OnClicked.AddDynamic(this, &URSpecialIcon::BtnEventOnClicked);
	}
	if (Button_Off)
	{
		Button_Off->OnClicked.RemoveDynamic(this, &URSpecialIcon::BtnEventOnClicked);
		Button_Off->OnClicked.AddDynamic(this, &URSpecialIcon::BtnEventOnClicked);
	}

	BasePlayerController = UUtilFunctionLibrary::GetBasePlayerController(this->GetWorld());
}

void URSpecialIcon::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsToggle)
	{
		if (IsReset && BasePlayerController)
		{
			AActor* Actor = BasePlayerController->GetSelectedCharacter();
			
			auto BattleActor = Cast<ABattleBaseCharacter>(Actor);
			if (IsValid(BattleActor) == false)
				return;
			
			auto SkeletalMeshComp = BattleActor->GetMesh();
			if (IsValid(SkeletalMeshComp) == false)
				return;
				
			if (SkeletalMeshComp->IsPlaying() == false)
			{
				InitLp();
				IsReset = false;
			}
		}
	}
}

void URSpecialIcon::BtnEventOnClicked()
{
	IsToggle = !(IsToggle);
	if (Switcher_IconImage)
	{
		Switcher_IconImage->SetActiveWidgetIndex(int32(IsToggle));
	}

	if (BasePlayerController)
	{
		AActor* Actor = BasePlayerController->GetSelectedCharacter();
		auto BattleActor = Cast<ABattleBaseCharacter>(Actor);
		if (BattleActor)
		{
			if (IsToggle)
			{
				UUIFunctionLibrary::ActivateJoystick(GetWorld(), false);
				UUIFunctionLibrary::GetSkillSet(this)->SetVisibility(ESlateVisibility::Collapsed);

				AttachTemp(IsToggle);
				//InitLp();
				PrepareAction(ESpecialActionStateEnum::VE_Prepare);
			}
			else
			{
				UUIFunctionLibrary::ActivateJoystick(GetWorld(), true);
				UUIFunctionLibrary::GetSkillSet(this)->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				
				AttachTemp(IsToggle);
				
				BattleActor->GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
				BattleActor->GetWorldTimerManager().ClearTimer(TimeHandler);
			}
		}
	}
}

void URSpecialIcon::AttachTemp(bool InVisible)
{
	AActor* Actor = BasePlayerController->GetSelectedCharacter();
	UBattleEntityComponent* BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Actor);
	if (IsValid(BattleEntityComponent))
	{
		auto RWea = BattleEntityComponent->GetWeaponActor(true);
		if (RWea)
		{
			RWea->SetVisibility(!(IsToggle));
		}
		auto LWea = BattleEntityComponent->GetWeaponActor(false);
		if (LWea)
		{
			LWea->SetVisibility(!(IsToggle));
		}
	}

	if (IsToggle)
	{
		UClass* classType = LoadClass<AWeaponBase>(NULL, TEXT("/Game/Character/Characters_Weapons/Rod/BP_FRod_01.BP_FRod_01_C"), NULL, LOAD_None, NULL);;
		if (IsValid(classType))
		{
			if (IsValid(BattleEntityComponent))
			{
				FName socketName = TEXT("Socket_RightWeapon");
				auto BattleActor = Cast<ABattleBaseCharacter>(Actor);
				if (BattleActor)
				{
					TArray<FName> SocketArray = BattleActor->GetMesh()->GetAllSocketNames();
					if (SocketArray.Contains(socketName) == false)
					{
						socketName = TEXT("Socket_LeftWeapon");
					}
				}
				TempWeapon = BattleEntityComponent->AttachWeaponBase(classType, socketName);
			}
		}
	}
	else 
	{
		if (TempWeapon)
		{
			TempWeapon->SetVisibility(false);
			TempWeapon->Destroy();
			TempWeapon = nullptr;
		}
	}
}

void URSpecialIcon::PrepareAction(ESpecialActionStateEnum InActionState, bool InLoop /*= false*/, float InPlayRate /*= 1.0f*/)
{
	if (BasePlayerController == nullptr)
		return;
	
	AActor* Actor = BasePlayerController->GetSelectedCharacter();
	auto BattleActor = Cast<ABattleBaseCharacter>(Actor);
	if (BattleActor)
	{
		auto BaseProperty = Cast<IEntityBaseProperty>(BattleActor);
		if (BaseProperty != nullptr)
		{
			FCharacterTableInfo* CharacterInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum::VE_Hero, BaseProperty->GetCharacterID());

			TAssetPtr< UAnimationAsset > ActionAsset = nullptr;
			switch (InActionState)
			{
			case ESpecialActionStateEnum::VE_Prepare:
				break;
			case ESpecialActionStateEnum::VE_Loop:
				break;
			case ESpecialActionStateEnum::VE_Action:
				break;
			default:
				break;
			}

			UAnimationAsset* AnimAsset = UUtilFunctionLibrary::GetAnimationAssetFromAssetID(ActionAsset);
			if (AnimAsset)
			{
				auto SkeletalMeshComp = BattleActor->GetMesh();
				if (IsValid(SkeletalMeshComp))
				{
					SkeletalMeshComp->PlayAnimation(AnimAsset, InLoop);
					SkeletalMeshComp->SetPlayRate(InPlayRate);
				}

				if (InLoop)
				{
					float SpecialCheckTime = ((FMath::Rand() % 400) / 100.0f) + 4.0f;
					BattleActor->GetWorldTimerManager().SetTimer(TimeHandler, this, &URSpecialIcon::CheckFunc, SpecialCheckTime);
				}
				IsReset = !(InLoop);
			}
		}
	}
}

void URSpecialIcon::InitLp()
{
	AActor* Actor = BasePlayerController->GetSelectedCharacter();
	auto BattleActor = Cast<ABattleBaseCharacter>(Actor);
	if (BattleActor)
	{
		auto BaseProperty = Cast<IEntityBaseProperty>(BattleActor);
		if (BaseProperty != nullptr)
		{
			FCharacterTableInfo* CharacterInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum::VE_Hero, BaseProperty->GetCharacterID());
			UAnimationAsset* AnimAsset = nullptr;
			if (AnimAsset)
			{
				auto SkeletalMeshComp = BattleActor->GetMesh();
				if (IsValid(SkeletalMeshComp))
				{
					SkeletalMeshComp->PlayAnimation(AnimAsset, true);
					SkeletalMeshComp->SetPlayRate(1.0f);
				}
			}
		}

		float SpecialCheckTime = ((FMath::Rand() % 400) / 100.0f) + 4.0f;
		BattleActor->GetWorldTimerManager().SetTimer(TimeHandler, this, &URSpecialIcon::CheckFunc, SpecialCheckTime);
	}
}

void URSpecialIcon::CheckFunc()
{
	UUtilFunctionLibrary::ShowIndicator(FText::FromString(TEXT("iiiiiiiiiiya!!!!!!!!!")));
	IsReset = true;


	AActor* Actor = BasePlayerController->GetSelectedCharacter();
	auto BattleActor = Cast<ABattleBaseCharacter>(Actor);
	if (BattleActor)
	{
		if (IsToggle)
		{
			auto BaseProperty = Cast<IEntityBaseProperty>(BattleActor);
			if (BaseProperty != nullptr)
			{
				FCharacterTableInfo* CharacterInfo = URenewal_BattleFunctionLibrary::GetCharacterTableInfo(ECharacterCategoryTypeEnum::VE_Hero, BaseProperty->GetCharacterID());
				UAnimationAsset* AnimAsset = nullptr;
				if (AnimAsset)
				{
					auto SkeletalMeshComp = BattleActor->GetMesh();
					if (IsValid(SkeletalMeshComp))
					{
						SkeletalMeshComp->PlayAnimation(AnimAsset, false);
						SkeletalMeshComp->SetPlayRate(0.5f);
					}
				}
			}
		}
	}
}