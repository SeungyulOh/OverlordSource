// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"

#include "SharedConstants/GlobalConstants.h"
#include "GlobalIntegrated.h"




#include "GameInfo/RewardInfo.h"
#include "GameInfo/UserInfo.h"

#include "GameState/Pvp/VictoryPosition.h"
#include "CustomActors/BaseSequenceActor.h"

#include "UtilFunctionIntegrated.h"


#include "UI/RWidgetManager.h"
#include "UI/Common/UC_Item_Icons.h"

#include "UP_ResultReward.h"


void UUP_ResultReward::NativeConstruct()
{
	Super::NativeConstruct();

	if (IsValid(RewardItemGroup))
		RewardItemGroup->SetVisibility(ESlateVisibility::Hidden);

	auto RGameInstance = RGAMEINSTANCE(this);
	
	{	
		RGameInstance->EventManager->OnSequence_CreateWidget.Clear();
		RGameInstance->EventManager->OnSequence_CreateWidget.AddDynamic(this, &UUP_ResultReward::CallbackCreateWidget);


		ABasePlayerController* PlayerCtrl = UUtilFunctionLibrary::GetBasePlayerController(RGameInstance->GetWorld());
		if (IsValid(PlayerCtrl))
			MainCameraActor = PlayerCtrl->GetCameraActor();


		TArray<AActor*> outActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVictoryPosition::StaticClass(), outActors);

		FTransform TargetTransform;
		if (outActors.Num() > 0)
		{
			TargetTransform = outActors[0]->GetActorTransform();
			
			FRotator rot = TargetTransform.Rotator();
			rot.Yaw = 0;

			TargetTransform.SetRotation(rot.Quaternion());
			
			SequenceActor = RGameInstance->SequenceManager->PlaySequenceAtTargetActorLocation(SequenceName, ESequenceActorType::VE_General, MainCameraActor.Get());
		}
		else
		{
			SequenceActor = RGameInstance->SequenceManager->SpawnSequence(SequenceName, false);
		}
		RGameInstance->EventManager->OnSequence_CreateWidget.Clear();
		RGameInstance->EventManager->OnSequence_CreateWidget.AddDynamic(this, &UUP_ResultReward::CallbackCreateWidget);


		if (SequenceActor.IsValid())
		{	
			SequenceActor.Get()->OnSequenceEndEvent.Clear();
			SequenceActor.Get()->OnSequenceEndEvent.AddDynamic(this, &UUP_ResultReward::CallbackSequenceEnd);
		}
	}

	SetRewardItemIcons();
	
}

void UUP_ResultReward::NativeDestruct()
{
// 	if (SequenceActor.IsValid())
// 		SequenceActor.Get()->OnSequenceEndEvent.Clear();

	auto RGameInstance = RGAMEINSTANCE(this);
	
		RGameInstance->EventManager->OnSequence_CreateWidget.Clear();

	//ReturnToMainCamera();
	Super::NativeDestruct();
}

void UUP_ResultReward::OnClick_Close()
{
	auto RGameInstance = RGAMEINSTANCE(this);
	
	{
		AProjectRGameMode* pGameMode = Cast<AProjectRGameMode>(UGameplayStatics::GetGameMode(RGAMEINSTANCE(this)));
// 		UIGamePlayMode*		pGamePlayMode = UUtilFunctionLibrary::GetGamePlayMode();
		if (pGameMode)
		{
			pGameMode->ObjectHelper.RemoveObjectAll();
		}
		
// 		RGameInstance->GameObjectMgr->RemoveObjectAll();

		RGameInstance->RWidgetManager->DetachUIPage(EUIPageEnum::UIPage_ResultReward);
		RGameInstance->RWidgetManager->AttachUIPage(EUIPageEnum::UIPage_CampaignResult);
	}
}

void UUP_ResultReward::CallbackSequenceEnd(FName SequenceKey)
{
	ReturnToMainCamera();	
}

void UUP_ResultReward::CallbackCreateWidget()
{
// 	if (IsValid(RewardItemGroup))
// 	{
// 		RewardItemGroup->SetVisibility(ESlateVisibility::Visible);
// 	}
	
	OnPlayAimation();
}

void UUP_ResultReward::SetRewardItemIcons()
{
// 	InitRewardItemIcons();
// 
// 	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
// 	if(!IsValid( RGameInstance) )
// 		return;
// 
// 	int32 ItemIndex = 0;
// 	for (auto& RewardElem : RGameInstance->RewardInfo->Rewards)
// 	{
// 		if (RewardElem.type == EREWARD_TYPE::GOLD ||
// 			RewardElem.type == EREWARD_TYPE::AVATAR_EXP ||
// 			RewardElem.type == EREWARD_TYPE::REDSTONE ||
// 			RewardElem.type == EREWARD_TYPE::HERO_EXP ||
// 			RewardElem.type == EREWARD_TYPE::SP1 )
// 			continue;
// 
// 		FITEM_ICON_INFO RewardIconInfo;
// 		UUIFunctionLibrary::GetRewardIconByRewardType(this->GetWorld(), RewardElem, RewardIconInfo);
// 	
// 		if (CanvasRewardItems.IsValidIndex(ItemIndex))
// 			CanvasRewardItems[ItemIndex]->SetVisibility(ESlateVisibility::Visible);
// 
// 		if (RewardItemIcons.IsValidIndex(ItemIndex))
// 			RewardItemIcons[ItemIndex]->SetItemIconStructure(RewardIconInfo);
// 
// 		++ItemIndex;
// 	}
}

void UUP_ResultReward::InitRewardItemIcons()
{
	for (UCanvasPanel* SlotPanel : CanvasRewardItems)
	{
		SlotPanel->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UUP_ResultReward::ReturnToMainCamera()
{
	if (!MainCameraActor.IsValid())
		return;

	auto RGameInstance = RGAMEINSTANCE(this);

	if (RGameInstance->GetWorld() && RGameInstance->GetWorld()->GetFirstPlayerController())
		RGameInstance->GetWorld()->GetFirstPlayerController()->SetViewTarget(MainCameraActor.Get());
}
