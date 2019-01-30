// Fill out your copyright notice in the Description page of Project Settings.

#include "ProjectR.h"
#include "ProjectRGameMode.h"
#include "SequenceManager.h"

#include "GlobalIntegrated.h"

#include "Table/ResourceSequenceTableInfo.h"
#include "Table/ResourceCustomActorTableInfo.h"
#include "Object/EmptyBasicActor.h"

#include "UtilFunctionIntegrated.h"

#include "UI/MediaFrameWork/UP_MediaFrameWorkPlayer.h"
#include "Gameinfo/PlayStateInfo.h"
#include "Skill_v2/SharedSkill/ISkill_V2.h"
#include "LeaderControlHelper.h"
#include "ContentGameMode/Controllers/BasePlayerController.h"
#include "ContentGameMode/Controllers/StagePlayerController.h"
#include "DefaultLevelSequenceInstanceData.h"

#include "DirectionTrackManager.h"



void USequenceManager::Initialize(void)
{
	//GameInstance = RGameInstance;
}

void USequenceManager::ClearSequence()
{
	for ( auto Sequence : SequenceList)
	{
		if (Sequence.IsValid())
		{
			Sequence->OnSequenceEvent.Clear();
			Sequence->OnSequenceEndEvent.Clear();
			Sequence->OnSequencePauseEvent.Clear();

			Sequence->Stop();

			//check
			if (IsValid(Sequence->SequencePlayer))
			{
				Sequence->SequencePlayer->CleanUp();
			}
			Sequence->Destroy();
		}
	}
	SequenceList.Empty();
	SequenceMap.Empty();

	CurPlaySequenceActor = nullptr;

	RemoveSubTitle();
}

void USequenceManager::ClearSequence_ForGacha()
{
	for (size_t i = 1; i<SequenceList.Num(); )
	{
		if (SequenceList[i].IsValid())
		{
			SequenceList[i]->OnSequenceEvent.Clear();
			SequenceList[i]->OnSequenceEndEvent.Clear();
			SequenceList[i]->OnSequencePauseEvent.Clear();

			SequenceList[i]->Stop();

			SequenceMap.Remove(SequenceList[i]->SequenceTableKey);

			//check
			if (IsValid(SequenceList[i]->SequencePlayer))
			{
				SequenceList[i]->SequencePlayer->CleanUp();
			}
			SequenceList[i]->Destroy();

			SequenceList.RemoveAt(i);
			continue;
		}
		else
			break;
	}

	RemoveSubTitle();
}

ABaseSequenceActor* USequenceManager::SpawnSequence(FName id, bool bPlay, bool bLoop)
{
	ABaseSequenceActor* SequenceActor = nullptr;

	URGameInstance*		RGameInstance = RGAMEINSTANCE(this);

	if (SequenceMap.Contains(id))
	{
		if (SequenceMap[id].IsValid())
		{
			SequenceActor = SequenceMap[id].Get();
		}			
	}
	else
	{
		FResourceSequenceTableInfo* SequenceTableInfo = UTableManager::GetInstancePtr()->GetResourceSequenceRow(id);
		if (SequenceTableInfo)
		{
			FTransform SpawnTransform;
			SpawnTransform = FTransform::Identity;

			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			UClass* BPClass = SequenceTableInfo->GetBlueprintClass();
			SequenceActor = RGameInstance->GetWorld()->SpawnActor<ABaseSequenceActor>(BPClass, SpawnTransform, SpawnParams);
			if (IsValid(SequenceActor))
			{
				SequenceActor->SequenceTableKey = id;

				SequenceList.Emplace(SequenceActor);
				SequenceMap.Emplace(id, SequenceActor);
			}
			else
			{
				UE_LOG(LogGamePlay, Log, TEXT("Failed to load sequence - %s"), *(id.ToString()));
			}
		}
	}
	
	if (bPlay)
	{
		if (IsValid(SequenceActor))
		{
			CurPlaySequenceActor = SequenceActor;
			SequenceActor->Play(bLoop);
			SetSubTitleLegacy(id);
		}
	}

	return SequenceActor;
}



ABaseSequenceActor* USequenceManager::SpawnSequenceWithTransformOrigin(FName id, const FTransform& Offset, bool bPlay /*= true*/, bool bLoop /*= false*/)
{
	ABaseSequenceActor* SequenceActor = nullptr;

	if (RGAMEINSTANCE(this) != nullptr)
	{
		if (SequenceMap.Contains(id))
		{
			if (SequenceMap[id].IsValid())
			{
				SequenceActor = SequenceMap[id].Get();
			}
		}
		else
		{
			FResourceSequenceTableInfo* SequenceTableInfo = UTableManager::GetInstancePtr()->GetResourceSequenceRow(id);
			if (SequenceTableInfo)
			{
				FTransform SpawnTransform;
				SpawnTransform = FTransform::Identity;

				FActorSpawnParameters SpawnParams;
				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SequenceActor = RGAMEINSTANCE(this)->GetWorld()->SpawnActor<ABaseSequenceActor>(SequenceTableInfo->GetBlueprintClass(), SpawnTransform, SpawnParams);
				if (IsValid(SequenceActor))
				{
					SequenceActor->SequenceTableKey = id;

					auto InstanceData = NewObject<URDefaultLevelSequenceInstanceData>();
					InstanceData->TransformOrigin = Offset;
					SequenceActor->DefaultInstanceData = InstanceData;
					SequenceActor->bOverrideInstanceData = true;

					// to make it apply for bOverrideInstanceData
					SequenceActor->InitializePlayer();

					SequenceList.Emplace(SequenceActor);
					SequenceMap.Emplace(id, SequenceActor);
				}
				else
				{
					UE_LOG(LogGamePlay, Log, TEXT("Failed to load sequence - %s"), *(id.ToString()));
				}
			}
		}
	}

	if (bPlay)
	{
		if (IsValid(SequenceActor))
		{
			CurPlaySequenceActor = SequenceActor;
			SequenceActor->Play(bLoop);
			SetSubTitleLegacy(id);
		}
	}

	return SequenceActor;
}

ABaseSequenceActor* USequenceManager::FindSequence(FName name)
{
	if (SequenceMap.Contains(name))
	{
		if (SequenceMap[name].IsValid())
		{
			return SequenceMap[name].Get();
		}
	}

	return nullptr;
}

ABaseSequenceActor* USequenceManager::PlaySequence(FName name)
{
	if (name.IsNone())
	{
		if (CurPlaySequenceActor.IsValid())
		{
			if (!CurPlaySequenceActor->IsPlaying())
			{
				CurPlaySequenceActor->Play();
				return CurPlaySequenceActor.Get();
			}
		}
	}
	else
	{
		ABaseSequenceActor* SequenceActor = FindSequence(name);
		if (IsValid(SequenceActor))
		{
			if (!SequenceActor->IsPlaying())
			{
				CurPlaySequenceActor = SequenceActor;
				CurPlaySequenceActor->Play();
				return CurPlaySequenceActor.Get();
			}
		}
	}

	return nullptr;
}

bool USequenceManager::IsPlaying(FName name)
{
	if (name.IsNone())
	{
		if (CurPlaySequenceActor.IsValid())
		{
			return CurPlaySequenceActor->IsPlaying();
		}
	}
	else
	{
		ABaseSequenceActor* SequenceActor = FindSequence(name);
		if (IsValid(SequenceActor))
		{
			return SequenceActor->IsPlaying();
		}
	}

	return false;
}

void USequenceManager::StopSequence(FName name)
{
	if (name.IsNone())
	{
		if (CurPlaySequenceActor.IsValid())
		{
			CurPlaySequenceActor->Stop();
		}
	}
	else
	{
		ABaseSequenceActor* SequenceActor = FindSequence(name);
		if (IsValid(SequenceActor))
		{
			SequenceActor->Stop();
		}
	}

	RemoveSubTitle();
}

void USequenceManager::PauseSequence(FName name)
{
	if (name.IsNone())
	{
		if (CurPlaySequenceActor.IsValid())
		{
			CurPlaySequenceActor->Pause();
		}
	}
	else
	{
		ABaseSequenceActor* SequenceActor = FindSequence(name);
		if (IsValid(SequenceActor))
		{
			SequenceActor->Pause();
		}
	}
}

void USequenceManager::ResumeSequence(FName name)
{
	if (name.IsNone())
	{
		if (CurPlaySequenceActor.IsValid())
		{
			CurPlaySequenceActor->Resume();
		}
	}
	else
	{
		ABaseSequenceActor* SequenceActor = FindSequence(name);
		if (IsValid(SequenceActor))
		{
			SequenceActor->Resume();
		}
	}
}

void USequenceManager::CleanUpSequence(FName name)
{
	if (name.IsNone())
	{
		if (CurPlaySequenceActor.IsValid())
		{
			if (false == CurPlaySequenceActor->IsPlaying())
			{
				CurPlaySequenceActor->SequencePlayer->CleanUp();
			}
		}
	}
	else
	{
		ABaseSequenceActor* SequenceActor = FindSequence(name);
		if (IsValid(SequenceActor))
		{
			SequenceActor->SequencePlayer->CleanUp();
		}
	}
}

void USequenceManager::AllPause()
{
	for (auto Sequence : SequenceList)
	{
		if (Sequence.IsValid())
		{
			Sequence->Pause();
		}
	}
}

//////////////////////////////////////////////////////////////////////////

void USequenceManager::CreateSubTitle()
{
	if (RGAMEINSTANCE(this) == nullptr)
		return;

	UUserWidget* Widget = UUtilFunctionLibrary::CreateUserWidget(RGAMEINSTANCE(this)->GetWorld(), WIDGETID_MEDIAFRAMEWORKUI);
	if (Widget)
	{
		SubtitleWidget = Cast<UUP_MediaFrameWorkPlayer>(Widget);
		SubtitleWidget->ChangeSequenceSubtitleType();
		SubtitleWidget->AddToViewport(MEDIA_ZOEDER);
		SubtitleWidget->SetVisibility(ESlateVisibility::SelfHitTestInvisible);
	}
}

void USequenceManager::RemoveSubTitle()
{
	if (SubtitleWidget)
	{
		SubtitleWidget->RemoveFromViewport();
		SubtitleWidget = nullptr;
	}
}

void USequenceManager::SetSubTitleLegacy(FName id)
{
	auto Table = UTableManager::GetInstancePtr()->GetResourceSequenceRow(id);

	if (Table == nullptr)
		return;

	//if (Table->Subtitles.Num() == 0)
	//	return;
	int32 StartSubtitle = Table->StartSubtitle;
	if (StartSubtitle == 0)
		return;
	
	if (SubtitleWidget == nullptr)
	{
		CreateSubTitle();
	}

	if(SubtitleWidget)
	{
		SubtitleWidget->ClearSubtitleInfos();//IsReadySubtitle->false
		SubtitleWidget->ChangeSequenceSubtitleType();
		
		int32 EndSubtitle = Table->EndSubtitle;
		for (int32 i = StartSubtitle; i <= EndSubtitle; ++i)
		{
			FString Test = FString::FromInt(i);
			FName Element = FName(*Test);
			FSubtitleTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetSubtitleTableRow(Element);
			if (TableInfo)
			{
				SubtitleWidget->SetSubtitleInfo(TableInfo->StartTime, TableInfo->EndTime, TableInfo->StringKey);
			}
		}

		/*for (auto Element : Table->Subtitles)
		{
			FSubtitleTableInfo* TableInfo = UTableManager::GetInstancePtr()->GetSubtitleTableRow(Element);
			if (TableInfo)
			{
				SubtitleWidget->SetSubtitleInfo(TableInfo->StartTime, TableInfo->EndTime, TableInfo->StringKey);
			}
		}*/
	}
}

void USequenceManager::SetSubTitle(FName InNextSubtitle)
{
	if (CurrentSubtitle == InNextSubtitle)
	{
		InNextSubtitle = FName();
	}

	CurrentSubtitle = InNextSubtitle;

	if (SubtitleWidget == nullptr)
	{
		CreateSubTitle();
	}

	if (SubtitleWidget)
	{
		SubtitleWidget->SetSequenceSubtitle(CurrentSubtitle);
	}
}


//////////////////////////////////////////////////////////////////////////

ABaseSequenceActor* USequenceManager::PlaySequenceAtTargetActorLocation(FName SequenceKey, ESequenceActorType InSequenceType, AActor* TargetActor)
{
	FName TargetActorName;
	FTransform TargetActorTransform = TargetActor->GetActorTransform();

	URGameInstance*		RGameInstance = RGAMEINSTANCE(this);
	EGamePlayModeTypeEnum	CurrentGamePlayMode = UUtilFunctionLibrary::GetGamePlayModeEnum();

	if (CurrentGamePlayMode == EGamePlayModeTypeEnum::VE_Arena || CurrentGamePlayMode == EGamePlayModeTypeEnum::VE_MirrorDungeon)
		return nullptr;

	FViewTargetTransitionParams TransitionParam;

	if (InSequenceType == ESequenceActorType::VE_UltimateSkill)
	{
		TargetActorName = SEQUENCE_ATTACH_ULTIMATE_ACTOR;

// 		AStagePlayerController* BasePC = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(RGAMEINSTANCE(this)->GetWorld()));
// 		if (IsValid(BasePC))
// 		{
// 			TransitionParam.BlendTime = RGameInstance->GameEnvironmentVar.CameraBlendTime;
// 			TransitionParam.BlendFunction = RGameInstance->GameEnvironmentVar.BlendFunction;
// 			TransitionParam.bLockOutgoing = RGameInstance->GameEnvironmentVar.bOutGoing;
// 		}

		if (CurPlaySequenceActor.IsValid())
		{
			if (CurPlaySequenceActor->IsPlaying())
			{
				if (CurPlaySequenceActor->SequenceActorType == ESequenceActorType::VE_General)
					return nullptr;
				else if (CurPlaySequenceActor->SequenceActorType == ESequenceActorType::VE_UltimateSkill)
				{
					CurPlaySequenceActor->Stop();
				}
			}
		}
	}
	else
		TargetActorName = SEQUENCE_ATTACH_GENETAL_ACTOR;

	// Sequence Target Actor
	AActor* UltimateActor = SpawnSequenceTargetActor(TargetActorName, TargetActorTransform);
	UltimateActor->AttachToActor(TargetActor, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));

	UCapsuleComponent* ActorCapsuleComponent = UCharacterHelpFunctionLibrary::GetCapsuleComponent(TargetActor);
	if (ActorCapsuleComponent)
	{
		FVector NewLocation = TargetActor->GetActorLocation();
		NewLocation.Z -= ActorCapsuleComponent->GetScaledCapsuleHalfHeight();
		UltimateActor->SetActorLocation(NewLocation);
	}
	

	//block game till asyncloading is finished.
	FlushAsyncLoading();


	// Spawn Sequence
	ABaseSequenceActor* SequenceActor = SpawnSequence(SequenceKey, false);

	// Play Sequence
	if (IsValid(SequenceActor))
	{
		SequenceActor->SequencePlayer->SetTransitionParams(TransitionParam);

		SequenceActor->SequenceActorType = InSequenceType;

		if (ESequenceActorType::VE_UltimateSkill == InSequenceType)
		{
			if (false == SequenceActor->OnSequenceEndEvent.IsAlreadyBound(this, &USequenceManager::OnSequenceEnd_UltimateSkill))
			{
				SequenceActor->OnSequenceEndEvent.AddDynamic(this, &USequenceManager::OnSequenceEnd_UltimateSkill);
			}
		}
		
		CurPlaySequenceActor = SequenceActor;
		SequenceActor->Play(false);
	}
	
	

	return SequenceActor;
}

AActor* USequenceManager::SpawnSequenceTargetActor(FName& InTargetActorName, FTransform& InTargetTransform)
{
	if (RGAMEINSTANCE(this) == nullptr)
		return nullptr;

	AActor* SpawnedTargetActor = UUtilFunctionLibrary::FindActor<AActor>(RGAMEINSTANCE(this)->GetWorld(), AActor::StaticClass(), InTargetActorName);
	if (IsValid(SpawnedTargetActor))
	{
		SpawnedTargetActor->SetActorLocation(InTargetTransform.GetLocation());
		SpawnedTargetActor->SetActorRotation(InTargetTransform.GetRotation());
	}
	else
	{
		InTargetTransform.SetScale3D(FVector(1.f));

		FActorSpawnParameters SpawnParams;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Name = InTargetActorName;

		SpawnedTargetActor = RGAMEINSTANCE(this)->GetWorld()->SpawnActor<AEmptyBasicActor>(AEmptyBasicActor::StaticClass(), InTargetTransform, SpawnParams);

	}



	return SpawnedTargetActor;
}


//////////////////////////////////////////////////////////////////////////
// Event
void USequenceManager::OnSequenceEnd_UltimateSkill(FName InSequenceKey)
{
	URGameInstance* RGameInstance = RGAMEINSTANCE(this);
	if (RGameInstance != nullptr)
	{
		AStagePlayerController* PlayerController = Cast<AStagePlayerController>(UUtilFunctionLibrary::GetBasePlayerController(RGameInstance->GetWorld()));
		if (!IsValid(PlayerController))
			return;

		if (RGAMEINSTANCE(GEngine)->OptionManager->IsUltimateMatineeView())
		{
			UUtilFunctionLibrary::GetDirectionTrackManager()->Pop_DirectionEffect(EDirectionEffectTypeEnum::DE_Battle_CombineEffect);
			//this is deleted requested by A.D.
			//UUtilFunctionLibrary::GetDirectionTrackManager()->Push_DirectionEffect(EDirectionEffectTypeEnum::DE_Battle_Vignette);
		}

		PlayerController->ActorSettingOnTheCamera(PlayerController->GetLeaderCharacter());
		
		UGameplayStatics::SetGlobalTimeDilation(RGameInstance->GetWorld(), 1.f);
	
		APlayerCameraManager* PCM = UGameplayStatics::GetPlayerCameraManager(RGameInstance->GetWorld(), 0);
		if (PCM)
		{
			if (PCM->ViewTarget.Target != RGameInstance->CameraActor)
			{
				if (IsValid(RGameInstance->CameraActor))
				{
					PlayerController->SetViewTarget(RGameInstance->CameraActor);
					RGameInstance->CameraActor->AutoZoomCameraByValue(-1000.f, 0.1f, 3.f);
				}
			}
				
		}

	}
}