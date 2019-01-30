// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "SkillIcon_New.h"

#include "UtilFunctionIntegrated.h"
#include "Skill_V2/SharedSkill/ISkill_V2.h"

#include "Table/SkillResourceTableInfo.h"
#include "Table/SkillTableInfo_V2.h"

#include "Global/TableManager.h"

#include "Network/RTSManager.h"

#include "GameInfo/PlayStateInfo.h"


void USkillIcon_New::NativeConstruct()
{
	Super::NativeConstruct();

	/*Blackboard Settings*/
	Blackboard.BlackBoardArray.Init(false, (int32)EIconState::MAX);
	if (URTSManager::GetInstancePtr()->IsConnectedMode(EPVP::CS))
	{
		Blackboard.bColosseumMode = true;
		Blackboard.CurrentCoolTime = 0.f;
		Blackboard.CurrentState = EIconState::MAX;
	}
	/*Blackboard Settings End*/

	/*Renderer Settings*/
	Renderer.variables = &Variables;
	Renderer.blackboard = &Blackboard;
	Renderer.Parent = this;
	/*Renderer Settings End*/

	/*FStateMachine_SkillIcon Settings*/
	FSM.variables = &Variables;
	FSM.blackboard = &Blackboard;
	//FSM.Prepare();
	/*FStateMachine_SkillIcon Settings End*/

}

void USkillIcon_New::NativeDestruct()
{
	Super::NativeDestruct();
}

void USkillIcon_New::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Blackboard.OwnerCharacter.Get());
	if (SkillEntityComponent)
	{
		UISkill_V2* skill = SkillEntityComponent->GetSkillByCategory_V2(Blackboard.SkillCategory);
		if (skill && skill->IsSkillCooling())
			Blackboard.SetBlackboardData(EIconState::COOL, true);
	}

	EIconState NextState = FSM.ChangeState();
	if (Blackboard.CurrentState != NextState || NextState == EIconState::COOL)
	{
		Blackboard.CurrentState = NextState;
		Renderer.Render(InDeltaTime);
	}
	
}

void USkillIcon_New::IncreaseCoolTime(float InCool)
{
	USkillEntityComponent* SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Blackboard.OwnerCharacter.Get());
	if (SkillEntityComponent)
	{
		UISkill_V2* skill = SkillEntityComponent->GetSkillByCategory_V2(Blackboard.SkillCategory);
		if (skill)
			skill->IncreaseCoolTime(InCool);
	}
}

bool USkillIcon_New::isDPadEnable()
{
	auto OwnerActor_SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(Blackboard.OwnerCharacter.Get());
	if (!IsValid(OwnerActor_SkillEntityComponent))
		return false;

	UISkill_V2* MySkill = OwnerActor_SkillEntityComponent->GetSkillByCategory_V2(Blackboard.SkillCategory);
	if (IsValid(MySkill))
	{
		FSkillTableInfo_V2* SkillInfo = MySkill->GetSkillInfo();
		if (SkillInfo)
			return SkillInfo->VisibleDPAD;
	}

	return false;
}

void FRenderer_SkillIcon::Prepare(FHeroSlotInfo slotinfo, ESkillCategoryTypeEnum SkillCategory)
{
	if (slotinfo.SpawnedTargetActor->IsValidLowLevel() && IsValid(slotinfo.SpawnedTargetActor))
	{
		blackboard->OwnerCharacter = slotinfo.SpawnedTargetActor;
		blackboard->SkillCategory = SkillCategory;

		auto OwnerActor_SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(blackboard->OwnerCharacter.Get());
		if (!IsValid(OwnerActor_SkillEntityComponent))
			return;

		UISkill_V2* MySkill = OwnerActor_SkillEntityComponent->GetSkillByCategory_V2(SkillCategory);
		if (IsValid(MySkill))
		{
			FSkillTableInfo_V2* SkillInfo = MySkill->GetSkillInfo();
			if (SkillInfo)
			{
				if (variables->Image_SkillPad)
				{
					if (!SkillInfo->VisibleDPAD)
						variables->Image_SkillPad->SetVisibility(ESlateVisibility::Collapsed);
					else
						variables->Image_SkillPad->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
			}

			FSkillResourceTableInfo* SkillResourceInfo = MySkill->GetSkillResourceInfo();
			if (SkillResourceInfo)
			{
				if (IsValid(variables->Image_SkillIcon))
					variables->Image_SkillIcon->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromAssetID(SkillResourceInfo->SkillIconRef));
			}

			//Check if owner is leader
			AActor* Leader = RGAMEINSTANCE(GEngine)->PlayStateInfo->GetCurrentSelectedCharacterActor();
			if (Leader != blackboard->OwnerCharacter.Get())
			{
				if (IsValid(variables->PortraitOverlay))
				{
					variables->PortraitOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);

					IEntityBaseProperty* Prop = Cast<IEntityBaseProperty>(blackboard->OwnerCharacter.Get());
					if (Prop)
					{
						UObject* Portrait = UUIFunctionLibrary::GetHeroRoundPortrait(Prop->GetCharacterID());
						UUIFunctionLibrary::SetImageBrush(Portrait, variables->Image_Portrait);
					}
				}
			}
			else
			{
				if (IsValid(variables->PortraitOverlay))
					variables->PortraitOverlay->SetVisibility(ESlateVisibility::Collapsed);
			}
		}
		else
		{
			blackboard->SetBlackboardData(EIconState::LOCK, true);
			if (IsValid(variables->PortraitOverlay))
				variables->PortraitOverlay->SetVisibility(ESlateVisibility::Collapsed);
		}
	}
	else
	{
		blackboard->OwnerCharacter = nullptr;
		blackboard->SkillCategory = SkillCategory;

		FCharacterTableInfo* CharacterTableInfo1 = UTableManager::GetInstancePtr()->GetCharacterRow(slotinfo.HeroIDName);
		if (CharacterTableInfo1)
		{
			FString SkillIDName = CharacterTableInfo1->ActiveSkill02.ToString() + "_1";
			FSkillTableInfo_V2* skillData = UTableManager::GetInstancePtr()->GetSkillRow_V2(FName(*SkillIDName));
			if (skillData)
			{
				FSkillResourceTableInfo* ResourceInfo = UTableManager::GetInstancePtr()->GetSkillResourceRow(skillData->SkillResourceID);
				if (ResourceInfo)
				{
					if (IsValid(variables->Image_SkillIcon))
						variables->Image_SkillIcon->SetBrushFromTexture(UUtilFunctionLibrary::GetTexture2DFromAssetID(ResourceInfo->SkillIconRef));
				}

				UObject* Portrait = UUIFunctionLibrary::GetHeroRoundPortrait(slotinfo.HeroIDName);
				UUIFunctionLibrary::SetImageBrush(Portrait, variables->Image_Portrait);
				variables->PortraitOverlay->SetVisibility(ESlateVisibility::HitTestInvisible);
			}
		}
		else
		{
			blackboard->SetBlackboardData(EIconState::LOCK, true);
			if (IsValid(variables->PortraitOverlay))
				variables->PortraitOverlay->SetVisibility(ESlateVisibility::Collapsed);
		}

		
	}
	
}

void FRenderer_SkillIcon::Render(float DeltaTime)
{
	switch (blackboard->CurrentState)
	{
		case EIconState::IDLE:
		{
			variables->WidgetSwitcher_State->SetVisibility(ESlateVisibility::Collapsed);
			variables->Image_SkillIcon->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			variables->PortraitOverlay->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			variables->PortraitSwitcher->SetActiveWidgetIndex(0);
			if (blackboard->OwnerCharacter.IsValid())
			{
				AActor* Leader = RGAMEINSTANCE(GEngine)->PlayStateInfo->GetBattleActiveCrewActor(ECharacterRoleEnum::VE_Leader);
				if (blackboard->OwnerCharacter.Get() != Leader)
				{
					variables->PortraitSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
					variables->PortraitOverlay->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
				}
				else
				{
					variables->PortraitSwitcher->SetVisibility(ESlateVisibility::Collapsed);
					variables->PortraitOverlay->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
			
			break;
		}

		case EIconState::LOCK:
		{
			Parent->SetVisibility(ESlateVisibility::Hidden);
			variables->Image_SkillIcon->SetVisibility(ESlateVisibility::Collapsed);
			variables->WidgetSwitcher_State->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			variables->WidgetSwitcher_State->SetActiveWidgetIndex(1);
			break;
		}

		case EIconState::DEAD:
		{
			variables->Image_SkillIcon->SetVisibility(ESlateVisibility::Collapsed);
			variables->WidgetSwitcher_State->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			variables->WidgetSwitcher_State->SetActiveWidgetIndex(2);
			variables->PortraitOverlay->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			variables->PortraitSwitcher->SetActiveWidgetIndex(1);
			variables->PortraitSwitcher->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			break;
		}

		case EIconState::COOL:
		{
			variables->Image_SkillIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
			variables->WidgetSwitcher_State->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			variables->WidgetSwitcher_State->SetActiveWidgetIndex(0);


			USkillEntityComponent* OwnerCharacter_SkillEntityComponent = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(blackboard->OwnerCharacter.Get());
			if (IsValid(OwnerCharacter_SkillEntityComponent))
			{
				UISkill_V2* skill = OwnerCharacter_SkillEntityComponent->GetSkillByCategory_V2(blackboard->SkillCategory);
				if (skill)
				{
					blackboard->CurrentCoolTime = skill->GetCurrentCoolTime();
					blackboard->MaxCoolTime = skill->GetSkillInfo()->CoolTime;
				}
			}
			else
			{
				if (blackboard->SkillCategory == ESkillCategoryTypeEnum::VE_ActiveSkill02 && blackboard->CurrentCoolTime > 0)
					blackboard->CurrentCoolTime -= DeltaTime;
			}
					
			variables->Text_CoolTime->SetText(FText::AsNumber((int32)blackboard->CurrentCoolTime + 1));
			if (variables->Image_CoolGauge)
			{
				UMaterialInstanceDynamic* gauge_material = variables->Image_CoolGauge->GetDynamicMaterial();
				if (gauge_material)
				{
					FName param_name = "Percent";
					float rate = blackboard->CurrentCoolTime / blackboard->MaxCoolTime;
					gauge_material->SetScalarParameterValue(param_name, rate);
				}
			}

			if (blackboard->CurrentCoolTime <= 0)
			{
				blackboard->CurrentCoolTime = 0;
				blackboard->SetBlackboardData(EIconState::COOL, false);
			}
			break;
		}

		case EIconState::ABNORMAL:
		{
			variables->Image_SkillIcon->SetVisibility(ESlateVisibility::HitTestInvisible);
			variables->WidgetSwitcher_State->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
			variables->WidgetSwitcher_State->SetActiveWidgetIndex(2);
			break;
		}
		

	}
}

EIconState FStateMachine_SkillIcon::ChangeState()
{
	if (blackboard->BlackBoardArray[(int32)EIconState::DEAD] == true)
		return EIconState::DEAD;
	else if (blackboard->BlackBoardArray[(int32)EIconState::LOCK] == true)
		return EIconState::LOCK;
	else if (blackboard->BlackBoardArray[(int32)EIconState::ABNORMAL] == true)
		return EIconState::ABNORMAL;
	else if (blackboard->BlackBoardArray[(int32)EIconState::COOL] == true)
		return EIconState::COOL;
	else
		return EIconState::IDLE;
}

void FBlackboard_SkillIcon::SetBlackboardData(EIconState InState, bool bOn)
{
	if (BlackBoardArray.IsValidIndex((int32)InState))
		BlackBoardArray[(int32)InState] = bOn;

}

void FBlackboard_SkillIcon::ClearBlackboardData()
{
	BlackBoardArray.Init(false, (int32)EIconState::MAX);
}
