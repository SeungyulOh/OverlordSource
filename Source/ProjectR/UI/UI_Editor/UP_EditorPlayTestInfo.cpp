// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "UP_EditorPlayTestInfo.h"

#include "Character/Interfaces/EntityBaseProperty.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityAIComponent.h"
#include "UtilFunctionIntegrated.h"

#include "Skill_V2/SharedSkill/ISkill_V2.h"

#if WITH_EDITOR
#include "Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#endif

void UUP_EditorPlayTestInfo::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUP_EditorPlayTestInfo::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (OwnerActor.IsValid())
	{
		USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerActor.Get());
		if (IsValid(SkillEntityComponent))
		{
			ActiveSkill1 = SkillEntityComponent->GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_ActiveSkill01);
			if (IsValid(ActiveSkill1))
			{
				if (IsValid(Skill1CoolTimeText))
				{
					float cooltime = SkillEntityComponent->GetSkillCoolTime_V2(ActiveSkill1->GetSkillID());
					Skill1CoolTimeText->SetText(FText::AsNumber((int32)cooltime));
				}
			}

			ActiveSkill2 = SkillEntityComponent->GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_ActiveSkill02);
			if (IsValid(ActiveSkill2))
			{
				if (IsValid(Skill2CoolTimeText))
				{
					float cooltime = SkillEntityComponent->GetSkillCoolTime_V2(ActiveSkill2->GetSkillID());
					Skill2CoolTimeText->SetText(FText::AsNumber((int32)cooltime));
				}
			}

			ActiveSkill3 = SkillEntityComponent->GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_ActiveSkill03);
			if (IsValid(ActiveSkill3))
			{
				if (IsValid(Skill3CoolTimeText))
				{
					float cooltime = SkillEntityComponent->GetSkillCoolTime_V2(ActiveSkill3->GetSkillID());
					Skill3CoolTimeText->SetText(FText::AsNumber((int32)cooltime));
				}
			}

			PassiveSkill1 = SkillEntityComponent->GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_PassiveSkill01);
			if (IsValid(PassiveSkill1))
			{
				if (IsValid(Skill4CoolTimeText))
				{
					float cooltime = SkillEntityComponent->GetSkillCoolTime_V2(PassiveSkill1->GetSkillID());
					Skill4CoolTimeText->SetText(FText::AsNumber((int32)cooltime));
				}
			}

			PassiveSkill2 = SkillEntityComponent->GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_PassiveSkill02);
			if (IsValid(PassiveSkill2))
			{
				if (IsValid(Skill5CoolTimeText))
				{
					float cooltime = SkillEntityComponent->GetSkillCoolTime_V2(PassiveSkill2->GetSkillID());
					Skill5CoolTimeText->SetText(FText::AsNumber((int32)cooltime));
				}
			}
		}
	}
}

void UUP_EditorPlayTestInfo::SetActor(AActor* InActor)
{
	OwnerActor = InActor;

	ActiveSkill1 = nullptr;
	ActiveSkill2 = nullptr;
	ActiveSkill3 = nullptr;
	PassiveSkill1 = nullptr;
	PassiveSkill2 = nullptr;

	if (OwnerActor.IsValid())
	{
		USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerActor.Get());
		if (IsValid(SkillEntityComponent))
		{
			ActiveSkill1 = SkillEntityComponent->GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_ActiveSkill01);
			if (IsValid(ActiveSkill1))
			{
				if (IsValid(Skill1Btn))
				{
					Skill1Btn->SetVisibility(ESlateVisibility::Visible);
				}

				if (IsValid(Skill1CoolTimeText))
				{
					Skill1CoolTimeText->SetVisibility(ESlateVisibility::Visible);
					Skill1CoolTimeText->SetText(FText::AsNumber(0));
				}
			}
			else
			{
				if (IsValid(Skill1Btn))
				{
					Skill1Btn->SetVisibility(ESlateVisibility::Collapsed);
				}

				if (IsValid(Skill1CoolTimeText))
				{
					Skill1CoolTimeText->SetVisibility(ESlateVisibility::Collapsed);
				}
			}

			ActiveSkill2 = SkillEntityComponent->GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_ActiveSkill02);
			if (IsValid(ActiveSkill2))
			{
				if (IsValid(Skill2Btn))
				{
					Skill2Btn->SetVisibility(ESlateVisibility::Visible);
				}

				if (IsValid(Skill2CoolTimeText))
				{
					Skill2CoolTimeText->SetVisibility(ESlateVisibility::Visible);
					Skill2CoolTimeText->SetText(FText::AsNumber(0));
				}
			}
			else
			{
				if (IsValid(Skill2Btn))
				{
					Skill2Btn->SetVisibility(ESlateVisibility::Collapsed);
				}

				if (IsValid(Skill2CoolTimeText))
				{
					Skill2CoolTimeText->SetVisibility(ESlateVisibility::Collapsed);
				}
			}

			ActiveSkill3 = SkillEntityComponent->GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_ActiveSkill03);
			if (IsValid(ActiveSkill3))
			{
				if (IsValid(Skill3Btn))
				{
					Skill3Btn->SetVisibility(ESlateVisibility::Visible);
				}

				if (IsValid(Skill3CoolTimeText))
				{
					Skill3CoolTimeText->SetVisibility(ESlateVisibility::Visible);
					Skill3CoolTimeText->SetText(FText::AsNumber(0));
				}
			}
			else
			{
				if (IsValid(Skill3Btn))
				{
					Skill3Btn->SetVisibility(ESlateVisibility::Collapsed);
				}

				if (IsValid(Skill3CoolTimeText))
				{
					Skill3CoolTimeText->SetVisibility(ESlateVisibility::Collapsed);
				}
			}

			PassiveSkill1 = SkillEntityComponent->GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_PassiveSkill01);
			if (IsValid(PassiveSkill1))
			{
				if (IsValid(Skill4Btn))
				{
					Skill4Btn->SetVisibility(ESlateVisibility::Visible);
				}

				if (IsValid(Skill4CoolTimeText))
				{
					Skill4CoolTimeText->SetVisibility(ESlateVisibility::Visible);
					Skill4CoolTimeText->SetText(FText::AsNumber(0));
				}
			}
			else
			{
				if (IsValid(Skill4Btn))
				{
					Skill4Btn->SetVisibility(ESlateVisibility::Collapsed);
				}

				if (IsValid(Skill4CoolTimeText))
				{
					Skill4CoolTimeText->SetVisibility(ESlateVisibility::Collapsed);
				}
			}

			PassiveSkill2 = SkillEntityComponent->GetSkillByCategory_V2(ESkillCategoryTypeEnum::VE_PassiveSkill02);
			if (IsValid(PassiveSkill2))
			{
				if (IsValid(Skill5Btn))
				{
					Skill5Btn->SetVisibility(ESlateVisibility::Visible);
				}

				if (IsValid(Skill5CoolTimeText))
				{
					Skill5CoolTimeText->SetVisibility(ESlateVisibility::Visible);
					Skill5CoolTimeText->SetText(FText::AsNumber(0));
				}
			}
			else
			{
				if (IsValid(Skill5Btn))
				{
					Skill5Btn->SetVisibility(ESlateVisibility::Collapsed);
				}

				if (IsValid(Skill5CoolTimeText))
				{
					Skill5CoolTimeText->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
		}

		UEntityAIComponent* AIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerActor.Get());
		if (IsValid(AIComponent))
		{
			if (IsValid(AICheckBox))
			{
				AICheckBox->SetCheckedState(AIComponent->IsActiveAI() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
			}
		}
	}
}

void UUP_EditorPlayTestInfo::AICheckBoxExe(bool bActive)
{
	if (OwnerActor.IsValid())
	{
		UEntityAIComponent* AIComponent = UCharacterHelpFunctionLibrary::GetEntityAIComponent(OwnerActor.Get());
		if (IsValid(AIComponent))
		{
			if (bActive)
			{
				AIComponent->AIRestart();
			}
			else
			{
				AIComponent->AIStop();
			}

			if (IsValid(AICheckBox))
			{
				AICheckBox->SetCheckedState(AIComponent->IsActiveAI() ? ECheckBoxState::Checked : ECheckBoxState::Unchecked);
			}
		}
	}
}

void UUP_EditorPlayTestInfo::Skill1Exe()
{
	if (OwnerActor.IsValid())
	{
		USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerActor.Get());
		if (IsValid(SkillEntityComponent))
		{
			if (SkillEntityComponent->IsSkillCasting())
				return;

			if (SkillEntityComponent->IsSkillActing())
				return;

			if (IsValid(ActiveSkill1))
			{
				FVector SkillLocation = FVector::ZeroVector;
				if (SkillEntityComponent->CastInputSkill(ActiveSkill1->GetSkillID(), OwnerActor.Get(), nullptr))
				{

				}
			}
		}
	}
}

void UUP_EditorPlayTestInfo::Skill2Exe()
{
	if (OwnerActor.IsValid())
	{
		USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerActor.Get());
		if (IsValid(SkillEntityComponent))
		{
			if (SkillEntityComponent->IsSkillCasting())
				return;

			if (SkillEntityComponent->IsSkillActing())
				return;

			if (IsValid(ActiveSkill2))
			{
				FVector SkillLocation = FVector::ZeroVector;
				if (SkillEntityComponent->CastInputSkill(ActiveSkill2->GetSkillID(), OwnerActor.Get(), nullptr))
				{

				}
			}
		}
	}
}

void UUP_EditorPlayTestInfo::Skill3Exe()
{
	if (OwnerActor.IsValid())
	{
		USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerActor.Get());
		if (IsValid(SkillEntityComponent))
		{
			if (SkillEntityComponent->IsSkillCasting())
				return;

			if (SkillEntityComponent->IsSkillActing())
				return;

			if (IsValid(ActiveSkill3))
			{
				FVector SkillLocation = FVector::ZeroVector;
				if (SkillEntityComponent->CastInputSkill(ActiveSkill3->GetSkillID(), OwnerActor.Get(), nullptr))
				{

				}
			}
		}
	}
}

void UUP_EditorPlayTestInfo::Skill4Exe()
{
	if (OwnerActor.IsValid())
	{
		USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerActor.Get());
		if (IsValid(SkillEntityComponent))
		{
			if (SkillEntityComponent->IsSkillCasting())
				return;

			if (SkillEntityComponent->IsSkillActing())
				return;

			if (IsValid(PassiveSkill1))
			{
				FVector SkillLocation = FVector::ZeroVector;
				if (SkillEntityComponent->CastInputSkill(PassiveSkill1->GetSkillID(), OwnerActor.Get(), nullptr))
				{

				}
			}
		}
	}
}

void UUP_EditorPlayTestInfo::Skill5Exe()
{
	if (OwnerActor.IsValid())
	{
		USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(OwnerActor.Get());
		if (IsValid(SkillEntityComponent))
		{
			if (SkillEntityComponent->IsSkillCasting())
				return;

			if (SkillEntityComponent->IsSkillActing())
				return;

			if (IsValid(PassiveSkill2))
			{
				FVector SkillLocation = FVector::ZeroVector;
				if (SkillEntityComponent->CastInputSkill(PassiveSkill2->GetSkillID(), OwnerActor.Get(), nullptr))
				{

				}
			}
		}
	}
}