// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "CombineSkillNotify.h"
#include "GlobalIntegrated.h"





#include "Character/BattleBaseAnimInstance.h"
#include "Character/BattleObject/BattleObjectAnimInstance.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "ContentGameMode/Controllers/StagePlayerController.h"
#include "GameInfo/PlayStateInfo.h"
#include "UI/UI_Battle/RCampaignHUD_ThreeControl.h"

#include "LeaderControlHelper.h"
#include "UtilFunctionIntegrated.h"
#include "DirectionTrackManager.h"

void UCombineInputStartNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	AActor* Owner = MeshComp->GetOwner();
	IEntityBaseProperty* BaseProperty = Cast<IEntityBaseProperty>(Owner);
	if (BaseProperty)
	{
		if (BaseProperty->GetPartyID() != PartyID_PC_MY)
			return;
	}
	
	AStagePlayerController* BasePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(MeshComp->GetWorld()));
	if (IsValid(BasePC))
	{
		
		UBattleDirectionState* BDS = Cast<UBattleDirectionState>(UUtilFunctionLibrary::GetDirectionTrackManager()->GetCurrentState());
		if (BDS)
		{
			EDirectionEffectTypeEnum CurrentEffect = UUtilFunctionLibrary::GetDirectionTrackManager()->GetTopEffect();
			if (CurrentEffect == EDirectionEffectTypeEnum::DE_Battle_CombineEffect)
			{
				FPostProcessStruct PPStruct;
				PPStruct.ePPType = FPostProcessStruct::BLOOM;
				PPStruct.InitialValue = BDS->GetCurrentPPVolume()->Settings.BloomIntensity;
				PPStruct.TargetValue = 3.f;
				PPStruct.Duration = 0.5f;
				BDS->AddPostProcessStruct(PPStruct);

				FPostProcessStruct PPStructRadial;
				PPStructRadial.ePPType = FPostProcessStruct::RADIALBLUR;
				PPStructRadial.TargetValue = 0.8f;
				PPStructRadial.Duration = 0.5f;
				BDS->AddPostProcessStruct(PPStructRadial);
				
			}
		}
	}
}


void UCharacterHighlightStartNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	AActor* Owner = MeshComp->GetOwner();
	IEntityBaseProperty* BaseProperty = Cast<IEntityBaseProperty>(Owner);
	if (BaseProperty)
	{
		if (BaseProperty->GetPartyID() != PartyID_PC_MY)
			return;
	}

	int32 MatNum = MeshComp->GetNumMaterials();
	for (size_t i = 0; i < MatNum; ++i)
	{
		UMaterialInstanceDynamic* MID = Cast<UMaterialInstanceDynamic>(MeshComp->GetMaterial(i));
		if (nullptr != MID)
		{
			MID->SetVectorParameterValue(FName(TEXT("CustomLightColor")), Color);
			MeshComp->SetMaterial(i, MID);
		}
	}

	return;
}

void UCharacterHighlightEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	//Actor Highlight Disable

	AActor* Owner = MeshComp->GetOwner();
	IEntityBaseProperty* BaseProperty = Cast<IEntityBaseProperty>(Owner);
	if (BaseProperty)
	{
		if (BaseProperty->GetPartyID() != PartyID_PC_MY)
			return;
	}

	int32 MatNum = MeshComp->GetNumMaterials();
	for (size_t i = 0; i < MatNum; ++i)
	{
		UMaterialInstanceDynamic* MID = Cast<UMaterialInstanceDynamic>(MeshComp->GetMaterial(i));
		if (nullptr != MID)
		{
			MID->SetVectorParameterValue(FName(TEXT("CustomLightColor")), FLinearColor(0.f, 0.f, 0.f));
			MeshComp->SetMaterial(i, MID);
		}
	}
	return;
}
