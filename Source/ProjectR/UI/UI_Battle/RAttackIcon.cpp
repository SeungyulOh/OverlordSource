// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "RAttackIcon.h"

#include "GlobalIntegrated.h"

#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "ContentGameMode/Controllers/StagePlayerController.h"
//#include "Controllers/DPadController.h"
#include "Character/Components/FunctionalComponents/BattleEntityComponent.h"
#include "Character/Components/FunctionalComponents/EntityAIComponent.h"
#include "Character/AnimState/AttackAnimState.h"

#include "UtilFunctionIntegrated.h"

#include "LeaderControlHelper.h"




void URAttackIcon::NativeConstruct()
{
	Super::NativeConstruct();

	BasePlayerController = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(GetWorld()));

	if (Button_Click)
	{
		Button_Click->OnPressed.RemoveDynamic(this, &URAttackIcon::BtnEventOnPressed);
		Button_Click->OnPressed.AddDynamic(this, &URAttackIcon::BtnEventOnPressed);
		Button_Click->OnReleased.RemoveDynamic(this, &URAttackIcon::BtnEventOnReleased);
		Button_Click->OnReleased.AddDynamic(this, &URAttackIcon::BtnEventOnReleased);
		Button_Click->SetVisibility(ESlateVisibility::Visible);
		this->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}

	AttackButtonActive = false;
}

void URAttackIcon::NativeDestruct()
{
	
	EventManagerPtr = nullptr;
	BasePlayerController = nullptr;
	AttackButtonActive = false;

	Super::NativeDestruct();
}



void URAttackIcon::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void URAttackIcon::BtnEventOnPressed()
{
	if (this->ColorAndOpacity.A != 1.0)
		return;
	
	BtnEventOn();
	PlayAni(TEXT("Click_Pressed"));
	
}

void URAttackIcon::BtnEventOnReleased()
{
	if (this->ColorAndOpacity.A != 1.0)
		return;

	
	BtnEventOff();
	PlayAni("Click_Pressed", true);
	
}

void URAttackIcon::BtnEventOn()
{
	AttackButtonActive = true;

	if (BasePlayerController)
	{
		if (BasePlayerController->AIControlMode_UserInput != EAIControlModeEnum::VE_ManualMode)
			return;

		ULeaderControlHelper* LeaderHelper = BasePlayerController->GetLeaderControlHelper();
		if (!LeaderHelper)
			return;

		LeaderHelper->ShowAttackRange_OnlyManualMode(true);
		//LeaderHelper->GetLeaderMoveControl()->CallbackInputAttackValue(EInputCommandTypeEnum::VE_Attack);

		AActor* Leader = BasePlayerController->GetLeaderCharacter();
		if (Leader)
		{
			UAnimStateComponent* AnimStateComp = UCharacterHelpFunctionLibrary::GetAnimStateComponent(Leader);
			if (AnimStateComp)
			{
				UAttackAnimState* AttackAnimState = Cast<UAttackAnimState>(AnimStateComp->GetCurrentAnimState());
				if (AttackAnimState)
				{
					if (AttackAnimState->IsCancelAnim())
					{
						if (URealTimeModManager::IsRealTimeGameMode())
						{
							if (!AttackAnimState->AttackQueue.IsEmpty())
								return;

							if (!LeaderHelper->ManualAIBlackBoard.GetBlackBoardData(FManualAI_BlackBoard::EAIStopFrom::STAYAI_FROMATTACK))
							{
								LeaderHelper->ManualAIBlackBoard.SetBlackBoardData(FManualAI_BlackBoard::EAIStopFrom::STAYAI_FROMATTACK, true);
							}

							UBattleEntityComponent* Owner_BattleEntityComponent = UCharacterHelpFunctionLibrary::GetBattleEntityComponent(Leader);
							if (Owner_BattleEntityComponent)
							{
								int32 Animidx = AttackAnimState->GetCurrentAttackAnimIdx();
								Owner_BattleEntityComponent->SendRTS_AttackNow(true, Animidx);
							}
						}
						else
						{
							if (!LeaderHelper->ManualAIBlackBoard.GetBlackBoardData(FManualAI_BlackBoard::EAIStopFrom::STAYAI_FROMATTACK))
							{
								LeaderHelper->ManualAIBlackBoard.SetBlackBoardData(FManualAI_BlackBoard::EAIStopFrom::STAYAI_FROMATTACK, true);
							}
						}
						
					}
				}
			}
		}
	}

	
}

void URAttackIcon::BtnEventOff()
{
	AttackButtonActive = false;

	if (BasePlayerController)
	{
		if (BasePlayerController->AIControlMode_UserInput == EAIControlModeEnum::VE_ManualMode)
			BasePlayerController->GetLeaderControlHelper()->ShowAttackRange_OnlyManualMode(false);

		//BasePlayerController->GetLeaderControlHelper()->GetLeaderMoveControl()->CallbackInputAttackValue(EInputCommandTypeEnum::VE_AttackRelease);
	}
	
}

