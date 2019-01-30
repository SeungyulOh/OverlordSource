// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "GachaAnimNotify.h"

#include "GlobalIntegrated.h"

#include "Utils/UtilFunctionLibrary.h"
#include "Utils/ResourceFunctionLibrary.h"


#include "Lobby/IndoorLobbyStateActor.h"
#include "Lobby/TowerLobbyLevelScriptActor.h"

#include "CustomActors/RHeroGacha.h"
#include "CustomActors/BaseSequenceActor.h"

#include "UI/Shop/UP_HeroShop.h"
#include "UI/RWidgetManager.h"



void UGachaBloomNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	if (!IsValid(RGameInstance))
		return;

	UWorld* world = RGameInstance->GetWorld();
	if (!IsValid(world))
		return;

	ARHeroGachaCard* GachaCard = Cast<ARHeroGachaCard>(MeshComp->GetOwner());

	if (GachaCard)
	{
		FPostProcessTimer PPTimer;
		PPTimer.Curve = Curve;
		PPTimer.ElapsedTime = 0;
		PPTimer.LifeTime = LifeTime;
		PPTimer.PPType = FPostProcessTimer::BLOOM;


		for (auto& postProcessVolume : world->PostProcessVolumes)
		{
			APostProcessVolume* PPVolume = Cast<APostProcessVolume>(postProcessVolume);
			if (PPVolume)
			{
				PPTimer.InitialValue = PPVolume->Settings.BloomIntensity;
				break;
			}
		}
		GachaCard->PPTimerArray.Emplace(PPTimer);
	}

	
}

void UGachaFadeOutNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	if (!IsValid(RGameInstance))
		return;

	UWorld* world = RGameInstance->GetWorld();
	if (!IsValid(world))
		return;

	ARHeroGachaCard* GachaCard = Cast<ARHeroGachaCard>(MeshComp->GetOwner());

	if (GachaCard)
	{
		APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(RGAMEINSTANCE(this)->GetWorld(), 0);
		PCM->StartCameraFade(0, 1, LifeTime, FadeColor, false, true);
	}
}

void UGachaFadeInNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	Super::Notify(MeshComp, Animation);

	URGameInstance* RGameInstance = RGAMEINSTANCE(GEngine);
	if (!IsValid(RGameInstance))
		return;
	
	UWorld* world = RGameInstance->GetWorld();
	if (!IsValid(world))
		return;

	if (world)
	{
		AGameModeBase* gamemode = UGameplayStatics::GetGameMode(world);
		if (!gamemode)
			return;
	}

	ARHeroGachaCard* GachaCard = Cast<ARHeroGachaCard>(MeshComp->GetOwner());

	if (GachaCard)
	{
		APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(RGAMEINSTANCE(this)->GetWorld(), 0);
		PCM->StartCameraFade(1, 0, LifeTime, FadeColor, false, false);

		ATowerLobbyLevelScriptActor* LLSA = Cast<ATowerLobbyLevelScriptActor>(RGameInstance->GetWorld()->GetLevelScriptActor());
		if(LLSA)
		{
			if(LLSA->HeroGachaActor)
				LLSA->HeroGachaActor->PrevCameraActor = PCM->ViewTarget.Target;
		}

		if (GachaCard->eGradeType == EGradeType::VE_SPECIAL_SPIRIT)
			return;

		ATowerLobbyGameMode* TowerLobbyGameMode = RGameInstance->GetTowerLobbyGameMode();
		AIndoorLobbyStateActor* IndoorLSActor = nullptr;
		if(TowerLobbyGameMode)
			IndoorLSActor = Cast<AIndoorLobbyStateActor>(TowerLobbyGameMode->GetLobbyStateActor(ETowerLobbyStateEnum::Indoor));
		if (IndoorLSActor)
		{
			
			FCharacterTableInfo* characterinfo = UTableManager::GetInstancePtr()->GetCharacterRow(GachaCard->CurrentHero.heroId);
			if (characterinfo)
			{
				IndoorLSActor->heroName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, characterinfo->DisplayName);
// 				IndoorLSActor->heroNickName = UUtilFunctionLibrary::GetLocalizedString(EStringTableType::VE_Hero, characterinfo->Nick);
				IndoorLSActor->heroSprite = UResourceFunctionLibrary::GetHeroClassTypeSprite(characterinfo->Class);
				IndoorLSActor->heroGrade = characterinfo->Grade;
			}

			IndoorLSActor->ChangeHero_Owned(GachaCard->CurrentHero.heroUD);
			PCM->SetViewTarget(IndoorLSActor->IndoorNormalViewCameraActor.Get());

			IndoorLSActor->SetVisibilityHero(true);
			IndoorLSActor->PlayHeroTouchActionAnimationAndSequence();			
			GachaCard->Binding_SpecialSequence();
		}

// 		ABaseSequenceActor* BaseSeqActor = RGameInstance->SequenceManager->GetCurrentSequenceActor();
// 		if (BaseSeqActor)
// 		{
// 			FViewTargetTransitionParams Params;
// 			Params.BlendTime = 1.f;
// 			Params.bLockOutgoing = true;
// 			BaseSeqActor->SequencePlayer->SetTransitionParams(Params);
// 		}
			
	}

	
	UUP_HeroShop* HeroShop = RGameInstance->RWidgetManager->GetUserWidgetT<UUP_HeroShop>(EWidgetBluePrintEnum::WBP_ShopHero);
	if (HeroShop)
	{
		if (HeroShop->PS_Component)
		{
			HeroShop->PS_Component->DestroyComponent();
			HeroShop->PS_Component = nullptr;
		}

		UUC_HeroSummonMenus* SummonMenus = HeroShop->HeroSummonMenus;
		if (SummonMenus)
			SummonMenus->WidgetSwitcher_Gacha->SetVisibility(ESlateVisibility::Collapsed);
	}
	

}

