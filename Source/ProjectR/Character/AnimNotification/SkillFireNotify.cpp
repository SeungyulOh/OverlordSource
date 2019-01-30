// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "SkillFireNotify.h"
#include "Character/BattleBaseAnimInstance.h"



#include "UtilFunctionIntegrated.h"

#include "Character/Components/FunctionalComponents/SkillEntityComponent.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "ContentGameMode/Controllers/StagePlayerController.h"
#include "GlobalIntegrated.h"


#include "LeaderControlHelper.h"

#include "Table/SkillResourceTableInfo.h"

#include "Global/RGameInstance.h"
#include "DirectionTrackManager.h"

#include "Skill_V2/SharedSkill/ISkill_V2.h"


void USkillFireNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);
	
	UAnimInstance* AnimInstance						= MeshComp->GetAnimInstance();
	UBattleBaseAnimInstance* BattleBaseAnimInstance = Cast<UBattleBaseAnimInstance>(AnimInstance);

	if (IsValid(BattleBaseAnimInstance))
	{
		BattleBaseAnimInstance->bEnterCasting = false;
		BattleBaseAnimInstance->SkillFireNotify();
	}
}


void USkillCastingNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	UAnimInstance* AnimInstance = MeshComp->GetAnimInstance();
	UBattleBaseAnimInstance* BattleBaseAnimInstance = Cast<UBattleBaseAnimInstance>(AnimInstance);
	if (IsValid(BattleBaseAnimInstance))
	{
		if (BattleBaseAnimInstance->bEnterCasting)
			return;
		BattleBaseAnimInstance->bEnterCasting = true;
		BattleBaseAnimInstance->SkillCastingNotify();
	}

}

void UPointLightNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	AStagePlayerController* BasePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(MeshComp->GetWorld()));
	if (BasePC == nullptr)
		return;
	UBaseStageInfo*	BaseStageInfo = UGamePlayBluePrintFunction::GetBaseStageInfo();
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);

	checkf(RGAMEINSTANCE(this)->GetWorld(), TEXT("World invalid"));

	checkf(BasePC, TEXT("BasePC invalid"));

// 	if (EGamePlayModeTypeEnum::VE_Arena == BaseStageInfo->GetGameMode() ||
// 		EGamePlayModeTypeEnum::VE_MirrorDungeon == BaseStageInfo->GetGameMode())
// 		return;

	ULeaderControlHelper* LeaderControlHelper = BasePC->GetLeaderControlHelper();
	checkf(LeaderControlHelper, TEXT("GetLeaderControlHelper invalid"));
	
	if (LeaderControlHelper->LeaderCharacter.IsValid())
	{

		USkillEntityComponent* SkillComp = UCharacterHelpFunctionLibrary::GetSkillEntityComponent(MeshComp->GetOwner());
		FName SocketName;

		bool bisAttachtoBullet = false;
		if (IsValid(SkillComp))
		{
			UISkill_V2* OwnerSkill = SkillComp->GetCurrentSkill_V2();// nullptr;// SkillComp->GetSkill_V2(LeaderSkillControl->InputSkillName);
			if (OwnerSkill)
			{
				FSkillResourceTableInfo* SkillResourceTable = OwnerSkill->GetSkillResourceInfo();
				SocketName = SkillResourceTable->ActionEffectSocket;

				if (!SkillResourceTable->BulletID.ToString().Contains(TEXT("None")))
					bisAttachtoBullet = true;
			}
		}

		if (MeshComp->GetOwner() == LeaderControlHelper->LeaderCharacter.Get())
		{
			checkf(RGAMEINSTANCE(this)->PointLightManager, TEXT("PointLightManager invalid"));

			FVector location = MeshComp->GetSocketLocation(SocketName);

			if (!bisAttachtoBullet)
				RGAMEINSTANCE(this)->PointLightManager->AddPointLight(location, LifeTime, AttenuationRadius, LightColor, LightIntensity);
			else
			{
				int32 BulletNum = RGAMEINSTANCE(this)->PointLightManager->TargetBulletArray.Num();
				for (size_t i = 0; i < BulletNum; ++i)
				{
					RGAMEINSTANCE(this)->PointLightManager->AddPointLight(location, LifeTime, AttenuationRadius, LightColor, LightIntensity, true);
				}
			}
		}
	}
	//checkf(LeaderControlHelper->LeaderCharacter.Get(), TEXT("LeaderCharacter invalid"));
}


